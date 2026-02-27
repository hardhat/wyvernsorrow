#include "main.h"
#include "menu.h"
#include "game.h"

#define TITLE_TILE 0x80
#define MENU_TILE 0x90
#define CHOICE_TILE 0xA0
#define CHOICE_TILE2 0xB0
#define CHOICE_TILE3 0xC0
#define ARROW_TILE 0xAF

#define MAX_CHOICE_TIMER 4

int choice=0;
int last_choice;
int choice_timer=MAX_CHOICE_TIMER;
int choice_color=COL_RED;   // Selected color, which rotates

void init_menu(void)
{
    char buffer[16];

    last_choice=-1;
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
    clear_text_tiles(COL_BLACK);
    draw_text(0,0,">",COL_RED);
    draw_text(0,0,"-",COL_RED);
    render_text(ARROW_TILE, 1);
    clear_text_tiles(COL_WHITE);
    draw_text(8,4,"Play as Swordsman",COL_DARK_GREEN);
    render_text(CHOICE_TILE, 9);
    clear_text_tiles(COL_WHITE);
    draw_text(8,4,"Play as Mage",COL_DARK_GREEN);
    render_text(CHOICE_TILE2, 9);
    clear_text_tiles(COL_WHITE);
    draw_text(8,4,"Play as Wyvern",COL_DARK_GREEN);
    render_text(CHOICE_TILE3, 9);
    for(int i=0;i<9;i++) {
        draw_tilemap(7+i, 9, CHOICE_TILE+i);
        draw_tilemap(7+i, 10, CHOICE_TILE2+i);
        draw_tilemap(7+i, 11, CHOICE_TILE3+i);
    }
    for(int i=0;i<16;i++) buffer[i]=TILE_COLOR_DEMONLORD+i;
    for(int i=0;i<16;i+=4) draw_tilemap_array(9,2+(i/4),(uint8_t *)buffer+i,4);
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
        last_choice=-1;
    }
}

void draw_menu(void)
{
    if(last_choice!=choice) {
        reset_sprite();
        add_sprite(7*16,choice*16+9*16+4,ARROW_TILE);
        add_sprite(6*16,choice*16+9*16,TILE_PC_SWORDSMAN+choice);
        render_sprites();
        last_choice=choice;
    }
}