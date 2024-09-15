#include "mouse.h"

//Global Variables
int mouse_hook_id=2;
struct packet mouse_packet;
int mouse_byte_counter=0;
uint8_t processed_bytes[3];

//Initial x and y values
int mouse_x = 0;
int mouse_y = 0;

//mouse_packet getter function
void (get_mouse_packet)(struct packet *p){
    if(p != NULL) *p = mouse_packet;
    else printf("Error getting mouse_packet");
}

int (mouse_subscribe_int)(uint8_t *bit_no){
    //Preliminary check
    if(bit_no == NULL) return 1;
    
    //Store bitmask
    *bit_no = BIT(mouse_hook_id);

    //Subscribe interrupts
    return sys_irqsetpolicy(MOUSE_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int (mouse_unsubscribe_int)(){
    return sys_irqrmpolicy(&mouse_hook_id);
}

void (mouse_ih)(){
    //Attempt to read new byte from mouse, exit if error
    if(KBC_read(KBC_OBF_REG, &processed_bytes[mouse_byte_counter],0)){
        printf("Error reading mouse data\n");
        return;
    }
    //Successful read
    //Check if, it's the first byte (control), it's valid
    //If so, increment counter to write next byte, otherwise do nothing to overwrite it
    if(mouse_byte_counter == 0 && (processed_bytes[mouse_byte_counter] & CONTROL)) mouse_byte_counter++;

    //IMPORTANT NOTE: if counter reaches this point and is ==0, it means the first byte wasn't valid, so
    //Leave it be so the next call to interrupt handler overwrites it!

    //If the counter is other than 0, increment it
    else if(mouse_byte_counter != 0){
        mouse_byte_counter++;

        //If mouse_byte_counter is more than 2 (basically 3), it means all 3 bytes were
        //Successfully read so the packet can be built.
        //Let mouse_test_packet handle this case because of the counter variable
    }
}

void (build_packet)(){
    //Copy the processed_bytes to the mouse_packet struct
    mouse_packet.bytes[0] = processed_bytes[0];
    mouse_packet.bytes[1] = processed_bytes[1];
    mouse_packet.bytes[2] = processed_bytes[2];

    //Following the way the struct is built, update the variables using the values in byte 0
    mouse_packet.lb = mouse_packet.bytes[0] & MOUSE_LB;
    mouse_packet.rb = mouse_packet.bytes[0] & MOUSE_RB;
    mouse_packet.mb = mouse_packet.bytes[0] & MOUSE_MB;
    mouse_packet.x_ov = mouse_packet.bytes[0] & MOUSE_X_OVF;
    mouse_packet.y_ov = mouse_packet.bytes[0] & MOUSE_Y_OVF;

    //Handle if either x sign bit and/or y sign bit are set
    if(mouse_packet.bytes[0] & MOUSE_X_SIGN){
        mouse_packet.delta_x = EXTEND(mouse_packet.bytes[1]);
    }
    else mouse_packet.delta_x = mouse_packet.bytes[1];

    if(mouse_packet.bytes[0] & MOUSE_Y_SIGN){
        mouse_packet.delta_y = EXTEND(mouse_packet.bytes[2]);
    }
    else mouse_packet.delta_y = mouse_packet.bytes[2];
}

int (mouse_write_command)(uint8_t command){
    //Variable to store the outcome
    uint8_t outcome;

    //Loop over MAX_TRIES to attempt to write the command
    for(int i = 0; i < MAX_TRIES; i++){
        //Warn the KBC that the next command is directed towards the mouse
        if(KBC_write(KBC_CMD_REG, WRITE_BYTE_TO_MOUSE)) return 1;
        //Next, write the command to the mouse
        if(KBC_write(KBC_OBF_REG, command)) return 1;
        //Wait a bit to ensure correctly read data
        tickdelay(micros_to_ticks(DELAY));
        //Get the response from the output buffer (port 0x60) making use of the util_sys_inb function defined in utils.c
        if(util_sys_inb(KBC_OBF_REG, &outcome)) return 1;
        //Check if any error ocurred
        if(outcome == NACK || outcome == MOUSE_ERROR){
            //If so, wait a bit before trying again
            tickdelay(micros_to_ticks(DELAY));
        }
        //Check if outcome is as expected (ACK -> 0xFA)
        //If so return 0
        else if(outcome == ACK) return 0;
    }
    //If maximum number of tries was exeeded, return 1
    return 1;
}

void current_mouse_position(){
    //Value of x
    if(!mouse_packet.x_ov) mouse_x = get_current_value(mouse_x, mouse_packet.delta_x, 0, 1151);
    //Value of y
    if(!mouse_packet.y_ov) mouse_y = get_current_value(mouse_y, -mouse_packet.delta_y, 0, 863);
}

int get_current_value(int value, int delta, int min, int max){
    int current_value = value + delta;
    if(current_value < min) return min;
    else if(current_value > max) return max;
    return current_value;
}
