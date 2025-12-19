#pragma once
#include <stdio.h>
#include <malloc.h>

struct inner
{
    char szTimeLog[60];
    char szAction[20];
    int iAmount;
    float fBalance;
};

struct details
{
    struct inner *pTransDetailsPtr;
    struct details *pNext;
};

void Push(struct details **, struct inner *);
int CountNodes(struct details *);
void Display(struct details *);
void DeleteAllStack(struct details **);

