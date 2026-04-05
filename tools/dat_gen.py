#!/usr/bin/env python3
"""
dat_gen.py — Build tools/wsorrow.dat from art assets and script.json.

Layout
------
  [DAT_BLOCK_COUNT × DatEntry]   header (104 bytes, entry 0 is magic cookie)
  [terrain tileset]
  [terrain palette]
  [enemies tileset]
  [enemies palette]
  [npc_pc tileset]
  [npc_pc palette]
  [bosses tileset]
  [bosses palette]
  [demonlord tileset]
  [demonlord palette]
  [script bytecode block]
  [string block]

Run from the repository root:
    python3 tools/dat_gen.py
"""

import json
import os
import struct
import sys

# ---------------------------------------------------------------------------
# Constants — must match dat.h
# ---------------------------------------------------------------------------

DAT_MAGIC = 0x4F535957          # 'W','Y','S','O' as little-endian uint32

DAT_TYPE_MAGIC_COOKIE = 0
DAT_TYPE_TILESET      = 1
DAT_TYPE_PALETTE      = 2
DAT_TYPE_SCRIPTS      = 3
DAT_TYPE_STRINGS      = 4

DAT_BLOCK_MAGIC         = 0
DAT_BLOCK_TERRAIN_TS    = 1
DAT_BLOCK_TERRAIN_PAL   = 2
DAT_BLOCK_ENEMIES_TS    = 3
DAT_BLOCK_ENEMIES_PAL   = 4
DAT_BLOCK_NPC_PC_TS     = 5
DAT_BLOCK_NPC_PC_PAL    = 6
DAT_BLOCK_BOSSES_TS     = 7
DAT_BLOCK_BOSSES_PAL    = 8
DAT_BLOCK_DEMONLORD_TS  = 9
DAT_BLOCK_DEMONLORD_PAL = 10
DAT_BLOCK_SCRIPTS       = 11
DAT_BLOCK_STRINGS       = 12
DAT_BLOCK_COUNT         = 13

# DatEntry: uint32 offset, uint16 size, uint8 type, uint8 id
ENTRY_FMT  = '<IHBB'
ENTRY_SIZE = struct.calcsize(ENTRY_FMT)   # 8 bytes
HEADER_SIZE = DAT_BLOCK_COUNT * ENTRY_SIZE  # 104 bytes

# ---------------------------------------------------------------------------
# Script / string generation (mirrors script_gen.py logic)
# ---------------------------------------------------------------------------

OPS = {
    "END": 0, "SAY": 1, "SET_FLAG": 2, "CLEAR_FLAG": 3,
    "MOVE": 4, "SET_STATE": 5, "GOTO": 6, "IF_FLAG_GOTO": 7,
}

FLAGS = {
    "WFLAG_VISIBLE": 0x0001, "WFLAG_INTERACTABLE": 0x0002,
    "WFLAG_DEFEATED": 0x0004, "WFLAG_RESCUED": 0x0008,
    "WFLAG_CAPTIVE": 0x0010, "WFLAG_TALKED": 0x0020,
    "WFLAG_IS_SWORDSMAN": 0x0040, "WFLAG_IS_MAGE": 0x0080,
    "WFLAG_IS_WYVERN": 0x0100,
}

OBJS = {
    "WOBJ_PLAYER": 32,
    "WNPC_SWORDSMAN": 33, "WNPC_MAGE": 34, "WNPC_WYVERN": 35,
    "WNPC_LITTLE_GIRL": 36, "WNPC_MOTHER": 37, "WNPC_FATHER": 38,
    "WNPC_CARPENTER": 39, "WNPC_OLD_MAN": 40, "WNPC_KING": 41,
    "WNPC_MERCHANT": 42,
    "WENT_GOBLIN": 64, "WENT_ORC": 65, "WENT_WEREWOLF": 66,
    "WENT_SKELETON": 67, "WENT_SILVER_KNIGHT": 68, "WENT_DARK_KNIGHT": 69,
    "WENT_GHOST": 70, "WENT_TROLL": 71, "WENT_GENIE": 72,
    "WITEM_FISH": 80,
    "WBOSS_OGRE": 112, "WBOSS_LAND_DRAGON": 113, "WBOSS_WIND_DRAGON": 114,
    "WBOSS_FIRE_DRAGON": 115, "WBOSS_PUMPKIN_HEAD": 116,
    "WDEMONLORD": 127,
}


