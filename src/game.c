
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "main.h"
#include "game.h"
#include "world.h"
#include "story.h"

struct Game game;

int cursor_x=0;
int cursor_y=0;
bool held[MAX_INPUT];

static uint8_t current_room = WROOM_TOWN;

#define DIALOG_TILE 0xB0

static void draw_dialog_text(const char *text)
{
    // Draw up to two lines split on '\n' (optional).
    const char *nl = NULL;
    for(const char *p = text; *p != 0; p++) {
        if(*p == '\n') { nl = p; break; }
    }

    clear_text_tiles(COL_DARK_BLUE);
    set_font(FONT_FLAMBOYANT);
    if(nl == NULL) {
        draw_text_opaque(4, 4, text, COL_WHITE, COL_BLUE);
    } else {
        // Render first line
        char line1[33];
        uint8_t len = (uint8_t)(nl - text);
        if(len > 32) len = 32;
        for(uint8_t i = 0; i < len; i++) line1[i] = text[i];
        line1[len] = 0;
        draw_text_opaque(4, 0, line1, COL_WHITE, COL_BLUE);
        // Render second line
        draw_text_opaque(4, 8, nl + 1, COL_WHITE, COL_BLUE);
    }
    render_text(DIALOG_TILE, 16);

    // Place the 16 text tiles at the bottom of the screen.
    for(uint8_t i = 0; i < 16; i++) {
        draw_tilemap(2 + i, 14, DIALOG_TILE + i);
    }
    render_tilemap(0);
}

// Called by the story VM.
void game_show_dialog(const char *text)
{
    draw_dialog_text(text);
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
    // Simple backdrop for now; the story/world layer sits on top via sprites.
    fill_tilemap(TILE_OVERMAP_GRASS, 0, 0, tilemap_width, tilemap_height);
    render_tilemap(0);
    log("Game started.");
    set_font(FONT_FLAMBOYANT);

    // Initialize story/world state (Infocom-style object table)
    world_init();
    world_setup_demo();
    current_room = WROOM_TOWN;

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
        case INPUT_UP:
            cursor_y--;
            if(cursor_y<0) cursor_y=6;
            break;
        case INPUT_DOWN:
            cursor_y++;
            if(cursor_y>6) cursor_y=0;
            break;
        case INPUT_LEFT:
            cursor_x--;
            if(cursor_x<0) cursor_x=6;
            break;
        case INPUT_RIGHT:
            cursor_x++;
            if(cursor_x>6) cursor_x=0;
            break;
        case INPUT_SELECT:
            // Allow player to manually end their turn
            logf("Player manually ended turn");
            end_turn();
            break;
        case INPUT_A:
        {
            // Interact with the object under the cursor.
            uint8_t tile_x = (uint8_t)(cursor_x * 2);
            uint8_t tile_y = (uint8_t)(cursor_y * 2);
            uint8_t target = find_interactable_at(current_room, tile_x, tile_y);
            if(target != WOBJ_NONE) {
                if(!story_interact(target)) {
                    // Script ran but had nothing to say; show a tiny default.
                    game_show_dialog("...");
                }
            } else {
                game_show_dialog("Nothing here.");
            }
            logf("Interact at (%d,%d)", cursor_x, cursor_y);
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


    logf("Turn ended. ");
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

    // Draw the cursor as 4 sprites (2x2 tiles).
    add_sprite(cursor_x*32, cursor_y*32, TILE_CURSOR);
    add_sprite(cursor_x*32+16, cursor_y*32, TILE_CURSOR+1);
    add_sprite(cursor_x*32, cursor_y*32+16, TILE_CURSOR+2);
    add_sprite(cursor_x*32+16, cursor_y*32+16, TILE_CURSOR+3);
    render_sprites();

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
    clear_text_tiles(COL_DARK_RED);
    sprintf(buffer, "Game over.");
    draw_text(4, 4, buffer, COL_RED);
    render_text(0x80, 7);
    for(int i=0;i<6;i++)
        draw_tilemap(7+i, 6, 0x80+i);
    render_tilemap(0);
    clear_sprites();
    log("Game over.");
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
