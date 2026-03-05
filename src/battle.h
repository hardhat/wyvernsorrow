#ifndef BATTLE_H
#define BATTLE_H

#include <stdint.h>

enum BATTLE_POSITION {
    POS_FRONT = 0,
    POS_MID = 1,
    POS_BACK = 2
};

enum BATTLE_STANCE {
    STANCE_DEFAULT = 0,
    STANCE_SPECIAL = 1
};

enum BATTLE_ACTION {
    BACT_ATTACK,
    BACT_SPECIAL,
    BACT_GUARD,
    BACT_MOVE_FORWARD,
    BACT_MOVE_BACK,
    BACT_MAX
};

typedef struct {
    uint8_t classType;
    uint8_t hp;
    uint8_t maxHp;
    uint8_t atk;
    uint8_t def;
    uint8_t spd;
    uint8_t position;
    uint8_t stance;
    int8_t momentum;
    uint8_t statusFlags;
    uint8_t objectId; // WORLD_OBJECT_ID
    uint8_t spriteId;
} Combatant;

void init_battle(void);
void input_battle(uint8_t key, bool down);
void update_battle(void);
void draw_battle(void);

#endif
