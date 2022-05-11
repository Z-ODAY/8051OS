#ifndef _TASK_H_
#define _TASK_H_

#include "rtos.h"

typedef struct CPU_OS_TCB CPU_OS_TCB;

struct CPU_OS_TCB {

    TickType tick_wait;
    CPU_OS_TCB xdata *next;
    CPU_OS_TCB xdata *Previous;
    uBaseType  uxPriority;
    uStackType volatile TopofStack;
    uBaseType runtime;
    uBaseType runtimectr;
};

typedef struct OS_TCB_LIST_HEAD
{
    CPU_OS_TCB xdata *Headptr;
    CPU_OS_TCB xdata *Tailptr;
    CPU_OS_TCB xdata *Index;
    uShortType NumberOfItems;
}OS_TCB_LIST_HEAD;

typedef struct LIST_GROUP
{
    OS_TCB_LIST_HEAD  volatile os_rdy_list[PRI_MAX];
    OS_TCB_LIST_HEAD  volatile os_delay_list;
    OS_TCB_LIST_HEAD  volatile os_Overflow_delay_list;
    OS_TCB_LIST_HEAD  volatile os_Suspend_list;
}LIST_GROUP;


void pubTask_Init(CPU_OS_TCB  xdata *const  newtcb,
                  uStackType  idata *const  pStack,
                  uShortType  const  StackSize,
                  uTask_Entry  const  entry,
                  uBaseType  const pri);

void OS_Start(void);
void OS_Delay(TickType const ms);


#endif //_TASK_H_

