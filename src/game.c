
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "game.h"
#include "world.h"
#include "story.h"
#include "map.h"

struct Game game;

int cursor_x=0;
int cursor_y=0;
bool held[MAX_INPUT];

static uint8_t current_room = WROOM_TOWN;
static uint8_t choice_index = 0;
static uint8_t last_choice_index;
static bool choice_has_battle = false;

// World-map player tile position (separate from location-map cursor).
static uint8_t worldmap_x = 8;
static uint8_t worldmap_y = 13;

// World-map icon positions for each room (indexed by WROOM_* ID 0-14).
// Order: TOWN, FOREST, OGRE_LAIR, CASTLE, CRYPT, TOWER, CAVE, RUINS,
//        SHRINE, HARBOR, MOUNTAIN_PASS, VOLCANO, SKY_PEAK, SWAMP, DEMON_GATE
static const uint8_t room_wm_x[15] = { 8,  4,  2,  5,  1,  7, 10, 13, 11, 16, 12, 17, 14,  2,  9};
static const uint8_t room_wm_y[15] = {13, 10,  7,  5,  3,  3,  8,  9, 12,  7,  4,  4,  2, 12,  1};
// Icon tile displayed on the world map when a room is unlocked.
static const uint8_t room_icon[15] = {
    TILE_OVERMAP_CITY,        // TOWN
    TILE_OVERMAP_FOREST,      // FOREST
    TILE_DECAYED_WALL,        // OGRE_LAIR
    TILE_BRICK_WALL,          // CASTLE
    TILE_STATUE,              // CRYPT
    TILE_BRIGHT_WALL,         // TOWER
    TILE_WALL_TAPESTRY,       // CAVE
    TILE_DECAYED_WALL,        // RUINS
    TILE_WALL_OPENING,        // SHRINE
    TILE_OVERMAP_WATER,       // HARBOR
    TILE_OVERMAP_FOOTHILLS,   // MOUNTAIN_PASS
    TILE_FIREBALL,            // VOLCANO
    TILE_OVERMAP_MOUNTAIN,    // SKY_PEAK
    TILE_OVERMAP_ROUGH_WATER, // SWAMP
    TILE_WALL_TAPESTRY,       // DEMON_GATE
};

// Tile solidity bitmask.  Bit (id & 7) of byte (id >> 3) is 1 for solid tiles.
// Solid tiles: OVERMAP_FOREST(19), OVERMAP_MOUNTAIN(21), OVERMAP_WATER(23),
//   OVERMAP_ROUGH_WATER(24), BRICK_WALL(25), DECAYED_WALL(26), BRIGHT_WALL(27),
//   STATUE(28), WALL_TAPESTRY(30), ROOF(31), CHAIR(32), TABLE(33),
//   PIT(36), WOOD_WALL(39), WOOD_CABINET(40).
static const uint8_t tile_solid[32] = {
    0x00, 0x00,  // tiles   0-15: passable (color/fill tiles)
    0xA8,        // tiles  16-23: FOREST(19) MOUNTAIN(21) WATER(23)
    0xDF,        // tiles  24-31: ROUGH_WATER BRICK DECAYED BRIGHT STATUE *TAPESTRY ROOF
    0x93,        // tiles  32-39: CHAIR TABLE *PIT WOOD_WALL
    0x01,        // tiles  40-47: WOOD_CABINET
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00
};

// Precomputed passability map, rebuilt on every room enter / resume.
//   0x00        = solid tile (impassable, no object)
//   0xFF        = free to walk
//   anything else = world-object ID blocking that tile (bump = interact)
static uint8_t passable[MAP_HEIGHT][MAP_WIDTH];

// Cardinal direction offsets for the A-button adjacent scan.
static const int8_t adj_dx[4] = { 0,  0, -1,  1};
static const int8_t adj_dy[4] = {-1,  1,  0,  0};

#define FIRST_ROOM WROOM_TOWN
#define LAST_ROOM WROOM_DEMON_GATE

#define DIALOG_TILE 0xB0