def compile_scripts(data):
    """Return (strings_list, {script_name: bytearray})."""
    strings = []
    string_to_id = {}

    def get_string_id(text):
        if text not in string_to_id:
            sid = len(strings)
            string_to_id[text] = sid
            strings.append(text)
            return sid
        return string_to_id[text]

    compiled = {}
    for name, script in data['scripts'].items():
        # Pass 1: calculate label byte offsets
        labels = {}
        byte_offset = 0
        for cmd in script:
            op_name = cmd[0]
            if op_name == "LABEL":
                labels[cmd[1]] = byte_offset
                continue
            size = 1
            if op_name == "SAY":             size += 1
            elif op_name in ("SET_FLAG", "CLEAR_FLAG"): size += 3
            elif op_name == "MOVE":          size += 2
            elif op_name == "SET_STATE":     size += 2
            elif op_name == "GOTO":          size += 1
            elif op_name == "IF_FLAG_GOTO":  size += 4
            byte_offset += size

        # Pass 2: emit bytes
        buf = bytearray()
        for cmd in script:
            op_name = cmd[0]
            if op_name == "LABEL":
                continue
            buf.append(OPS[op_name])
            if op_name == "SAY":
                buf.append(get_string_id(cmd[1]))
            elif op_name in ("SET_FLAG", "CLEAR_FLAG"):
                obj  = OBJS[cmd[1]]
                flag = FLAGS[cmd[2]]
                buf.append(obj)
                buf.append(flag & 0xFF)
                buf.append((flag >> 8) & 0xFF)
            elif op_name == "MOVE":
                buf.append(OBJS[cmd[1]])
                buf.append(OBJS[cmd[2]])
            elif op_name == "SET_STATE":
                buf.append(OBJS[cmd[1]])
                buf.append(int(cmd[2]))
            elif op_name == "GOTO":
                target = cmd[1]
                if isinstance(target, str):
                    target = labels[target]
                buf.append(target)
            elif op_name == "IF_FLAG_GOTO":
                obj  = OBJS[cmd[1]]
                flag = FLAGS[cmd[2]]
                target = cmd[3]
                if isinstance(target, str):
                    target = labels[target]
                buf.append(obj)
                buf.append(flag & 0xFF)
                buf.append((flag >> 8) & 0xFF)
                buf.append(target)
        compiled[name] = buf

    return strings, compiled


def build_script_block(compiled):
    """
    Script block binary layout:
      uint8_t  count
      uint8_t  obj_id[count]
      uint16_t data_offset[count]   (from start of data area)
      uint16_t data_size[count]
      uint8_t  data[...]
    """
    names = list(compiled.keys())
    count = len(names)

    # Map script name → world object id
    obj_ids = []
    for name in names:
        if name in OBJS:
            obj_ids.append(OBJS[name])
        else:
            print(f"  WARNING: no OBJS entry for script '{name}', using 0xFF", file=sys.stderr)
            obj_ids.append(0xFF)

    # Calculate data offsets
    data_offsets = []
    offset = 0
    for name in names:
        data_offsets.append(offset)
        offset += len(compiled[name])

    # Build index header
    index = bytearray()
    index.append(count)
    for oid in obj_ids:
        index.append(oid)
    for off in data_offsets:
        index += struct.pack('<H', off)
    for name in names:
        index += struct.pack('<H', len(compiled[name]))

    # Concatenate bytecode
    data = bytearray()
    for name in names:
        data += compiled[name]

    return bytes(index + data)


def build_string_block(strings):
    """
    String block binary layout:
      uint8_t  count
      uint16_t offset[count]   (from start of packed string data area)
      char     data[...]       (packed NUL-terminated strings)
    """
    count = len(strings)
    packed = bytearray()
    offsets = []
    for s in strings:
        offsets.append(len(packed))
        packed += s.encode('utf-8') + b'\x00'

    index = bytearray()
    index.append(count)
    for off in offsets:
        index += struct.pack('<H', off)

    return bytes(index + packed)


# ---------------------------------------------------------------------------
# DatEntry helpers
# ---------------------------------------------------------------------------

def make_entry(offset, size, entry_type, entry_id):
    return struct.pack(ENTRY_FMT, offset, size, entry_type, entry_id)


