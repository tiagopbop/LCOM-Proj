#include "game.h"

extern struct packet mouse_packet;
extern int mouse_x;
extern int mouse_y;
extern bool running;
int grid_x = 67;
int grid_y = 142;
static SHIP *selected_ship = NULL;
typedef enum {MENU, INSTRUCTIONS, WAIT_PLAYER, INVITE, EDIT_BOARD,EDIT_BOARD_SOLO,EXPECT, ATTACK, DEFEND} MODE;
extern MODE mode;
static int player_count =0;

Sprite hit;
Sprite miss;

void init_game(GAME *game, int role) {
    game->player_mode = GAME_EDIT_BOARD;
    game->player_state = GAMESTART;
    game->placed_ships = 0;
    game->role = role;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            game->player_grid.cells[i][j].cell_state = EMPTY;
            game->enemy_grid.cells[i][j].cell_state = EMPTY;
            game->player_grid.cells[i][j].top_left_x = 67 + i * 80;
            game->player_grid.cells[i][j].top_left_y = 143 + j * 80;
            game->enemy_grid.cells[i][j].top_left_x = 682 + i * 80;
            game->enemy_grid.cells[i][j].top_left_y = 143 + j * 80;
        }
    }
    
    // Ship 0
    game->player_ships[0].length = 1;
    game->player_ships[0].x = 200;
    game->player_ships[0].y = 690;
    game->player_ships[0].initial_x = 200;
    game->player_ships[0].initial_y = 690;
    game->player_ships[0].orientation = HORIZONTAL;
    if (load_Sprite((xpm_map_t)ship1_xpm, &game->player_ships[0].horizontal_ship)) return;
    if (load_Sprite((xpm_map_t)ship1_xpm, &game->player_ships[0].vertical_ship)) return;
    
    // Ship 1
    game->player_ships[1].length = 2;
    game->player_ships[1].x = 400;
    game->player_ships[1].y = 690;
    game->player_ships[1].initial_x = 400;
    game->player_ships[1].initial_y = 690;
    game->player_ships[1].orientation = HORIZONTAL;
    if (load_Sprite((xpm_map_t)ship2_hor_xpm, &game->player_ships[1].horizontal_ship)) return;
    if (load_Sprite((xpm_map_t)ship2_ver_xpm, &game->player_ships[1].vertical_ship)) return;
    
    // Ship 2
    game->player_ships[2].length = 3;
    game->player_ships[2].x = 700;
    game->player_ships[2].y = 690;
    game->player_ships[2].initial_x = 700;
    game->player_ships[2].initial_y = 690;
    game->player_ships[2].orientation = HORIZONTAL;
    if (load_Sprite((xpm_map_t)ship3_hor_xpm, &game->player_ships[2].horizontal_ship)) return;
    if (load_Sprite((xpm_map_t)ship3_ver_xpm, &game->player_ships[2].vertical_ship)) return;

    if(load_Sprite((xpm_map_t)hit_xpm, &hit)) return;
    if(load_Sprite((xpm_map_t)miss_xpm, &miss)) return;
}

void draw_game(GAME *game) {
    for (int i = 0; i < 3; i++) {
        if (game->player_ships[i].orientation == HORIZONTAL) {
            draw_Sprite(&game->player_ships[i].horizontal_ship, game->player_ships[i].x, game->player_ships[i].y);
        } else {
            draw_Sprite(&game->player_ships[i].vertical_ship, game->player_ships[i].x, game->player_ships[i].y);
        }
    }
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            switch(game->player_grid.cells[i][j].cell_state){
                case HIT:
                    draw_Sprite(&hit, game->player_grid.cells[j][i].top_left_x, game->player_grid.cells[j][i].top_left_y);
                    break;
                case MISS:
                    draw_Sprite(&miss, game->player_grid.cells[j][i].top_left_x, game->player_grid.cells[j][i].top_left_y);
                    break;
                default:
                    break;
            }
            switch(game->enemy_grid.cells[i][j].cell_state){
                case HIT:
                    draw_Sprite(&hit, game->enemy_grid.cells[j][i].top_left_x, game->enemy_grid.cells[j][i].top_left_y);
                    break;
                case MISS:
                    draw_Sprite(&miss, game->enemy_grid.cells[j][i].top_left_x, game->enemy_grid.cells[j][i].top_left_y);
                    break;
                default:
                    break;
            }
        }
    }
}

void handleEditBoardMouse(GAME *game) {
    if (mouse_packet.lb) {
        if (selected_ship == NULL) {
            for (int i = 0; i < 3; i++) {
                if (game->player_ships[i].orientation == HORIZONTAL) {
                    if (mouse_x >= game->player_ships[i].x && mouse_x <= game->player_ships[i].x + game->player_ships[i].length * 80) {
                        if (mouse_y >= game->player_ships[i].y && mouse_y <= game->player_ships[i].y + 80) {
                            selected_ship = &game->player_ships[i];
                            selected_ship->initial_x = selected_ship->x;
                            selected_ship->initial_y = selected_ship->y;
                            break;
                        }
                    }
                } else {
                    if (mouse_x >= game->player_ships[i].x && mouse_x <= game->player_ships[i].x + 80) {
                        if (mouse_y >= game->player_ships[i].y && mouse_y <= game->player_ships[i].y + game->player_ships[i].length * 80) {
                            selected_ship = &game->player_ships[i];
                            selected_ship->initial_x = selected_ship->x;
                            selected_ship->initial_y = selected_ship->y;
                            break;
                        }
                    }
                }
            }
        } else {
            // Update ship position while dragging
            selected_ship->x = mouse_x;
            selected_ship->y = mouse_y;
        }
    } else if (selected_ship != NULL) {
        place_ship(game);
        selected_ship = NULL;
    }
}

