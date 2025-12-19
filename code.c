#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <windows.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <conio.h>
#include "stack.h"

#define ACCOUNT_NO 1000
#define CUSTOMER_ID 5000

struct account
{
    struct account *pPrev;
    int iAccountNo;
    int iCustomerID;
    char szName[50];
    char szAddress[100];
    char szPhoneNo[13];
    char szDOB[11];
    char chGender;
    float fBalance;
    struct inner *pTransDetails;
    struct details **pStackPtr;
    struct account *pNext;
};

int g_iUnique;

void Menu(struct account **, struct account **);
void CreateNewAccount(struct account **);
void DepositMoney(struct account **);
void WithdrawMoney(struct account **);
void BalanceInquiry(struct account *);
void MiniStatement(struct account *);
void TransferMoney(struct account **, struct account **);
struct account *UserLogin(struct account *);
void AccountDetails(struct account *);
void GetTimeLog(char []);
void DeleteAll(struct account **);

int main(void)
{
    system("cls");
    short int iCounter;
    struct account *pFirst = NULL;
    struct account *pReturn = NULL;

    while (1)
    {
        for (iCounter = 0; iCounter < 30; iCounter++)
            printf("=");

        printf("\nWelcome to Online Banking Management System!\n");
        printf("1. Existing Customer Login\n2. New Customer\n3. Press any key to exit\nEnter your choice : ");
        scanf("%d", &iCounter);

        if (iCounter == 1)
        {
            pReturn = UserLogin(pFirst);
        }
        else if (iCounter == 2)
        {
            CreateNewAccount(&pFirst);
            continue;
        }
        else
            break;

        Menu(&pFirst, &pReturn);
    }

    DeleteAll(&pFirst);
    return 0;
}

void Menu(struct account **ppHead, struct account **ppPtr)
{
    short int iCounter;

    while(1)
    {
        printf("\nWelcome!!What would you like to do? ");
        printf("\n1. Deposit Money\n2. Withdraw Money\n3. Transfer Money\n4. Balance Inquiry\n5. Mini Statement\n6. Account Details\n7. Logout\n");

        for (iCounter = 0; iCounter < 30; iCounter++)
            printf("=");

        printf("\nEnter your choice : ");
        scanf("%d", &iCounter);

        switch (iCounter)
        {
        case 1:
            // system("cls");
            DepositMoney(ppPtr);
            break;
        case 2:
            // system("cls");
            WithdrawMoney(ppPtr);
            break;
        case 3:
            // system("cls");
            TransferMoney(ppHead, ppPtr);
            break;
        case 4:
            // system("cls");
            BalanceInquiry(*ppPtr);
            break;
        case 5:
            // system("cls");
            MiniStatement(*ppPtr);
            break;
        case 6:
            // system("cls");
            AccountDetails(*ppPtr);
            break;
        case 7:
            printf("Logging out..");
            return;
        default:
            printf("Invalid Choice. Try Again\n");
        }
    }
    sleep(1);
}

struct account *UserLogin(struct account *pHead)
{
    system("cls");
    int iConsumerID;
    printf("\n****************Customer login***************\n");
    printf("Enter your Customer ID : ");
    scanf("%d", &iConsumerID);

    while (pHead != NULL)
    {
        if (pHead->iCustomerID == iConsumerID)
        {
            printf("Login Successfull\n");
            return pHead;
        }
        pHead = pHead->pNext;
    }

    printf("Incorrect Consumer ID\n");
    return NULL;
}

