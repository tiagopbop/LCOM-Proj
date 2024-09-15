/**
 * @file sprites.h
 * @brief Functions and structures for handling sprites.
 */

#ifndef _SPRITES_H_
#define _SPRITES_H_

#include <lcom/lcf.h>
#include "../drivers/video/video.h"

#include "../xpms/backgrounds/menu_background.xpm"
#include "../xpms/backgrounds/instructions.xpm"
#include "../xpms/backgrounds/game_background_day.xpm"
#include "../xpms/backgrounds/game_background_night.xpm"

#include "../xpms/mouse/mouse_cursor.xpm"

/**
 * @brief Structure representing a sprite.
 */
typedef struct {
  xpm_image_t img; /**< XPM image */
  uint32_t *pixels; /**< Array of pixel values */
} Sprite;

/**
 * @brief Loads a sprite from an XPM file.
 *
 * @param xpm XPM image to load.
 * @param sprite Pointer to the sprite structure to store the loaded sprite.
 * @return 0 on success, non-zero otherwise.
 */
int load_Sprite(const xpm_map_t xpm, Sprite *sprite);

/**
 * @brief Frees the memory used by a sprite.
 *
 * @param sprite Pointer to the sprite structure.
 */
void free_Sprite(Sprite *sprite);

/**
 * @brief Loads the background images.
 */
void load_backgrounds();

/**
 * @brief Sets the background image to be displayed.
 *
 * @param index Index of the background image.
 */
void set_background(int index);

/**
 * @brief Restores the previous background image.
 */
void restore_background();

/**
 * @brief Loads the mouse cursor image.
 */
void load_mouse();

/**
 * @brief Draws the mouse cursor at the specified coordinates.
 *
 * @param x X-coordinate of the mouse cursor.
 * @param y Y-coordinate of the mouse cursor.
 */
void draw_mouse(int x, int y);

/**
 * @brief Draws a sprite at the specified coordinates.
 *
 * @param sprite Pointer to the sprite structure.
 * @param x X-coordinate where the sprite will be drawn.
 * @param y Y-coordinate where the sprite will be drawn.
 */
void draw_Sprite(const Sprite *sprite, int x, int y);

/**
 * @brief Deletes all loaded sprites.
 */
void delete_Sprites();

#endif /* _SPRITES_H_ */
