#include <stdio.h>
#include <malloc.h>
#include "stack.h"

void Push(struct details **ppHead, struct inner *pIPPushPtr)    // insert first
{
    struct details *pNewNode = NULL;

    pNewNode = (struct details*)malloc(sizeof(struct details));
    if(NULL == pNewNode)
    {
        printf("Memory Allocation Failed\n");
        return;
    }

    pNewNode->pTransDetailsPtr = pIPPushPtr;

    pNewNode->pNext = *ppHead;
    *ppHead = pNewNode;
}

void Display(struct details *pHead)
{
    if(NULL == pHead)
    {   
        printf("No transactions recorded yet\n");
        return;
    }

    while(pHead != NULL)
    {
        printf("%s\t%d\t\t%s\t\t\t%f\n", pHead->pTransDetailsPtr->szTimeLog, pHead->pTransDetailsPtr->iAmount, pHead->pTransDetailsPtr->szAction, pHead->pTransDetailsPtr->fBalance);
        pHead = pHead->pNext;
    }
}

void DeleteAllStack(struct details **ppHead)
{
    struct details *pTemp = NULL;

    if(*ppHead == NULL)
        return;

    while(*ppHead != NULL)
    {
        pTemp = *ppHead;
        *ppHead = (*ppHead)->pNext;
        pTemp->pNext = NULL;
        if(pTemp->pTransDetailsPtr != NULL)
            free(pTemp->pTransDetailsPtr);
        free(pTemp);
    }
    pTemp = NULL;

}
