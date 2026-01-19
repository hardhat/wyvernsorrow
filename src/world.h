#ifndef WORLD_H
#define WORLD_H

#include <stdbool.h>
#include <stdint.h>

// Infocom-style object model (object containment + flags) adapted for a sprite/tile game.
// Everything story-relevant is an object (rooms, NPCs, bosses, items, triggers).

#define WORLD_OBJECT_COUNT 128
#define WOBJ_NONE 0xFF

// Keep IDs stable: these are save-game / script stable identifiers.
enum WORLD_OBJECT_ID {
    // Rooms / submaps (add up to ~15 as needed)
    WROOM_TOWN = 0,
    WROOM_FOREST,
    WROOM_OGRE_LAIR,
    WROOM_CASTLE,
    WROOM_CRYPT,
    WROOM_TOWER,
    WROOM_CAVE,
    WROOM_RUINS,
    WROOM_SHRINE,
    WROOM_HARBOR,
    WROOM_MOUNTAIN_PASS,
    WROOM_VOLCANO,
    WROOM_SKY_PEAK,
    WROOM_SWAMP,
    WROOM_DEMON_GATE,

    // Actors / story objects
    WOBJ_PLAYER = 32,

    // NPCs
    WNPC_LITTLE_GIRL,
    WNPC_MOTHER,
    WNPC_FATHER,
    WNPC_CARPENTER,
    WNPC_OLD_MAN,
    WNPC_KING,
    WNPC_MERCHANT,

    // Items (example)
    WITEM_FISH = 64,

    // Bosses / major enemies
    WBOSS_OGRE = 80,

    // Demon Lord (major boss)
    WDEMONLORD = 96,
};

enum WORLD_OBJECT_TYPE {
    WTYPE_ROOM = 0,
    WTYPE_ACTOR,
    WTYPE_ITEM,
    WTYPE_BOSS,
    WTYPE_TRIGGER,
};

enum WORLD_FLAGS {
    WFLAG_VISIBLE       = 0x0001,
    WFLAG_INTERACTABLE  = 0x0002,
    WFLAG_DEFEATED      = 0x0004,
    WFLAG_RESCUED       = 0x0008,
    WFLAG_CAPTIVE       = 0x0010,
    WFLAG_TALKED        = 0x0020,
};

struct World {
    // Containment links (object tree)
    uint8_t parent[WORLD_OBJECT_COUNT];
    uint8_t child[WORLD_OBJECT_COUNT];
    uint8_t sibling[WORLD_OBJECT_COUNT];

    // Story/behavior state
    uint16_t flags[WORLD_OBJECT_COUNT];
    uint8_t type[WORLD_OBJECT_COUNT];
    uint8_t state[WORLD_OBJECT_COUNT];

    // Visual placement (tile coordinates within a submap)
    uint8_t x[WORLD_OBJECT_COUNT];
    uint8_t y[WORLD_OBJECT_COUNT];
    uint8_t sprite[WORLD_OBJECT_COUNT];
};

extern struct World world;

void world_init(void);

// Configure a small demo setup (town with a merchant and a fish).
// This is a pattern to expand into full story scripting.
void world_setup_demo(void);

// Object containment helpers
void wobj_remove_from_parent(uint8_t obj);
void wobj_add_child(uint8_t parent, uint8_t obj);
void wobj_move(uint8_t obj, uint8_t new_parent);

// Flag helpers
// Implemented in world.c (not inline) to avoid SDCC "unreachable code" warnings
// triggered by some inline patterns in headers.
bool wobj_has_flag(uint8_t obj, uint16_t flag);
void wobj_set_flag(uint8_t obj, uint16_t flag, bool on);

// Rendering helper: spawns sprites for visible objects whose parent is the room.
// Uses each object's (x,y) tile coordinates and sprite tile id.
void world_render_room_sprites(uint8_t room);

#endif