def make_magic_entry():
    return make_entry(DAT_MAGIC, HEADER_SIZE, DAT_TYPE_MAGIC_COOKIE, 0)


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    base = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
    img_dir    = os.path.join(base, 'img')
    tools_dir  = os.path.join(base, 'tools', 'story_editor')
    out_path   = os.path.join(base, 'bin', 'wsorrow.dat')

    # Asset pairs: (block_id_ts, block_id_pal, stem)
    asset_pairs = [
        (DAT_BLOCK_TERRAIN_TS,   DAT_BLOCK_TERRAIN_PAL,   'terrain'),
        (DAT_BLOCK_ENEMIES_TS,   DAT_BLOCK_ENEMIES_PAL,   'enemies'),
        (DAT_BLOCK_NPC_PC_TS,    DAT_BLOCK_NPC_PC_PAL,    'npc_pc'),
        (DAT_BLOCK_BOSSES_TS,    DAT_BLOCK_BOSSES_PAL,    'bosses'),
        (DAT_BLOCK_DEMONLORD_TS, DAT_BLOCK_DEMONLORD_PAL, 'demonlord'),
    ]

    # ------------------------------------------------------------------
    # Load art assets
    # ------------------------------------------------------------------
    asset_data = {}   # block_id -> bytes
    for (bid_ts, bid_pal, stem) in asset_pairs:
        ts_path  = os.path.join(img_dir, f'{stem}.zts')
        pal_path = os.path.join(img_dir, f'{stem}.ztp')
        with open(ts_path, 'rb') as f:
            asset_data[bid_ts] = f.read()
        with open(pal_path, 'rb') as f:
            asset_data[bid_pal] = f.read()
        print(f'  {stem}: tileset {len(asset_data[bid_ts])} bytes, '
              f'palette {len(asset_data[bid_pal])} bytes')

    # ------------------------------------------------------------------
    # Compile scripts and strings from script.json
    # ------------------------------------------------------------------
    json_path = os.path.join(tools_dir, 'script.json')
    with open(json_path, 'r') as f:
        script_data = json.load(f)

    strings, compiled = compile_scripts(script_data)
    script_block = build_script_block(compiled)
    string_block = build_string_block(strings)
    asset_data[DAT_BLOCK_SCRIPTS] = script_block
    asset_data[DAT_BLOCK_STRINGS] = string_block
    print(f'  scripts: {len(script_block)} bytes ({len(compiled)} scripts)')
    print(f'  strings: {len(string_block)} bytes ({len(strings)} strings)')

    # ------------------------------------------------------------------
    # Build header
    # ------------------------------------------------------------------
    # Calculate data offsets (data starts after header)
    data_offset = HEADER_SIZE
    block_offsets = {}
    for bid in range(1, DAT_BLOCK_COUNT):   # skip magic cookie slot
        block_offsets[bid] = data_offset
        data_offset += len(asset_data[bid])

    header = bytearray()
    header += make_magic_entry()
    for bid in range(1, DAT_BLOCK_COUNT):
        data = asset_data[bid]
        if bid in [DAT_BLOCK_TERRAIN_TS, DAT_BLOCK_ENEMIES_TS, DAT_BLOCK_NPC_PC_TS,
                   DAT_BLOCK_BOSSES_TS, DAT_BLOCK_DEMONLORD_TS]:
            entry_type = DAT_TYPE_TILESET
        elif bid in [DAT_BLOCK_TERRAIN_PAL, DAT_BLOCK_ENEMIES_PAL, DAT_BLOCK_NPC_PC_PAL,
                     DAT_BLOCK_BOSSES_PAL, DAT_BLOCK_DEMONLORD_PAL]:
            entry_type = DAT_TYPE_PALETTE
        elif bid == DAT_BLOCK_SCRIPTS:
            entry_type = DAT_TYPE_SCRIPTS
        else:
            entry_type = DAT_TYPE_STRINGS
        header += make_entry(block_offsets[bid], len(data), entry_type, bid)

    assert len(header) == HEADER_SIZE

    # ------------------------------------------------------------------
    # Write output
    # ------------------------------------------------------------------
    os.makedirs(os.path.dirname(out_path), exist_ok=True)
    with open(out_path, 'wb') as f:
        f.write(header)
        for bid in range(1, DAT_BLOCK_COUNT):
            f.write(asset_data[bid])

    total = HEADER_SIZE + sum(len(asset_data[bid]) for bid in range(1, DAT_BLOCK_COUNT))
    print(f'\nWrote {out_path}: {total} bytes total')


if __name__ == '__main__':
    main()
