# Memory Refactor Plan: wsorrow.dat

## Problem

The Z80 target (Zeal OS) has 48 KB of usable program memory. The binary is currently
oversized because all art and script data are compiled in via `img.asm` (`.incbin`) and
`script.h` (`static const uint8_t`/`static const char`). Rough breakdown of what is
baked into the binary today:

| Asset               | Compressed size (in binary) |
| :------------------ | :-------------------------- |
| terrain tileset     | ~4 206 bytes (.zx0)         |
| enemies tileset     | ~1 788 bytes (.zx0)         |
| npc_pc tileset      | ~2 272 bytes (.zx0)         |
| bosses tileset      | ~3 602 bytes (.zx0)         |
| demonlord tileset   | ~2 660 bytes (.zx0)         |
| All palettes        | ~600 bytes (5× ~120 bytes)  |
| script strings      | ~8 KB (138 strings)         |
| script bytecode     | ~1 KB (22 scripts)          |
| **Total (approx)**  | **~24 KB**                  |

Removing this from the binary and loading on demand recovers ~24 KB of program space.
The existing 5 KB `text_tiles` buffer is large enough to stream one tileset at a time.

---

## Target Architecture

### wsorrow.dat — flat binary data file

A single file on the Zeal filesystem. Structured as a header table followed by data
blocks. The game opens it once at startup and keeps the file descriptor open for
on-demand seeks and reads.

#### File layout

```
Offset 0x0000  DAT_HEADER  (fixed-size table of block descriptors)
               ...
               BLOCK_0     magic cookie      (validation entry)
               BLOCK_1     terrain tileset   (.zts, uncompressed)
               BLOCK_2     terrain palette   (.ztp)
               BLOCK_3     enemies tileset
               BLOCK_4     enemies palette
               BLOCK_5     npc_pc tileset
               BLOCK_6     npc_pc palette
               BLOCK_7     bosses tileset
               BLOCK_8     bosses palette
               BLOCK_9     demonlord tileset
               BLOCK_10    demonlord palette
               BLOCK_11    script bytecode   (all 22 scripts, concatenated)
               BLOCK_12    script strings    (all 138 strings, NUL-terminated, packed)
```

#### Header entry (8 bytes each)

```c
typedef struct {
    uint32_t offset;      // file offset in bytes; for MAGIC_COOKIE entry: 'W','Y','S','O' (0x4F535957 LE)
    uint16_t size;        // block size in bytes; for MAGIC_COOKIE entry: total header size in bytes
    uint8_t  type;        // DAT_TYPE_MAGIC_COOKIE, DAT_TYPE_TILESET, DAT_TYPE_PALETTE,
                          // DAT_TYPE_SCRIPTS, DAT_TYPE_STRINGS
    uint8_t  id;          // asset id (PAL_TERRAIN, PAL_ENEMIES, … or 0 for non-asset blocks)
} DatEntry;
```

Entry 0 is always the magic cookie:

```c
// Entry 0 — magic cookie
{
    .offset = 0x4F535957,   // 'W','Y','S','O' as a little-endian uint32
    .size   = sizeof(DatEntry) * DAT_BLOCK_COUNT,   // full header size in bytes
    .type   = DAT_TYPE_MAGIC_COOKIE,
    .id     = 0
}
```

`dat_open()` reads entry 0 and rejects the file if `offset != 0x4F535957` or
`size != sizeof(DatEntry) * DAT_BLOCK_COUNT`.

13 entries × 8 bytes = 104 bytes header.

---

## In-Memory Script State (lightweight VM)

The current `script.h` keeps all bytecode and all strings resident. The replacement
keeps only:

```c
// dat.h  (new file)
#define SCRIPT_MAX_BYTECODE  128   // max bytes for one script
#define SCRIPT_MAX_STRING    96    // max chars for one dialog string (2 lines × 36 + NUL + slack)

typedef struct {
    uint8_t  bytecode[SCRIPT_MAX_BYTECODE];   // currently-executing script
    uint16_t bytecode_size;
    char     dialog[SCRIPT_MAX_STRING];       // current dialog string (loaded on SAY)
    uint8_t  loaded_script_obj;               // which obj's script is loaded (0xFF = none)
} ScriptCache;

extern ScriptCache script_cache;
```

Total in-RAM cost: 128 + 2 + 96 + 1 = **227 bytes** (down from ~9 KB).

The VM in `story.c` is unchanged except that `get_script()` calls `dat_load_script()`
instead of returning a compile-time pointer, and `game_show_dialog()` reads from
`script_cache.dialog` instead of `story_strings[]`.

---

