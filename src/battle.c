#include "battle.h"
#include "game.h"
#include "main.h"
#include "world.h"
#include <stdio.h>
#include <string.h>

#define BATTLE_TILE 0x80
#define BATTLE_TILE2 0x90

static Combatant p_combatant;
static Combatant e_combatant;
static bool player_turn = true;
static bool extra_turn_pending = false;
static bool wyvern_free_move_used = false;
static bool stats_updated = false;

// Animation state
static int16_t p_vis_offY = 0;
static int16_t e_vis_offY = 0;
static int p_anim_timer = 0;
static int e_anim_timer = 0;
static uint8_t anim_type = 0; // 0=none, 1=move, 2=attack

static int16_t dmg_offY = 0;
static int dmg_timer = 0;
static int dmg_val = 0;
static int last_dmg_val = -1;
static bool dmg_is_enemy = false;

static bool fireball_active = false;
static int16_t fireball_x = 0;
static int16_t fireball_y = 0;
static int fireball_timer = 0;

static bool waiting_for_enemy = false;
static int enemy_wait_timer = 0;

void init_battle(void)
{
    // Initialize player combatant
    p_combatant.objectId = WOBJ_PLAYER;
    p_combatant.classType = game.player.type;
    p_combatant.hp = 20;
    p_combatant.maxHp = 20;
    p_combatant.atk = 5;
    p_combatant.def = 2;
    p_combatant.spd = 5;
    p_combatant.position = POS_MID;
    p_combatant.stance = STANCE_DEFAULT;
    p_combatant.momentum = 0;
    p_combatant.statusFlags = 0;
    p_combatant.spriteId = game.player.type==PLAYER_TYPE_SWORDSMAN?TILE_PC_SWORDSMAN:game.player.type==PLAYER_TYPE_MAGE?TILE_PC_MAGE:TILE_PC_WYVERN;

    // Initialize enemy combatant based on choice_target
    e_combatant.objectId = game.choice_target;
    e_combatant.classType = world.type[game.choice_target]; // Neutral/Monster
    e_combatant.hp = 15;
    e_combatant.maxHp = 15;
    e_combatant.atk = 4;
    e_combatant.def = 1;
    e_combatant.spd = 3;
    e_combatant.position = POS_MID;
    e_combatant.stance = STANCE_DEFAULT;
    e_combatant.momentum = 0;
    e_combatant.statusFlags = 0;
    e_combatant.spriteId = world.sprite[game.choice_target]; 

    player_turn = (p_combatant.spd >= e_combatant.spd);
    extra_turn_pending = false;
    wyvern_free_move_used = false;

    p_vis_offY = 0;
    e_vis_offY = 0;
    p_anim_timer = 0;
    e_anim_timer = 0;
    anim_type = 0;
    dmg_timer = 0;
    fireball_active = false;
    waiting_for_enemy = false;
    enemy_wait_timer = 0;

    // Draw the battle background
    fill_tilemap(TILE_OVERMAP_GRASS, 0, 0, 20, 12);
    fill_tilemap(TILE_WOOD_FLOOR, 6, 1, 8, 8);
    fill_tilemap(COL_BLACK, 0, 12, 20, 3);  // Clear the bottom of the screen

    clear_sprites();

    debug_log("Battle initialized.");
    stats_updated = true;   // Initialize the stats display
}

static int range_modifier(Combatant* a, Combatant* d)
{
    int diff = (int)a->position - (int)d->position;
    if (diff < 0) diff = -diff;

    if (a->classType == PLAYER_TYPE_SWORDSMAN) {
        if (diff == 0 && a->position == POS_FRONT) return 2;
        return -2;
    }
    if (a->classType == PLAYER_TYPE_MAGE) {
        if (a->position == POS_BACK) return 2;
        if (a->position == POS_FRONT) return -3;
    }
    if (a->classType == PLAYER_TYPE_WYVERN) {
        return (int)a->spd / 2;
    }
    return 0;
}

static int stance_modifier(Combatant* a)
{
    if (a->classType == PLAYER_TYPE_SWORDSMAN) {
        if (a->stance == STANCE_SPECIAL) return -2; // Guard stance: lower damage
    }
    if (a->classType == PLAYER_TYPE_MAGE) {
        if (a->stance == STANCE_SPECIAL) return 4; // Channel: next attack stronger
    }
    if (a->classType == PLAYER_TYPE_WYVERN) {
        if (a->stance == STANCE_SPECIAL) return -2; // Soar: lower damage
    }
    return 0;
}

static void apply_momentum(Combatant* c, int8_t delta)
{
    int val = (int)c->momentum + delta;
    if (val > 10) val = 10;
    if (val < -10) val = -10;
    c->momentum = (int8_t)val;
}

static void trigger_damage(int val, bool is_enemy)
{
    dmg_val = val;
    dmg_timer = 30; // 0.5s
    dmg_offY = 0;
    dmg_is_enemy = is_enemy;
    stats_updated = true;
}

static void trigger_fireball(int16_t x, int16_t y, bool to_enemy)
{
    fireball_active = true;
    fireball_timer = 20;
    fireball_x = x;
    fireball_y = y;
}

