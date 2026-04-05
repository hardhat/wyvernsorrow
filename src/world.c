#include <string.h>

#include "main.h"
#include "world.h"

struct World world;

static void wobj_clear(uint8_t obj)
{
    world.parent[obj] = WOBJ_NONE;
    world.child[obj] = WOBJ_NONE;
    world.sibling[obj] = WOBJ_NONE;
    world.flags[obj] = 0;
    world.type[obj] = 0;
    world.state[obj] = 0;
    world.x[obj] = 0;
    world.y[obj] = 0;
    world.sprite[obj] = 0;
}

void world_init(void)
{
    for(uint16_t i = 0; i < WORLD_OBJECT_COUNT; i++) {
        wobj_clear((uint8_t)i);
        // By convention, objects 0-31 are rooms.
        if (i < 32) {
            world.type[i] = WTYPE_ROOM;
        }
    }
}

void wobj_remove_from_parent(uint8_t obj)
{
    uint8_t p = world.parent[obj];
    if(p == WOBJ_NONE) return;

    uint8_t cur = world.child[p];
    uint8_t prev = WOBJ_NONE;

    while(cur != WOBJ_NONE) {
        if(cur == obj) {
            if(prev == WOBJ_NONE) world.child[p] = world.sibling[cur];
            else world.sibling[prev] = world.sibling[cur];
            break;
        }
        prev = cur;
        cur = world.sibling[cur];
    }

    world.parent[obj] = WOBJ_NONE;
    world.sibling[obj] = WOBJ_NONE;
}

void wobj_add_child(uint8_t parent, uint8_t obj)
{
    // Ensure the object isn't in another container.
    wobj_remove_from_parent(obj);

    world.parent[obj] = parent;

    // Prepend to parent's child list.
    world.sibling[obj] = world.child[parent];
    world.child[parent] = obj;
}

void wobj_move(uint8_t obj, uint8_t new_parent)
{
    wobj_add_child(new_parent, obj);
}

bool wobj_has_flag(uint8_t obj, uint16_t flag)
{
    return (world.flags[obj] & flag) != 0;
}

void wobj_set_flag(uint8_t obj, uint16_t flag, bool on)
{
    if(on) world.flags[obj] |= flag;
    else world.flags[obj] &= (uint16_t)~flag;
}

void world_setup_demo(void)
{
    // The first 32 objects are rooms (set in world_init).
    // Town and Forest are always unlocked; all other rooms revealed via unlock chain.
    wobj_set_flag(WROOM_TOWN,   WFLAG_UNLOCKED, true);
    wobj_set_flag(WROOM_FOREST, WFLAG_UNLOCKED, true);

    // Player object exists for story/state purposes (inventory, flags, etc.)
    wobj_clear(WOBJ_PLAYER);
    world.type[WOBJ_PLAYER] = WTYPE_ACTOR;
    world.flags[WOBJ_PLAYER] = WFLAG_VISIBLE;
    world.sprite[WOBJ_PLAYER] = TILE_PC_SWORDSMAN;

#include "world_entities.h"
}

void world_render_room_sprites(uint8_t room)
{
    uint8_t obj = world.child[room];
    uint8_t x, y, sprite;
    const uint8_t stride = 10; // Row stride for boss/demonlord IDs

    while(obj != WOBJ_NONE) {
        if(wobj_has_flag(obj, WFLAG_VISIBLE)) {
            x = world.x[obj];
            y = world.y[obj];
            sprite = world.sprite[obj];

            if (world.type[obj] == WTYPE_BOSS) {
                if (sprite == TILE_COLOR_DEMONLORD) {
                    // 4x4 Demon Lord
                    for (uint8_t r = 0; r < 4; r++) {
                        for (uint8_t c = 0; c < 4; c++) {
                            add_sprite((uint16_t)(x + c) * 16, (y + r) * 16, sprite + (r * 4) + c);
                        }
                    }
                } else {
                    // 2x2 Bosses (Ogre, Dragons, etc.)
                    for (uint8_t r = 0; r < 2; r++) {
                        for (uint8_t c = 0; c < 2; c++) {
                            add_sprite((uint16_t)(x + c) * 16, (y + r) * 16, sprite + (r * stride) + c);
                        }
                    }
                }
            } else {
                // 1x1 standard actor/item
                add_sprite((uint16_t)x * 16, y * 16, sprite);
            }
        }
        obj = world.sibling[obj];
    }
}

// Character progression tracks: order each player type visits rooms.
// Defeating the boss/enemy in track[i] reveals track[i+1] on the world map.
static const uint8_t track_swordsman[14] = {
    WROOM_TOWN, WROOM_FOREST, WROOM_OGRE_LAIR, WROOM_CASTLE,
    WROOM_MOUNTAIN_PASS, WROOM_CAVE, WROOM_HARBOR, WROOM_RUINS,
    WROOM_SWAMP, WROOM_SHRINE, WROOM_TOWER, WROOM_VOLCANO,
    WROOM_SKY_PEAK, WROOM_DEMON_GATE
};
static const uint8_t track_mage[14] = {
    WROOM_TOWN, WROOM_FOREST, WROOM_CRYPT, WROOM_TOWER,
    WROOM_SHRINE, WROOM_RUINS, WROOM_CAVE, WROOM_HARBOR,
    WROOM_SWAMP, WROOM_CASTLE, WROOM_MOUNTAIN_PASS, WROOM_SKY_PEAK,
    WROOM_VOLCANO, WROOM_DEMON_GATE
};
static const uint8_t track_wyvern[14] = {
    WROOM_TOWN, WROOM_FOREST, WROOM_SKY_PEAK, WROOM_VOLCANO,
    WROOM_MOUNTAIN_PASS, WROOM_HARBOR, WROOM_CAVE, WROOM_SWAMP,
    WROOM_RUINS, WROOM_SHRINE, WROOM_TOWER, WROOM_CRYPT,
    WROOM_CASTLE, WROOM_DEMON_GATE
};

void world_unlock_next(uint8_t defeated_enemy_obj, uint8_t player_type)
{
    uint8_t defeated_room = world.parent[defeated_enemy_obj];
    if (defeated_room == WOBJ_NONE) return;

    const uint8_t *track;
    switch (player_type) {
        case 0:  track = track_swordsman; break;
        case 1:  track = track_mage;      break;
        case 2:  track = track_wyvern;    break;
        default: return;
    }

    for (uint8_t i = 0; i < 13; i++) {
        if (track[i] == defeated_room) {
            wobj_set_flag(track[i + 1], WFLAG_UNLOCKED, true);
            return;
        }
    }
}
