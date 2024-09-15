#include "instructions.h"

extern struct  packet mouse_packet;
extern int mouse_x;
extern int mouse_y;
extern bool running;

typedef enum {MENU, INSTRUCTIONS, EDIT_BOARD,EDIT_BOARD_SOLO,EXPECT, ATTACK, DEFEND} MODE;
extern MODE mode;

bool over_back = false;

InstructionsOption instructions_back;

void init_instructions(){
    if(load_Sprite((xpm_map_t) instructions_back_xpm, &instructions_back.normal)) return;
    if(load_Sprite((xpm_map_t) instructions_back_highlight_xpm, &instructions_back.highlighted)) return;

    instructions_back.x = 0;
    instructions_back.y = 811;
}

void draw_instructions(){
    printf("x: %d, y: %d\n", instructions_back.x, instructions_back.y);
    if(over_back){
        draw_Sprite(&instructions_back.highlighted, instructions_back.x, instructions_back.y);
    }
    else{
        draw_Sprite(&instructions_back.normal, instructions_back.x, instructions_back.y);
    }
}

void instructionsKB(uint8_t scancode){
    switch(scancode){
        case 0x1C:
        case 0x4B:
            mode = MENU;
            set_background(0);
            break;
        default:
            break;
    }
}

void instructionsMouse(){
    if(mouse_x >= instructions_back.x && mouse_x <= instructions_back.x + instructions_back.normal.img.width && mouse_y >= instructions_back.y && mouse_y <= instructions_back.y + instructions_back.normal.img.height){
        over_back = true;
        if(mouse_packet.lb){
            mode = MENU;
            set_background(0);
        }
    }
    else{
        over_back = false;
    }
}

void delete_instructions(){
    free_Sprite(&instructions_back.normal);
    free_Sprite(&instructions_back.highlighted);
}
