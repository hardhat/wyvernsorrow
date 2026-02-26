import sys

GR = "TILE_OVERMAP_GRASS"
FO = "TILE_OVERMAP_FOREST"
MT = "TILE_OVERMAP_MOUNTAIN"
FM = "TILE_OVERMAP_FOOTHILLS"
WA = "TILE_OVERMAP_WATER"
RW = "TILE_OVERMAP_ROUGH_WATER"
BW = "TILE_BRICK_WALL"
WW = "TILE_WOOD_WALL"
DW = "TILE_DECAYED_WALL"
BR = "TILE_BRIGHT_WALL"
SW = "TILE_STONE_FLOOR"
WF = "TILE_WOOD_FLOOR"
LF = "TILE_LIGHT_WOOD_FLOOR"
DO = "TILE_WOOD_DOOR"
OP = "TILE_WALL_OPENING"
RO = "TILE_ROOF"
ST = "TILE_STATUE"
CH = "TILE_CHAIR"
TA = "TILE_TABLE"
CA = "TILE_CANDLE"
PT = "TILE_PIT"

W, H = 20, 15

def make_grid(bg):
    return [[bg for _ in range(W)] for _ in range(H)]

def rect(g, x, y, w, h, t):
    for i in range(y, min(H, y+h)):
        for j in range(x, min(W, x+w)):
            g[i][j] = t

def hollow(g, x, y, w, h, t):
    for j in range(x, min(W, x+w)):
        if y < H: g[y][j] = t
        if y+h-1 < H: g[y+h-1][j] = t
    for i in range(y, min(H, y+h)):
        if x < W: g[i][x] = t
        if x+w-1 < W: g[i][x+w-1] = t

def write_grid(f, name, g):
    f.write(f"const uint8_t {name}[MAP_HEIGHT][MAP_WIDTH] = {{\n")
    for r in g:
        line = "    {" + ", ".join(r) + "},"
        f.write(line + "\n")
    f.write("};\n")

