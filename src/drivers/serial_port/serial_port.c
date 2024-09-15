#include "serial_port.h"
#include "serialport.h"


int hook_id_sp=4;

int serialport_subscribe_int(uint8_t* bitn){

    *bitn = BIT(hook_id_sp);

    if(sys_irqsetpolicy(COM1_IRQ, IRQ_REENABLE | IRQ_EXCLUSIVE, &hook_id_sp)) {printf("subs");return 1;}
    printf("Subscribed! \n");
    return 0;
}

int serialport_unsubscribe_int(){

    if(sys_irqrmpolicy(&hook_id_sp)) return 1;

    return 0;
}


void serialport_init(){
    uint8_t IER_v;
    if(util_sys_inb(COM1_UART_BASE+ IER, &IER_v)) return;
    IER_v &= 0xF0;
    if(sys_outb(COM1_UART_BASE + IER,IER_v | IER_ERBFI)) return;
    queue = initQueue(256);
}

int serialport_clearInterrupts(){
    if(sys_outb(COM1_UART_BASE + FCR,FCR_CLEAR)) return 1;
    while(!isQueueEmpty(queue))  pop(queue);
    return 0;
}


int send_byte(uint8_t byte) {
    uint8_t status;
    for (int i = 0; i < 12; i++) {
      if(serialport_getStats(&status)) return 1;
      if(status & LSR_THRE) {
        printf("Sending byte: %d \n", byte);
        return sys_outb(COM1_UART_BASE,byte);
      }
    }
    return 1;
}

int read_byte(){
    uint8_t status, received_byte;
    if(serialport_getStats(&status)) return 1;
    if(status & LSR_DATA_READY){
      if(util_sys_inb(COM1_UART_BASE + RBR, &received_byte)) return 1;
     
      if((status & (LSR_OE| LSR_PE| LSR_FE))==0){ 
          push(queue,received_byte);
          printf("Receiving byte: %d \n", received_byte);
          return 0;
      }
    }
    return 1;
}





void serialport_ih() {
    uint8_t IR;
    if (util_sys_inb(COM1_UART_BASE + IIR, &IR)) return;

    if (!(IR & IIR_NO_INT_PENDING)) {
        uint8_t interrupt_type = IR & IIR_ID;

        if (interrupt_type == IIR_OGN_RCVD_DATA_AVL || interrupt_type == IIR_FIFO_CT) {
            while (read_byte());  
        }
    }
}



void delete_serialport(){
    clearQueue(queue);
}
int serialport_getStats(uint8_t* stats){
    return util_sys_inb(COM1_UART_BASE + LSR, stats);
}



Queue* get_Queue(){
    return queue;
}

void addToQueue(uint8_t data)
{
    push(queue, data);
}
