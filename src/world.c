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
    // Rooms
    wobj_clear(WROOM_TOWN);
    world.type[WROOM_TOWN] = WTYPE_ROOM;

    wobj_clear(WROOM_FOREST);
    world.type[WROOM_FOREST] = WTYPE_ROOM;

    wobj_clear(WROOM_OGRE_LAIR);
    world.type[WROOM_OGRE_LAIR] = WTYPE_ROOM;

    // Player object exists for story/state purposes (inventory, flags, etc.)
    wobj_clear(WOBJ_PLAYER);
    world.type[WOBJ_PLAYER] = WTYPE_ACTOR;
    world.flags[WOBJ_PLAYER] = WFLAG_VISIBLE;
    world.sprite[WOBJ_PLAYER] = TILE_PC_SWORDSMAN;

    // Merchant in town
    wobj_clear(WNPC_MERCHANT);
    world.type[WNPC_MERCHANT] = WTYPE_ACTOR;
    world.flags[WNPC_MERCHANT] = WFLAG_VISIBLE | WFLAG_INTERACTABLE;
    world.sprite[WNPC_MERCHANT] = TILE_NPC_MERCHANT;
    world.x[WNPC_MERCHANT] = 10;
    world.y[WNPC_MERCHANT] = 6;
    wobj_add_child(WROOM_TOWN, WNPC_MERCHANT);

    // A fish item near the merchant
    wobj_clear(WITEM_FISH);
    world.type[WITEM_FISH] = WTYPE_ITEM;
    world.flags[WITEM_FISH] = WFLAG_VISIBLE | WFLAG_INTERACTABLE;
    world.sprite[WITEM_FISH] = TILE_ITEM_FISH;
    world.x[WITEM_FISH] = 12;
    world.y[WITEM_FISH] = 6;
    wobj_add_child(WROOM_TOWN, WITEM_FISH);

    // A simple boss in the ogre lair (2x2 sprite: you'll likely want multi-sprite later)
    wobj_clear(WBOSS_OGRE);
    world.type[WBOSS_OGRE] = WTYPE_BOSS;
    world.flags[WBOSS_OGRE] = WFLAG_VISIBLE | WFLAG_INTERACTABLE;
    world.sprite[WBOSS_OGRE] = TILE_BOSS_OGRE;
    world.x[WBOSS_OGRE] = 9;
    world.y[WBOSS_OGRE] = 6;
    wobj_add_child(WROOM_OGRE_LAIR, WBOSS_OGRE);

    // Captive girl in the forest (story hook)
    wobj_clear(WNPC_LITTLE_GIRL);
    world.type[WNPC_LITTLE_GIRL] = WTYPE_ACTOR;
    world.flags[WNPC_LITTLE_GIRL] = WFLAG_VISIBLE | WFLAG_INTERACTABLE | WFLAG_CAPTIVE;
    world.sprite[WNPC_LITTLE_GIRL] = TILE_NPC_LITTLE_GIRL;
    world.x[WNPC_LITTLE_GIRL] = 6;
    world.y[WNPC_LITTLE_GIRL] = 6;
    wobj_add_child(WROOM_FOREST, WNPC_LITTLE_GIRL);
}

void world_render_room_sprites(uint8_t room)
{
    uint8_t obj = world.child[room];

    while(obj != WOBJ_NONE) {
        if(wobj_has_flag(obj, WFLAG_VISIBLE)) {
            // Convert tile coords to pixels. (Sprites are 16x16.)
            add_sprite((uint16_t)world.x[obj] * 16, world.y[obj] * 16, world.sprite[obj]);

            // If you later add 2x2 bosses, you can emit 4 sprites here.
        }
        obj = world.sibling[obj];
    }
}
