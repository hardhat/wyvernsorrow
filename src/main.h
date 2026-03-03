#ifndef MAIN_H
#define MAIN_H

#include<stdint.h>

enum GAME_STATE
{
    GAME_STATE_MENU,
    GAME_STATE_GAME,
    GAME_STATE_CHOICE,
    GAME_STATE_BATTLE,
    GAME_STATE_GAMEOVER
};

enum INPUT
{
    INPUT_UP,
    INPUT_DOWN,
    INPUT_LEFT,
    INPUT_RIGHT,
    INPUT_A,
    INPUT_B,
    INPUT_X,
    INPUT_Y,
    INPUT_START,
    INPUT_SELECT,
    INPUT_L,
    INPUT_R,
    MAX_INPUT
};

enum {
    COL_BLACK         = 0x0,
    COL_DARK_BLUE     = 0x1,
    COL_DARK_GREEN    = 0x2,
    COL_DARK_CYAN     = 0x3,
    COL_DARK_RED      = 0x4,
    COL_DARK_MAGENTA  = 0x5,
    COL_BROWN         = 0x6,
    COL_LIGHT_GRAY    = 0x7,
    COL_DARK_GRAY     = 0x8,
    COL_BLUE          = 0x9,
    COL_GREEN         = 0xa,
    COL_CYAN          = 0xb,
    COL_RED           = 0xc,
    COL_MAGENTA       = 0xd,
    COL_YELLOW        = 0xe,
    COL_WHITE         = 0xf,
};

enum TILE_COLORS
{
    TILE_COLOR_BLACK = 0,
    TILE_COLOR_WHITE = 1, // 1-4 are white
    TILE_COLOR_RED = 5, // 5-8 are red
    TILE_COLOR_GREEN = 9, // 9-12 are green
    TILE_COLOR_BLUE = 13,
    TILE_CURSOR = 14,    // This is 4 tiles in a 2x2 square
    TILE_COLOR_TERRAIN = 18,    // This is 7x4 tiles = 28 tiles
    TILE_OVERMAP_GRASS = 18,
    TILE_OVERMAP_FOREST = 19,
    TILE_OVERMAP_FOOTHILLS = 20,        // Low mountains
    TILE_OVERMAP_MOUNTAIN = 21,         // High mountains
    TILE_OVERMAP_CITY = 22,
    TILE_OVERMAP_WATER = 23,
    TILE_OVERMAP_ROUGH_WATER = 24,
    TILE_BRICK_WALL = 25,
    TILE_DECAYED_WALL = 26,
    TILE_BRIGHT_WALL = 27,
    TILE_STATUE = 28,
    TILE_WALL_OPENING = 29,
    TILE_WALL_TAPESTRY = 30,
    TILE_ROOF = 31,
    TILE_CHAIR = 32,
    TILE_TABLE = 33,
    TILE_CANDLE = 34,
    TILE_STONE_FLOOR = 35,
    TILE_PIT = 36,
    TILE_UNUSED_1 = 37,
    TILE_UNUSED_2 = 38,
    TILE_WOOD_WALL = 39,
    TILE_WOOD_CABINET = 40,
    TILE_WOOD_DOOR = 41,
    TILE_WOOD_FLOOR = 42,
    TILE_LIGHT_WOOD_FLOOR = 43,
    TILE_UNUSED_3 = 44,
    TILE_UNUSED_4 = 45,
    TILE_COLOR_ENEMIES = 46,    // This is 10 tiles
    TILE_GOBLIN = 46,
    TILE_ORC = 47,
    TILE_WEREWOLF = 48,
    TILE_SKELETON = 49,
    TILE_SILVER_KNIGHT = 50,
    TILE_DARK_KNIGHT = 51,
    TILE_GHOST = 52,
    TILE_TROLL = 53,
    TILE_GENIE = 54,
    TILE_UNUSED_5 = 55,
    TILE_COLOR_NPC_PC = 56,    // This is 7x2 tiles = 14 tiles
    TILE_PC_SWORDSMAN = 56,
    TILE_PC_MAGE = 57,
    TILE_PC_WYVERN = 58,
    TILE_NPC_LITTLE_GIRL = 59,
    TILE_NPC_MOTHER = 60,
    TILE_ITEM_FISH = 61,
    TILE_NPC_MERCHANT = 62,
    TILE_PC_SWORDSMAN_DEAD = 63,
    TILE_PC_MAGE_DEAD = 64,
    TILE_PC_WYVERN_DEAD = 65,
    TILE_NPC_FATHER = 66,
    TILE_NPC_CARPENTER = 67,
    TILE_NPC_OLD_MAN = 68,
    TILE_NPC_KING = 69,
    TILE_COLOR_BOSSES = 70,    // This is 10x2 tiles = 20 tiles (alla are 2x2 tiles)
    TILE_BOSS_OGRE = 70,
    TILE_BOSS_LAND_DRAGON = 72,
    TILE_BOSS_WIND_DRAGON = 74,
    TILE_BOSS_FIRE_DRAGON = 76,
    TILE_BOSS_PUMPKIN_HEAD = 78,
    TILE_COLOR_DEMONLORD = 90,    // This is 4x4 tiles = 16 tiles
};

#define PAL_TERRAIN 32 // index for terrain palette
#define PAL_ENEMIES 64    // index for enemies palette
#define PAL_NPC_PC 96 // index for npc/pc palette
#define PAL_BOSSES 128 // index for bosses palette
#define PAL_DEMONLORD 160 // index for demonlord palette