static void do_attack_logical(Combatant* a, Combatant* d)
{
    int damage = (int)a->atk + range_modifier(a, d) + stance_modifier(a) - (int)d->def;
    if (damage < 1) damage = 1;

    // Trigger fireball for mage
    if (a->classType == PLAYER_TYPE_MAGE) {
        trigger_fireball(152, 96 + (d->position<<4), true);
    }
    
    // Check for counter-attack: Swordsman in Guard stance at FRONT
    if (d->classType == PLAYER_TYPE_SWORDSMAN && d->stance == STANCE_SPECIAL && d->position == POS_FRONT) {
        int counter = (int)d->atk / 2;
        if (a->hp > (uint8_t)counter) a->hp -= (uint8_t)counter;
        else a->hp = 0;
        apply_momentum(d, 1);
        game_show_dialog("Counter!");
        trigger_damage(counter, (a == &p_combatant));
    }

    if (d->hp > (uint8_t)damage) d->hp -= (uint8_t)damage;
    else d->hp = 0;
    
    apply_momentum(a, 2);
    trigger_damage(damage, (d == &e_combatant));

    // Reset Mage stance after attack
    if (a->classType == PLAYER_TYPE_MAGE) a->stance = STANCE_DEFAULT;
}

static void do_special_logical(Combatant* a, Combatant* d)
{
    switch (a->classType) {
        case PLAYER_TYPE_SWORDSMAN:
            a->stance = STANCE_SPECIAL; // Guard
            apply_momentum(a, 1);
            game_show_dialog("Guarding...");
            break;
        case PLAYER_TYPE_MAGE:
            a->stance = STANCE_SPECIAL; // Channel
            apply_momentum(a, 1);
            game_show_dialog("Channelling...");
            break;
        case PLAYER_TYPE_WYVERN:
            // Dive: high damage but forced FRONT
            {
                int damage = (int)a->atk + 4;
                if (d->hp > (uint8_t)damage) d->hp -= (uint8_t)damage;
                else d->hp = 0;
                a->position = POS_FRONT;
                apply_momentum(a, 3);
                game_show_dialog("Dive!");
                trigger_damage(damage, (d == &e_combatant));
            }
            break;
    }
}

void input_battle(uint8_t key, bool down)
{
    if (!down || !player_turn || p_anim_timer > 0 || e_anim_timer > 0 || waiting_for_enemy) return;

    bool action_taken = false;

    switch(key) {
        case INPUT_A: // Attack
            p_anim_timer = 16;
            anim_type = 2; // Attack lunge
            action_taken = true;
            break;
        case INPUT_X: // Special
            if (p_combatant.classType == PLAYER_TYPE_WYVERN) {
                p_anim_timer = 16;
                anim_type = 2; // Dive lunge
            } else {
                do_special_logical(&p_combatant, &e_combatant);
            }
            action_taken = true;
            break;
        case INPUT_UP: // Move Forward
            if (p_combatant.position > POS_FRONT) {
                p_combatant.position--;
                p_vis_offY = 16; // Start offset
                p_anim_timer = 16;
                anim_type = 1;
                if (p_combatant.classType == PLAYER_TYPE_WYVERN && !wyvern_free_move_used) {
                    wyvern_free_move_used = true;
                    apply_momentum(&p_combatant, 1);
                } else {
                    action_taken = true;
                }
            }
            break;
        case INPUT_DOWN: // Move Back
            if (p_combatant.position < POS_BACK) {
                p_combatant.position++;
                p_vis_offY = -16; // Start offset
                p_anim_timer = 16;
                anim_type = 1;
                if (p_combatant.classType == PLAYER_TYPE_WYVERN && !wyvern_free_move_used) {
                    wyvern_free_move_used = true;
                    apply_momentum(&p_combatant, 1);
                } else {
                    action_taken = true;
                }
            }
            break;
        case INPUT_B: // End turn
            action_taken = true;
            break;
    }

    if (action_taken) {
        player_turn = false;
        stats_updated = true;
    }
}

