#include "Sch.h"
#include <pic18f4550.h>

sTask SCH_tasks_G[SCH_MAX_TASKS];
uint8_t Error_code_G;

// Keeps track of time since last error was recorded (see below)
static uint32_t Error_tick_count_G;
// The code of the last error (reset after ~1 minute)
static uint8_t Last_error_code_G;

void SCH_Init_T2(void){
    uint8_t i;
    for (i = 0; i < SCH_MAX_TASKS; i++){
        SCH_Delete_Task(i);
    }
    // Reset the global error variable
    // - SCH_Delete_Task() will generate an error code,
    // (because the task array is empty)
    Error_code_G = 0;
    // The required Timer 2 overflow is 0.001 seconds (1 ms)
    // automatic reload available in Timer2
    OSCCON = 0x70;      // OSC 8 MHz
    T2CON = 0x02;       // no post scale, TMR2 OFF and pre scale 16
    PR2 = 62;          // match register
    INTCON |= (1<<6);   // Enables all unmasked peripheral interrupts
    PIE1 |= (1<<1);     // interrupt is enabled by setting the TMR2 Match Interrupt Enable bit
}

void __interrupt() SCH_Update(){
    if((PIR1&(1<<1)) == 2){
        uint8_t index;
        for(index=0; index<SCH_MAX_TASKS; index++){
            if(SCH_tasks_G[index].pTask){
                if(SCH_tasks_G[index].Delay==0){
                    SCH_tasks_G[index].RunMe++;
                    if(SCH_tasks_G[index].Period){
                        SCH_tasks_G[index].Delay = SCH_tasks_G[index].Period;
                    }
                }
                else{
                    SCH_tasks_G[index].Delay -= 1;
                }
            }
        }
        PIR1 &= ~(1<<1);    // clear Timer2 flag
    }
}

uint8_t SCH_Add_Task(void (* pTask_fun)(void), uint32_t Delay_fun, uint32_t Period_fun){
    uint8_t index=0;
    while((SCH_tasks_G[index].pTask != 0) && (index<SCH_MAX_TASKS)){
        index++;
    }
    if(index == SCH_MAX_TASKS){
        Error_code_G = ERROR_SCH_TOO_MANY_TASKS;
        return SCH_MAX_TASKS;
    }
    SCH_tasks_G[index].pTask = pTask_fun;
    SCH_tasks_G[index].Delay = Delay_fun;
    SCH_tasks_G[index].Period = Period_fun;
    SCH_tasks_G[index].RunMe = 0;
    return index;
}

void SCH_Dispatch_Tasks(void){
    uint8_t index=0;
    for(index=0; (SCH_tasks_G[index].pTask != 0); index++){
        if(SCH_tasks_G[index].RunMe > 0){
            (*SCH_tasks_G[index].pTask)();
            SCH_tasks_G[index].RunMe -= 1;
            if(SCH_tasks_G[index].Period == 0){
                SCH_Delete_Task(index);
            }
        }
    }
    // Report system status
    //SCH_Report_Status();
    // The scheduler enters idle mode at this point
    //SCH_Go_To_Sleep();
}

void SCH_Start(void){
    INTCON |= (1<<7);   // Enables all high priority interrupts
    //T2CON |= (1<<3);    // TMR2 ON
    T2CON = 0x06;           // TMR2 ON and pre scale 16
}

uint8_t SCH_Delete_Task(const uint8_t TASK_INDEX){
    uint8_t Return_code;
    if(SCH_tasks_G[TASK_INDEX].pTask == 0){
        Error_code_G = ERROR_SCH_CANNOT_DELETE_TASK;
        Return_code = RETURN_ERROR;
    }
    else{
        Return_code = RETURN_NORMAL;
    }
    SCH_tasks_G[TASK_INDEX].pTask = 0x0000;
    SCH_tasks_G[TASK_INDEX].Delay = 0;
    SCH_tasks_G[TASK_INDEX].Period = 0;
    SCH_tasks_G[TASK_INDEX].RunMe = 0;
    return Return_code;
}

/*void SCH_Go_To_Sleep(void){
    // enter sleep mode
    OSCCON &= ~(1<<7);  // clear IDLEN bit
    asm SLEEP;          // sleep instruction
}*/

/*void SCH_Report_Status(void){
#if SCH_REPORT_ERRORS
    
#endif
}*/