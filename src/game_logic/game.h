/**
 * @file game.h
 * @brief Functions and structures for handling game logic.
 */

#ifndef _GAME_H
#define _GAME_H

#include <lcom/lcf.h>
#include "../drivers/video/video.h"
#include "../drivers/keyboard_and_mouse/mouse.h"
#include "../sprites/sprites.h"
#include "../drivers/serial_port/serial_port.h"

#include "../xpms/ships/ship1.xpm"
#include "../xpms/ships/ship2_h.xpm"
#include "../xpms/ships/ship2_v.xpm"
#include "../xpms/ships/ship3_h.xpm"
#include "../xpms/ships/ship3_v.xpm"
#include "../xpms/hit_or_miss/hit.xpm"
#include "../xpms/hit_or_miss/miss.xpm"

/**
 * @brief Enumeration defining the orientation of a ship.
 */
typedef enum { HORIZONTAL, VERTICAL } SHIP_ORIENTATION;

/**
 * @brief Enumeration defining the state of a cell in the game grid.
 */
typedef enum { EMPTY, SHIP_HERE, HIT, MISS } CELL_STATE;

/**
 * @brief Enumeration defining the type of ship.
 */
typedef enum { SHIP1, SHIP2, SHIP3 } SHIP_TYPE;

/**
 * @brief Structure representing a ship.
 */
typedef struct {
    int x, y; /**< Coordinates of the ship */
    int initial_x, initial_y; /**< Initial coordinates of the ship */
    SHIP_ORIENTATION orientation; /**< Orientation of the ship */
    Sprite horizontal_ship; /**< Sprite representing the ship in horizontal orientation */
    Sprite vertical_ship; /**< Sprite representing the ship in vertical orientation */
    int length; /**< Length of the ship */
} SHIP;

/**
 * @brief Structure representing a cell in the game grid.
 */
typedef struct {
    int top_left_x, top_left_y; /**< Coordinates of the top-left corner of the cell */
    CELL_STATE cell_state; /**< State of the cell */
} GRID_CELL;

/**
 * @brief Structure representing the game grid.
 */
typedef struct {
    GRID_CELL cells[5][5]; /**< Cells of the grid */
} GRID;

/**
 * @brief Enumeration defining the game mode.
 */
typedef enum { GAME_EDIT_BOARD, GAME_EXPECT,GAME_ATTACK, GAME_DEFEND } GAMEMODE;

/**
 * @brief Enumeration defining the game state.
 */
typedef enum { GAMESTART, GAMEWAIT, GAMEPLAY, GAMEEND } GAMESTATE;

/**
 * @brief Structure representing the game.
 */
typedef struct {
    GAMEMODE player_mode; /**< Mode of the player */
    GAMESTATE player_state; /**< State of the player */
    GRID player_grid; /**< Grid of the player */
    GRID enemy_grid; /**< Grid of the enemy */
    SHIP player_ships[3]; /**< Ships of the player */
    int placed_ships; /**< Number of ships placed */
    int role; /**< Role of the player */
    int last_x, last_y; /**< Last attack coordinates */
    int sunken_ships_opp; /**< Number of opponent sunken ships */
    int sunken_ships_player; /**< Number of player sunken ships */
} GAME;

/**
 * @brief Initializes the game.
 *
 * @param game Pointer to the game structure.
 * @param role The role of the player.
 */
void init_game(GAME *game, int role);

/**
 * @brief Draws the game.
 *
 * @param game Pointer to the game structure.
 */
void draw_game(GAME *game);

/**
 * @brief Handles mouse input for the edit board mode.
 *
 * @param game Pointer to the game structure.
 */
void handleEditBoardMouse(GAME *game);

/**
 * @brief Handles keyboard input for the edit board mode.
 *
 * @param scancode The scancode received from the keyboard.
 * @param game Pointer to the game structure.
 */
void handleEditBoardKB(uint8_t scancode, GAME *game);

/**
 * @brief Snaps the position to the grid.
 *
 * @param pos The position to snap.
 * @return The snapped position.
 */
int snap_to_grid(int pos);

/**
 * @brief Checks if a ship can be placed in the current position.
 *
 * @param game Pointer to the game structure.
 * @return True if the ship can be placed, false otherwise.
 */
bool can_place_ship(GAME *game);

/**
 * @brief Places a ship on the grid.
 *
 * @param game Pointer to the game structure.
 */
void place_ship(GAME *game);

void setPlayerMode(GAME *game, int mode);

void handleAttackMouse(GAME *game);

void attackOutcome(GAME *game, int outcome);

void defendBoard(GAME *game, int x, int y);

#endif /* _GAME_H */