## New Module: dat.c / dat.h

```c
// dat.h

#define DAT_BLOCK_MAGIC         0   // validation entry — always first
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

// Open wsorrow.dat and read the header. Call once at startup.
bool dat_open(void);

// Load a tileset block into dst (caller provides the buffer — use text_tiles).
// size_out receives actual bytes read.
bool dat_load_tileset(uint8_t block_id, uint8_t *dst, uint16_t *size_out);

// Load a palette block directly (small; can use a stack buffer or a 512-byte static).
bool dat_load_palette(uint8_t block_id, uint16_t *dst, uint16_t *size_out);

// Load a script for obj into script_cache.bytecode. Sets loaded_script_obj.
// The bytecode block is a packed index (sorted by WORLD_OBJECT_ID) + data.
bool dat_load_script(uint8_t obj);

// Load one string by index into script_cache.dialog.
bool dat_load_string(uint8_t string_id);
```

The script block internal layout:

```
Script block:
  uint8_t  count                    // number of scripts (22)
  uint8_t  obj_id[count]            // world object id for each script
  uint16_t offset[count]            // byte offset from start of block data
  uint16_t size[count]              // size in bytes of each script
  uint8_t  data[...]                // concatenated bytecode
```

Searching 22 entries linearly is negligible on Z80.

String block internal layout:

```
  uint8_t  count                    // number of strings (138)
  uint16_t offset[count]            // byte offset from start of string data area
  char     data[...]                // packed NUL-terminated strings
```

---

## Build Tooling: dat_gen.py (new tool)

A Python script that takes the `.zts` (uncompressed), `.ztp`, `script.json` and
`story_strings[]` (parsed from the existing generated `script.h` or directly from
`script_gen.py`), and writes `wsorrow.dat`. Integrate into the Makefile.

```
tools/dat_gen.py
  inputs:  img/terrain.zts  img/terrain.ztp
           img/enemies.zts  img/enemies.ztp
           img/npc_pc.zts   img/npc_pc.ztp
           img/bosses.zts   img/bosses.ztp
           img/demonlord.zts img/demonlord.ztp
           tools/story_editor/script.json
  output:  bin/wsorrow.dat
```

Extend `script_gen.py` (or make `dat_gen.py` call it as a library) so both the C
header and the dat file are produced together, ensuring string IDs stay in sync.

---

## Changes to Existing Files

### img.asm / img.h → removed

`img.asm` is deleted. `img.h` is replaced by `dat.h`. All callers in `main.c` that do:

```c
dzx0_standard(terrain_tileset, text_tiles);
gfx_tileset_load(&ctx, text_tiles, terrain_tileset_sz, &options0);
gfx_palette_load(&ctx, terrain_palette, terrain_palette_sz, PAL_TERRAIN);
```

become:

```c
uint16_t sz;
dat_load_tileset(DAT_BLOCK_TERRAIN_TS, text_tiles, &sz);
gfx_tileset_load(&ctx, text_tiles, sz, &options0);
dat_load_palette(DAT_BLOCK_TERRAIN_PAL, pal_buf, &sz);
gfx_palette_load(&ctx, pal_buf, sz, PAL_TERRAIN);
```

The `dzx0_standard` call and the associated decompressor object (`dzx0_standard.rel`,
`dzx0_standard.asm`) can be removed once `.zts` is stored uncompressed in the dat file.

### script.h → removed

Replaced entirely by `dat.c` / `dat.h` + the in-memory `ScriptCache`. The `story.c`
VM replaces the direct array indexing of `story_strings[]` and `script_*[]` with calls
to `dat_load_string()` and `dat_load_script()`.

### story.c — minimal changes

- `get_script(obj)` → calls `dat_load_script(obj)`; returns `script_cache.bytecode`
- `SOP_SAY` handler → calls `dat_load_string(string_id)` before `game_show_dialog(script_cache.dialog)`

### Makefile

- Remove `obj/img.rel` and `obj/dzx0_standard.rel` from `OBJ`.
- Add `obj/dat.rel`.
- Add `bin/wsorrow.dat` target, produced by `python3 tools/dat_gen.py`.
- Remove `.zx0` targets (no longer needed).
- The `.dat` file must be deployed alongside the binary on the Zeal filesystem.

---

## Implementation Steps (ordered)

