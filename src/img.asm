	.module img
	.optsdcc -mz80

	.area _TEXT

    .globl _terrain_tileset
    .globl _terrain_palette
    .globl _enemies_tileset
    .globl _enemies_palette
    .globl _npc_pc_tileset
    .globl _npc_pc_palette
    .globl _bosses_tileset
    .globl _bosses_palette
    .globl _demonlord_tileset
    .globl _demonlord_palette

_terrain_tileset:
    .dw _terrain_tileset_data
_terrain_tileset_data:
    .incbin "img/terrain.zts"
_terrain_palette:
    .dw _terrain_palette_data
_terrain_palette_data:
    .incbin "img/terrain.ztp"

_enemies_tileset:
    .dw _enemies_tileset_data
_enemies_tileset_data:
    .incbin "img/enemies.zts"
_enemies_palette:
    .dw _enemies_palette_data
_enemies_palette_data:
    .incbin "img/enemies.ztp"

_npc_pc_tileset:
    .dw _npc_pc_tileset_data
_npc_pc_tileset_data:
    .incbin "img/npc_pc.zts"
_npc_pc_palette:
    .dw _npc_pc_palette_data
_npc_pc_palette_data:
    .incbin "img/npc_pc.ztp"

_bosses_tileset:
    .dw _bosses_tileset_data
_bosses_tileset_data:
    .incbin "img/bosses.zts"
_bosses_palette:
    .dw _bosses_palette_data
_bosses_palette_data:
    .incbin "img/bosses.ztp"

_demonlord_tileset:
    .dw _demonlord_tileset_data
_demonlord_tileset_data:
    .incbin "img/demonlord.zts"
_demonlord_palette:
    .dw _demonlord_palette_data
_demonlord_palette_data:
    .incbin "img/demonlord.ztp"
