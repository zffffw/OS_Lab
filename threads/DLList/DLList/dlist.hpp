//
//  dlist.hpp
//  DLList
//
//  Created by 张 on 2019/3/6.
//  Copyright © 2019 张. All rights reserved.
//




#include <iostream>
using namespace std;
class DLLElement {
public:
    DLLElement( void *itemPtr, int sortKey ); // initialize a list element
    DLLElement *next;
    DLLElement *prev;
    int key;
    void *item;
};

class DLList {
public:
    DLList();
    ~DLList();
    // next element on list
    // NULL if this is the last // previous element on list // NULL if this is the first
    // priority, for a sorted list // pointer to item on the list
    // initialize the list // de-allocate the list
    void Prepend(void *item);
    void Append(void *item);
    void *Remove(int *keyPtr); // remove from head of list
    // add to head of list (set key = min_key-1) // add to tail of list (set key = max_key+1)
    // set *keyPtr to key of the removed item // return item (or NULL if list is empty)
    bool IsEmpty(); // return true if list has elements
    // routines to put/get items on/off list in order (sorted by key)
    void SortedInsert(void *item, int sortKey);
    void *SortedRemove(int sortKey); // remove first item with key==sortKey
    // return NULL if no such item exists
    void printList();
private:
    DLLElement *first; // head of the list, NULL if empty
    DLLElement *last; // last element of the list, NULL if empty
};
