#include "c51list.h"

void listInit(C51_LIST_HEAD xdata *head)
{
    head->pindex = &(head->enditem);
    head->pindex->next = head->pindex;
    head->Num = 0;
}

void insertHead(C51_LIST_HEAD xdata *head,C51_LIST_ITEM xdata *newitem)
{
    newitem->next = head->pindex->next;
    head->pindex->next = newitem;
    head->Num++ ;
}

void deleteitem(C51_LIST_HEAD xdata *head,C51_LIST_ITEM xdata *item)
{
    C51_LIST_ITEM xdata *pindex,xdata *Previous;

    for (pindex = head->pindex->next,Previous = head->pindex;
        pindex != &(head->enditem);
        Previous = pindex,pindex = pindex->next){
        if (pindex == item)
        {
            Previous->next = pindex->next;
            pindex->next = 0;
            head->Num--;
            return;
        }
    }
}