// Typewriter dialog state.
#define DIALOG_PHASE_IDLE  0  // nothing animating
#define DIALOG_PHASE_BOX   1  // uploading background tiles left-to-right
#define DIALOG_PHASE_TEXT  2  // dripping characters into uploaded tiles

static uint8_t dialog_phase    = DIALOG_PHASE_IDLE;
static uint8_t dialog_tile_idx;   // next tile (0-19) to upload in BOX phase
static uint8_t dialog_char_idx;   // next character index in TEXT phase
static uint8_t dialog_total;      // total printable chars across both lines
static uint8_t dialog_nl;         // length of line 1 (index of '\n', or full length)
static bool    dialog_newline;    // true when text contains a '\n' split
static char    dialog_buf[82];    // raw stored text (up to 2x40 + '\n' + NUL)
#define ROOM_TILE 0xA0
#define ROOM_TILE_COUNT 8
#define ROOM_TILE_X 6

static void draw_dialog_text(const char *text)
{
    // Draw up to two lines split on '\n' (optional).
    const char *nl = NULL;
    for(const char *p = text; *p != 0; p++) {
        if(*p == '\n') { nl = p; break; }
    }

    clear_text_tiles(COL_DARK_BLUE, 20);
    set_font(FONT_FLAMBOYANT);
    if(nl == NULL) {
        draw_text_opaque(4, 4, text, COL_WHITE, COL_BLUE);
    } else {
        // Render first line
        char line1[41];
        uint8_t len = (uint8_t)(nl - text);
        if(len > 40) len = 40;
        for(uint8_t i = 0; i < len; i++) line1[i] = text[i];
        line1[len] = 0;
        draw_text_opaque(4, 0, line1, COL_WHITE, COL_BLUE);
        // Render second line
        draw_text_opaque(4, 8, nl + 1, COL_WHITE, COL_BLUE);
    }
    render_text(DIALOG_TILE, 20);

    // Place the 20 text tiles at the bottom of the screen.
    for(uint8_t i = 0; i < 20; i++) {
        draw_tilemap(2 + i, 14, DIALOG_TILE + i);
    }
    render_tilemap(0);
}

// Instantly complete any in-progress typewriter and leave the dialog visible.
static void dialog_flush(void)
{
    if(dialog_phase == DIALOG_PHASE_IDLE) return;
    draw_dialog_text(dialog_buf);
    dialog_phase = DIALOG_PHASE_IDLE;
}

// Called by the story VM.
void game_show_dialog(const char *text)
{
    // If a previous typewriter is still running, finish it first.
    if(dialog_phase != DIALOG_PHASE_IDLE)
        dialog_flush();

    // Copy text and locate the newline split.
    uint8_t len = 0;
    while(text[len] != '\0' && len < 81) len++;
    for(uint8_t i = 0; i <= len; i++) dialog_buf[i] = text[i];

    dialog_newline = false;
    dialog_nl = len;
    for(uint8_t i = 0; i < len; i++) {
        if(dialog_buf[i] == '\n') {
            dialog_newline = true;
            dialog_nl = i;
            break;
        }
    }

    // Count printable characters (excluding the '\n' itself).
    if(!dialog_newline) {
        dialog_total = len;
    } else {
        uint8_t len2 = 0;
        const char *p = dialog_buf + dialog_nl + 1;
        while(*p != '\0' && len2 < 40) { p++; len2++; }
        dialog_total = dialog_nl + len2;
    }

    // Fill text_tiles with the background color (no text yet) and wire up
    // the tilemap row so the hardware references these tile slots.
    clear_text_tiles(COL_DARK_BLUE, 20);
    set_font(FONT_FLAMBOYANT);
    for(uint8_t i = 0; i < 20; i++)
        draw_tilemap((uint16_t)(2 + i), 14, DIALOG_TILE + i);

    // Push the first blank tile immediately so the box begins appearing.
    render_text_offset(DIALOG_TILE, 0, 1);
    render_tilemap(0);

    dialog_tile_idx = 1;
    dialog_char_idx = 0;
    dialog_phase    = DIALOG_PHASE_BOX;
}

