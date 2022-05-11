#ifndef _RTOS_H_
#define _RTOS_H_

#ifndef _ASM_
#define _ASM_         __asm
#else
#define _ASM_
#endif

#ifndef _IDATA_
#define _IDATA_        idata
#else
#define _IDATA_
#endif

#ifndef _XDATA_
#define _XDATA_         xdata
#else
#define _XDATA_ 
#endif


#define PUBLIC_STACK_START_ADDR     (unsigned char)(0xEBu)

#ifndef _AT_
#define _AT_          _at_
#else
#define _AT_
#endif

#ifndef EXTERN
#define EXTERN        extern
#else
#define EXTERN 
#endif


#define TIMER0_12T              0u
#define TIME0_L                 0x20u
#define TIME0_H                 0xd1u
#define PRI_MAX                 4u
#define PUBLIC_STACK_SIZE       20u
#define IDLESTACKSIZE           20u
#define DELAY_MAX               0xfffffffful

#include "stc15.h"
#include <intrins.h>
#include "cpudatatype.h"
#include "cpuport.h"
#include "task.h"


#endif //_RTOS_H_