void CreateNewAccount(struct account **ppHead)              // Insert Last
{
    system("cls");

    struct account *pNewNode = NULL;
    struct account *pTemp = NULL;

    pNewNode = (struct account *)malloc(sizeof(struct account));
    if (NULL == pNewNode)
    {
        perror("Memory Allocation Failed");
        return;
    }

    printf("\nWelcome New User!!!\nLet's create you a new account\n");
    fflush(stdin);
    printf("Enter your name : ");
    gets(pNewNode->szName);

    printf("Enter your Date of Birth (DD/MM/YYYY): ");
    gets(pNewNode->szDOB);

    printf("Enter your address : ");
    gets(pNewNode->szAddress);

    printf("Enter your phone no. : ");
    gets(pNewNode->szPhoneNo);

    printf("Enter your Gender (M/F) : ");
    fflush(stdin);
    scanf("%c", &pNewNode->chGender);

    g_iUnique++;
    pNewNode->iAccountNo = ACCOUNT_NO + g_iUnique;
    pNewNode->iCustomerID = CUSTOMER_ID + g_iUnique;
    pNewNode->fBalance = 0.0f;
    pNewNode->pTransDetails = NULL;
    
    pNewNode->pTransDetails = (struct inner*)malloc(sizeof(struct inner));  
    if(NULL == pNewNode->pTransDetails)
    {
        printf("memory allocation failed\n");
        return;
    }

    pNewNode->pTransDetails->iAmount = 0;
    GetTimeLog(pNewNode->pTransDetails->szTimeLog);
    strcpy(pNewNode->pTransDetails->szAction, "Account Created");
    pNewNode->pTransDetails->fBalance = pNewNode->fBalance;

    pNewNode->pNext = NULL;
    pTemp = *ppHead;
    if (NULL == *ppHead)
    {
        *ppHead = pNewNode;
        pNewNode->pPrev = NULL;
    }
    else
    {
        while (pTemp->pNext != NULL)
            pTemp = pTemp->pNext;

        pNewNode->pPrev = pTemp;
        pTemp->pNext = pNewNode;
    }

    pNewNode->pStackPtr =  malloc(sizeof(struct details));
    *(pNewNode->pStackPtr) = NULL;
    if(pNewNode->pStackPtr == NULL)
    {
        perror("Memory allocation failed\n");
        return;
    }
    Push(pNewNode->pStackPtr, pNewNode->pTransDetails);

    printf("Account created successfully!!\n");
    AccountDetails(pNewNode);
    printf("\nPlease deposit minimum amount of 1000 in your account\n");
    DepositMoney(&pNewNode);
}

void DepositMoney(struct account **ppPtr)
{
    int iDepositAmount;
    char szAction[3] = "Cr";

    printf("Enter the amount you want to deposit : ");
    scanf("%d", &iDepositAmount);

    (*ppPtr)->fBalance += iDepositAmount;

    (*ppPtr)->pTransDetails = (struct inner*) malloc (sizeof(struct inner));
    if(NULL == (*ppPtr)->pTransDetails)
    {
        printf("memory allocation failed\n");
        return;
    }

    GetTimeLog((*ppPtr)->pTransDetails->szTimeLog);
    (*ppPtr)->pTransDetails->iAmount = iDepositAmount;
    (*ppPtr)->pTransDetails->fBalance = (*ppPtr)->fBalance;
    strcpy((*ppPtr)->pTransDetails->szAction, szAction);

    Push((*ppPtr)->pStackPtr, (*ppPtr)->pTransDetails);

    printf("Amount %d deposited successfully\n", (*ppPtr)->pTransDetails->iAmount);
    printf("Your current balance : %f", (*ppPtr)->fBalance);

    printf("\nPress any key to continue\n");
    getch();
}

void WithdrawMoney(struct account **ppPtr)
{
    int iWithdrawAmt;

    if(NULL == *ppPtr)
    {
        return;
    }

    printf("Enter amount to withdraw : ");
    scanf("%d", &iWithdrawAmt);

    if ((*ppPtr)->fBalance >= iWithdrawAmt)
        (*ppPtr)->fBalance -= iWithdrawAmt;
    else
    {    
        printf("Insufficient funds in your account\n");
        return;
    }

    (*ppPtr)->pTransDetails = (struct inner*) malloc (sizeof(struct inner));
    if(NULL == (*ppPtr)->pTransDetails)
    {
        printf("memory allocation failed\n");
        return;
    }
    GetTimeLog((*ppPtr)->pTransDetails->szTimeLog);
    (*ppPtr)->pTransDetails->iAmount = iWithdrawAmt;
    strcpy((*ppPtr)->pTransDetails->szAction, "Deb");
    (*ppPtr)->pTransDetails->fBalance = (*ppPtr)->fBalance;

    Push((*ppPtr)->pStackPtr, (*ppPtr)->pTransDetails);

    printf("Amount %d withdrawn successfully\n", iWithdrawAmt);
    printf("Current Balance : %f", (*ppPtr)->fBalance);
    printf("\nPress any key to continue\n");
    getch();
}

