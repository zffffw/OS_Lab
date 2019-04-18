//
//  main.cpp
//  DLList2
//
//  Created by 张 on 2019/3/6.
//  Copyright © 2019 张. All rights reserved.
//


#include "dllist.h"
#include "thread.h"
#include "system.h"
void outputStatus(DLList*, int , char* );

void showAllElem(DLList *D) {
    DLLElement *p = D->getfirst();
    while(p != NULL && p != D->getlast()) {
        printf("%d-->", *((int*)p->item));
        p = p->next;
    }
    printf("%d\n", *(int*)p->item);
}

//for thread i, insert i*5, i*5 + 1, ... , i*5 + 4
void insertOrderNum(DLList *D, int which) {
    for(int num = which*5; num < which*5 + 5; num++) {
        int *key = new int;
        *key = num;
        D->Append(key);
    }
    outputStatus(D, which, " is complete");
}

//for thread i, insert i*5, i*5 + 1, ... , i*5 + 4
void sortInsertNum(DLList *D, int which) {
    for(int num = which*5; num < which*5 + 5; num++) {
        int *key = new int;
        *key = num;
        D->SortedInsert(key, *key);
    }
    outputStatus(D, which, " is complete");
}

//remove the list until it is empty
void removeElem(DLList *D, int which) {
    while(!D->IsEmpty()) {
        int tmp;
        D->Remove(&tmp);
        outputStatus(D, which, "'s current status");
    }
    
}

//prepend i*5, i*5 + 1, ... , i*5 + 4
void insertPrepend(DLList *D, int which) {
    for(int num = which*5; num < which*5 + 5; num++) {
        int *key = new int;
        *key = num;
        D->Prepend(key);
    }
    outputStatus(D, which, " is complete");
}



void outputStatus(DLList *D,  int which, char* current) {
    printf("|");
    MOVERIGHT(40);
    printf("thread%d%s: ", which, current);
    MOVELEFT(100);
    MOVERIGHT(99);
    printf("|\n");
    printf("|");
    MOVERIGHT(5);
    D->printList();
    MOVELEFT(100);
    MOVERIGHT(99);
    printf("|\n");
    for(int i = 0; i < 100; ++i) {
		putchar('-');
	}
	putchar('\n');
}



