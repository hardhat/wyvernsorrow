#ifndef STORY_H
#define STORY_H

#include <stdbool.h>
#include <stdint.h>

// Minimal story scripting for interactions.
// Scripts are bytecode arrays keyed by world object id.
// The VM runs from a per-object program counter stored in world.state[obj].

bool story_interact(uint8_t obj);

#endif
