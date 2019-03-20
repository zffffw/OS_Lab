//
//  main.cpp
//  DLList2
//
//  Created by 张 on 2019/3/6.
//  Copyright © 2019 张. All rights reserved.
//


#include "dlist.cc"

void showAllElem(DLList *D) {
    DLLElement *p = D->getfirst();
    while(p != D->getlast()) {
        printf("%d-->", *((int*)p->item));
        p = p->next;
    }
    printf("%d\n", *(int*)p->item);
}

void insertOrderNum(DLList *D, int which) {
    for(int num = which*5; num < which*5 + 5; num++) {
        int *key = new int;
        *key = num;
        printf("*** thread %d insert number %d\n", which, num);
        D->Append(key);
        showAllElem(D);
        // currentThread->Yield(); 
    }
    showAllElem(D);
}


void removeAndPrintIt(DLList *D) {
    int *ptr;
    while(!D->IsEmpty()) {
        DLLElement *tmp = (DLLElement *)(D->Remove(ptr));
        printf("[%d %d]  ", *((int *)tmp->item), tmp->key);
    }
}

// int main() {
//     DLList D;
//     generateNandInsert(&D);
//     removeAndPrintIt(&D);
// }
