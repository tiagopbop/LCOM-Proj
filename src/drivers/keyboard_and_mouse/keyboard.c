#include "keyboard.h"

#include <lcom/lcf.h>

/*Note: changed both returns on subscribe and unsubscribe
 since it doesn't make sense to check for an error
 instead of just returning the result of the call.*/

//Global Variables
int keyboard_hook_id = 1;
uint8_t scancode = 0;

//Subscribe to interrupts
int (keyboard_subscribe_int)(uint8_t *bit_no){
    //Preliminary check
    if(bit_no == NULL) return 1;
    
    //Store bitmask
    *bit_no = BIT(keyboard_hook_id);

    //Subscribe interrupts
    return sys_irqsetpolicy(KBD_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &keyboard_hook_id);
}

//Unsubscribe from interrupts
int (keyboard_unsubscribe_int)(){
    return sys_irqrmpolicy(&keyboard_hook_id);
}

void (kbc_ih)(){
    //Read the status and then the scancode byte
    //(All done by the function KBC_read_data)
    if(KBC_read(KBC_OBF_REG, &scancode, 1)) printf("KBC_read_data function error!\n");
}
