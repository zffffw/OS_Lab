//
//  dlist.cpp
//  DLList2
//s shabi
//  Created by 张 on 2019/3/6.
//  Copyright © 2019 张. All rights reserved.
//

#include "dllist.h"
#include "system.h"

int lineNum = 0;
Lock mutex("test");
void outputCont(const char* s, int which) {
    printf("|(%d) ", lineNum);
    if(which == 1) {
        MOVERIGHT(50);
        printf("%s", s);
    } else if(which == 0){
        MOVERIGHT(3);
        printf("%s", s);
    }
    MOVELEFT(100);
    MOVERIGHT(50);
    printf("|");
    MOVELEFT(100);
    MOVERIGHT(99);
    printf("|\n");
    for(int i = 0; i < 100; ++i) {
		putchar('-');
	}
	putchar('\n');
    lineNum ++;
}

DLLElement::DLLElement(void *itemPtr, int sortKey) {
    item = itemPtr;
    key = sortKey;
    next = NULL;
    prev = NULL;
}

DLList::DLList() {
    first = NULL;
    last = NULL;
}

DLList::~DLList() {
    if(!IsEmpty()) {
        DLLElement *p = first;
        while(p -> next != NULL) {
            p = p->next;
            delete p->prev;
        }
        delete p;
    }
}

void DLList::Prepend(void *item) {
    
    if (IsEmpty()) {
        DLLElement *insertElement = new DLLElement(item, 0);
        first = insertElement;
        last = insertElement;
    } else {
        char tmp[128];
        char* name = currentThread->getName();
        int which = name[1] - '0';
        DLLElement *insertElement = new DLLElement(item, first->key - 1);
        sprintf(tmp, "new DLLElement(%d, first->key - 1)", *((int*)insertElement->item));
        outputCont(tmp, which);
        if(changePoint[1]) {
            currentThread->Yield();
        }
        DLLElement *p = first;
        insertElement->next = p;
        outputCont("insertElement->next = p", which);
        if(changePoint[2]) {
            currentThread->Yield();
        }
        p->prev = insertElement;
        outputCont("p->prev = insertElement", which);
        if(changePoint[3]) {
            currentThread->Yield();
        }
        first = insertElement;
        outputCont("first = insertElement", which);
        if(changePoint[4]) {
            currentThread->Yield();
        }
    }
}




void DLList::Append(void *item) {
    // mutex.Acquire();
    //empty
    if(IsEmpty()) {
        DLLElement *insertElement;
        insertElement = new DLLElement(item, 0);
        first = insertElement;
        last = insertElement;
        // not empty
    } else {
        char tmp[128];
        char* name = currentThread->getName();
        int which = name[1] - '0';

        DLLElement *insertElement = new DLLElement(item, last->key + 1);
        sprintf(tmp, "new DLLElement(%d, last->key + 1)", *((int*)insertElement->item));
        outputCont(tmp, which);

        if(changePoint[1]) {
            currentThread->Yield();
        }
        DLLElement *p = last;
        p->next = insertElement;
        outputCont("p->next = insertElement", which);

        if(changePoint[2]) {
            currentThread->Yield();
        }
        insertElement->prev = p;
        outputCont("insertElement->prev = p", which);

        if(changePoint[3]) {
            currentThread->Yield();
        }
        last = insertElement;
        outputCont("last = insertElement", which);

        if(changePoint[4]) {
            currentThread->Yield();
        }
    }
    // mutex.Release();
}


void *DLList::Remove(int *keyPtr) {
    //not empty
    if(!IsEmpty()) {
        DLLElement *p = first;
        int *tmp = &(first->key);
        keyPtr = tmp;
        //not last
        if(p->next) {
            char t[128];
            char* name = currentThread->getName();
            int which = name[1] - '0';
            sprintf(t, "remove element %d, begin && p=first", *((int*) first->item));
            outputCont(t, which);
            if(changePoint[5]) {
                currentThread->Yield();
            }
            first = p->next;
            outputCont("first = p->next", which);
            if(changePoint[6]) {
                currentThread->Yield();
            }
            
            p->next->prev=NULL;
            outputCont("p->next->prev = NULL", which);
            if(changePoint[7]) {
                currentThread->Yield();
            }
        //last
        } else {
            first = NULL;
            last = NULL;
        }
        return p;
    }
    return NULL;
}

bool DLList::IsEmpty() {
    return first || last? false: true;
}

DLLElement* DLList::getfirst() {
    return first;
}


DLLElement* DLList::getlast() {
    return last;
}

