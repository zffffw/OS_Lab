//
//  main.cpp
//  DLList2
//
//  Created by 张 on 2019/3/6.
//  Copyright © 2019 张. All rights reserved.
//


#include <random>
#include "dlist.cc"

void generateNandInsert(DLList *D) {
    srand((unsigned)time(NULL));
    for (size_t i = 0; i < 10; ++i) { //生成十个随
        int* key = new int;
        *key = (int)((rand() % 100));
//        cout << key << endl;
        D->SortedInsert(key, *key);
    }
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
