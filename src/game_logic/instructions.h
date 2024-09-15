/**
 * @file instructions.h
 * @brief Functions and structures for handling game instructions.
 */

#ifndef __INSTRUCTIONS_H
#define __INSTRUCTIONS_H

#include <lcom/lcf.h>
#include "../drivers/video/video.h"
#include "../drivers/keyboard_and_mouse/mouse.h"
#include "../sprites/sprites.h"

#include "../xpms/instructions_options/back.xpm"
#include "../xpms/instructions_options/back_highlight.xpm"

/**
 * @brief Structure representing an option in the instructions screen.
 */
typedef struct {
    int x, y; /**< Coordinates of the option */
    Sprite normal; /**< Normal sprite of the option */
    Sprite highlighted; /**< Highlighted sprite of the option */
} InstructionsOption;

/**
 * @brief Initializes the instructions screen.
 */
void init_instructions();

/**
 * @brief Draws the instructions screen.
 */
void draw_instructions();

/**
 * @brief Handles keyboard input for the instructions screen.
 *
 * @param scancode The scancode received from the keyboard.
 */
void instructionsKB(uint8_t scancode);

/**
 * @brief Handles mouse input for the instructions screen.
 */
void instructionsMouse();

/**
 * @brief Deletes the instructions screen.
 */
void delete_instructions();

#endif /* __INSTRUCTIONS_H */
