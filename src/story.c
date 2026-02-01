#include <stdint.h>
#include <stddef.h>

#include "main.h"
#include "story.h"
#include "world.h"

// Bytecode VM
// - Runs until it hits a SAY (shows text and stops), or END.
// - Uses world.state[obj] as the script program counter (byte index).

enum STORY_OP {
    SOP_END = 0,
    SOP_SAY = 1,          // u8 string_id
    SOP_SET_FLAG = 2,     // u8 obj, u16 flag_mask
    SOP_CLEAR_FLAG = 3,   // u8 obj, u16 flag_mask
    SOP_MOVE = 4,         // u8 obj, u8 new_parent
    SOP_SET_STATE = 5,    // u8 obj, u8 state
    SOP_GOTO = 6,         // u8 pc
    SOP_IF_FLAG_GOTO = 7, // u8 obj, u16 flag_mask, u8 pc
};

// Forward-declared UI hook implemented in game.c
extern void game_show_dialog(const char *text);

static uint16_t read_u16(const uint8_t *p)
{
    return (uint16_t)p[0] | ((uint16_t)p[1] << 8);
}

// ---- Strings ----
static const char s_hello[] = "Hello, traveler.";
static const char s_merchant_1[] = "Fresh fish for sale!";
static const char s_merchant_2[] = "Come back anytime.";
static const char s_fish_take[] = "You got a fish.";
static const char s_girl_captive[] = "Help me!";
static const char s_girl_rescued[] = "Thank you for saving me.";
static const char s_ogre_1[] = "The ogre roars!";
static const char s_ogre_2[] = "The ogre is defeated.";

static const char * const story_strings[] = {
    s_hello,        // 0
    s_merchant_1,   // 1
    s_merchant_2,   // 2
    s_fish_take,    // 3
    s_girl_captive, // 4
    s_girl_rescued, // 5
    s_ogre_1,       // 6
    s_ogre_2,       // 7
};

// ---- Scripts ----
// Notes:
// - pc values are absolute byte indices within the script array.
// - Keep scripts short and branchy; use flags for story state.

// Merchant: first talk -> line 1 + set TALKED, then next time -> line 2.
static const uint8_t script_merchant[] = {
    SOP_IF_FLAG_GOTO, WNPC_MERCHANT, (uint8_t)(WFLAG_TALKED & 0xFF), (uint8_t)(WFLAG_TALKED >> 8), 10,
    SOP_SAY, 1,
    SOP_SET_FLAG, WNPC_MERCHANT, (uint8_t)(WFLAG_TALKED & 0xFF), (uint8_t)(WFLAG_TALKED >> 8),
    SOP_END,
    // pc=10
    SOP_SAY, 2,
    SOP_END,
};

// Fish item: say + move to player + hide.
static const uint8_t script_fish[] = {
    SOP_SAY, 3,
    SOP_MOVE, WITEM_FISH, WOBJ_PLAYER,
    SOP_CLEAR_FLAG, WITEM_FISH, (uint8_t)(WFLAG_VISIBLE & 0xFF), (uint8_t)(WFLAG_VISIBLE >> 8),
    SOP_END,
};

// Little girl: if CAPTIVE -> "Help me!" else -> "Thank you..."
static const uint8_t script_little_girl[] = {
    SOP_IF_FLAG_GOTO, WNPC_LITTLE_GIRL, (uint8_t)(WFLAG_CAPTIVE & 0xFF), (uint8_t)(WFLAG_CAPTIVE >> 8), 6,
    SOP_SAY, 5,
    SOP_END,
    // pc=6
    SOP_SAY, 4,
    SOP_END,
};

// Ogre boss: if DEFEATED -> line 2 else line 1 + set defeated + hide.
static const uint8_t script_ogre[] = {
    SOP_IF_FLAG_GOTO, WBOSS_OGRE, (uint8_t)(WFLAG_DEFEATED & 0xFF), (uint8_t)(WFLAG_DEFEATED >> 8), 14,
    SOP_SAY, 6,
    SOP_SET_FLAG, WBOSS_OGRE, (uint8_t)(WFLAG_DEFEATED & 0xFF), (uint8_t)(WFLAG_DEFEATED >> 8),
    SOP_CLEAR_FLAG, WBOSS_OGRE, (uint8_t)(WFLAG_VISIBLE & 0xFF), (uint8_t)(WFLAG_VISIBLE >> 8),
    SOP_END,
    // pc=14
    SOP_SAY, 7,
    SOP_END,
};

static const uint8_t *get_script(uint8_t obj)
{
    switch(obj) {
        case WNPC_MERCHANT:
            return script_merchant;
        case WITEM_FISH:
            return script_fish;
        case WNPC_LITTLE_GIRL:
            return script_little_girl;
        case WBOSS_OGRE:
            return script_ogre;
        default:
            return 0;
    }
}

static bool story_step(uint8_t obj, const uint8_t *script)
{
    uint8_t pc = world.state[obj];

    for(;;) {
        uint8_t op = script[pc++];
        switch(op) {
            case SOP_END:
                world.state[obj] = 0;
                return false;

            case SOP_SAY: {
                uint8_t sid = script[pc++];
                game_show_dialog(story_strings[sid]);
                world.state[obj] = pc;
                return true;
            }

            case SOP_SET_FLAG: {
                uint8_t target = script[pc++];
                uint16_t mask = read_u16(&script[pc]);
                pc += 2;
                wobj_set_flag(target, mask, true);
                break;
            }

            case SOP_CLEAR_FLAG: {
                uint8_t target = script[pc++];
                uint16_t mask = read_u16(&script[pc]);
                pc += 2;
                wobj_set_flag(target, mask, false);
                break;
            }

            case SOP_MOVE: {
                uint8_t target = script[pc++];
                uint8_t dest = script[pc++];
                wobj_move(target, dest);
                break;
            }

            case SOP_SET_STATE: {
                uint8_t target = script[pc++];
                uint8_t state = script[pc++];
                world.state[target] = state;
                break;
            }

            case SOP_GOTO: {
                uint8_t new_pc = script[pc++];
                pc = new_pc;
                break;
            }

            case SOP_IF_FLAG_GOTO: {
                uint8_t target = script[pc++];
                uint16_t mask = read_u16(&script[pc]);
                pc += 2;
                uint8_t new_pc = script[pc++];
                if(wobj_has_flag(target, mask)) pc = new_pc;
                break;
            }

            default:
                // Fail safe: reset script.
                world.state[obj] = 0;
                return false;
        }
    }
}

bool story_interact(uint8_t obj)
{
    const uint8_t *script = get_script(obj);
    if(script == 0) {
        return false;
    }

    // Reset PC to start of script for each interaction
    world.state[obj] = 0;
    
    return story_step(obj, script);
}