static void game_clear_dialog(void)
{
    // Remove the dialog tiles from the tilemap.
    fill_tilemap(TILE_OVERMAP_GRASS,0,14,20,1);
    render_tilemap(0);
}

static const char *get_room_name(uint8_t room)
{
    switch(room) {
        case WROOM_TOWN: return "Town";
        case WROOM_FOREST: return "Forest";
        case WROOM_OGRE_LAIR: return "Ogre Lair";
        case WROOM_CASTLE: return "Castle";
        case WROOM_CRYPT: return "Crypt";
        case WROOM_TOWER: return "Tower";
        case WROOM_CAVE: return "Cave";
        case WROOM_RUINS: return "Ruins";
        case WROOM_SHRINE: return "Shrine";
        case WROOM_HARBOR: return "Harbor";
        case WROOM_MOUNTAIN_PASS: return "Mountain Pass";
        case WROOM_VOLCANO: return "Volcano";
        case WROOM_SKY_PEAK: return "Sky Peak";
        case WROOM_SWAMP: return "Swamp";
        case WROOM_DEMON_GATE: return "Demon Gate";
        default: return "";
    }
}

static void draw_room_name(uint8_t room)
{
    const char *name = get_room_name(room);

    clear_text_tiles(COL_DARK_BLUE,ROOM_TILE_COUNT);
    set_font(FONT_FLAMBOYANT);
    if(name[0] != '\0') {
        draw_text_opaque(4, 4, name, COL_WHITE, COL_BLUE);
    }
    render_text(ROOM_TILE, ROOM_TILE_COUNT);

    for(uint8_t i = 0; i < ROOM_TILE_COUNT; i++) {
        draw_tilemap(ROOM_TILE_X + i, 0, ROOM_TILE + i);
    }
    render_tilemap(0);
}

static const uint8_t (*get_room_map_ptr(uint8_t room))[MAP_WIDTH]
{
    switch(room) {
        case WROOM_TOWN:          return town_map;
        case WROOM_FOREST:        return forest_map;
        case WROOM_OGRE_LAIR:     return ogre_lair_map;
        case WROOM_CASTLE:        return castle_map;
        case WROOM_CRYPT:         return crypt_map;
        case WROOM_TOWER:         return tower_map;
        case WROOM_CAVE:          return cave_map;
        case WROOM_RUINS:         return ruins_map;
        case WROOM_SHRINE:        return shrine_map;
        case WROOM_HARBOR:        return harbor_map;
        case WROOM_MOUNTAIN_PASS: return mountain_pass_map;
        case WROOM_VOLCANO:       return volcano_map;
        case WROOM_SKY_PEAK:      return sky_peak_map;
        case WROOM_SWAMP:         return swamp_map;
        case WROOM_DEMON_GATE:    return demon_gate_map;
    }
    return NULL;
}

static void draw_room_map(uint8_t room)
{
    const uint8_t (*map)[MAP_WIDTH] = get_room_map_ptr(room);
    if(map) {
        for(uint8_t y = 0; y < MAP_HEIGHT; y++) {
            for(uint8_t x = 0; x < MAP_WIDTH; x++) {
                draw_tilemap(x, y, map[y][x]);
            }
        }
    }
    render_tilemap(0);
}

static void change_room(int8_t delta)
{
    int16_t next = (int16_t)current_room + delta;
    if(next < (int16_t)FIRST_ROOM) next = (int16_t)LAST_ROOM;
    else if(next > (int16_t)LAST_ROOM) next = (int16_t)FIRST_ROOM;

    current_room = (uint8_t)next;
    clear_sprites();
    draw_room_map(current_room);
    world_render_room_sprites(current_room);
    render_sprites();
    game_clear_dialog();
    draw_room_name(current_room);
}

static uint8_t find_interactable_at(uint8_t room, uint8_t tile_x, uint8_t tile_y)
{
    uint8_t obj = world.child[room];
    while(obj != WOBJ_NONE) {
        if(wobj_has_flag(obj, WFLAG_VISIBLE) && wobj_has_flag(obj, WFLAG_INTERACTABLE)) {
            // Cursor is a 2x2 tile block (32x32). Treat any object in that block as selectable.
            if(world.x[obj] >= tile_x && world.x[obj] <= (uint8_t)(tile_x + 1) &&
               world.y[obj] >= tile_y && world.y[obj] <= (uint8_t)(tile_y + 1)) {
                return obj;
            }
        }
        obj = world.sibling[obj];
    }
    return WOBJ_NONE;
}

