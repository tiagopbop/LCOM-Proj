#include "menu.h"

static MENU_OPTION highlighted_option = START_GAME;

extern struct packet mouse_packet;
extern int mouse_x;
extern int mouse_y;
extern bool running;
typedef enum {MENU, INSTRUCTIONS, WAIT_PLAYER, INVITE, EDIT_BOARD,EDIT_BOARD_SOLO,EXPECT, ATTACK, DEFEND} MODE;
extern MODE mode;
extern int player_role;
MenuOption menu_options[3];




void init_menu(){
    if(load_Sprite((xpm_map_t) menu_start_xpm, &menu_options[0].normal)) return;
    if(load_Sprite((xpm_map_t) menu_start_highlight_xpm, &menu_options[0].highlighted)) return;
    if(load_Sprite((xpm_map_t) menu_instructions_xpm, &menu_options[1].normal)) return;
    if(load_Sprite((xpm_map_t) menu_instructions_highlight_xpm, &menu_options[1].highlighted)) return;
    if(load_Sprite((xpm_map_t) menu_exit_xpm, &menu_options[2].normal)) return;
    if(load_Sprite((xpm_map_t) menu_exit_highlight_xpm, &menu_options[2].highlighted)) return;

    menu_options[0].x = 457;
    menu_options[0].y = 375;
    menu_options[1].x = 484;
    menu_options[1].y = 450;
    menu_options[2].x = 492;
    menu_options[2].y = 525;

}

void draw_menu(){
    for(int i = 0; i < 3; i++){
        if(i == highlighted_option) {
            draw_Sprite(&menu_options[i].highlighted, menu_options[i].x, menu_options[i].y);
        }
        else{
            draw_Sprite(&menu_options[i].normal, menu_options[i].x, menu_options[i].y);
        }
    }

}

void menuKB(uint8_t scancode){
    switch(scancode){
        case 0x48:
            if(highlighted_option == START_GAME) highlighted_option = EXIT_GAME;
            else highlighted_option--;
            break;
        case 0x50:
            if(highlighted_option == EXIT_GAME) highlighted_option = START_GAME;
            else highlighted_option++;
            break;
        case 0x1C:
            select_option(highlighted_option);
            break;
        default:
            break;
    }
}

void menuMouse(){
    MENU_OPTION option = mouse_hover_option();

    if(option != highlighted_option) highlight_option(option);

    if(mouse_packet.lb && mouse_x >= menu_options[highlighted_option].x && mouse_x <= menu_options[highlighted_option].x + menu_options[highlighted_option].normal.img.width && mouse_y >= menu_options[highlighted_option].y && mouse_y <= menu_options[highlighted_option].y + menu_options[highlighted_option].normal.img.height){
        select_option(option);
    }
}

MENU_OPTION mouse_hover_option(){
    for(int i = 0; i < 3; i++){
        if(mouse_x >= menu_options[i].x && mouse_x <= menu_options[i].x + menu_options[i].normal.img.width){
            if(mouse_y >= menu_options[i].y && mouse_y <= menu_options[i].y + menu_options[i].normal.img.height){
                return (MENU_OPTION) i;
            }
        }
    }
    return highlighted_option;
}

void select_option(MENU_OPTION option){
    switch(option){
        case START_GAME:
            mode = WAIT_PLAYER;
            player_role = INVITER;
            send_byte(0x01);
            tickdelay(micros_to_ticks(5));
            break;
        case ENTER_INSTRUCTIONS:
            mode = INSTRUCTIONS;
            set_background(1);
            break;
        case EXIT_GAME:
            running = false;
            break;
        default:
            break;
    }
}

void highlight_option(MENU_OPTION option){
    highlighted_option = option;
}

void delete_menu(){
    for(int i = 0; i < 3; i++){
        free_Sprite(&menu_options[i].normal);
        free_Sprite(&menu_options[i].highlighted);
    }
}
