#include "KBC.h"

//Slight change to how KBC_read_data works
//Added keyboard parameter to distinguish between keyboard and mouse
int (KBC_read) (uint8_t port, uint8_t *data, uint8_t keyboard){
    //Preliminary checks
    if(data == NULL) return 1;
    //Variable to store the status
    uint8_t status;

    //Try until maximum value is reached
    for(int i = 0; i < MAX_TRIES; i++){
        //Get kbc status, return 1 if error
        if(util_sys_inb(KBC_STAT_REG, &status)){
            printf("util_sys_inb function error!\n");
            return 1;
        }

        //In order to read, OBF must be full
        //(& operation must be != 0)
        if((status & OBF)){
            //Check possible errors before reading data
            //Check for Parity error, return 1 if so
            if(status & PARITY){
                printf("Parity Error!\n");
                return 1;
            }

            //Check for Timeout error, return 1 if so
            if(status & TIMEOUT){
                printf("Timeout Error!\n");
                return 1;
            }

            //Check if mouse bit is active at the same time as we attempt to read keyboard
            if((status & BIT(5)) && keyboard){
                printf("Mouse bit and keyboard active at the same time!\n");
                return 1;
            }

            //Check if mouse bit is inactive at the same time as we attempt to read mouse (!keyboard)
            if(!(status & BIT(5) || keyboard)){
                printf("Mouse bit inactive when attempting to read from mouse!\n");
                return 1;
            }

            //Finally read data, return 1 if error
            if(util_sys_inb(port, data)){
                printf("Reading Error!\n");
                return 1;
            }
            
            //Successful read, exit and return 0
            return 0;
        }
        //If OBF is not full, delay and try again
        tickdelay(micros_to_ticks(DELAY));
    }
    //If maximum value is reached, give up and return 1
    printf("Maximum tries reached!\n");
    return 1;
}

int (KBC_write) (uint8_t port, uint8_t command_byte){
    //Variable to store status
    uint8_t status;

    //Try until maximum value is reached
    for(int i = 0; i < MAX_TRIES; i++){
        //Get kbc status, return 1 if error
        if(util_sys_inb(KBC_STAT_REG, &status)){
            printf("util_sys_inb function error!\n");
            return 1;
        }

        //In order to write, IBF must be empty (not set)
        //(& operation must be == 0)
        if((status & IBF) == 0){
            //Write command byte
            if(sys_outb(port, command_byte)){
                printf("sys_outb function error!\n");
                return 1;
            }
            //Successful, return 0
            return 0;
        }

        //Delay and try again
        tickdelay(micros_to_ticks(DELAY));
    }
    //If maximum tries reached, give up and return 1
    printf("Maximum tries reached!\n");
    return 1;
}