void TransferMoney(struct account **ppHead, struct account **ppPtr)
{
    int iTransferAmt;
    int iAccountNo;
    int iAccountNo2;
    struct account *pTemp = NULL;

    if(*ppHead == NULL || *ppPtr == NULL)
        return;

    while(1)
    {
        printf("Enter Account number of Receipent : ");
        scanf("%d", &iAccountNo);
        pTemp = *ppHead;

        while (pTemp != NULL)
        {
            if (iAccountNo == pTemp->iAccountNo)
            {
                break;
            }
            else if(iAccountNo == (*ppPtr)->iAccountNo)
            {
                printf("Transferring to Self Account not allowed\n");
                return;
            }
            pTemp = pTemp->pNext;
        }

        if (pTemp == NULL)
        {
            char chChar;
            printf("Account not found\n Try again? (Y/N) : ");
            fflush(stdin);
            scanf("%c", &chChar);
            if(chChar == 'Y'|| chChar == 'y')
                continue; 
            else
                break;
        }

        printf("Enter amount to transfer : ");
        scanf("%d", &iTransferAmt);

        if ((*ppPtr)->fBalance >= iTransferAmt)
        {
            (*ppPtr)->fBalance -= iTransferAmt;
            pTemp->fBalance += iTransferAmt;
            break;
        }
        else
        {
            printf("Insufficent funds in your account\n");
            printf("Transfer failed\n");
            return;
        }
    }

    (*ppPtr)->pTransDetails = (struct inner*) malloc (sizeof(struct inner));
    if(NULL == (*ppPtr)->pTransDetails)
    {
        printf("memory allocation failed\n");
        return;
    }

    GetTimeLog((*ppPtr)->pTransDetails->szTimeLog);
    (*ppPtr)->pTransDetails->iAmount = iTransferAmt;
    strcpy((*ppPtr)->pTransDetails->szAction, "Deb");
    (*ppPtr)->pTransDetails->fBalance = (*ppPtr)->fBalance;

    Push((*ppPtr)->pStackPtr, (*ppPtr)->pTransDetails);

    pTemp->pTransDetails = (struct inner*) malloc (sizeof(struct inner));
    if(NULL == pTemp->pTransDetails)
    {
        printf("memory allocation failed\n");
        return;
    }
    GetTimeLog(pTemp->pTransDetails->szTimeLog);
    pTemp->pTransDetails->iAmount = iTransferAmt;
    strcpy(pTemp->pTransDetails->szAction, "Cr");
    pTemp->pTransDetails->fBalance = pTemp->fBalance;

    Push(pTemp->pStackPtr, pTemp->pTransDetails);

    printf("Amount %d transferred to Account no. %d successfully\n", iTransferAmt, iAccountNo);
    printf("Current Balance : %f", (*ppPtr)->fBalance);

    pTemp = NULL;
    printf("\nPress any key to continue\n");
    getch();
}

void BalanceInquiry(struct account *pHead)
{
    if(pHead == NULL)
        return;

    printf("**********Showing Current Balance***********\n");

    printf("Current Balance : %f\n", pHead->fBalance);

    printf("Press any key to continue\n");
    getch();
}

void MiniStatement(struct account *pPtr)
{
    if(NULL == pPtr)
        return;

    printf("**********Mini Statement***********\n");
    printf("Name of Account Holder\t: %s\n", pPtr->szName);
    printf("Account No\t\t: %d\n", pPtr->iAccountNo);
    printf("Customer ID\t\t: %d\n", pPtr->iCustomerID);
    printf("Available Balance\t: Rs.%f\n\n", pPtr->fBalance);
    printf("Recent Transactions\n");
    printf("Date & Time\t\tAmount\t\tCredit/Debit\t\tBalance\n");
    Display(*(pPtr->pStackPtr)); 
    printf("\nPress any key to continue\n");
    getch();
}

void AccountDetails(struct account *pHead)
{
    if(NULL == pHead)
        return;

    printf("\nAccount Details are as follows :\n");
    printf("Customer ID\t: %d\n", pHead->iCustomerID);
    printf("Account No.\t: %d\n", pHead->iAccountNo);
    printf("Name\t\t: %s\n", pHead->szName);
    printf("Date of Birth\t: %s\n", pHead->szDOB);
    printf("Phone No.\t: %s\n", pHead->szPhoneNo);
    printf("Address\t\t: %s\n", pHead->szAddress);
    printf("Gender\t\t: %c\n", pHead->chGender);
    printf("Current Balance\t: %.2f\n", pHead->fBalance);

    printf("\nPress any key to continue\n");
    getch();
}

void GetTimeLog(char szBuffer[])
{
    time_t Time = time(NULL);
    struct tm oTime = *localtime(&Time);

    strftime(szBuffer, 60, "%d-%m-%Y %I:%M:%S", &oTime);
}

void DeleteAll(struct account **ppHead)
{
    if(NULL == *ppHead)
        return;

    struct account *pTemp = NULL;
    while(*ppHead != NULL)
    {
        pTemp = *ppHead;
        *ppHead = (*ppHead)->pNext;
        if(pTemp->pTransDetails != NULL)
        {
            pTemp->pTransDetails->fBalance = 0;
            pTemp->pTransDetails->iAmount = 0;
            free(pTemp->pTransDetails);
            pTemp->pTransDetails = NULL;
        }
        if(pTemp->pStackPtr != NULL)
        {
            DeleteAllStack(pTemp->pStackPtr);
            free(pTemp->pStackPtr);
            pTemp->pStackPtr = NULL;
        }
        pTemp->pNext = NULL;
        free(pTemp);
    }
    pTemp = NULL;
    // printf("All memory freed successfully\n");
}