#include "main.h"
#include "rtos.h"

bit test1;
bit test2;

void Delay200ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 10;
	j = 31;
	k = 147;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}


#define TASK1STACKSIZE      20
uidataStackType task1stack[TASK1STACKSIZE];
CPU_OS_TCB xdata task1_tcb;

void task1(void)
{
    for (; ;)
    {
        P10 = 0;
        test1 = 1;
        //Delay200ms();
        P10 = 1;
        test1 = 0;
        //Delay200ms();
        //Callcontext_switch();
    }
}

#define TASK2STACKSIZE      20
uidataStackType task2stack[TASK2STACKSIZE];
CPU_OS_TCB xdata task2_tcb;

void task2(void)
{
    for (;;)
    {

        P11 = 0;
        test2 = 1;
        //Delay200ms();
        P11 = 1;
        test2 = 0;
        //Delay200ms();
        //Callcontext_switch();
    }
}


int main(void)
{
   	// OS_Init();
    // pubTask_Init(&task1_tcb,&task1stack,TASK1STACKSIZE,task1,0);
    // pubTask_Init(&task2_tcb,&task2stack,TASK2STACKSIZE,task2,1);
    // OS_Start();
Callcontext_switch();
    for (; ;)
    {
        
    }
    


}


