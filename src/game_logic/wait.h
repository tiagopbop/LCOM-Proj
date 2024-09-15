/**
 * @file wait.h
 * @brief Functions and structures for handling the waiting screen.
 */

#ifndef __WAIT_H
#define __WAIT_H

#include <lcom/lcf.h>
#include "../drivers/video/video.h"
#include "../drivers/keyboard_and_mouse/mouse.h"
#include "../sprites/sprites.h"

#include "../xpms/wait_options/gobackwait.xpm"
#include "../xpms/wait_options/gobackwait_highlight.xpm"
#include "../xpms/wait_screens/waiting_for_player.xpm"
#include "../drivers/serial_port/serial_port.h"

/**
 * @brief Structure representing an option in the waiting screen.
 */
typedef struct {
    int x, y; /**< Coordinates of the option */
    Sprite normal; /**< Normal sprite of the option */
    Sprite highlighted; /**< Highlighted sprite of the option */
} WaitOptions;

/**
 * @brief Initializes the waiting screen.
 */
void init_wait();

/**
 * @brief Draws the waiting screen.
 */
void draw_wait();

/**
 * @brief Handles keyboard input for the waiting screen.
 *
 * @param scancode The scancode received from the keyboard.
 */
void waitKB(uint8_t scancode);

/**
 * @brief Handles mouse input for the waiting screen.
 */
void waitMouse();

/**
 * @brief Deletes the waiting screen.
 */
void delete_wait();

#endif /* __WAIT_H */
