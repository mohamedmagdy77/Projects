#include "tasks.h"
#include "std_types.h"

void LED_init(void){
    TRISC &= ~(1<<0);
    TRISC &= ~(1<<1);
    PORTC &= ~(1<<0);
    PORTC &= ~(1<<1);
    }

void LED0_TOGGLE(void){
    PORTC ^= (1<<0);
}

void LED1_TOGGLE(void){
    PORTC ^= (1<<1);
}
