#ifndef _CPU_PORT_H_
#define _CPU_PORT_H_

#include "rtos.h"

#define SWITCH_STACK()  {SP = (uStackType)pubStack;}
#define OS_Init()   {SWITCH_STACK();}
uStackType Stack_Init(uStackType _IDATA_ * pStack,uTask_Entry const entry);
void Callcontext_switch(void);


// uStackType Stack_Init(uStackType _IDATA_ *const pStack,
// 											uTask_Entry const entry)
// {
//   uStackType _IDATA_ *prStack_of_task = pStack;
// 	*++prStack_of_task = (u16)entry;         //pc low
// 	*++prStack_of_task = (u16)entry >> 8u;    //pc hight
//     *++prStack_of_task = 0u;                   //acc reg
//     *++prStack_of_task = 0u;                   //b reg
//     *++prStack_of_task = 0u;                   //dptr hight
//     *++prStack_of_task = 0u;                   //dptr low
//     *++prStack_of_task = 0u;                   //psw
//     *++prStack_of_task = 0u;                      //r0
//     *++prStack_of_task = 1u<<1u;                   //r1
//     *++prStack_of_task = 1u<<2u;                   //r2
//     *++prStack_of_task = 1u<<3u;                   //r3
//     *++prStack_of_task = 1u<<4u;                   //r4
//     *++prStack_of_task = 1u<<5u;                   //r5
//     *++prStack_of_task = 1u<<6u;                   //r6
//     *++prStack_of_task = 1u<<7u;                    //r7
//     //prStack_of_task += 13u;
// 	return (uStackType)prStack_of_task;
// }



#endif //_CPU_PORT_H_