// Rebuild the passable[] map from the room's tilemap and live object positions.
// Call whenever the active room or its objects change (room enter, battle end).
static void rebuild_passable(uint8_t room)
{
    const uint8_t (*map)[MAP_WIDTH] = get_room_map_ptr(room);
    if(!map) return;

    // Step 1: tile solidity.
    for(uint8_t y = 0; y < MAP_HEIGHT; y++) {
        for(uint8_t x = 0; x < MAP_WIDTH; x++) {
            uint8_t t = map[y][x];
            passable[y][x] = (tile_solid[t >> 3] & (1 << (t & 7))) ? 0x00 : 0xFF;
        }
    }

    // Step 2: overlay blocking objects.
    uint8_t obj = world.child[room];
    while(obj != WOBJ_NONE) {
        if(wobj_has_flag(obj, WFLAG_VISIBLE) && !wobj_has_flag(obj, WFLAG_DEFEATED)
           && world.type[obj] != WTYPE_ROOM) {
            uint8_t ox = world.x[obj];
            uint8_t oy = world.y[obj];
            if(world.sprite[obj] == TILE_COLOR_DEMONLORD) {
                // 4x4 Demon Lord
                for(uint8_t r = 0; r < 4; r++)
                    for(uint8_t c = 0; c < 4; c++)
                        passable[oy + r][ox + c] = obj;
            } else if(world.type[obj] == WTYPE_BOSS) {
                // 2x2 bosses
                for(uint8_t r = 0; r < 2; r++)
                    for(uint8_t c = 0; c < 2; c++)
                        passable[oy + r][ox + c] = obj;
            } else {
                passable[oy][ox] = obj;
            }
        }
        obj = world.sibling[obj];
    }
}

// Turn Management Constants
#define MAX_ACTIONS_PER_TURN 3
#define DISTRICTS_FOR_WIN 35  // 70% of 49 districts

void resume_game(void)
{
    set_font(FONT_FLAMBOYANT);
    game_clear_dialog();
    draw_room_map(current_room);
    rebuild_passable(current_room);
    draw_room_name(current_room);
    game.choice_target = WOBJ_NONE;
}

void init_game(void)
{
    debug_log("Game started.");
    set_font(FONT_FLAMBOYANT);

    // Initialize story/world state (Infocom-style object table)
    world_init();
    world_setup_demo();
    current_room = WROOM_TOWN;
    game.choice_target = WOBJ_NONE;

    // Set player sprite based on menu choice
    switch(game.player.type) {
        case PLAYER_TYPE_SWORDSMAN:
            world.sprite[WOBJ_PLAYER] = TILE_PC_SWORDSMAN;
            break;
        case PLAYER_TYPE_MAGE:
            world.sprite[WOBJ_PLAYER] = TILE_PC_MAGE;
            break;
        case PLAYER_TYPE_WYVERN:
            world.sprite[WOBJ_PLAYER] = TILE_PC_WYVERN;
            break;
        default:
            world.sprite[WOBJ_PLAYER] = TILE_PC_SWORDSMAN;
            break;
    }
}

// Trigger the appropriate response for bumping into or pressing A on an object.
static void interact_with_object(uint8_t obj)
{
    if(world.type[obj] == WTYPE_ENEMY || world.type[obj] == WTYPE_BOSS || obj == WDEMONLORD) {
        game.choice_target = obj;
        set_state(GAME_STATE_CHOICE);
    } else if(wobj_has_flag(obj, WFLAG_INTERACTABLE)) {
        if(!story_interact(obj, current_room, (uint8_t)game.player.type))
            game_clear_dialog();
    }
}

