// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "dlist.cc"
#include "thread.h"
// testnum is set in main.cc
int testnum = 1;

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------
DLList D;
void showAllElem();
void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}


void ASimpleThreadOne(int which) {
    for(int num = 0; num < 5; num++) {
	    int *key = new int;
	    *key = num;
	    printf("*** thread %d insert number %d\n", which, num);
	    D.Append(key);
        showAllElem();
        // currentThread->Yield(); 
    }
    showAllElem();
}
void ASimpleThreadTwo(int which) {
    for(int num = 5; num < 10; num++) {
	    int *key = new int;
	    *key = num;
	    printf("*** thread %d insert number %d\n", which, num);
	    D.Append(key);
        showAllElem();
        // currentThread->Yield(); 
    }
    showAllElem();
    
}


void showAllElem() {
    // int *ptr;
    DLLElement *p = D.getfirst();
    while(p != D.getlast()) {
        printf("%d-->", *((int*)p->item));
        p = p->next;
    }
    printf("%d\n", *(int*)p->item);
}


//----------------------------------------------------------------------
// ThreadTest1
// 	Set up a ping-pong between two threads, by forking a thread 
//	to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t1 = new Thread("thread1");
    Thread *t2 = new Thread("thread2");
    int *key = new int;
    *key = 100;
    D.Append(key);
    t1->Fork(ASimpleThreadOne, 1);
    t2->Fork(ASimpleThreadTwo, 2);

}



//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 1:
	ThreadTest1();
	break;
    default:
	printf("No test specified.\n");
	break;
    }
}