1. **Write `dat_gen.py`** — reads `.zts`/`.ztp`/`script.json`, writes `bin/wsorrow.dat`.
2. **Write `dat.h` / `dat.c`** — open, header parse, `dat_load_tileset`, `dat_load_palette`, `dat_load_script`, `dat_load_string`.
3. **Update `story.c`** — replace `get_script` and `SOP_SAY` to use dat API.
4. **Update `main.c`** — replace all `dzx0_standard` + `gfx_tileset_load` + `gfx_palette_load` calls to use dat API.
5. **Delete `img.asm`, `img.h`, `script.h`** — remove from build.
6. **Update `Makefile`** — remove old targets, add `dat.rel` and `wsorrow.dat`.
7. **Test** — confirm binary fits in 48 KB, all tilesets load correctly, all scripts execute.

---

## Notes

- **Palette buffer**: palettes are small (~120 bytes each, RGB565 × ~60 entries). A
  static `uint16_t pal_buf[64]` (128 bytes) in `dat.c` is sufficient and avoids stack
  pressure on Z80.
- **Seek cost**: Zeal OS `lseek` is available. Each tileset load is one seek + one read.
  The palette immediately follows its tileset in the file so a second `read` (no seek) is
  enough.
- **No zx0 at runtime**: storing `.zts` uncompressed in the dat file means the
  decompressor is no longer needed, saving ~700 bytes of code space.
- **Script loading frequency**: scripts are small (≤128 bytes) and loaded once per
  interact event, not per frame. Disk latency is acceptable here.
- **String loading frequency**: strings are loaded only when `SOP_SAY` fires, which is
  also infrequent. No caching of the string array is needed.

---

## Checklist

### Step 1 — dat_gen.py
- [ ] **1.1** Create `tools/dat_gen.py` scaffold (imports, arg parsing, `main()`)
- [ ] **1.2** Integrate `script_gen.py` logic to collect bytecode + strings
- [ ] **1.3** Write magic cookie entry (`0x4F535957`, header size, `DAT_TYPE_MAGIC_COOKIE`)
- [ ] **1.4** Write tileset + palette block entries and data (5 pairs)
- [ ] **1.5** Write script bytecode block (index + data)
- [ ] **1.6** Write string block (index + packed NUL-terminated strings)
- [ ] **1.7** Verify output with a hex dump / sanity print

### Step 2 — dat.h / dat.c
- [ ] **2.1** Write `src/dat.h` — constants, `DatEntry` struct, `ScriptCache` struct, API declarations
- [ ] **2.2** Write `dat_open()` — open file, read + validate magic cookie entry
- [ ] **2.3** Write `dat_load_tileset()` — seek to block offset, read into caller buffer
- [ ] **2.4** Write `dat_load_palette()` — seek to block offset, read into `pal_buf`
- [ ] **2.5** Write `dat_load_script()` — seek to scripts block, linear search by obj_id, read bytecode
- [ ] **2.6** Write `dat_load_string()` — seek to strings block, seek to string offset, read into `script_cache.dialog`

### Step 3 — story.c
- [ ] **3.1** Replace `#include "script.h"` with `#include "dat.h"`
- [ ] **3.2** Replace `get_script()` to call `dat_load_script(obj)` and return `script_cache.bytecode`
- [ ] **3.3** Replace `SOP_SAY` handler to call `dat_load_string(string_id)` then use `script_cache.dialog`

### Step 4 — main.c
- [ ] **4.1** Replace `#include "img.h"` with `#include "dat.h"`
- [ ] **4.2** Call `dat_open()` early in `init()`
- [ ] **4.3** Replace each `dzx0_standard` + `gfx_tileset_load` call with `dat_load_tileset` + `gfx_tileset_load`
- [ ] **4.4** Replace each `gfx_palette_load` (from img pointers) with `dat_load_palette` + `gfx_palette_load`

### Step 5 — Remove old files
- [ ] **5.1** Delete `src/img.asm`
- [ ] **5.2** Delete `src/img.h`
- [ ] **5.3** Delete `src/script.h` (superseded by dat API)
- [ ] **5.4** Delete `src/dzx0_standard.asm`

### Step 6 — Makefile
- [ ] **6.1** Remove `obj/img.rel` and `obj/dzx0_standard.rel` from `OBJ`
- [ ] **6.2** Add `obj/dat.rel` to `OBJ`
- [ ] **6.3** Add `bin/wsorrow.dat` target (`python3 tools/dat_gen.py`)
- [ ] **6.4** Remove `.zx0` compression targets
- [ ] **6.5** Add `obj/dat.rel` dependency line

### Step 7 — Verify
- [ ] **7.1** Build succeeds with no linker overflow
- [ ] **7.2** `bin/wsorrow.dat` is present and passes magic cookie check at runtime
- [ ] **7.3** All tilesets render correctly in-game
- [ ] **7.4** Dialog scripts execute correctly for all player types
