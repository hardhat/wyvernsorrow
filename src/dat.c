#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <zos_vfs.h>

#include "dat.h"
#include "main.h"   // debug_log / debug_logf

// ---------------------------------------------------------------------------
// Module-private state
// ---------------------------------------------------------------------------

static zos_dev_t    dat_fd = -1;
static DatEntry     header[DAT_BLOCK_COUNT];

// Palette data buffer — avoids Z80 stack pressure for up to 128-byte palettes.
static uint8_t pal_buf[128];

// Exported script cache — used by story.c
ScriptCache script_cache;

// ---------------------------------------------------------------------------
// Internal helpers
// ---------------------------------------------------------------------------

static bool dat_seek(uint32_t offset)
{
    int32_t off = (int32_t)offset;
    return seek(dat_fd, &off, SEEK_SET) == 0;
}

static bool dat_read(void *buf, uint16_t size)
{
    uint16_t n = size;
    return read(dat_fd, buf, &n) == 0 && n == size;
}

// ---------------------------------------------------------------------------
// dat_open — open wsorrow.dat, read and validate the header.
// ---------------------------------------------------------------------------

bool dat_open(void)
{
    dat_fd = open("wsorrow.dat", O_RDONLY);
    if (dat_fd < 0) {
        debug_log("dat: failed to open wsorrow.dat");
        return false;
    }

    // Read full header in one shot.
    uint16_t hdr_size = sizeof(header);
    if (read(dat_fd, header, &hdr_size) != 0 || hdr_size != sizeof(header)) {
        debug_log("dat: header read failed");
        close(dat_fd);
        dat_fd = -1;
        return false;
    }

    // Validate magic cookie entry.
    if (header[DAT_BLOCK_MAGIC].offset != DAT_MAGIC ||
        header[DAT_BLOCK_MAGIC].size   != sizeof(header) ||
        header[DAT_BLOCK_MAGIC].type   != DAT_TYPE_MAGIC_COOKIE) {
        debug_log("dat: bad magic cookie");
        close(dat_fd);
        dat_fd = -1;
        return false;
    }

    script_cache.loaded_script_obj = 0xFF;
    debug_log("dat: opened wsorrow.dat");
    return true;
}

// ---------------------------------------------------------------------------
// dat_load_tileset
// ---------------------------------------------------------------------------

bool dat_load_tileset(uint8_t block_id, uint8_t *dst, uint16_t *size_out)
{
    if (dat_fd < 0 || block_id >= DAT_BLOCK_COUNT) return false;

    DatEntry *e = &header[block_id];
    if (e->type != DAT_TYPE_TILESET) return false;

    if (!dat_seek(e->offset)) return false;

    uint16_t n = e->size;
    if (read(dat_fd, dst, &n) != 0 || n != e->size) return false;

    if (size_out) *size_out = e->size;
    return true;
}

// ---------------------------------------------------------------------------
// dat_load_palette
// ---------------------------------------------------------------------------

bool dat_load_palette(uint8_t block_id, uint8_t **dst_out, uint16_t *size_out)
{
    if (dat_fd < 0 || block_id >= DAT_BLOCK_COUNT) return false;

    DatEntry *e = &header[block_id];
    if (e->type != DAT_TYPE_PALETTE) return false;
    if (e->size > sizeof(pal_buf)) return false;

    if (!dat_seek(e->offset)) return false;

    uint16_t n = e->size;
    if (read(dat_fd, pal_buf, &n) != 0 || n != e->size) return false;

    if (dst_out)  *dst_out  = pal_buf;
    if (size_out) *size_out = e->size;
    return true;
}

// ---------------------------------------------------------------------------
// dat_load_script
//
// Script block layout (from dat_gen.py build_script_block):
//   uint8_t  count
//   uint8_t  obj_id[count]
//   uint16_t data_offset[count]
//   uint16_t data_size[count]
//   uint8_t  data[...]
// ---------------------------------------------------------------------------

