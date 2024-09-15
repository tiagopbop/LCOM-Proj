/*!
 * @file proj.c
 * \mainpage Afrontier Battles
 *
 * This project implements a game that utilizes various drivers for video, keyboard, mouse, rtc and serial port communication. 
 * The game involves multiple modes such as MENU, INSTRUCTIONS, WAIT_PLAYER, INVITE, EDIT_BOARD, EXPECT, ATTACK, and DEFEND. 
 *
 *
 * This game was developed by :
 * @author João Miguel Peixoto Lamas, Pedro Afonso Nunes Fernandes, Tiago de Pinho Bastos de Oliveira Pinheiro, Tiago Grilo Ribeiro Rocha.
 */

// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

#include "drivers/video/video.h"
#include "drivers/keyboard_and_mouse/keyboard.h"
#include "drivers/keyboard_and_mouse/mouse.h"
#include "drivers/keyboard_and_mouse/KBC.h"
#include "drivers/keyboard_and_mouse/i8042.h"
#include "drivers/serial_port/serial_port.h"
#include "drivers/serial_port/queue.h"
#include "sprites/sprites.h"
#include "game_logic/menu.h"
#include "game_logic/instructions.h"
#include "game_logic/game.h"
#include "game_logic/accept.h"
#include "game_logic/wait.h"


extern uint8_t scancode;
extern vbe_mode_info_t vbe_mode_info;

// Any header files included below this line should have been created by you

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/proj/src/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

//Timer
extern int timer_counter;
//Keyboard
extern uint8_t scancode;
//Mouse
extern struct packet mouse_packet;
extern uint8_t processed_bytes[3];
extern int mouse_byte_counter;
extern int mouse_x;
extern int mouse_y;

typedef enum {MENU, INSTRUCTIONS, WAIT_PLAYER, INVITE, EDIT_BOARD,EDIT_BOARD_SOLO,EXPECT, ATTACK, DEFEND} MODE;
MODE mode = MENU;
typedef enum {START, WAIT, PLAY, END} STATE;
STATE state = START;
bool running = true;
int player_role=0;

