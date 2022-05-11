#include "rtos.h"

extern void TaskSwitchContext(void);
extern void task_switch_context(void);
extern void TaskIncTick(void);
extern CPU_OS_TCB xdata *volatile xdata CurrentOSTCB;
uStackType volatile _IDATA_ pubStack[PUBLIC_STACK_SIZE] _AT_ PUBLIC_STACK_START_ADDR;

uStackType Stack_Init(uStackType _IDATA_ * pStack,uTask_Entry const entry)
{
	*++pStack = (u16)entry;          //pc low
	*++pStack = (u16)entry >> 8u;    //pc hight
  pStack += 13u;
	return (uStackType)pStack;
}


//006BH -13
//0073H -14
//007BH -15

void Callcontext_switch(void)
{
  _ASM_ LCALL 007BH
}


void os_context_switch(void) interrupt 15
{
  EA = 0;
  CurrentOSTCB->TopofStack = SP;
  SWITCH_STACK();
  task_switch_context();
  SP = CurrentOSTCB->TopofStack;
  EA = 1;
}

void OS_Timer0_Context_switch(void) interrupt 1
{
    EA = 0;
    CurrentOSTCB->TopofStack = SP;

    SWITCH_STACK();
    TaskIncTick();
    TaskSwitchContext();
    #if (TIMER0_12T) 
    TL0 = 0x18;
    TH0 = 0xFC;
    #else
    TH0 = TIME0_H;
    TL0 = TIME0_L;
    #endif
    SP = CurrentOSTCB->TopofStack;
    TF0 = 0;
    EA = 1;
}

