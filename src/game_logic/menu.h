/**
 * @file menu.h
 * @brief Functions and structures for handling the game menu.
 */

#ifndef __MENU_H
#define __MENU_H

#include <lcom/lcf.h>
#include "../drivers/video/video.h"
#include "../drivers/keyboard_and_mouse/mouse.h"
#include "../drivers/serial_port/serial_port.h"
#include "../sprites/sprites.h"

#include "../xpms/menu_options/start.xpm"
#include "../xpms/menu_options/inst.xpm"
#include "../xpms/menu_options/exit.xpm"
#include "../xpms/menu_options/start_highlight.xpm"
#include "../xpms/menu_options/inst_highlight.xpm"
#include "../xpms/menu_options/exit_highlight.xpm"

/**
 * @brief Role identifiers for game players.
 */
#define INVITER 1 /**< Inviter player role */
#define INVITEE 2 /**< Invitee player role */

/**
 * @brief Enumeration representing the menu options.
 */
typedef enum {
    START_GAME, /**< Start game option */
    ENTER_INSTRUCTIONS, /**< Enter instructions option */
    EXIT_GAME /**< Exit game option */
} MENU_OPTION;

/**
 * @brief Structure representing an option in the game menu.
 */
typedef struct {
    int x, y; /**< Coordinates of the option */
    Sprite normal; /**< Normal sprite of the option */
    Sprite highlighted; /**< Highlighted sprite of the option */
} MenuOption;

/**
 * @brief Initializes the game menu.
 */
void init_menu();

/**
 * @brief Draws the game menu.
 */
void draw_menu();

/**
 * @brief Handles keyboard input for the game menu.
 *
 * @param scancode The scancode received from the keyboard.
 */
void menuKB(uint8_t scancode);

/**
 * @brief Handles mouse input for the game menu.
 */
void menuMouse();

/**
 * @brief Determines which menu option the mouse is hovering over.
 *
 * @return The menu option currently hovered by the mouse.
 */
MENU_OPTION mouse_hover_option();

/**
 * @brief Selects a menu option.
 *
 * @param option The menu option to be selected.
 */
void select_option(MENU_OPTION option);

/**
 * @brief Highlights a menu option.
 *
 * @param option The menu option to be highlighted.
 */
void highlight_option(MENU_OPTION option);

/**
 * @brief Deletes the game menu.
 */
void delete_menu();

#endif /* __MENU_H */
