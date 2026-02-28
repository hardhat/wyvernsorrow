#include <stdint.h>
#include <stddef.h>
#include <string.h>

#include "main.h"
#include "story.h"
#include "world.h"
#include "script.h"

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

static const uint8_t *get_script(uint8_t obj, uint8_t room, uint8_t player_type)
{
    // Simplified resolution: just map objects to their scripts regardless of room for now,
    // as per script.json "scripts" key.
    // In a full implementation, we'd use the "scenes" mapping.
    (void)room;
    (void)player_type;

    switch(obj) {
        case WNPC_MERCHANT: return script_WNPC_MERCHANT;
        case WITEM_FISH: return script_WITEM_FISH;
        case WBOSS_OGRE: return script_WBOSS_OGRE;
        case WBOSS_LAND_DRAGON: return script_WBOSS_LAND_DRAGON;
        case WBOSS_WIND_DRAGON: return script_WBOSS_WIND_DRAGON;
        default: return 0;
    }
}

// Simple word wrap helper for 40 column display.
// Splits text into a buffer with a single newline.
// Returns true if there is more text to show (multipart).
static bool wrap_text(const char *text, char *out_buf, uint8_t max_chars)
{
    uint8_t line1_end = 0;
    uint8_t line2_end = 0;
    uint8_t last_space = 0xFF;

    // Find the end of the first line
    for(uint8_t i = 0; i < max_chars && text[i] != '\0' && text[i] != '\n'; i++) {
        if(text[i] == ' ') last_space = i;
        line1_end = i + 1;
    }

    if(line1_end == max_chars && text[line1_end] != '\0' && text[line1_end] != ' ' && text[line1_end] != '\n' && last_space != 0xFF) {
        line1_end = last_space;
    }

    strncpy(out_buf, text, line1_end);
    out_buf[line1_end] = '\n';
    
    const char *next_line = text + line1_end;
    if(*next_line == ' ' || *next_line == '\n') next_line++;

    last_space = 0xFF;
    for(uint8_t i = 0; i < max_chars && next_line[i] != '\0' && next_line[i] != '\n'; i++) {
        if(next_line[i] == ' ') last_space = i;
        line2_end = i + 1;
    }
    
    if(line2_end == max_chars && next_line[line2_end] != '\0' && next_line[line2_end] != ' ' && next_line[line2_end] != '\n' && last_space != 0xFF) {
        line2_end = last_space;
    }

    strncpy(out_buf + line1_end + 1, next_line, line2_end);
    out_buf[line1_end + 1 + line2_end] = '\0';

    // See if there's a third line or more
    const char *excess = next_line + line2_end;
    if(*excess == ' ' || *excess == '\n') excess++;
    return *excess != '\0';
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
                const char *full_text = story_strings[sid];
                game_show_dialog(full_text);
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
                world.state[obj] = 0;
                return false;
        }
    }
}

bool story_interact(uint8_t obj, uint8_t room, uint8_t player_type)
{
    const uint8_t *script = get_script(obj, room, player_type);
    if(script == 0) {
        return false;
    }

    return story_step(obj, script);
}