// Try to move the player to (nx, ny).  If the destination holds an object,
// bump-interact instead of moving.  If it's a solid tile, do nothing.
static void try_move_game(int nx, int ny)
{
    if(nx < 0 || nx >= MAP_WIDTH || ny < 0) return;
    if(ny >= MAP_HEIGHT) {
        set_state(GAME_STATE_WORLDMAP);
        return;
    }
    uint8_t cell = passable[ny][nx];
    if(cell == 0xFF) {
        cursor_x = nx;
        cursor_y = ny;
    } else if(cell != 0x00) {
        interact_with_object(cell);
    }
    // cell == 0x00: solid tile, movement silently blocked.
}

void input_game(uint8_t key, bool down)
{
    held[key] = down;
    if(!down) return;
    switch(key) {
        case INPUT_START:
            set_state(GAME_STATE_GAMEOVER);
            break;
        case INPUT_UP:
            if(dialog_phase == DIALOG_PHASE_IDLE)
                try_move_game(cursor_x, cursor_y - 1);
            break;
        case INPUT_DOWN:
            if(dialog_phase == DIALOG_PHASE_IDLE)
                try_move_game(cursor_x, cursor_y + 1);
            break;
        case INPUT_LEFT:
            if(dialog_phase == DIALOG_PHASE_IDLE)
                try_move_game(cursor_x - 1, cursor_y);
            break;
        case INPUT_RIGHT:
            if(dialog_phase == DIALOG_PHASE_IDLE)
                try_move_game(cursor_x + 1, cursor_y);
            break;
        case INPUT_SELECT:
            debug_logf("Player manually ended turn");
            end_turn();
            break;
        case INPUT_A:
        {
            if(dialog_phase != DIALOG_PHASE_IDLE) {
                // Skip / complete the typewriter instantly.
                dialog_flush();
            } else {
                // Scan the four adjacent tiles for a reachable object and interact.
                uint8_t target = WOBJ_NONE;
                for(uint8_t d = 0; d < 4 && target == WOBJ_NONE; d++) {
                    int nx = cursor_x + adj_dx[d];
                    int ny = cursor_y + adj_dy[d];
                    if(nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT) {
                        uint8_t cell = passable[ny][nx];
                        if(cell != 0x00 && cell != 0xFF)
                            target = cell;
                    }
                }
                if(target != WOBJ_NONE)
                    interact_with_object(target);
                debug_logf("Interact at (%d,%d)", cursor_x, cursor_y);
            }
            break;
        }
        default:
            break;
    }
}

bool check_win_condition(void)
{
    // TODO: Check if current player has beat the demon lord
    return false;
}

void end_turn(void)
{
    // Check for win condition before ending turn
    if(check_win_condition()) {
        set_state(GAME_STATE_GAMEOVER);
        return;
    }


    debug_logf("Turn ended. ");
}

void update_game(void)
{
    if(dialog_phase == DIALOG_PHASE_BOX) {
        // Phase 1: grow the solid background box one tile per frame.
        if(dialog_tile_idx < 20) {
            render_text_offset(DIALOG_TILE, dialog_tile_idx, 1);
            dialog_tile_idx++;
        } else {
            dialog_phase = DIALOG_PHASE_TEXT;
        }
    } else if(dialog_phase == DIALOG_PHASE_TEXT) {
        // Phase 2: reveal one character per frame, left-to-right.
        if(dialog_char_idx < dialog_total) {
            uint8_t  ci = dialog_char_idx;
            uint16_t px;
            uint8_t  py;
            char c;
            if(!dialog_newline || ci < dialog_nl) {
                c  = dialog_buf[ci];
                px = 4 + (uint16_t)ci * 8;
                py = dialog_newline ? 0 : 4;
            } else {
                uint8_t ci2 = ci - dialog_nl;
                c  = dialog_buf[dialog_nl + 1 + ci2];
                px = 4 + (uint16_t)ci2 * 8;
                py = 8;
            }
            char tmp[2] = {c, '\0'};
            draw_text_opaque(px, py, tmp, COL_WHITE, COL_BLUE);
            // Upload only the 1-2 tiles this character's pixels touch.
            uint8_t first_tile = (uint8_t)(px >> 4);
            uint8_t last_tile  = (uint8_t)((px + 7) >> 4);
            render_text_offset(DIALOG_TILE, first_tile, last_tile - first_tile + 1);
            dialog_char_idx++;
        } else {
            dialog_phase = DIALOG_PHASE_IDLE;
        }
    }
}

