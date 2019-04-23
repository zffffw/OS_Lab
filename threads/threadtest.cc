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

#include "Table.h"
#include "thread.h"
#include "dllist.h"
#include "system.h"
// testnum is set in main.cc
int testnum = 1;
Table testTable(10);
Lock test_lock("test_lock");
Condition test_condition("test_condition");

//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------
void outputStatus(DLList*, int , char* );
void showAllElem(DLList *D);
void insertOrderNum(DLList *D, int which);
void sortInsertNum(DLList *D, int which);
void outputStatus(DLList *D,  int which, char* current);
void ASimpleRemoveTest(int which);
void removeElem(DLList *D, int which);

void
SimpleThread(int which)
{
    int num;
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}


void ASimpleThread(int which) {
    insertOrderNum(&DL, which);
}

void ASimpleRemoveTest(int which) {
    // test_lock.Acquire();
    insertOrderNum(&DL, which);
    removeElem(&DL, which);
    // test_lock.Release();
}
void ASimpleRemoveTest2(int which) {
    sortInsertNum(&DL, which);
}


// thread0 and thread1 do different things
void TableTest(int which) {
    const char test1[10][10] = {"一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
    const char test2[10][10] = {"幺", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖", "拾"};
    switch (which)
    {
        case 0: {
            for(int i = 0; i < 10; ++i) {
                printf("%s: %d \n", currentThread->getName(), testTable.Alloc((void *)test1[i]));
            }
            // testTable.Release(5);
        } break;
        case 1: {
            for(int i = 0; i < 10; ++i) {
                printf("%s: %d \n", currentThread->getName(), testTable.Alloc((void *)test2[i]));
            }
        }
    }
    testTable.Release(5);
    testTable.Release(4);
}

void TableTestDriver() {
    Thread* t0 = new Thread("t0");
    Thread* t1 = new Thread("t1");
    t0->Fork(TableTest, 0);
    t1->Fork(TableTest, 1);
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

    Thread *t1 = new Thread("t0");
    Thread *t2 = new Thread("t1");
    int *key = new int;
    *key = 100;
    DL.Append(key);
    t1->Fork(ASimpleThread, 0);
    t2->Fork(ASimpleThread, 1);
    
}

void ThreadTest2() {
    Thread *t1 = new Thread("t0");
    Thread *t2 = new Thread("t1");
    int *key = new int;
    *key = 100;
    DL.Append(key);
    t1->Fork(ASimpleRemoveTest, 0);
    t2->Fork(ASimpleRemoveTest, 1);
}
void ThreadTest3() {
    Thread *t1 = new Thread("t0");
    Thread *t2 = new Thread("t1");
    int *key = new int;
    *key = 100;
    DL.Append(key);
    t1->Fork(ASimpleRemoveTest2, 0);
    t2->Fork(ASimpleRemoveTest2, 1);
}



//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest()
{
    switch (testnum) {
    case 0:
        TableTestDriver();
    break;
    case 1:
	    ThreadTest1();
    break;
    case 2:
        ThreadTest2();
    break;
    case 3:
        ThreadTest3();
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}

