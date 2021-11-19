/* 
 * File:   Sch.h
 * Author: M.Magdy
 *
 * Created on September 16, 2020, 10:36 PM
 */

#ifndef SCH_H
#define	SCH_H

#include "std_types.h"

// The maximum number of tasks required at any one time
// during the execution of the program
// MUST BE ADJUSTED FOR EACH NEW PROJECT
#define SCH_MAX_TASKS (2)

// enables (1) or disables (0) error reporting
#define SCH_REPORT_ERRORS 0

#define ERROR_SCH_TOO_MANY_TASKS 0
#define ERROR_SCH_CANNOT_DELETE_TASK 0
#define RETURN_ERROR 0
#define RETURN_NORMAL 1

typedef struct{
    // Pointer to the task (must be a 'void (void)' function)
    void (* pTask)(void);
    // Delay (ticks) until the function will (next) be run
    // - see SCH_Add_Task() for further details
    uint32_t Delay;
    // Interval (ticks) between subsequent runs.
    // - see SCH_Add_Task() for further details
    uint32_t Period;
    // Incremented (by scheduler) when task is due to execute
    uint8_t RunMe;
}sTask;

/**
 * @brief   scheduler initialization function
 * @note    uses Timer2
 * @param   None
 * @retval  None
 */
void SCH_Init_T2(void);
/**
 * @brief   scheduler add task function
 * @note    adds the task to "SCH_tasks_G" array
 * @param1  pointer to the task
 * @param2  delay before the task first run
 * @param3  period till task run again
 * @retval  index of task structure in "SCH_tasks_G" array
 */
uint8_t SCH_Add_Task(void (* pTask_fun)(void), uint32_t Delay_fun, uint32_t Period_fun);
/**
 * @brief   executes ready tasks
 * @note    when Delay equal to 0
 * @param   None
 * @retval  None
 */
void SCH_Dispatch_Tasks(void);
/**
 * @brief   scheduler starting function
 * @note    turns on Timer2
 * @param   None
 * @retval  None
 */
void SCH_Start(void);
/**
 * @brief   delete task structures from array  
 * @param   task index in array
 * @retval  status of deletion (success or fail..)
 */
uint8_t SCH_Delete_Task(const uint8_t TASK_INDEX);

#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* SCH_H */