void draw_game(void)
{
    // Render all sprites from the current room's object list.
    reset_sprite();

    world_render_room_sprites(current_room);

    // Draw the player as a single tile sprite at the cursor position.
    add_sprite((uint16_t)cursor_x * 16, (uint8_t)(cursor_y * 16), world.sprite[WOBJ_PLAYER]);
    render_sprites();
}

// -------------------------------------------------------------------------
// World Map state
// -------------------------------------------------------------------------

static void draw_worldmap_tiles(void)
{
    // Draw background terrain.
    for(uint8_t y = 0; y < MAP_HEIGHT; y++) {
        for(uint8_t x = 0; x < MAP_WIDTH; x++) {
            draw_tilemap(x, y, world_map[y][x]);
        }
    }
    // Overlay unlocked location icons.
    for(uint8_t r = WROOM_TOWN; r <= WROOM_DEMON_GATE; r++) {
        if(wobj_has_flag(r, WFLAG_UNLOCKED)) {
            draw_tilemap(room_wm_x[r], room_wm_y[r], room_icon[r]);
        }
    }
    render_tilemap(0);
}

// Return the room whose icon tile occupies (x,y) on the world map, or WOBJ_NONE.
static uint8_t worldmap_room_at(uint8_t x, uint8_t y)
{
    for(uint8_t r = WROOM_TOWN; r <= WROOM_DEMON_GATE; r++) {
        if(wobj_has_flag(r, WFLAG_UNLOCKED) && room_wm_x[r] == x && room_wm_y[r] == y) {
            return r;
        }
    }
    return WOBJ_NONE;
}

void init_worldmap(void)
{
    // Place the player at the icon of the last-visited (or starting) room.
    if (current_room <= WROOM_DEMON_GATE) {
        worldmap_x = room_wm_x[current_room];
        worldmap_y = room_wm_y[current_room];
    } else {
        worldmap_x = room_wm_x[WROOM_TOWN];
        worldmap_y = room_wm_y[WROOM_TOWN];
    }
    set_font(FONT_FLAMBOYANT);
    draw_worldmap_tiles();
    draw_room_name(current_room);
}

void input_worldmap(uint8_t key, bool down)
{
    if(!down) return;
    switch(key) {
        case INPUT_START:
            set_state(GAME_STATE_GAMEOVER);
            break;
        case INPUT_UP:
            if(worldmap_y > 0) worldmap_y--;
            break;
        case INPUT_DOWN:
            if(worldmap_y < MAP_HEIGHT - 1) worldmap_y++;
            break;
        case INPUT_LEFT:
            if(worldmap_x > 0) worldmap_x--;
            break;
        case INPUT_RIGHT:
            if(worldmap_x < MAP_WIDTH - 1) worldmap_x++;
            break;
        case INPUT_A:
        {
            // Enter a revealed location under the player.
            uint8_t r = worldmap_room_at(worldmap_x, worldmap_y);
            if(r != WOBJ_NONE) {
                current_room = r;
                // Enter from the bottom of the location map.
                cursor_x = 9;
                cursor_y = MAP_HEIGHT - 1;
                set_state(GAME_STATE_GAME);
            }
            break;
        }
        default:
            break;
    }
    // Update room-name label whenever the player moves.
    {
        uint8_t r = worldmap_room_at(worldmap_x, worldmap_y);
        if(r != WOBJ_NONE)
            draw_room_name(r);
        else {
            // Clear the name row tiles to grass.
            fill_tilemap(TILE_OVERMAP_GRASS, 0, 0, MAP_WIDTH, 1);
            render_tilemap(0);
            // Redraw any icons that may be on row 0.
            for(uint8_t ri = WROOM_TOWN; ri <= WROOM_DEMON_GATE; ri++) {
                if(wobj_has_flag(ri, WFLAG_UNLOCKED) && room_wm_y[ri] == 0) {
                    draw_tilemap(room_wm_x[ri], 0, room_icon[ri]);
                }
            }
            render_tilemap(0);
        }
    }
}

