#include <lcom/lcf.h>
#include <lcom/timer.h>

#include <stdint.h>

#include "i8254.h"

//Global Variables
int timer_hook_id = 0;
int timer_counter = 0;

int (timer_set_frequency)(uint8_t timer, uint32_t freq) {
  //Preliminary checks
  if(timer < 0 || timer > 2) return 1;
  if(freq < 19 || freq > TIMER_FREQ) return 1;

  //Get the selected timer register
  uint8_t selection = TIMER_0 + timer;

  //Get Timer configuration
  uint8_t control;
  if(timer_get_conf(timer, &control)) return 1;

  //Following the documentation, making sure to not change the 4 LSBs -> (&) and 
  //then Initialize as "LSB followed by MSB" (TIMER_LSB_MSB) -> (|).
  control = ((control & 0x0F) | TIMER_LSB_MSB);

  //Finally, update control to target the selected timer
  switch(timer){
    case 0:
    control |= TIMER_SEL0;
    break;
    case 1:
    control |= TIMER_SEL1;
    break;
    case 2:
    control |= TIMER_SEL2;
    break;
  }

  //Calculate the frequency following the equation val = clock/freq,
  //where clock is the frequency of the Clock input and freq is the value loaded initially in the timer.
  uint32_t val = TIMER_FREQ/freq;
  uint8_t lsb, msb;
  util_get_LSB(val, &lsb);
  util_get_MSB(val, &msb);

  //To finish, configure the timer
  if(sys_outb(TIMER_CTRL, control)) return 1;
  if(sys_outb(selection, lsb)) return 1;
  if(sys_outb(selection, msb)) return 1;
  
  return 0;
}

int (timer_subscribe_int)(uint8_t *bit_no) {
  //Preliminary check
  if(bit_no == NULL) return 1;

  //Store bitmask
  *bit_no = BIT(timer_hook_id);

  //Subscribe interrupts
  if(sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &timer_hook_id)) return 1;

  return 0;
}

int (timer_unsubscribe_int)() {
  if(sys_irqrmpolicy(&timer_hook_id)) return 1;
  return 0;
}

void (timer_int_handler)() {
  //Increment the timer_counter global variable
  timer_counter++;
}

int (timer_get_conf)(uint8_t timer, uint8_t *st) {
  //Preliminary checks
  if(st == NULL || timer<0 || timer>2) return 1;

  //Calculate the Read-Back Command (rbc)
  uint8_t rbc = (TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer));
  if(sys_outb(TIMER_CTRL, rbc)) return 1;
  if(util_sys_inb(TIMER_0 + timer, st)) return 1;
  return 0;
}

int (timer_display_conf)(uint8_t timer, uint8_t st,
                        enum timer_status_field field) {
  //Preliminary checks
  if(timer < 0 || timer > 2) return 1;

  //Creation of a union element, later used for timer_print_config()
  union timer_status_field_val element;

  //Check what field of the i8254 status word must be displayed
  switch(field){
    //Configuration status, same as st
    case tsf_all:
      element.byte = st;
      break;

    //Timer initialization mode, so get the Initialization of st
    case tsf_initial:
      st = (st & TIMER_LSB_MSB) >> 4;
      if(st == 1) element.in_mode = LSB_only;
      else if(st == 2) element.in_mode = MSB_only;
      else if(st == 3) element.in_mode = MSB_after_LSB;
      else element.in_mode = INVAL_val;
      break;

    //Timer counting mode, so get the Operating/Counting mode of st
    case tsf_mode:
      st = (st & (BIT(3)|BIT(2)|BIT(1))) >> 1;
      //Take into account that 2 can be both 010 and 110
      //And 3 can be both 011 and 111
      if(st == 6) element.count_mode=2;
      else if(st == 7) element.count_mode=3;
      else element.count_mode=st;
      break;
    
    //Timer counting base, get leftmost bit
    case tsf_base:
    element.bcd = st & TIMER_BCD;
      break;
    default:
      return 1;
  }

  //Check for errors inside timer_print_config
  if(timer_print_config(timer, field, element)) return 1;

  return 0;
}
