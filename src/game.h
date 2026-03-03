#ifndef GAME_H
#define GAME_H

#include<stdbool.h>
#include<stdint.h>


#define PLAYER_TYPE_SWORDSMAN 0
#define PLAYER_TYPE_MAGE 1
#define PLAYER_TYPE_WYVERN 2

struct Player
{
    uint16_t x;
    uint16_t y;

    uint8_t type;
};

struct Game
{
    struct Player player;
    uint8_t choice_target;  // Object ID of the chosen target to battle
};
extern struct Game game;

void init_game(void);
void input_game(uint8_t key, bool down);
void update_game(void);
void draw_game(void);
void end_turn(void);
bool check_win_condition(void);
void game_show_dialog(const char* text);

void init_gameover(void);
void input_gameover(uint8_t key, bool down);
void update_gameover(void);
void draw_gameover(void);

void init_choice(void);
void input_choice(uint8_t key, bool down);
void update_choice(void);
void draw_choice(void);

void init_battle(void);
void input_battle(uint8_t key, bool down);
void update_battle(void);
void draw_battle(void);

#endif