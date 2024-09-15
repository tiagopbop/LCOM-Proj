#include "wait.h"

extern struct  packet mouse_packet;
extern int mouse_x;
extern int mouse_y;
extern bool running;

typedef enum {MENU, INSTRUCTIONS, WAIT_PLAYER, INVITE, EDIT_BOARD,EDIT_BOARD_SOLO,EXPECT, ATTACK, DEFEND} MODE;
extern MODE mode;

bool over_wait_back = false;

WaitOptions wait_back;
WaitOptions WaitScreen;

void init_wait(){
    if(load_Sprite((xpm_map_t) waiting_for_player_xpm, &WaitScreen.normal)) return;
    if(load_Sprite((xpm_map_t) go_back_xpm, &wait_back.normal)) return;
    if(load_Sprite((xpm_map_t) go_back_highlight_xpm, &wait_back.highlighted)) return;

    WaitScreen.x = 430;
    WaitScreen.y = 370;

    wait_back.x = 480;
    wait_back.y = 500;
}

void draw_wait(){
    draw_Sprite(&WaitScreen.normal, WaitScreen.x, WaitScreen.y);
    if(over_wait_back){
        draw_Sprite(&wait_back.highlighted, wait_back.x, wait_back.y);
    }
    else{
        draw_Sprite(&wait_back.normal, wait_back.x, wait_back.y);
    }
}

void waitKB(uint8_t scancode){
    switch(scancode){
        case 0x1C:
        case 0x4B:
            mode = MENU;
            send_byte(0x04);
            tickdelay(micros_to_ticks(5));
            break;
        default:
            break;
    }
}

void waitMouse(){
    if(mouse_x >= wait_back.x && mouse_x <= wait_back.x + wait_back.normal.img.width && mouse_y >= wait_back.y && mouse_y <= wait_back.y + wait_back.normal.img.height){
        over_wait_back = true;
        if(mouse_packet.lb){
            mode = MENU;
        }
    }
    else{
        over_wait_back = false;
    }
}

void delete_wait(){
    free_Sprite(&WaitScreen.normal);
    free_Sprite(&wait_back.normal);
    free_Sprite(&wait_back.highlighted);
}
