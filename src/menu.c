#include "main.h"
#include "menu.h"
#include "game.h"

#define TITLE_TILE 0x80
#define MENU_TILE 0x90
#define CHOICE_TILE 0xA0
#define CHOICE_TILE2 0xB0

#define MAX_CHOICE_TIMER 4

int choice=0;
int choice_timer=MAX_CHOICE_TIMER;
int choice_color=COL_RED;   // Selected color, which rotates

void init_menu(void)
{
    set_font(FONT_FLAMBOYANT);
    clear_text_tiles(COL_DARK_BLUE);
    draw_text_opaque(4, 4, " Wyvern's Sorrow ", COL_WHITE, COL_BLUE);
    render_text(TITLE_TILE, 9);
    //fill_tilemap(TILE_COLOR_WHITE, 0, 0, tilemap_width, tilemap_height);
    for(int j=0;j<15;j++)
    {
        for(int i=0;i<20;i++)
        {
            draw_tilemap(i, j, TILE_COLOR_WHITE+(j%2)*2+(i%2));
        }
    }
    for(int i=0;i<9;i++) {
        draw_tilemap(7+i, 0, TITLE_TILE+i);
    }
    clear_text_tiles(COL_DARK_GREEN);
    set_font(FONT_FLAMBOYANT);
    draw_text_opaque(4, 4, " Press Start ", COL_WHITE, COL_BLUE);
    render_text(MENU_TILE, 7);
    for(int i=0;i<7;i++) {
        draw_tilemap(7+i, 7, MENU_TILE+i);
    }
    set_font(FONT_FLAMBOYANT);
    draw_text(0,0,"Play as Swordsman",COL_RED);
    draw_text(0,8,"Play as Mage",COL_LIGHT_GRAY);
    render_text(CHOICE_TILE, 9);

    clear_text_tiles(COL_DARK_GREEN);
    draw_text(0,0,"Play as Wyvern",COL_LIGHT_GRAY);
    render_text(CHOICE_TILE2, 7);
    for(int i=0;i<9;i++) {
        draw_tilemap(7+i, 9, CHOICE_TILE+i);
    }
    for(int i=0;i<9;i++) {
        draw_tilemap(7+i, 10, CHOICE_TILE2+i);
    }
    render_tilemap(0);
}

void input_menu(uint8_t key, bool down)
{
    if(down && key == INPUT_START)
    {
        if(choice==0) game.player.type=PLAYER_TYPE_SWORDSMAN;
        else if(choice==1) game.player.type=PLAYER_TYPE_MAGE;
        else game.player.type=PLAYER_TYPE_WYVERN;
        set_state(GAME_STATE_GAME);
    }
    if(down && key == INPUT_UP)
    {
        choice--;
        if(choice<0) choice=2;
    }
    if(down && key == INPUT_DOWN)
    {
        choice++;
        if(choice>2) choice=0;
    }
}

void update_menu(void)
{
    choice_timer--;
    if(choice_timer==0)
    {
        choice_timer=MAX_CHOICE_TIMER;
        choice_color=(choice_color==COL_RED)?COL_LIGHT_GRAY:COL_RED;
    }
}

void draw_menu(void)
{
    char buffer[16];
    clear_text_tiles(COL_WHITE);
    for(int i=0;i<16;i++) buffer[i]=TILE_COLOR_DEMONLORD+i;
    for(int i=0;i<16;i+=4) draw_tilemap_array(9,2+(i/4),(uint8_t *)buffer+i,4);
    set_font(FONT_FLAMBOYANT);
    if(choice<2) draw_text(0,choice*8,">",COL_RED);
    //set_font(choice==0?FONT_SPEEDWAY_BOLD:FONT_SPEEDWAY);
    draw_text(8,0,"Play as Swordsman",choice==0?choice_color:COL_DARK_RED);
    //set_font(choice==1?FONT_SPEEDWAY_BOLD:FONT_SPEEDWAY);
    draw_text(8,8,"Play as Mage",choice==1?choice_color:COL_DARK_RED);
    render_text(CHOICE_TILE, 9);
    clear_text_tiles(COL_WHITE);
    if(choice==2) draw_text(0,0,">",COL_RED);
    draw_text(8,0,"Play as Wyvern",choice==2?choice_color:COL_DARK_RED);
    render_text(CHOICE_TILE2, 9);
    render_tilemap(0);
}