void DLList::SortedInsert(void *item, int sortKey) {
    DLLElement *insertElement = new DLLElement(item, sortKey);
    char t[128];
    char* name = currentThread->getName();
    int which = name[1] - '0';
    //list is empty
    if(IsEmpty()) {
        first = insertElement;
        last = insertElement;
    //not empty
    } else {
        DLLElement *p = first, *prePtr = NULL;
        //find the appropriate position
        while(p->next != NULL && p->key < sortKey) {
            prePtr = p;
            p = p->next;
        }
        sprintf(t, "begin insert element %d (now p=%d,key=%d)", *((int*) insertElement->item), *((int*) p->item), p->key);
        outputCont(t, which);
        //p =first
        if(!prePtr) {
            insertElement->next = p;
            p->prev = insertElement;
            first = insertElement;
        //p != last
        }else if(p->key < sortKey) {
            
            if(changePoint[10]) {
                currentThread->Yield();
            }
            p->next = insertElement;
            outputCont("p->next = insertElement", which);
            if(changePoint[11]) {
                currentThread->Yield();
            }
            insertElement->prev = p;
            outputCont("insertElement->prev = p", which);
            if(changePoint[12]) {
                currentThread->Yield();
            }
            last = insertElement;
            outputCont("last = insertElement", which);
            if(changePoint[13]) {
                currentThread->Yield();
            }
        //prePtr = last
        } else {
            prePtr->next = insertElement;
            outputCont("prePtr->next = insertElement", which);
            if(changePoint[14]) {
                currentThread->Yield();
            }
            insertElement->prev = prePtr;
            outputCont("insertElement->prev = prePtr", which);
            if(changePoint[15]) {
                currentThread->Yield();
            }
            insertElement->next = p;
            outputCont("insertElement->next = p", which);
            if(changePoint[16]) {
                currentThread->Yield();
            }
            p->prev = insertElement;
            outputCont("p->prev = inserELement", which);
            if(changePoint[17]) {
                currentThread->Yield();
            }
            while (p->next != NULL) {
                p->key += 1;
                p = p->next;
            }
        }
        
    }
}

void *DLList::SortedRemove(int sortKey) {
    if(!IsEmpty()) {
        DLLElement *p = first, *prePtr = NULL;
        while(p->next != NULL && p->key != sortKey) {
            prePtr = p;
            p = p->next;
        }
        if(p == first) {
            
            if(p->next) {
                first = p->next;
                p->next->prev = NULL;
            } else {
                last = NULL;
                first = NULL;
            }
            if(p->key == sortKey) 
                    return p;
            else
                return NULL;
        } else if(p->key == sortKey && p->next != NULL) {
            prePtr->next = p->next;
            
            p->next->prev = prePtr;
            
            DLLElement *tmp = p;
            
            return tmp;
        } else if(p->key == sortKey && p->next == NULL){
            prePtr->next = NULL;
            last = prePtr;
            return p;
        } else {
            return NULL;
        }
        
    }
    return NULL;
}



void DLList::printList() {
    DLLElement *p = first;
    if(!p) {
        printf("list is null.");
        return ;
    }

    while(p != NULL && p->next != NULL) {
        if(p == first) {
            printf("[%d %d]", *((int*)p->item), p->key);
        } else {
            printf("-->[%d %d]", *((int*)p->item), p->key);
        }
        p = p->next;
    }
    if(p != NULL && p != first) {
        printf("-->[%d %d]", *((int*)p->item), p->key);
    } else {
        printf("[%d %d]", *((int*)p->item), p->key);
    }
    // printf("\n");
    
}

// int main() {
//     DLList D;
//     for(int i = 0; i < 5; ++i) {
//         int *key = new int;
//         *key = i;

//         D.SortedInsert(key, *key);
//     }
//     for(int i = 0; i < 5; ++i) {
//         DLLElement *tmp = (DLLElement *)(D.SortedRemove(i));
//         printf("[%d %d]\n", *((int *)(tmp->item)), tmp->key);
//     }
//     // char s[]="abcdefg";
//     // // printf("11\n");
//     // char *p = s;
//     // for(int i = 0; i < 6; ++i) {
//     //     putchar(*p);
//     //     D.Prepend(p);
//     //     p++;
//     // }
//     // p = s;
//     // for(int i = 0; i < 6; ++i) {
//     //     putchar(*p);
//     //     D.Append(p);
//     //     p++;
//     // }
//     // putchar('\n');
//     // int *a;
//     // D.Remove(a);
//     // D.printList();
//     // D.Remove(a);
//     // D.printList();
//     // D.Remove(a);
//     // D.printList();
//     // D.SortedInsert(&s[3], 0);
//     // D.printList();
//     // D.SortedRemove(-1);
//     // D.SortedRemove(3);
//     // D.printList();
// }






