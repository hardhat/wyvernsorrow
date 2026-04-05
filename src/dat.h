#ifndef DAT_H
#define DAT_H

#include <stdbool.h>
#include <stdint.h>

// ---------------------------------------------------------------------------
// wsorrow.dat — on-disk asset file
// ---------------------------------------------------------------------------

#define DAT_MAGIC           0x4F535957UL  /* 'W','Y','S','O' little-endian */

#define DAT_TYPE_MAGIC_COOKIE  0
#define DAT_TYPE_TILESET       1
#define DAT_TYPE_PALETTE       2
#define DAT_TYPE_SCRIPTS       3
#define DAT_TYPE_STRINGS       4

#define DAT_BLOCK_MAGIC         0
#define DAT_BLOCK_TERRAIN_TS    1
#define DAT_BLOCK_TERRAIN_PAL   2
#define DAT_BLOCK_ENEMIES_TS    3
#define DAT_BLOCK_ENEMIES_PAL   4
#define DAT_BLOCK_NPC_PC_TS     5
#define DAT_BLOCK_NPC_PC_PAL    6
#define DAT_BLOCK_BOSSES_TS     7
#define DAT_BLOCK_BOSSES_PAL    8
#define DAT_BLOCK_DEMONLORD_TS  9
#define DAT_BLOCK_DEMONLORD_PAL 10
#define DAT_BLOCK_SCRIPTS       11
#define DAT_BLOCK_STRINGS       12
#define DAT_BLOCK_COUNT         13

// On-disk header entry (8 bytes, packed).
// Entry 0 is always the magic cookie:
//   offset = DAT_MAGIC ('WYSO'), size = header size in bytes, type = DAT_TYPE_MAGIC_COOKIE
typedef struct {
    uint32_t offset;   // file byte offset of block data (or magic word for entry 0)
    uint16_t size;     // block size in bytes (or header size for entry 0)
    uint8_t  type;     // DAT_TYPE_*
    uint8_t  id;       // block id (DAT_BLOCK_*)
} DatEntry;

// ---------------------------------------------------------------------------
// In-memory script cache — only the currently-executing script is resident
// ---------------------------------------------------------------------------

#define SCRIPT_MAX_BYTECODE  128
#define SCRIPT_MAX_STRING    128   // max UTF-8 dialog string including NUL

typedef struct {
    uint8_t  bytecode[SCRIPT_MAX_BYTECODE];
    uint16_t bytecode_size;
    char     dialog[SCRIPT_MAX_STRING];
    uint8_t  loaded_script_obj;   // 0xFF = none loaded
} ScriptCache;

extern ScriptCache script_cache;

// ---------------------------------------------------------------------------
// API
// ---------------------------------------------------------------------------

// Open wsorrow.dat, read and validate the header. Call once at startup.
// Returns false and prints an error if the file is missing or corrupt.
bool dat_open(void);

// Load a tileset block into dst (use the existing text_tiles buffer).
// *size_out receives the number of bytes read.
bool dat_load_tileset(uint8_t block_id, uint8_t *dst, uint16_t *size_out);

// Load a palette block. Reads into an internal static buffer; the pointer
// returned via *dst_out is valid until the next dat_load_palette() call.
bool dat_load_palette(uint8_t block_id, uint8_t **dst_out, uint16_t *size_out);

// Load the bytecode for obj into script_cache.bytecode.
// Sets script_cache.loaded_script_obj on success.
bool dat_load_script(uint8_t obj);

// Load one dialog string by index into script_cache.dialog.
bool dat_load_string(uint8_t string_id);

#endif // DAT_H
