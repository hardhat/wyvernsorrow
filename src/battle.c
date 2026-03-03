#include "battle.h"
#include "game.h"
#include "main.h"
#include "world.h"
#include <stdio.h>
#include <string.h>

#define DIALOG_TILE 0xB0

static Combatant p_combatant;
static Combatant e_combatant;
static bool player_turn = true;
static bool extra_turn_pending = false;
static bool wyvern_free_move_used = false;

void init_battle(void)
{
    // Initialize player combatant
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

    // Initialize enemy combatant based on choice_target
    e_combatant.objectId = game.choice_target;
    e_combatant.classType = 0xFF; // Neutral/Monster
    e_combatant.hp = 15;
    e_combatant.maxHp = 15;
    e_combatant.atk = 4;
    e_combatant.def = 1;
    e_combatant.spd = 3;
    e_combatant.position = POS_MID;
    e_combatant.stance = STANCE_DEFAULT;
    e_combatant.momentum = 0;
    e_combatant.statusFlags = 0;

    player_turn = (p_combatant.spd >= e_combatant.spd);
    extra_turn_pending = false;
    wyvern_free_move_used = false;
    debug_log("Battle initialized.");
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

static void do_attack(Combatant* a, Combatant* d)
{
    int damage = (int)a->atk + range_modifier(a, d) + stance_modifier(a) - (int)d->def;
    if (damage < 1) damage = 1;
    
    // Check for counter-attack: Swordsman in Guard stance at FRONT
    if (d->classType == PLAYER_TYPE_SWORDSMAN && d->stance == STANCE_SPECIAL && d->position == POS_FRONT) {
        int counter = (int)d->atk / 2;
        if (a->hp > (uint8_t)counter) a->hp -= (uint8_t)counter;
        else a->hp = 0;
        apply_momentum(d, 1);
        game_show_dialog("Counter!");
    }

    if (d->hp > (uint8_t)damage) d->hp -= (uint8_t)damage;
    else d->hp = 0;
    
    apply_momentum(a, 2);

    // Reset Mage stance after attack
    if (a->classType == PLAYER_TYPE_MAGE) a->stance = STANCE_DEFAULT;
}

static void do_special(Combatant* a, Combatant* d)
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
            }
            break;
    }
}

void input_battle(uint8_t key, bool down)
{
    if (!down || !player_turn) return;

    bool action_taken = false;

    switch(key) {
        case INPUT_A: // Attack
            do_attack(&p_combatant, &e_combatant);
            action_taken = true;
            break;
        case INPUT_X: // Special
            do_special(&p_combatant, &e_combatant);
            action_taken = true;
            break;
        case INPUT_UP: // Move Forward
            if (p_combatant.position > POS_FRONT) {
                p_combatant.position--;
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
        if (p_combatant.momentum >= 8) {
            p_combatant.momentum = 0;
            game_show_dialog("Extra Turn!");
            wyvern_free_move_used = false; // Reset free move
        } else {
            player_turn = false;
        }
    }
}

void update_battle(void)
{
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

    if (!player_turn) {
        // Simple Enemy AI
        if (e_combatant.position != p_combatant.position) {
            if (e_combatant.position > p_combatant.position) e_combatant.position--;
            else e_combatant.position++;
        } else {
            do_attack(&e_combatant, &p_combatant);
        }
        player_turn = true;
        wyvern_free_move_used = false;
    }
}

void draw_battle(void)
{
    char buffer[64];
    clear_text_tiles(COL_BLACK);
    
    sprintf(buffer, "HP:%d POS:%d MOM:%d", p_combatant.hp, p_combatant.position, p_combatant.momentum);
    draw_text(4, 0, buffer, COL_WHITE);
    if (p_combatant.stance == STANCE_SPECIAL) draw_text(160, 0, "STANCE", COL_YELLOW);
    
    sprintf(buffer, "ENM HP:%d POS:%d", e_combatant.hp, e_combatant.position);
    draw_text(4, 8, buffer, COL_RED);

    if (player_turn) {
        draw_text(4, 20, "A:ATK X:SPC UP/DN:MOV B:END", COL_YELLOW);
    } else {
        draw_text(4, 20, "ENEMY TURN...", COL_WHITE);
    }

    render_text(DIALOG_TILE, 20);
    for(uint8_t i = 0; i < 20; i++) {
        draw_tilemap(2 + i, 13, DIALOG_TILE + i);
    }
    render_tilemap(0);
}
