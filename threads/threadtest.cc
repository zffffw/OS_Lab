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
#include "EventBarrier.h"
#include "BoundedBuffer.h"
class EventBarrier;
// testnum is set in main.cc
int testnum = 1;
Table testTable(10);
Lock test_lock("test_lock");
Condition test_condition("test_condition");
BoundedBuffer buffer(30);
EventBarrier barrier("test barrier");
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

void doBarrierThreadTestWait(int which) {
    printf("%d is doing test.\n", which);
    barrier.Wait();
}

void BarrierThreadTest() {
    Thread* t[5];
    // barrier.Wait();
    for (int i = 0; i < 5; ++i) {
        char* name;
        name = new char[2];
        name[0] = i + '0';
        t[i] = new Thread(name);
    }
    for(int i = 0; i < 5; ++i) {
        t[i]->Fork(doBarrierThreadTestWait, i);
    }
    currentThread->Yield();
    printf("%s 111\n", currentThread->getName());
    barrier.Signal();
    
    currentThread->Yield();
    printf("main-complete.\n");

    for (int i = 0; i < 5; ++i) {
        char* name;
        name = new char[3];
        name[0] = i + '0';
        name[1] = 0;
        t[i] = new Thread(name);
    }
    for(int i = 0; i < 5; ++i) {
        t[i]->Fork(doBarrierThreadTestWait, i);
    }
    currentThread->Yield();
    printf("%s 111\n", currentThread->getName());
    barrier.Signal();
    currentThread->Yield();
}

void alarmOperate(int arg) {
    
    printf("%s is doing alarm test.\n", currentThread->getName());
    myalarm->Pause(arg);
    printf("%s is wake up ...\n", currentThread->getName());
}

void alarmtest() {
    Thread *t[10];
    for(int i = 0; i < 10; ++i) {
        char *name = new char[3];
        name[0] = i + '0';
        t[i] = new Thread(name);
        t[i]->Fork(alarmOperate, (10 - i)*100);
    }
    currentThread->Yield();
}

void TestBoundedBuffer(int which) {
    printf("*** thread %d begins to run\n", which);
    int num;
    for (num = 0; num < 5; num++) {
        if (which) {
            char writeMsg[21] = "WriteByThread1,Time1";
            writeMsg[13] = which + '0';
            writeMsg[19] = num + '0';

            printf("   Thread %d, Times %d, Write: %s\n", which, num, writeMsg);
            buffer.Write(writeMsg, 20);
        } else {
            char readMsg[21] = "                   ";
            buffer.Read(readMsg, 20);
            printf("   Thread %d, Times %d, Read: %s\n", which, num, readMsg);
            printf("*** thread %d looped %d times\n", which, num + 1);
        }
        
	    printf("*** thread %d looped %d times\n", which, num + 1);
    }
}

int ThreadTestForBoundedBuffer() {
    Thread *t0 = new Thread("t0");
    Thread *t1 = new Thread("t1");
    t0->Fork(TestBoundedBuffer, 0);
    t1->Fork(TestBoundedBuffer, 1);
    return 0;
}

// thread0 and thread1 do different things
void TableTest(int which) {
    const char test1[10][10] = {"一", "二", "三", "四", "五", "六", "七", "八", "九", "十"};
    const char test2[10][10] = {"幺", "贰", "叁", "肆", "伍", "陆", "柒", "捌", "玖", "拾"};
    switch (which)
    {
        case 0: {
            for(int i = 0; i < 10; ++i) {
                testTable.Alloc((void *)test1[i]);
            }
            // testTable.Release(5);
        } break;
        case 1: {
            for(int i = 0; i < 10; ++i) {
                testTable.Alloc((void *)test2[i]);
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
    case 4:
        TableTestDriver();
    break;
    case 5:
        ThreadTestForBoundedBuffer();
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
    case 6:
        BarrierThreadTest();   
    break;
    case 7:
        alarmtest();
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}

