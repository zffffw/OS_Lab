//
//  dlist.cpp
//  DLList2
//s
//  Created by 张 on 2019/3/6.
//  Copyright © 2019 张. All rights reserved.
//

#include "dlist.h"
#include "thread.h"
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
        DLLElement *insertElement = new DLLElement(item, first->key - 1);
        DLLElement *p = first;
        insertElement->next = p;
        p->prev = insertElement;
        first = insertElement;
    }
}

void DLList::Append(void *item) {
    
    //empty
    if(IsEmpty()) {
        DLLElement *insertElement;
        insertElement = new DLLElement(item, 0);
        first = insertElement;
        last = insertElement;
        // not empty
    } else {
        DLLElement *insertElement = new DLLElement(item, last->key + 1);
        printf("(1)--->%s: do new DLLElement, next command is [ p->next = insertElement ]\n", currentThread->getName());
        if(changePoint[1]) {
            currentThread->Yield();
        }
        DLLElement *p = last;
        p->next = insertElement;
        printf("(2)--->%s: do p->next = insertElement, next command is [ insertElement->prev = p ]\n", currentThread->getName());
        if(changePoint[2]) {
            currentThread->Yield();
        }
        insertElement->prev = p;
        printf("(3)--->%s: do insertElement->prev = p, next command is [ last = insertElement ]\n", currentThread->getName());
        if(changePoint[3]) {
            currentThread->Yield();
        }
        last = insertElement;
        printf("(4)--->%s: do last = insertElement, no next command\n", currentThread->getName());
        if(changePoint[4]) {
            currentThread->Yield();
        }
    }
}


void *DLList::Remove(int *keyPtr) {
    if(!IsEmpty()) {
        DLLElement *p = first;
        int *tmp = &(first->key);
        keyPtr = tmp;
        if(p->next) {
            first=p->next;
            p->next->prev=NULL;
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
    if(IsEmpty()) {
        first = insertElement;
        last = insertElement;
    } else {
        DLLElement *p = first, *prePtr = NULL;
        while(p->next != NULL && p->key < sortKey) {
            prePtr = p;
            p = p->next;
        }
        if(!prePtr) {
            insertElement->next = p;
            p->prev = insertElement;
            first = insertElement;
        }else if(p->key < sortKey) {
            p->next = insertElement;
            insertElement->prev = p;
            last = insertElement;
        } else {
            prePtr->next = insertElement;
            insertElement->prev = prePtr;
            insertElement->next = p;
            p->prev = insertElement;
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
            first = p->next;
            if(p->next)
                p->next->prev = NULL;
            else
                last = NULL;
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
    // cout << first->key << endl;
    while(p != NULL && p->next != NULL) {
        if(p == first) {
            printf("[%c %d]", *(char*)p->item, p->key);
        } else {
            printf("-->[%c %d]", *(char*)p->item, p->key);
        }
        p = p->next;
    }
    printf("-->[%c %d]", *(char*)p->item, p->key);
    printf("\n");
    
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






