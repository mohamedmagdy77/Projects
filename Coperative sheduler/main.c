/*
 * File:   main.c
 * Author: M.Magdy
 *
 * Created on November 4, 2020, 10:21 PM
 */


//#include <xc.h>
#include "Sch.h"
#include "tasks.h"

void main(void) {
    LED_init();
    SCH_Init_T2();
    SCH_Add_Task(LED0_TOGGLE,1000,1000);
    SCH_Add_Task(LED1_TOGGLE,100,300);
    SCH_Start();
    while(1){
        SCH_Dispatch_Tasks();
    }
    return;
}