enum FONT_FACE
{
    FONT_FLAMBOYANT,
};

extern uint8_t tilemap_width;
extern uint8_t tilemap_height;

/// @brief Set the game state for the game main loop.
/// @param  state the game state to set.
void set_state(enum GAME_STATE state);

/// @brief immediately sets a palette color.
/// @param index palette entry to set, 0-255.
/// @param r red component of the color, 0-255.
/// @param g green component of the color, 0-255.
/// @param b blue component of the color, 0-255.
/// @note The color is converted to 565 format (5 bits red, 6 bits green, 5 bits blue).
void set_palette_rgb(uint8_t index, uint8_t r, uint8_t g, uint8_t b);
/// @brief immediately sets a palette color.
/// @param index palette entry to set, 0-255.
/// @param color 565 color value to set the palette entry to.
void set_palette(uint8_t index, uint16_t color);
/// @brief File a tile in the tileset with a specific color.
/// @param tile the tile id from 0-255 to fill with the color.
/// @param color the palette index to fill the tile with, 0-255.
void fill_tileset(uint8_t tile, uint8_t color);
/// @brief draws a tile on on the off screen layer at the specified position.
/// @param x offset from the top left corner in tiles, 0-79 for the 16x16 tile.
/// @param y offset from the top left corner in tiles, 0-15 for the 16x16 tile.
/// @param tile the tile id from the tileset to draw, which must be in the tileset.
void draw_tilemap(uint16_t x, uint8_t y, uint8_t tile);
/// @brief Draws an array of tiles on the off screen tilemap at the specified position.
/// @param x top left position of the layer 
/// @param y top left position of the layer
/// @param tile pointer to an array of tile ids from the tileset to draw, which must be in the tileset.
/// @param count number of tiles to draw in the array, which must be less than or equal to 80.
void draw_tilemap_array(uint16_t x, uint8_t y, uint8_t *tile, uint8_t count);
/// @brief Fills a rectangular area of the off screen tilemap with a specific tile.
/// @param tile tile id from the tileset to fill the area with.
/// @param x position in tiles 0-79 for the 16x16 tile.
/// @param y position in tiles 0-15 for the 16x16 tile.
/// @param w tile count across (1-80)
/// @param h tile count down (1-16)
void fill_tilemap(uint8_t tile,uint16_t x, uint8_t y, uint8_t w, uint8_t h);
/// @brief  Renders the tilemap for a specific layer.
/// @param layer layer to render, 0 for the first layer, 1 for the second layer, etc.
/// @note The tilemap is rendered in a single pass, so it should be called after
void render_tilemap(uint8_t layer);
/// @brief Clear the off screen sprite buffer to be rendered later.  Fully zeros the buffer, and sets the current count to 0.
void reset_sprite(void);
/// @brief Adds a sprite to the sprite buffer to be rendered later.
/// @param x pixel position - 16 of the sprite, anchored to the top left corner.
/// @param y pixel position - 16 of the sprite, anchored to the top left corner.
/// @param sprite tile number of the sprite to render, which must be in the tileset.
/// @note all sprites are 16x16 pixels and the position is the top left corner of the sprite.
void add_sprite(uint16_t x, uint8_t y, uint8_t sprite);
/// @brief Renders all sprites in the sprite buffer to the screen.
void render_sprites(void);
/// @brief Clears all sprites by resetting the sprites, the rendering the empty sprites
void clear_sprites(void);

#define MAX_TEXT_TILES 16
/// @brief Set the offscreen text tiles to a specific color.
/// @note Each text tile is 16x16 pixels.  And there are 16 of them.  The fonts are 8x8, so 2x2 tiles.
/// @param color palette index for the text tiles.
void clear_text_tiles(uint8_t color, uint8_t count);
/// @brief Set the font to be used for text rendering
void set_font(enum FONT_FACE font);
/// @brief Draw a single pixel in the offscreen text tilemap.
/// @param x horizontal values 0-255 with clipping, with 0 being the left.
/// @param y vertical values 0-15, with 0 being the top.
/// @param color palette index for the pixel.
void draw_text_pixel(uint16_t x, uint8_t y, uint8_t color);
/// @brief Draw a character in the offscreen text tilemap.
/// @param x horizontal values 0-255 with clipping, with 0 being the left.
/// @param y vertical values 0-15, with 0 being the top.
/// @param c character to draw, which must be in the font.
/// @param color palette index for the character.
void draw_text(uint16_t x, uint8_t y, const char *text, uint8_t color);
/// @brief Draw a character in the offscreen text tilemap with a background color.
/// @param x horizontal values 0-255 with clipping, with 0 being the left.
/// @param y vertical values 0-15, with 0 being the top.
/// @param c character to draw, which must be in the font.
/// @param fg palette index for the character.
/// @param bg palette index for the background.
void draw_text_opaque(uint16_t x, uint8_t y, const char *text, uint8_t fg, uint8_t bg);
/// @brief Renders the text tiles to the screen from the offscreen buffer.
/// @param tile tile id to start rendering from, which must be in the tileset.
/// @param tile_count number of tiles to render, which must be less than or equal to 16.
void render_text(uint8_t tile, uint8_t tile_count);

/// @brief Log to the serial port.
/// @param message the message to log.
void debug_log(const char *message);
/// @brief Log to the serial port with a format string.
/// @param format the format string to use.
/// @param ... the arguments to the format string.
void debug_logf(const char *format, ...);

#endif