void update_battle(void)
{
    if (p_anim_timer > 0) {
        if (anim_type == 1) { // Move
            if (p_vis_offY > 0) p_vis_offY--;
            else if(p_vis_offY < 0) p_vis_offY++;
        } else if (anim_type == 2) { // Attack
            if (p_anim_timer > 8) p_vis_offY--; // Lunge
            else p_vis_offY++; // Return
            
            if (p_anim_timer == 8) {
                if (anim_type == 2) {
                    do_attack_logical(&p_combatant, &e_combatant);
                    // Check for Extra Turn
                    if (p_combatant.momentum >= 8) {
                        p_combatant.momentum = 0;
                        game_show_dialog("Extra Turn!");
                        player_turn = true;
                        wyvern_free_move_used = false;
                    }
                }
            }
        }
        p_anim_timer--;
        if (p_anim_timer == 0 && !player_turn) stats_updated = true;
        return; 
    }

    if (e_anim_timer > 0) {
        if (anim_type == 1) { // Move
            if (e_vis_offY > 0) e_vis_offY--;
            else if(e_vis_offY < 0) e_vis_offY++;
        } else if (anim_type == 2) { // Attack
            if (e_anim_timer > 8) e_vis_offY++; // Lunge down
            else e_vis_offY--; // Return
            
            if (e_anim_timer == 8) {
                do_attack_logical(&e_combatant, &p_combatant);
            }
        }
        e_anim_timer--;
        if (e_anim_timer == 0) {
            player_turn = true;
            stats_updated = true;
        }
        return;
    }

    if (fireball_active) {
        if (fireball_timer > 0) {
            fireball_timer--;
            fireball_y -= 2;
            return;
        }
        fireball_active = false;
    }

    if (dmg_timer > 0) {
        dmg_timer--;
        dmg_offY--; // Drift up
        return;
    }

    if (e_combatant.hp == 0) {
        wobj_set_flag(e_combatant.objectId, WFLAG_DEFEATED, true);
        set_state(GAME_STATE_GAME);
        game_show_dialog("Victory!");
        return;
    }
    if (p_combatant.hp == 0) {
        set_state(GAME_STATE_GAMEOVER);
        return;
    }

    if (!player_turn && !waiting_for_enemy) {
        waiting_for_enemy = true;
        enemy_wait_timer = 30; 
    }

    if (waiting_for_enemy) {
        if (enemy_wait_timer > 0) {
            enemy_wait_timer--;
        } else {
            waiting_for_enemy = false;
            // Enemy AI
            if (e_combatant.position != p_combatant.position) {
                if (e_combatant.position > p_combatant.position) {
                    e_combatant.position--;
                    e_vis_offY = -16;
                } else {
                    e_combatant.position++;
                    e_vis_offY = 16;
                }
                e_anim_timer = 16;
                anim_type = 1;
            } else {
                e_anim_timer = 16;
                anim_type = 2; // Attack
            }
        }
    }
}

void draw_battle(void)
{
    char buffer[64];

    if(stats_updated) {
        clear_text_tiles(COL_BLACK, 12);

        sprintf(buffer, "HP:%d POS:%d MOM:%d", p_combatant.hp, p_combatant.position, p_combatant.momentum);
        draw_text(4, 0, buffer, COL_WHITE);

        sprintf(buffer, "ENM HP:%d POS:%d", e_combatant.hp, e_combatant.position);
        draw_text(4, 8, buffer, COL_RED);
        render_text(BATTLE_TILE, 12);

        clear_text_tiles(COL_BLACK, 15);
        if (p_combatant.stance == STANCE_SPECIAL) draw_text(32, 0, "STANCE", COL_YELLOW);
        if (player_turn) {
            draw_text(4, 8, "A:ATK X:SPC UP/DN:MOV B:END", COL_YELLOW);
        } else {
            draw_text(4, 8, "ENEMY TURN...", COL_WHITE);
        }
        render_text(BATTLE_TILE2, 15);

        for(uint8_t i = 0; i < 15; i++) {
            if(i<12) draw_tilemap(2 + i, 12, BATTLE_TILE + i);
            draw_tilemap(2 + i, 13, BATTLE_TILE2 + i);
        }
        render_tilemap(0);

        stats_updated = false;
    }
    // Draw sprites
    reset_sprite();
    // Draw player sprite   
    add_sprite(152, 96 + (p_combatant.position<<4) + p_vis_offY, p_combatant.spriteId);
    
    // Draw enemy sprite
    uint8_t size=1;
    uint8_t stride=1;
    if(e_combatant.objectId==WDEMONLORD) {
        size=4;
        stride=1;
    } else if(e_combatant.classType==WTYPE_BOSS) {
        size=2;
        stride=8;
    }
    uint8_t tile=e_combatant.spriteId;
    uint16_t y = 72 - (size<<4) - (p_combatant.position<<4) + e_vis_offY;
    for(uint8_t i=0;i<size;i++) {
        uint16_t x = 152;
        if(size==4) x-=32;
        else if(size==2) x-=16;
        for(uint8_t j=0;j<size;j++) {
            add_sprite(x, y, tile);
            x+=16;
            tile++;
        }
        y+=16;
        tile+=stride;
    }

    // Draw Damage Float
    if (dmg_timer > 0) {
	if(last_dmg_val!=dmg_val) {
		last_dmg_val = dmg_val;
		sprintf(buffer, "-%d", dmg_val);
		// Use a temporary text tile for the damage sprite
		clear_text_tiles(COL_BLACK, 2);
		draw_text(0, 4, buffer, COL_RED);
		render_text(0xD0, 2); // Use tiles 0xD0-0xD1 for damage
	}
        
        uint16_t dX = 152;
        uint16_t dY = dmg_is_enemy ? (72 - (p_combatant.position<<4) - 32) : (96 + (p_combatant.position<<4));
        add_sprite(dX, dY + dmg_offY, 0xD0);
        add_sprite(dX + 16, dY + dmg_offY, 0xD1);
    }

    // Draw fireball
    if (fireball_active) {
        add_sprite(fireball_x, fireball_y, TILE_FIREBALL);
    }

    render_sprites();
}