bool dat_load_script(uint8_t obj)
{
    if (dat_fd < 0) return false;

    // Avoid redundant loads.
    if (script_cache.loaded_script_obj == obj) return true;

    DatEntry *e = &header[DAT_BLOCK_SCRIPTS];
    if (e->type != DAT_TYPE_SCRIPTS) return false;

    uint32_t base = e->offset;

    // Read count.
    if (!dat_seek(base)) return false;
    uint8_t count;
    uint16_t n = 1;
    if (read(dat_fd, &count, &n) != 0 || n != 1) return false;

    // Read obj_id table (count bytes, sequential after count byte).
    // Max 32 scripts: safe as a small stack buffer.
    uint8_t obj_ids[32];
    if (count > 32) return false;
    n = count;
    if (read(dat_fd, obj_ids, &n) != 0 || n != count) return false;

    // Linear search.
    uint8_t idx = 0xFF;
    for (uint8_t i = 0; i < count; i++) {
        if (obj_ids[i] == obj) { idx = i; break; }
    }
    if (idx == 0xFF) return false;

    // Seek to data_offset[idx]: base + 1 (count) + count (obj_ids) + idx*2
    uint32_t off_pos = base + 1u + count + (uint16_t)idx * 2u;
    if (!dat_seek(off_pos)) return false;
    uint16_t data_offset;
    n = 2;
    if (read(dat_fd, &data_offset, &n) != 0 || n != 2) return false;

    // Seek to data_size[idx]: base + 1 + count + count*2 + idx*2
    uint32_t sz_pos = base + 1u + count + (uint16_t)count * 2u + (uint16_t)idx * 2u;
    if (!dat_seek(sz_pos)) return false;
    uint16_t data_size;
    n = 2;
    if (read(dat_fd, &data_size, &n) != 0 || n != 2) return false;

    if (data_size > SCRIPT_MAX_BYTECODE) return false;

    // Seek to data area: base + 1 + count*5 (count + obj_ids + offsets + sizes)
    uint32_t data_start = base + 1u + (uint16_t)count * 5u;
    if (!dat_seek(data_start + data_offset)) return false;
    n = data_size;
    if (read(dat_fd, script_cache.bytecode, &n) != 0 || n != data_size) return false;

    script_cache.bytecode_size        = data_size;
    script_cache.loaded_script_obj    = obj;
    return true;
}

// ---------------------------------------------------------------------------
// dat_load_string
//
// String block layout (from dat_gen.py build_string_block):
//   uint8_t  count
//   uint16_t offset[count]   (from start of packed string data area)
//   char     data[...]       (NUL-terminated, packed)
// ---------------------------------------------------------------------------

bool dat_load_string(uint8_t string_id)
{
    if (dat_fd < 0) return false;

    DatEntry *e = &header[DAT_BLOCK_STRINGS];
    if (e->type != DAT_TYPE_STRINGS) return false;

    uint32_t base = e->offset;

    // Read count.
    if (!dat_seek(base)) return false;
    uint8_t count;
    uint16_t n = 1;
    if (read(dat_fd, &count, &n) != 0 || n != 1) return false;
    if (string_id >= count) return false;

    // Seek directly to offset[string_id]: base + 1 + string_id * 2
    uint32_t off_pos = base + 1u + (uint16_t)string_id * 2u;
    if (!dat_seek(off_pos)) return false;
    uint16_t str_offset;
    n = 2;
    if (read(dat_fd, &str_offset, &n) != 0 || n != 2) return false;

    // String data area starts at: base + 1 + count * 2
    uint32_t data_start = base + 1u + (uint16_t)count * 2u;
    if (!dat_seek(data_start + str_offset)) return false;

    // Read up to SCRIPT_MAX_STRING - 1 bytes, then NUL-terminate.
    n = SCRIPT_MAX_STRING - 1;
    if (read(dat_fd, script_cache.dialog, &n) != 0) return false;
    // Find NUL within what was read; n holds bytes actually read.
    script_cache.dialog[n] = '\0';

    return true;
}