int play_game(int role, GAME *game){

  if(mouse_write_command(ENABLE_DATA_REPORTING)) return 1;
  //30 fps
  serialport_init();
  if(timer_set_frequency(0, 30)) return 1;

  int ipc_status;
  uint8_t timer_irq_set, keyboard_irq_set, mouse_irq_set,serialport_irq_set;
  message msg;

  if(timer_subscribe_int(&timer_irq_set)) return 1;
  if(keyboard_subscribe_int(&keyboard_irq_set)) return 1;
  if(mouse_subscribe_int(&mouse_irq_set)) return 1;
  if (serialport_subscribe_int(&serialport_irq_set) != 0) return 1;

  printf("IRQ set is %x \n", &serialport_irq_set);

  while(running){
    if (driver_receive(ANY, &msg, &ipc_status) != 0) {
      printf("Error receiving message\n");
      continue;
    }

    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & timer_irq_set) {
            timer_int_handler();
            restore_background();
            if(mode == MENU) {draw_menu();}
            else if(mode == INSTRUCTIONS) {draw_instructions();}
            else if(mode == WAIT_PLAYER) {draw_wait();}
            else if(mode == INVITE) {draw_accept();}
            else if(mode == EDIT_BOARD) {draw_game(game);}
            else if(mode == EDIT_BOARD_SOLO) {draw_game(game);}
            else if(mode == EXPECT) {draw_game(game);}
            else if(mode == ATTACK) {draw_game(game);}
            else if(mode == DEFEND) {draw_game(game);}

            draw_mouse(mouse_x, mouse_y);
            swap_buffer();
          }
          if (msg.m_notify.interrupts & keyboard_irq_set) {
            kbc_ih();
            if(scancode == ESC_BREAKCODE){
              running = false;
            }
            if(mode == MENU){menuKB(scancode);}
            else if(mode == INSTRUCTIONS){instructionsKB(scancode);}
            else if(mode == WAIT_PLAYER){waitKB(scancode);}
            else if(mode == INVITE){acceptKB(scancode);}
            else if(mode == EDIT_BOARD) {handleEditBoardKB(scancode,game);}
            else if(mode == EDIT_BOARD_SOLO) {handleEditBoardKB(scancode,game);}
            else if(mode == EXPECT) {handleEditBoardKB(scancode,game);}
            else if(mode == ATTACK) {handleEditBoardKB(scancode,game);}

          }
          if (msg.m_notify.interrupts & mouse_irq_set) {
            mouse_ih();
            if(mouse_byte_counter == 3){
              build_packet();
              current_mouse_position();
              mouse_byte_counter = 0;
              if(mode == MENU){menuMouse();}
              else if(mode == INSTRUCTIONS){instructionsMouse();}
              else if(mode == WAIT_PLAYER){waitMouse();}
              else if(mode == INVITE){acceptMouse();}
              else if(mode == EDIT_BOARD) {handleEditBoardMouse(game);}
              else if(mode == EDIT_BOARD_SOLO) {handleEditBoardMouse(game);}
              else if(mode == ATTACK) {handleAttackMouse(game);}
            }
          }
          if (msg.m_notify.interrupts & serialport_irq_set) {
              printf("Entered serialport interrupt handler\n");
              serialport_ih();

              // Check if queue is not empty before popping
              if (!isQueueEmpty(get_Queue())) {
                  uint8_t data = pop(get_Queue());
                  printf("Popped data from queue: %d\n", data);

                  // Only process non-zero data
                  if (data != 0) {
                      printf("Processing data: %d\n", data);
                      if (data == 1 && mode == MENU) {
                          mode = INVITE;
                          player_role = INVITEE;
                          printf("Mode changed to INVITE, role set to INVITEE\n");
                      } else if (data == 2 && mode == WAIT_PLAYER) {
                          mode = EDIT_BOARD;
                          set_background(2);
                          printf("Mode changed to EDIT_BOARD\n");
                      } else if (data == 3 && mode == WAIT_PLAYER) {
                          mode = MENU;
                          printf("Mode changed to MENU\n");
                      }
                      else if (data == 4 && mode == INVITE) {
                          mode = MENU;
                          printf("Mode changed to MENU\n");
                      } 
                      else if (data == 5 && mode == EDIT_BOARD) {
                          mode = EDIT_BOARD_SOLO;
                          printf("Mode changed to EDITSOLO\n");
                      } 
                      else if (data == 6 && mode == EXPECT) {
                          mode = ATTACK;
                          setPlayerMode(game, 0);
                          printf("Mode changed to ATTACK\n");
                      }
                      else if (mode == DEFEND){
                        int x = (data >> 4) & 0x0F;
                        int y = data & 0x0F;
                        printf("Received attack at (%d, %d)\n", x, y);
                        defendBoard(game, x, y);
                        printf("Switch to attack");
                        if(game->sunken_ships_player == 6){
                          printf("Switch to menu");
                          mode = MENU;
                          set_background(0);
                        }
                        else{
                          mode = ATTACK;
                          setPlayerMode(game, 0);
                        }
                      }
                      else if (mode == ATTACK){
                        if(data == 0xF0){
                          attackOutcome(game, 0);
                        }
                        else if(data == 0xF1){
                          attackOutcome(game, 1);
                        }
                        if(game->sunken_ships_opp == 6){
                          printf("Switch to menu");
                          mode = MENU;
                          set_background(0);
                        }
                        else{
                          mode = DEFEND;
                          setPlayerMode(game, 1);
                        }
                      }                            
                      else {
                          printf("Unrecognized data: %d\n", data);
                      }
                  }
              } else {
                  printf("Queue is empty, no data to process\n");
              }
          }

          break;
        default:
          printf("Role is %d \n", role);
          break;
      }
    }
  
  }
  if(timer_unsubscribe_int()) return 1;
  if(keyboard_unsubscribe_int()) return 1;
  if(mouse_unsubscribe_int()) return 1;
  if(mouse_write_command(DISABLE_DATA_REPORTING)) return 1;
  if(serialport_unsubscribe_int()) return 1;


  return 0;
}
int (proj_main_loop)(int argc, char **argv) {

  if(argc != 1) {
    printf("Usage: lcom_run proj 0 | 1\n");
    return 1;
  }
  
  load_backgrounds();
  load_mouse();
  init_menu();
  init_instructions();
  init_accept();
  init_wait();
  if(map_vram(0x14C)) return 1;
  if(set_video_mode(0x14C)) return 1;
  init_back_buffer();
  set_background(0);
  GAME game;
  int role = atoi(argv[0]);
  init_game(&game, role);
  if(play_game(role, &game)) return 1;

  delete_Sprites();
  delete_menu();
  delete_instructions();
  delete_accept();
  delete_wait();
  delete_serialport();

  if(vg_exit()) return 1;

  return 0;
}
