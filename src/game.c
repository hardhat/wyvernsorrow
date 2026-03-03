
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
static bool choice_has_battle = false;

#define FIRST_ROOM WROOM_TOWN
#define LAST_ROOM WROOM_DEMON_GATE

#define DIALOG_TILE 0xB0
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

// Called by the story VM.
void game_show_dialog(const char *text)
{
    draw_dialog_text(text);
}

static void game_clear_dialog(void)
{
    // Remove the dialog tiles from the tilemap.
    for(uint8_t i = 0; i < 16; i++) {
        draw_tilemap(2 + i, 14, TILE_OVERMAP_GRASS);
    }
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

static void draw_room_map(uint8_t room)
{
    const uint8_t (*map)[MAP_WIDTH] = NULL;
    switch(room) {
        case WROOM_TOWN: map = town_map; break;
        case WROOM_FOREST: map = forest_map; break;
        case WROOM_OGRE_LAIR: map = ogre_lair_map; break;
        case WROOM_CASTLE: map = castle_map; break;
        case WROOM_CRYPT: map = crypt_map; break;
        case WROOM_TOWER: map = tower_map; break;
        case WROOM_CAVE: map = cave_map; break;
        case WROOM_RUINS: map = ruins_map; break;
        case WROOM_SHRINE: map = shrine_map; break;
        case WROOM_HARBOR: map = harbor_map; break;
        case WROOM_MOUNTAIN_PASS: map = mountain_pass_map; break;
        case WROOM_VOLCANO: map = volcano_map; break;
        case WROOM_SKY_PEAK: map = sky_peak_map; break;
        case WROOM_SWAMP: map = swamp_map; break;
        case WROOM_DEMON_GATE: map = demon_gate_map; break;
    }
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

// Turn Management Constants
#define MAX_ACTIONS_PER_TURN 3
#define DISTRICTS_FOR_WIN 35  // 70% of 49 districts

void init_game(void)
{
    debug_log("Game started.");
    set_font(FONT_FLAMBOYANT);

    // Initialize story/world state (Infocom-style object table)
    world_init();
    world_setup_demo();
    current_room = WROOM_TOWN;
    draw_room_map(current_room);
    draw_room_name(current_room);
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

void input_game(uint8_t key, bool down)
{
    held[key] = down;
    if(!down) return;
    switch(key) {
        case INPUT_START:
            set_state(GAME_STATE_GAMEOVER);
            break;
        case INPUT_X:
            change_room(1);
            break;
        case INPUT_Y:
            change_room(-1);
            break;
        case INPUT_UP:
            cursor_y--;
            if(cursor_y<0) cursor_y=14;
            break;
        case INPUT_DOWN:
            cursor_y++;
            if(cursor_y>14) cursor_y=0;
            break;
        case INPUT_LEFT:
            cursor_x--;
            if(cursor_x<0) cursor_x=19;
            break;
        case INPUT_RIGHT:
            cursor_x++;
            if(cursor_x>19) cursor_x=0;
            break;
        case INPUT_SELECT:
            // Allow player to manually end their turn
            debug_logf("Player manually ended turn");
            end_turn();
            break;
        case INPUT_A:
        {
            // Interact with the object under the cursor.
            uint8_t tile_x = (uint8_t)(cursor_x);
            uint8_t tile_y = (uint8_t)(cursor_y);
            uint8_t target = find_interactable_at(current_room, tile_x, tile_y);
            if(target != WOBJ_NONE) {
                // If it's an enemy, boss, or the demon lord, show the choice menu.
                if (world.type[target] == WTYPE_ENEMY || world.type[target] == WTYPE_BOSS || target == WDEMONLORD) {
                    game.choice_target = target;
                    set_state(GAME_STATE_CHOICE);
                } else {
                    if(!story_interact(target, current_room, (uint8_t)game.player.type)) {
                        // Script ended or had nothing to say; clear the dialog.
                        game_clear_dialog();
                    }
                }
            } else {
                game_show_dialog("Nothing here.");
            }
            debug_logf("Interact at (%d,%d)", cursor_x, cursor_y);
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
    // TODO: implement game update logic
    
}

void draw_game(void)
{
    // Render all sprites from the current room's object list.
    reset_sprite();

    world_render_room_sprites(current_room);

    // Draw the cursor as 4 sprites (1x1 tiles).
    add_sprite(cursor_x*16, cursor_y*16, TILE_CURSOR);
    add_sprite(cursor_x*16+16, cursor_y*16, TILE_CURSOR+1);
    add_sprite(cursor_x*16, cursor_y*16+16, TILE_CURSOR+2);
    add_sprite(cursor_x*16+16, cursor_y*16+16, TILE_CURSOR+3);
    render_sprites();
}

void init_choice(void)
{
    choice_index = 0;
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

    clear_text_tiles(COL_BLACK, 20);
    set_font(FONT_FLAMBOYANT);
    
    draw_text_opaque(4, 0, (choice_index == 0) ? "> Talk" : "  Talk", COL_WHITE, COL_BLUE);
    if (choice_has_battle) {
        draw_text_opaque(4, 8, (choice_index == 1) ? "> Battle" : "  Battle", COL_WHITE, COL_BLUE);
        draw_text_opaque(4, 16, (choice_index == 2) ? "> Done" : "  Done", COL_WHITE, COL_BLUE);
    } else {
        draw_text_opaque(4, 8, (choice_index == 1) ? "> Done" : "  Done", COL_WHITE, COL_BLUE);
    }

    render_text(DIALOG_TILE, 20);
    // Draw tiles at the bottom
    for(uint8_t i = 0; i < 20; i++) {
        draw_tilemap(2 + i, 13, DIALOG_TILE + i);
    }
    render_tilemap(0);
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