int snap_to_grid(int pos) {
    return (pos / 80) * 80;
}

bool can_place_ship(GAME *game) {
    int real_grid_x = (selected_ship->x - grid_x) / 80;
    int real_grid_y = (selected_ship->y - grid_y) / 80;

    if (selected_ship->orientation == HORIZONTAL) {
        if (real_grid_x + selected_ship->length > 5) {
            return false;
        }
        for (int i = 0; i < selected_ship->length; i++) {
            if (game->player_grid.cells[real_grid_y][real_grid_x + i].cell_state == SHIP_HERE) {
                return false;
            }
        }
    } else {
        if (real_grid_y + selected_ship->length > 5) {
            return false;
        }
        for (int i = 0; i < selected_ship->length; i++) {
            if (game->player_grid.cells[real_grid_y + i][real_grid_x].cell_state == SHIP_HERE) {
                return false;
            }
        }
    }
    return true;
}

void place_ship(GAME *game) {


    if (!can_place_ship(game)) {
        printf("Cannot place ship at (%d, %d)\n", selected_ship->x, selected_ship->y);
        selected_ship->x = selected_ship->initial_x;
        selected_ship->y = selected_ship->initial_y;
        return;
    }
    selected_ship->x = snap_to_grid(selected_ship->x)+68;
    selected_ship->y = snap_to_grid(selected_ship->y)-17;

    int grid_x = (selected_ship->x - 67) / 80;
    int grid_y = (selected_ship->y - 142) / 80;

    if (selected_ship->orientation == HORIZONTAL) {
        for (int i = 0; i < selected_ship->length; i++) {
            game->player_grid.cells[grid_y][grid_x + i].cell_state = SHIP_HERE;
        }
    } else {
        for (int i = 0; i < selected_ship->length; i++) {
            game->player_grid.cells[grid_y + i][grid_x].cell_state = SHIP_HERE;
        }
    }
    if(game->placed_ships<3)game->placed_ships++; 
    printf("Ship placed at (%d, %d)\n", selected_ship->x, selected_ship->y);
}

void handleEditBoardKB(uint8_t scancode, GAME *game) {
    if (selected_ship != NULL) {
        if (scancode == 0x1E || scancode == 0x20) { // Rotate ship
            selected_ship->orientation = (selected_ship->orientation == HORIZONTAL) ? VERTICAL : HORIZONTAL;
        }
    }
       if (scancode == 0x1C && mode == EDIT_BOARD) {
        if (game->placed_ships == 3) { 
            game->player_mode = GAME_EXPECT;
            send_byte(0x05);
            tickdelay(micros_to_ticks(5)); 
            mode = EXPECT; 
            player_count++;
            printf("ENTERED ATACK MODE");
        }
    }
           if (scancode == 0x1C && mode == EDIT_BOARD_SOLO) {
                if (game->placed_ships == 3) { 
                    printf("ENTERED DEFENDD MODE\n");
                    send_byte(0x06);
                    tickdelay(micros_to_ticks(5));   
                    game->player_mode = GAME_DEFEND;
                    mode = DEFEND;
            }
    }
}

void setPlayerMode(GAME *game, int mode) {
    if(mode == 0) game->player_mode = GAME_ATTACK;
    else if(mode == 1) game->player_mode = GAME_DEFEND;
}

void handleAttackMouse(GAME *game){
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(mouse_x >= game->enemy_grid.cells[i][j].top_left_x && mouse_x <= game->enemy_grid.cells[i][j].top_left_x + 80){
                if(mouse_y >= game->enemy_grid.cells[i][j].top_left_y && mouse_y <= game->enemy_grid.cells[i][j].top_left_y + 80){
                    if(game->enemy_grid.cells[i][j].cell_state == EMPTY && mouse_packet.lb){
                        game->last_x = i;
                        game->last_y = j;
                        printf("Sending attack");
                        send_byte((i << 4) | j);
                    }
                }
            }
        }
    }
}

void attackOutcome(GAME *game, int outcome){
    printf("Checking outcome");
    if(outcome == 0){
        game->enemy_grid.cells[game->last_y][game->last_x].cell_state = MISS;
    }
    else{
        game->enemy_grid.cells[game->last_y][game->last_x].cell_state = HIT;
        game->sunken_ships_opp++;
    }
}

void defendBoard(GAME *game, int x, int y){
    printf("Defending board");
    for(int i = 0; i < 5; i++){
        for(int j = 0; j < 5; j++){
            if(game->player_grid.cells[i][j].cell_state == SHIP_HERE){
                printf("Occupied cell at (%d, %d)\n", i, j);
            }
        }
    }
    if(game->player_grid.cells[y][x].cell_state == EMPTY){
        game->player_grid.cells[y][x].cell_state = MISS;
        send_byte(0xF0);
    }
    else{
        game->player_grid.cells[y][x].cell_state = HIT;
        game->sunken_ships_player++;
        send_byte(0xF1);
    }
}