void update_worldmap(void) {}

void draw_worldmap(void)
{
    reset_sprite();
    add_sprite((uint16_t)worldmap_x * 16, (uint8_t)(worldmap_y * 16),
               world.sprite[WOBJ_PLAYER]);
    render_sprites();
}

// -------------------------------------------------------------------------
void init_choice(void)
{
    choice_index = 0;
    last_choice_index = 255;
    choice_has_battle = false;
    if (world.type[game.choice_target] == WTYPE_BOSS || (game.choice_target >= 64 && game.choice_target <= 79)) {
        if (!wobj_has_flag(game.choice_target, WFLAG_DEFEATED)) {
            choice_has_battle = true;
        }
    }
}

void input_choice(uint8_t key, bool down)
{
    if (!down) return;
    uint8_t max_choice = choice_has_battle ? 2 : 1;

    switch(key) {
        case INPUT_UP:
            if (choice_index > 0) choice_index--;
            else choice_index = max_choice;
            break;
        case INPUT_DOWN:
            if (choice_index < max_choice) choice_index++;
            else choice_index = 0;
            break;
        case INPUT_A:
            if (choice_index == 0) { // Talk
                set_state(GAME_STATE_GAME);
                if (!story_interact(game.choice_target, current_room, (uint8_t)game.player.type)) {
                    game_clear_dialog();
                }
            } else if (choice_index == 1 && choice_has_battle) { // Battle
                set_state(GAME_STATE_BATTLE);
            } else { // Done / Cancel
                set_state(GAME_STATE_GAME);
                game_clear_dialog();
            }
            break;
        case INPUT_B:
            set_state(GAME_STATE_GAME);
            game_clear_dialog();
            break;
    }
}

void update_choice(void) {}

void draw_choice(void)
{
    draw_game();

    set_font(FONT_FLAMBOYANT);

    if(choice_index != last_choice_index) {
        clear_text_tiles(COL_BLACK, 10);
        draw_text_opaque(4, 0, (choice_index == 0) ? "> Talk  " : "  Talk  ", COL_WHITE, COL_BLUE);
        if (choice_has_battle) {
            draw_text_opaque(4, 8, (choice_index == 1) ? "> Battle" : "  Battle", COL_WHITE, COL_BLUE);
            draw_text_opaque(5*16+4, 0, (choice_index == 2) ? "> Done  " : "  Done  ", COL_WHITE, COL_BLUE);
        } else {
            draw_text_opaque(4, 8, (choice_index == 1) ? "> Done  " : "  Done  ", COL_WHITE, COL_BLUE);
        }

        render_text(DIALOG_TILE, 10);
        // Draw tiles at the bottom
        for(uint8_t i = 0; i < 5; i++) {
            draw_tilemap(2 + i, 13, DIALOG_TILE + i);
            draw_tilemap(2 + i, 14, DIALOG_TILE +5 + i);
        }
        render_tilemap(0);
    }
    last_choice_index = choice_index;
}


void init_gameover(void)
{
    for(int j=0;j<15;j++)
    {
        for(int i=0;i<20;i++)
        {
            draw_tilemap(i, j, TILE_COLOR_WHITE+(j%2)*2+(i%2));
        }
    }
    char buffer[64];
    clear_text_tiles(COL_DARK_RED, 7);
    sprintf(buffer, "Game over.");
    draw_text(4, 4, buffer, COL_RED);
    render_text(0x80, 7);
    for(int i=0;i<6;i++)
        draw_tilemap(7+i, 6, 0x80+i);
    render_tilemap(0);
    clear_sprites();
    debug_log("Game over.");
}

void input_gameover(uint8_t key, bool down)
{
    if(down && key == INPUT_START)
    {
        set_state(GAME_STATE_MENU);
    }
}

void update_gameover(void)
{
}

void draw_gameover(void)
{
    //
}
