#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"
#include "world.h"
#include "game.h"
#include "battle.h"

// Mock globals
struct World world;
struct Game game;

void debug_log(const char *msg) { printf("DEBUG: %s\n", msg); }
void set_state(enum GAME_STATE s) { printf("STATE CHANGE: %d\n", s); }
void game_show_dialog(const char *msg) { printf("DIALOG: %s\n", msg); }
void wobj_set_flag(uint8_t o, uint16_t f, bool on) { 
    if(on) world.flags[o] |= f;
    else world.flags[o] &= ~f;
    printf("FLAG SET %d on %d: %d\n", f, o, on); 
}

// Battle UI mocks
#define COL_WHITE 1
#define COL_RED 2
#define COL_YELLOW 3
#define COL_DARK_RED 4
#define DIALOG_TILE 0xB0
void clear_text_tiles(uint8_t c, uint8_t count) {}
void set_font(enum FONT_FACE f) {}
void draw_text(uint16_t x, uint8_t y, const char *t, uint8_t c) { printf("DRAW TEXT: %s at (%d,%d)\n", t, x, y); }
void draw_text_opaque(uint16_t x, uint8_t y, const char *t, uint8_t f, uint8_t b) {}
void render_text(uint8_t t, uint8_t c) {}
void draw_tilemap(uint16_t x, uint8_t y, uint8_t t) {}
void fill_tilemap(uint8_t t, uint16_t x, uint8_t y, uint8_t w, uint8_t h) {}
void render_tilemap(uint8_t l) {}

void clear_sprites() {}
void reset_sprite() {}
void add_sprite(uint16_t x, uint8_t y, uint8_t t) {}
void render_sprites() {}

// Include battle.c
#include "battle.c"

void pump_animations() {
    int timeout = 200;
    while(timeout--) {
        update_battle();
        if (p_anim_timer == 0 && e_anim_timer == 0 && !fireball_active && dmg_timer == 0 && !waiting_for_enemy) break;
    }
}

void test_battle_logic() {
    printf("Testing battle logic...\n");
    game.player.type = PLAYER_TYPE_SWORDSMAN;
    game.choice_target = 112; // WBOSS_OGRE
    world.type[112] = 10; // Not a player class
    
    init_battle();
    
    assert(p_combatant.classType == PLAYER_TYPE_SWORDSMAN);
    assert(e_combatant.objectId == 112);
    assert(player_turn == true);

    // Test Attack (Swordsman MID vs Ogre MID)
    // Swordsman MID range modifier should be -2
    // atk(5) + range(-2) - def(1) = 2 damage
    uint8_t start_hp = e_combatant.hp;
    input_battle(INPUT_A, true); 
    pump_animations();
    assert(e_combatant.hp == (start_hp - 2));
    assert(player_turn == false);

    // Run update_battle (Enemy turn)
    // Enemy at MID, Player at MID. Enemy attacks.
    // e_atk(4) + range(0) - p_def(2) = 2 damage
    uint8_t p_start_hp = p_combatant.hp;
    printf("Running update_battle (Enemy turn)...\n");
    pump_animations(); 
    assert(p_combatant.hp == (p_start_hp - 2));
    assert(player_turn == true);

    // Test Move Forward
    p_combatant.position = POS_MID;
    input_battle(INPUT_UP, true);
    pump_animations();
    assert(p_combatant.position == POS_FRONT);
    assert(player_turn == false);

    // Run update_battle (Enemy moves to match Player)
    printf("Running update_battle (Enemy moves to FRONT)...\n");
    pump_animations(); 
    assert(e_combatant.position == POS_FRONT);
    assert(player_turn == true);

    // Now both at FRONT
    // Swordsman FRONT vs Ogre FRONT -> diff 0, pos FRONT -> Range +2
    // atk(5) + range(2) - def(1) = 6 damage
    start_hp = e_combatant.hp;
    input_battle(INPUT_A, true);
    pump_animations();
    assert(e_combatant.hp == (start_hp - 6));

    printf("Battle logic basic tests passed!\n");

    // Test Wyvern Free Move
    printf("Testing Wyvern free move...\n");
    game.player.type = PLAYER_TYPE_WYVERN;
    init_battle(); 
    assert(p_combatant.position == POS_MID);
    input_battle(INPUT_UP, true); // Should be free
    pump_animations();
    assert(p_combatant.position == POS_FRONT);
    assert(player_turn == true); // Still player turn!
    input_battle(INPUT_A, true); // Now should end turn
    pump_animations();
    assert(player_turn == false);

    // Test Swordsman Counter
    printf("Testing Swordsman counter...\n");
    game.player.type = PLAYER_TYPE_SWORDSMAN;
    init_battle();
    p_combatant.position = POS_FRONT;
    e_combatant.position = POS_FRONT;
    input_battle(INPUT_X, true); // Special -> Guard stance
    pump_animations();
    assert(p_combatant.stance == STANCE_SPECIAL);
    assert(player_turn == true); // Enemy already attacked during pump
    
    // Counter should deal atk/2 = 2 damage to enemy
    // e_start_hp was 15, so should be 13
    assert(e_combatant.hp == 13);

    // Test Momentum Extra Turn
    printf("Testing Momentum extra turn...\n");
    game.player.type = PLAYER_TYPE_SWORDSMAN;
    init_battle();
    p_combatant.momentum = 6;
    input_battle(INPUT_A, true); // +2 momentum -> 8 -> Extra turn
    pump_animations();
    assert(p_combatant.momentum == 0);
    assert(player_turn == true); // Got extra turn!

    printf("All tactical battle logic tests passed!\n");
}

int main() {
    test_battle_logic();
    printf("All tests passed!\n");
    return 0;
}
