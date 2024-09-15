#include "accept.h"

extern struct  packet mouse_packet;
extern int mouse_x;
extern int mouse_y;
extern bool running;
static ACCEPT_OPTION highlighted_option = ACCEPT_INVITE;

typedef enum {MENU, INSTRUCTIONS, WAIT_PLAYER, INVITE, EDIT_BOARD,EDIT_BOARD_SOLO,EXPECT, ATTACK, DEFEND} MODE;
extern MODE mode;

AcceptOptions accept_back[2];
AcceptOptions AcceptScreen;

void init_accept(){
    if(load_Sprite((xpm_map_t) accept_invite_xpm, &AcceptScreen.normal)) return;
    if(load_Sprite((xpm_map_t) yes_xpm, &accept_back[0].normal)) return;
    if(load_Sprite((xpm_map_t) yes_highlight_xpm, &accept_back[0].highlighted)) return;
    if(load_Sprite((xpm_map_t) no_xpm, &accept_back[1].normal)) return;
    if(load_Sprite((xpm_map_t) no_highlight_xpm, &accept_back[1].highlighted)) return;

    AcceptScreen.x = 430;
    AcceptScreen.y = 370;
    accept_back[0].x = 480;
    accept_back[0].y = 530;
    accept_back[1].x = 620;
    accept_back[1].y = 530;
}

void draw_accept(){
    draw_Sprite(&AcceptScreen.normal, AcceptScreen.x, AcceptScreen.y);
    for(int i = 0; i < 2; i++){
        if(i == highlighted_option) {
            draw_Sprite(&accept_back[i].highlighted, accept_back[i].x, accept_back[i].y);
        }
        else{
            draw_Sprite(&accept_back[i].normal, accept_back[i].x, accept_back[i].y);
        }
    }
}

void acceptKB(uint8_t scancode){
    switch(scancode){
        case 0x4B:
            if(highlighted_option == ACCEPT_INVITE) highlighted_option = REFUSE;
            else highlighted_option--;
            break;
        case 0x4D:
            if(highlighted_option == REFUSE) highlighted_option = ACCEPT_INVITE;
            else highlighted_option++;
            break;
        case 0x1C:
            select_option_accept(highlighted_option);
            break;
        default:
            break;
    }
}

void acceptMouse(){
    ACCEPT_OPTION option = mouse_hover_option_accept();

    if(option != highlighted_option) highlight_Accept_option(option);

    if(mouse_packet.lb && mouse_x >= accept_back[highlighted_option].x && mouse_x <= accept_back[highlighted_option].x + accept_back[highlighted_option].normal.img.width && mouse_y >= accept_back[highlighted_option].y && mouse_y <= accept_back[highlighted_option].y + accept_back[highlighted_option].normal.img.height){
        select_option_accept(option);
    }
}

ACCEPT_OPTION mouse_hover_option_accept(){
    for(int i = 0; i < 2; i++){
        if(mouse_x >= accept_back[i].x && mouse_x <= accept_back[i].x + accept_back[i].normal.img.width){
            if(mouse_y >= accept_back[i].y && mouse_y <= accept_back[i].y + accept_back[i].normal.img.height){
                return (ACCEPT_OPTION) i;
            }
        }
    }
    return highlighted_option;
}

void select_option_accept(ACCEPT_OPTION option){
    switch(option){
        case ACCEPT_INVITE:
        mode = EDIT_BOARD;
        set_background(2);
        send_byte(0x02); // Send accept to the other player
            break;
        case REFUSE:
            mode = MENU;
            send_byte(0x03);
            break;
        default:
            break;
    }
}

void highlight_Accept_option(ACCEPT_OPTION option){
    highlighted_option = option;
}

void delete_accept(){
    free_Sprite(&AcceptScreen.normal);
    for(int i = 0; i < 2; i++){
        free_Sprite(&accept_back[i].normal);
        free_Sprite(&accept_back[i].highlighted);
    }
}
