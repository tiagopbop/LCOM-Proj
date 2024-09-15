#include "sprites.h"
#include "drivers/rtc/rtc.h"

//Possible menu, instructions and game, can add more
Sprite backgrounds[4];
Sprite *current_background;
Sprite mouse_cursor;

extern vbe_mode_info_t vbe_mode_info;
extern uint8_t *back_buffer;

int load_Sprite(const xpm_map_t xpm, Sprite *sprite){
    sprite->pixels = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &sprite->img);
    if(sprite->pixels == NULL){
        printf("Error loading sprite\n");
        return 1;
    }
    return 0;
}

void free_Sprite(Sprite *sprite){
    if(sprite == NULL) return;
    else if(sprite->pixels){
        free(sprite->pixels);
        sprite->pixels = NULL;
    }
    free(sprite);
    sprite = NULL;
}

void load_backgrounds(){
    if(load_Sprite((xpm_map_t) menu_background_xpm, &backgrounds[0])) return;
    if(load_Sprite((xpm_map_t) instructions_xpm, &backgrounds[1])) return;

    RTC_Time currentTime; 
    RTC_getTime(&currentTime);

    if (currentTime.hours < 7 || currentTime.hours > 19) {
        if(load_Sprite((xpm_map_t) game_background_night_xpm, &backgrounds[2])) return;
    } else {
        if(load_Sprite((xpm_map_t) game_background_day_xpm, &backgrounds[2])) return;
    }
}

void set_background(int index){
    current_background = &backgrounds[index];
}

void restore_background() {
    if (current_background == NULL) {
        printf("Error restoring background: current_background is NULL\n");
        return;
    }
    //Since bits per pixel is 32 (the video mode is the same across the game) we know that each pixel is 4 bytes

    memcpy(back_buffer, current_background->pixels, (vbe_mode_info.XResolution * vbe_mode_info.YResolution * 4));
}

void load_mouse(){
    if(load_Sprite((xpm_map_t) mouse_cursor_xpm, &mouse_cursor)) return;
}

void draw_mouse(int x, int y){
    draw_Sprite(&mouse_cursor, x, y);
}

void draw_Sprite(const Sprite *sprite, int x, int y) {
    if (sprite == NULL || sprite->pixels == NULL) {
        printf("Error drawing sprite: sprite or sprite->pixels is NULL\n");
        return;
    }

    for (int row = 0; row < sprite->img.height; row++) {
        for (int col = 0; col < sprite->img.width; col++) {
            int draw_x = x + col;
            int draw_y = y + row;

            //Draw pixel only if it is inside boundaries
            if (draw_x >= 0 && draw_x < vbe_mode_info.XResolution && draw_y >= 0 && draw_y < vbe_mode_info.YResolution) {
                color_pixel(draw_x, draw_y, sprite->pixels[sprite->img.width * row + col]);
            }
        }
    }
}

void delete_Sprites(){
    for(int i = 0; i < 3; i++){
        free_Sprite(&backgrounds[i]);
    }
    free_Sprite(&mouse_cursor);
}
