#include "rtos.h"


CPU_OS_TCB _XDATA_ * volatile _XDATA_ CurrentOSTCB;
static uBaseType volatile _XDATA_ ScheduleRunning;
static uBaseType volatile _XDATA_ SchedulerSuspended;
//static uBaseType volatile _XDATA_ xYieldPending;
static uBaseType _XDATA_ OS_tasknum;
static uBaseType _XDATA_ OS_currtasknum;
static TickType volatile _XDATA_ Tickcount;
static uBaseType  volatile _XDATA_ current_Highest_priority;
static LIST_GROUP volatile _XDATA_ list_group;
static TickType volatile _XDATA_ NextTaskUnblockTime;
static OS_TCB_LIST_HEAD _XDATA_ * volatile _XDATA_ pos_delay_list;
static OS_TCB_LIST_HEAD _XDATA_ * volatile _XDATA_ pOverflow_delay_list;

#define LIST_IS_EMPTY(list) ((uBaseType)(0 == (list)->NumberOfItems))
#define GET_LIST_ITEM_MEMBER_VAL(list_item,member) ((list_item)->member)

#define os_tcb_list_init(phead)                  \
do{                                              \
    phead->Headptr = 0u;                         \
    phead->Tailptr = 0u;                         \
    phead->Index = 0u;                           \
    phead->NumberOfItems = 0u;                   \
}while (0u)


#define FIND_HIGHTPRI(pri)                      \
do{                                             \
    if (pri > current_Highest_priority){        \
        current_Highest_priority = pri;         \
    }                                           \
}while (0u)


#define FIND_HIGHTPRI_TASK()                                                \
do{                                                                         \
    uBaseType _XDATA_ pri = current_Highest_priority;                       \
                                                                            \
    OS_TCB_LIST_HEAD _XDATA_ * _XDATA_ rdylist = list_group.os_rdy_list;    \
                                                                            \
    while (LIST_IS_EMPTY(&rdylist[pri])) --pri;                             \
                                                                            \
    CurrentOSTCB = rdylist[pri].Headptr;                                    \
                                                                            \
    current_Highest_priority = pri;                                         \
}while (0u)


#define ADD_TASK_TO_REDAY_LIST(tcb)                                         \
do{                                                                         \
    FIND_HIGHTPRI((tcb)->uxPriority);                                       \
    os_tcb_insterTail(&list_group.os_rdy_list[(tcb)->uxPriority],(tcb));    \
} while (0u)


#define RESET_NEXT_TASK_UNLOCK_TIME()                                       \
do{                                                                         \
    CPU_OS_TCB _XDATA_ * _XDATA_ TCB;                                       \
    if (LIST_IS_EMPTY(pos_delay_list)){                                     \
        NextTaskUnblockTime = DELAY_MAX;                                    \
    }                                                                       \
    else{                                                                   \
        TCB = GET_LIST_ITEM_MEMBER_VAL(pos_delay_list,Headptr);             \
        NextTaskUnblockTime = GET_LIST_ITEM_MEMBER_VAL(TCB,tick_wait);      \
    }                                                                       \
} while (0u)


#define SW_DELAY_LIST()                                                     \
do{                                                                         \
    OS_TCB_LIST_HEAD _XDATA_ * _XDATA_ temp;                                \
    temp = pos_delay_list;                                                  \
    pos_delay_list = pOverflow_delay_list;                                  \
    pOverflow_delay_list = temp;                                            \
    RESET_NEXT_TASK_UNLOCK_TIME();                                          \
} while (0u)



static void os_tcb_insterHead(OS_TCB_LIST_HEAD _XDATA_ *const phead,
                        CPU_OS_TCB _XDATA_ *const os_tcb)
{
    OS_TCB_LIST_HEAD _XDATA_ * _XDATA_ head = phead;
    CPU_OS_TCB _XDATA_ *_XDATA_ temp;
    if (head->NumberOfItems)
    {
        os_tcb->next = head->Headptr;
        os_tcb->Previous = 0u;
        temp = head->Headptr;
        temp->Previous = os_tcb;
        head->Headptr = os_tcb;
    }
    else
    {
        os_tcb->next = 0u;
        os_tcb->Previous = 0u;
        head->Headptr = os_tcb;
        head->Tailptr = os_tcb;
        head->Index = head->Headptr;
    }
    ++head->NumberOfItems;
}


static void os_tcb_insterTail(OS_TCB_LIST_HEAD _XDATA_ *const phead ,
                        CPU_OS_TCB _XDATA_ *const os_tcb)
{
    OS_TCB_LIST_HEAD _XDATA_ * _XDATA_ head = phead;
    CPU_OS_TCB _XDATA_ *_XDATA_ temp;
    if (head->NumberOfItems)
    {
        os_tcb->next = 0u;
        os_tcb->Previous = head->Tailptr;
        temp = head->Tailptr;
        temp->next = os_tcb;
        head->Tailptr = os_tcb;
    }
    else
    {
        os_tcb->next = 0u;
        os_tcb->Previous = 0u;
        head->Headptr = os_tcb;
        head->Tailptr = os_tcb;
        head->Index = head->Headptr;
    }
    ++head->NumberOfItems;    
}

