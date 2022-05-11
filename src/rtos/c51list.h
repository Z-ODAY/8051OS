#ifndef _C51_LIST_H_
#define _C51_LIST_H_

#include "cpudatatype.h"

typedef struct C51_LIST_ITEM C51_LIST_ITEM;
typedef struct C51_LIST_HEAD C51_LIST_HEAD;

struct C51_LIST_ITEM{
    C51_LIST_ITEM xdata *next;
};

struct C51_LIST_HEAD{
    C51_LIST_ITEM xdata *pindex;
    C51_LIST_ITEM enditem;
    uBaseType Num;
};

void listInit(C51_LIST_HEAD xdata *head);
void insertEnd(C51_LIST_HEAD xdata *head,C51_LIST_ITEM xdata *newitem);
void deleteitem(C51_LIST_HEAD xdata *head,C51_LIST_ITEM xdata *item);

#endif //_C51_LIST_H_

