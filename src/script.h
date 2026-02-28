#ifndef SCRIPT_H
#define SCRIPT_H

#include <stdint.h>

// ---- Strings ----
static const char s_str_0[] = "Hello, traveler.";
static const char s_str_1[] = "Fresh fish for sale!";
static const char s_str_2[] = "Come back anytime.";
static const char s_str_3[] = "You got a fish.";
static const char s_str_4[] = "Help me!";
static const char s_str_5[] = "Thank you for saving me.";
static const char s_str_6[] = "The ogre roars!\nIt looks hungry for adventurer steak.";
static const char s_str_7[] = "The ogre is defeated.\nHe drops a recipe book for swamp-moss reduction.";
static const char s_str_8[] = "I am a boulder.\nMove along, nothing to see here, definitely not a dragon.";
static const char s_str_9[] = "Wait, did you hear the latest?\nThe Demon Lord is buying pottery wheels from the harbor!";

static const char * const story_strings[] = {
    s_str_0,
    s_str_1,
    s_str_2,
    s_str_3,
    s_str_4,
    s_str_5,
    s_str_6,
    s_str_7,
    s_str_8,
    s_str_9,
};

// ---- Scripts ----
static const uint8_t script_WNPC_MERCHANT[] = { 7, 42, 0x20, 0x00, 4, 1, 1, 2, 42, 0x20, 0x00, 0, 1, 2, 0 };
static const uint8_t script_WITEM_FISH[] = { 1, 3, 4, 80, 32, 3, 80, 0x01, 0x00, 0 };
static const uint8_t script_WBOSS_OGRE[] = { 7, 112, 0x04, 0x00, 5, 1, 6, 2, 112, 0x04, 0x00, 3, 112, 0x01, 0x00, 0, 1, 7, 0 };
static const uint8_t script_WBOSS_LAND_DRAGON[] = { 1, 8, 0 };
static const uint8_t script_WBOSS_WIND_DRAGON[] = { 1, 9, 0 };

// ---- Scene Mapping ----
#endif // SCRIPT_H
