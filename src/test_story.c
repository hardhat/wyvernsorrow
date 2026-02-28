#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#include "main.h"
#include "world.h"
#include "story.h"

// Mock World structure (since world.c might not be linked easily in a simple test)
struct World world;

void wobj_set_flag(uint8_t obj, uint16_t flag, bool on) {
    if(on) world.flags[obj] |= flag;
    else world.flags[obj] &= ~flag;
}

bool wobj_has_flag(uint8_t obj, uint16_t flag) {
    return (world.flags[obj] & flag) != 0;
}

void wobj_move(uint8_t obj, uint8_t dest) {
    world.parent[obj] = dest;
}

// Mock game_show_dialog
char last_dialog[256];
void game_show_dialog(const char *text) {
    strncpy(last_dialog, text, 255);
    printf("DIALOG: %s\n", text);
}

// We need to include story.c directly or link it. 
// For a simple unit test in this environment, including it might be easiest if we want to avoid complex build setups.
// But story.c includes script.h, which we generated.
#include "story.c"

void test_sop_actions() {
    printf("Testing SOP actions...\n");
    
    // Reset world
    memset(&world, 0, sizeof(world));
    
    // Test Merchant Script (from script_gen.py)
    // 1. First talk: SOP_IF_FLAG_GOTO -> SAY(1) -> SET_FLAG(TALKED) -> END
    // Interaction 1: Part A (SAY)
    uint8_t obj = WNPC_MERCHANT;
    bool cont = story_interact(obj, WROOM_TOWN, 0);
    assert(cont == true);
    assert(strcmp(last_dialog, story_strings[1]) == 0); // "Fresh fish for sale!"
    
    // Interaction 1: Part B (SET_FLAG + END)
    cont = story_interact(obj, WROOM_TOWN, 0);
    assert(cont == false); // END returns false
    assert(wobj_has_flag(obj, WFLAG_TALKED) == true);
    
    // Interaction 2 (should be line 2)
    cont = story_interact(obj, WROOM_TOWN, 0);
    assert(cont == true);
    assert(strcmp(last_dialog, story_strings[2]) == 0); // "Come back anytime."
    
    // Interaction 3 (should end)
    cont = story_interact(obj, WROOM_TOWN, 0);
    assert(cont == false);

    printf("SOP actions test passed!\n");
}

void test_long_dialog_and_wrap() {
    printf("Testing long dialog and wrapping...\n");
    
    char out[128];
    const char *long_text = "This is a very long line of text that should definitely be wrapped to avoid going off the edge of the screen.";
    
    // wrap_text is static in story.c, so we can test it since we included story.c
    bool more = wrap_text(long_text, out, 40);
    printf("Wrapped: \n---\n%s\n---\n", out);
    assert(more == true);
    
    // Check line 1 length and content
    char *nl = strchr(out, '\n');
    assert(nl != NULL);
    assert((nl - out) <= 40);
    
    // Check line 2
    char *line2 = nl + 1;
    char *end = strchr(line2, '\0');
    assert((end - line2) <= 40);

    printf("Long dialog test passed!\n");
}

int main() {
    test_sop_actions();
    test_long_dialog_and_wrap();
    printf("All tests passed!\n");
    return 0;
}