static void os_tcb_remove(OS_TCB_LIST_HEAD _XDATA_ *const phead , 
                    CPU_OS_TCB _XDATA_ *const os_tcb)
{
    OS_TCB_LIST_HEAD _XDATA_ * _XDATA_ head =  phead;
    CPU_OS_TCB _XDATA_ *_XDATA_ tcb_next = os_tcb->next, _XDATA_ *_XDATA_ tcb_bprv = os_tcb->Previous,_XDATA_ *temp;

    if ((head->Headptr == os_tcb) && (head->Tailptr == os_tcb)){
        head->Headptr = 0u;
        head->Tailptr = 0u;
        head->Index = 0u;
    }
    else if (tcb_bprv == 0u){
        temp = head->Headptr;
        head->Headptr = temp->next;
    }
    else if (tcb_next == 0u){
        temp = head->Tailptr;
        head->Tailptr = temp->Previous;
    }
    else{
        tcb_bprv->next = tcb_next;
        tcb_next->Previous = tcb_bprv;
    }
    --head->NumberOfItems;
    os_tcb->next = 0u;
    os_tcb->Previous = 0u;
}


static void os_tcb_insterRise(OS_TCB_LIST_HEAD _XDATA_ *const phead, 
                    CPU_OS_TCB _XDATA_ *const os_tcb)
{
    




}


void TaskIncTick(void)
{
    if (!SchedulerSuspended){

        const TickType _XDATA_ ConstTickcount = ++Tickcount;
        CPU_OS_TCB _XDATA_ * volatile _XDATA_ TCB;
        TickType _XDATA_ TaskWaitTime;

        if (ConstTickcount == (TickType)0ul){
            SW_DELAY_LIST();
        }

        if (ConstTickcount >= NextTaskUnblockTime){

            for (;;){

                if (LIST_IS_EMPTY(pos_delay_list)){

                    NextTaskUnblockTime = DELAY_MAX;
                    break;
                }
                else{

                    TCB = GET_LIST_ITEM_MEMBER_VAL(pos_delay_list,Headptr);
                    TaskWaitTime = GET_LIST_ITEM_MEMBER_VAL(TCB,tick_wait);

                    if (ConstTickcount < TaskWaitTime){
                        NextTaskUnblockTime = TaskWaitTime;
                        break;
                    }

                    os_tcb_remove(pos_delay_list,TCB);

                    ADD_TASK_TO_REDAY_LIST(TCB);
                    
                }
                
            }
            
        }

    }

}


void TaskSwitchContext(void) 
{
    if (!SchedulerSuspended){
        FIND_HIGHTPRI_TASK();
    }
}

void task_switch_context(void)
{
    if (!SchedulerSuspended){
        FIND_HIGHTPRI_TASK();
    }
}

void OS_Delay(TickType const ms)
{
  TickType _XDATA_ timetowait;
  OS_TCB_LIST_HEAD _XDATA_ * _XDATA_ head;

   if (ms > 0){

       head = &list_group.os_rdy_list[CurrentOSTCB->uxPriority];

       os_tcb_remove(head,CurrentOSTCB);

       timetowait = Tickcount + ms;

       CurrentOSTCB->tick_wait = timetowait;

       if (timetowait < Tickcount){

           os_tcb_insterRise(pOverflow_delay_list,CurrentOSTCB);
       }
       else{

          os_tcb_insterRise(pos_delay_list,CurrentOSTCB);

          if (timetowait < NextTaskUnblockTime){
                NextTaskUnblockTime = timetowait;
          }

       }
   }

   Callcontext_switch();

}


void pubTask_Init(CPU_OS_TCB  _XDATA_ *const  newtcb,
                  uStackType  _IDATA_ *const  pStack,
                  uShortType  const  StackSize,
                  uTask_Entry  const  entry,
                  uBaseType  const pri)
{
    OS_TCB_LIST_HEAD _XDATA_ * _XDATA_ head = 0;
    EA = 0;
    if (StackSize || pStack || entry || (StackSize < 0xff) || (pri < PRI_MAX)){

        newtcb->TopofStack = Stack_Init(pStack, entry);
        newtcb->uxPriority = pri;
        newtcb->tick_wait = 0;
        head = &list_group.os_rdy_list[newtcb->uxPriority];
        ++OS_tasknum;
        ++OS_currtasknum;
        os_tcb_insterTail(head,newtcb);

        if (newtcb->uxPriority >= current_Highest_priority){
            current_Highest_priority = newtcb->uxPriority;
            CurrentOSTCB = newtcb;
        }

    }

    EA = 1;
}

static CPU_OS_TCB _XDATA_ IdleTask_tcb;
static uidataStackType IdleStack[IDLESTACKSIZE];
static void IdleTask(void)
{
    for (;;)
    {
       
    }
}


void OS_Start(void)
{

    EA = 0;
    ET0 = 0;
    TR0 = 0;
    TF0 = 0;		
#if (TIMER0_12T) 
    AUXR &= 0x7F;
    TMOD &= 0xF0;
    TL0 = TIME0_L;
    TH0 = TIME0_H;
#else
    AUXR |= 0x80;
    TMOD &= 0xF0;
    TH0 = TIME0_H;
    TL0 = TIME0_L;
#endif

    NextTaskUnblockTime = DELAY_MAX;
    pos_delay_list = &list_group.os_delay_list;
    pOverflow_delay_list = &list_group.os_Overflow_delay_list;
    
    pubTask_Init(&IdleTask_tcb,
                &IdleStack,
                IDLESTACKSIZE,
                IdleTask,0);
    ScheduleRunning = 1u;
    SchedulerSuspended = 0u;
    SP = (CurrentOSTCB->TopofStack - (uStackType)13u);
    ET0 = 1;
    TR0 = 1;
    EA = 1;
}


