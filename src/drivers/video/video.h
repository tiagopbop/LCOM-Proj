/**
 * @file video.h
 * @brief Functions for handling video mode setting, pixel drawing, and buffer swapping.
 */

#ifndef _LCOM_VIDEO_H
#define _LCOM_VIDEO_H

#include <lcom/lcf.h>

/**
 * @brief Sets the video mode to the specified mode.
 *
 * @param mode The video mode to set.
 * @return 0 on success, non-zero otherwise.
 */
int (set_video_mode)(uint16_t mode);

/**
 * @brief Maps the video memory to the specified mode.
 *
 * @param mode The video mode to map the video memory.
 * @return 0 on success, non-zero otherwise.
 */
int (map_vram)(uint16_t mode);

/**
 * @brief Colors the pixel at the specified position with the given color.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color to set the pixel.
 * @return 0 on success, non-zero otherwise.
 */
int (color_pixel)(uint16_t x, uint16_t y, uint32_t color);

/**
 * @brief Draws a horizontal line at the specified position with the given length and color.
 *
 * @param x The x-coordinate of the starting point of the line.
 * @param y The y-coordinate of the starting point of the line.
 * @param len The length of the line.
 * @param color The color of the line.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a rectangle at the specified position with the given width, height, and color.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle.
 * @param height The height of the rectangle.
 * @param color The color of the rectangle.
 * @return 0 on success, non-zero otherwise.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief Creates a direct color from the specified RGB components.
 *
 * @param R The red component.
 * @param G The green component.
 * @param B The blue component.
 * @return The direct color.
 */
uint32_t (direct_color)(uint32_t R, uint32_t G, uint32_t B);

/**
 * @brief Creates an indexed color based on the specified parameters.
 *
 * @param row The row index.
 * @param col The column index.
 * @param step The color step.
 * @param first The first color.
 * @param n The number of colors.
 * @return The indexed color.
 */
uint32_t (indexed_color)(uint16_t row, uint16_t col, uint8_t step, uint32_t first, uint8_t n);

/**
 * @brief Prints an XPM image at the specified position.
 *
 * @param xpm The XPM image to print.
 * @param x The x-coordinate of the top-left corner of the image.
 * @param y The y-coordinate of the top-left corner of the image.
 * @return 0 on success, non-zero otherwise.
 */
int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y);

/**
 * @brief Initializes the back buffer for double buffering.
 */
void init_back_buffer();

/**
 * @brief Swaps the front and back buffers for double buffering.
 */
void swap_buffer();

#endif /* _LCOM_VIDEO_H */
