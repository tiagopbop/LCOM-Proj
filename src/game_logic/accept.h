/**
 * @file accept.h
 * @brief Functions and structures for handling user acceptance of invites.
 */

#ifndef __ACCEPT_H
#define __ACCEPT_H

#include <lcom/lcf.h>
#include "../drivers/video/video.h"
#include "../drivers/keyboard_and_mouse/mouse.h"
#include "../drivers/serial_port/serial_port.h"
#include "../sprites/sprites.h"

#include "../xpms/accept_options/yes.xpm"
#include "../xpms/accept_options/yes_highlight.xpm"
#include "../xpms/accept_options/no.xpm"
#include "../xpms/accept_options/no_highlight.xpm"
#include "../xpms/wait_screens/accept_invite.xpm"

/**
 * @brief Enumeration defining the possible options for accepting invites.
 */
typedef enum { ACCEPT_INVITE, REFUSE } ACCEPT_OPTION;

/**
 * @brief Structure representing the accept options.
 */
typedef struct {
    int x, y; /**< Coordinates of the options */
    Sprite normal; /**< Normal state sprite */
    Sprite highlighted; /**< Highlighted state sprite */
} AcceptOptions;

/**
 * @brief Initializes the acceptance screen.
 */
void init_accept();

/**
 * @brief Draws the acceptance screen.
 */
void draw_accept();

/**
 * @brief Handles keyboard input for the acceptance screen.
 *
 * @param scancode The scancode received from the keyboard.
 */
void acceptKB(uint8_t scancode);

/**
 * @brief Handles mouse input for the acceptance screen.
 */
void acceptMouse();

/**
 * @brief Deletes the acceptance screen resources.
 */
void delete_accept();

/**
 * @brief Determines which option the mouse is hovering over.
 *
 * @return The option the mouse is hovering over.
 */
ACCEPT_OPTION mouse_hover_option_accept();

/**
 * @brief Selects the specified option for acceptance.
 *
 * @param option The option to select.
 */
void select_option_accept(ACCEPT_OPTION option);

/**
 * @brief Highlights the specified acceptance option.
 *
 * @param option The option to highlight.
 */
void highlight_Accept_option(ACCEPT_OPTION option);

#endif /* __ACCEPT_H */
