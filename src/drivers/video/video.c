#include "video.h"

uint8_t *video_mem;
vbe_mode_info_t vbe_mode_info;
uint8_t *back_buffer;
unsigned int bytesPerPixel;

int (set_video_mode)(uint16_t mode) {
    reg86_t reg86;

    memset(&reg86, 0, sizeof(reg86_t)); /* zero the structure */

    reg86.intno = 0x10;
    reg86.ah = 0x4F;
    reg86.al = 0x02;

    reg86.bx = mode | BIT(14);

    if( sys_int86(&reg86) != OK ) {
        printf("vg_exit(): sys_int86() failed \n");
        return 1;
    }

    return 0;
}

int (map_vram)(uint16_t mode){
    memset(&vbe_mode_info, 0, sizeof(vbe_mode_info_t));
    if(vbe_get_mode_info(mode, &vbe_mode_info)) return 1;

    struct minix_mem_range mr;
    unsigned int vram_base = vbe_mode_info.PhysBasePtr; 
    unsigned int vram_size;

    if(vbe_mode_info.BitsPerPixel % 8 == 0) bytesPerPixel = vbe_mode_info.BitsPerPixel / 8;
    else bytesPerPixel = (vbe_mode_info.BitsPerPixel / 8) + 1;

    vram_size = vbe_mode_info.XResolution * vbe_mode_info.YResolution * bytesPerPixel;

    int r;

    mr.mr_base = (phys_bytes) vram_base;
    mr.mr_limit = mr.mr_base + vram_size;

    if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr))){
        panic("sys_privctl (ADD_MEM) failed: %d\n", r);
        return 1;
    }

    video_mem = vm_map_phys(SELF, (void *)mr.mr_base, vram_size);

    if(video_mem == MAP_FAILED){
        panic("couldn't map video memory");
        return 1;
    }
    return 0;
}

int color_pixel(uint16_t x, uint16_t y, uint32_t color) {
    if (x >= vbe_mode_info.XResolution || y >= vbe_mode_info.YResolution || x < 0 || y < 0) {
        printf("Invalid coordinates: (%u, %u)\n", x, y);
        return 0;
    }

    if(color == 0xFF000000) return 0;
    
    //Since bits per pixel is 32 (the video mode is the same across the game) we know that each pixel is 4 bytes
    memcpy(&back_buffer[(vbe_mode_info.XResolution * y + x) * 4], &color, 4);
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    for(int col = 0; col < len; col++){
        if(color_pixel(x + col, y, color)) {
            printf("Error in color_pixel\n");
            return 1;
        }
    }
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    for(int row = 0; row < height; row++){
        if(vg_draw_hline(x, y + row, width, color)) {
            printf("Error in vg_draw_hline\n");
            vg_exit();
            return 1;
        }
    }
    return 0;
}

uint32_t (direct_color)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << vbe_mode_info.RedFieldPosition) | (G << vbe_mode_info.GreenFieldPosition) | (B << vbe_mode_info.BlueFieldPosition);
}

uint32_t (indexed_color)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
  return (first + (row * n + col) * step) % (1 << vbe_mode_info.BitsPerPixel);
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t to_draw;
    uint32_t *color_by_index = (uint32_t *) xpm_load(xpm, XPM_8_8_8_8, &to_draw);

    for(int row = 0; row < to_draw.height; row++){
        for(int col = 0; col < to_draw.width; col++){
            uint32_t index = to_draw.width * row + col;
            if(color_pixel(x + row, y + col, color_by_index[index])){
                printf("Error in color_pixel\n");
                return 1;
            }
        }
    }
    return 0;
}

void init_back_buffer(){
    back_buffer = (uint8_t *) malloc(vbe_mode_info.XResolution * vbe_mode_info.YResolution * bytesPerPixel);
}

void swap_buffer(){
    memcpy(video_mem, back_buffer, vbe_mode_info.XResolution * vbe_mode_info.YResolution * bytesPerPixel);
}