out_path = "map.c"
with open(out_path, "w") as f:
    f.write('#include "main.h"\n')
    f.write('#include "map.h"\n\n')

    # 1. town_map (walled, grass outside)
    g = make_grid(GR)
    hollow(g, 2, 1, 16, 13, BW)
    g[13][9] = OP
    g[13][10] = OP
    rect(g, 3, 2, 14, 11, SW) # stone paths
    rect(g, 4, 3, 4, 3, WW)
    rect(g, 5, 4, 2, 1, LF)
    g[5][5] = DO
    rect(g, 12, 3, 4, 4, BW)
    rect(g, 13, 4, 2, 2, WF)
    g[6][13] = DO
    rect(g, 4, 9, 5, 4, WW)
    rect(g, 5, 10, 3, 2, LF)
    g[12][6] = DO
    g[7][9] = ST
    g[7][10] = ST
    write_grid(f, "town_map", g)

    # 2. forest_map
    g = make_grid(GR)
    rect(g, 2, 2, 16, 11, FO)
    rect(g, 8, 4, 4, 7, GR)
    g[7][9] = CA
    g[7][10] = CA
    write_grid(f, "forest_map", g)

    # 3. ogre_lair_map
    g = make_grid(GR)
    rect(g, 4, 2, 12, 11, DW)
    rect(g, 5, 3, 10, 9, SW)
    g[12][9] = OP
    g[12][10] = OP
    g[6][6] = PT
    g[6][13] = PT
    g[9][6] = PT
    g[9][13] = PT
    write_grid(f, "ogre_lair_map", g)

    # 4. castle_map
    g = make_grid(GR)
    rect(g, 3, 1, 14, 13, BW)
    rect(g, 4, 2, 12, 11, SW)
    g[13][9] = OP
    g[13][10] = OP
    rect(g, 5, 3, 10, 5, LF)
    g[4][9] = ST
    g[4][10] = ST
    g[6][9] = TA
    g[6][10] = TA
    write_grid(f, "castle_map", g)

    # 5. crypt_map
    g = make_grid(GR)
    rect(g, 5, 3, 10, 9, DW)
    rect(g, 6, 4, 8, 7, SW)
    g[11][9] = OP
    g[11][10] = OP
    g[6][7] = ST
    g[6][12] = ST
    g[8][7] = PT
    g[8][12] = PT
    write_grid(f, "crypt_map", g)

    # 6. tower_map
    g = make_grid(GR)
    rect(g, 6, 2, 8, 11, BR)
    rect(g, 7, 3, 6, 9, SW)
    g[12][9] = OP
    g[12][10] = DO
    g[5][9] = CA
    g[5][10] = CA
    write_grid(f, "tower_map", g)

    # 7. cave_map
    g = make_grid(GR)
    rect(g, 2, 1, 16, 13, FM)
    rect(g, 4, 3, 12, 9, SW)
    g[12][9] = OP
    g[12][10] = OP
    g[5][5] = MT
    g[5][14] = MT
    g[9][5] = MT
    g[9][14] = MT
    write_grid(f, "cave_map", g)

    # 8. ruins_map
    g = make_grid(GR)
    rect(g, 4, 3, 12, 9, DW)
    rect(g, 5, 4, 10, 7, SW)
    g[11][9] = OP
    g[11][10] = OP
    g[3][6] = GR
    g[3][13] = GR
    g[6][6] = ST
    g[6][13] = ST
    write_grid(f, "ruins_map", g)

    # 9. shrine_map
    g = make_grid(GR)
    rect(g, 6, 4, 8, 7, WW)
    rect(g, 7, 5, 6, 5, WF)
    g[10][9] = OP
    g[10][10] = OP
    g[6][9] = ST
    g[6][10] = ST
    g[7][8] = CA
    g[7][11] = CA
    write_grid(f, "shrine_map", g)

    # 10. harbor_map
    g = make_grid(GR)
    rect(g, 0, 8, W, 7, WA)
    rect(g, 8, 6, 4, 5, WF) # dock
    rect(g, 3, 2, 5, 4, WW)
    rect(g, 4, 3, 3, 2, LF)
    g[5][5] = DO
    rect(g, 12, 2, 5, 4, WW)
    rect(g, 13, 3, 3, 2, LF)
    g[5][14] = DO
    write_grid(f, "harbor_map", g)

    # 11. mountain_pass_map
    g = make_grid(GR)
    rect(g, 2, 1, 16, 13, FM)
    rect(g, 0, 5, W, 5, SW) # pass path
    rect(g, 4, 2, 12, 3, MT)
    rect(g, 4, 10, 12, 3, MT)
    write_grid(f, "mountain_pass_map", g)

    # 12. volcano_map
    g = make_grid(GR)
    rect(g, 2, 1, 16, 13, FM)
    rect(g, 4, 3, 12, 9, MT)
    rect(g, 6, 5, 8, 5, PT) # lava pit?
    g[12][9] = OP
    g[12][10] = OP
    write_grid(f, "volcano_map", g)

    # 13. sky_peak_map
    g = make_grid(GR)
    rect(g, 4, 2, 12, 11, MT)
    rect(g, 6, 4, 8, 7, SW)
    g[12][9] = OP
    g[12][10] = OP
    g[5][9] = ST
    g[5][10] = ST
    write_grid(f, "sky_peak_map", g)

    # 14. swamp_map
    g = make_grid(GR)
    rect(g, 3, 2, 14, 11, WA)
    rect(g, 5, 4, 10, 7, RW)
    rect(g, 7, 6, 6, 3, GR) # island
    g[7][9] = DW
    g[7][10] = DW
    write_grid(f, "swamp_map", g)

    # 15. demon_gate_map
    g = make_grid(GR)
    rect(g, 4, 2, 12, 11, BR)
    rect(g, 5, 3, 10, 9, SW)
    g[12][9] = OP
    g[12][10] = OP
    g[4][9] = ST
    g[4][10] = ST
    g[7][8] = PT
    g[7][9] = PT
    g[7][10] = PT
    g[7][11] = PT
    write_grid(f, "demon_gate_map", g)
