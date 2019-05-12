// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    printf("%s Pvalue %d\n",name, value);
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	printf("%s is sleeping..\n", currentThread->getName());
    currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    printf("%s %d Vvalue\n",name, value);
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {
    sem = new Semaphore("sem1", 1);
    name = debugName;
}

Lock::~Lock() {
    delete sem;
}
bool Lock::isHeldByCurrentThread() {
    return lockThread != NULL && lockThread == currentThread;
}

void Lock::Acquire() {
    printf("%s acquare lock %s \n", currentThread->getName(), name);
    sem->P();
    lockThread = currentThread;
}

void Lock::Release() {
    printf("%s release lock %s \n", currentThread->getName(), name);
    ASSERT(isHeldByCurrentThread());
    sem->V();
    lockThread = NULL;
}

Condition::Condition(char* debugName, bool isBarrier) { 
    name = debugName;
    if (isBarrier) {
        sem = new Semaphore("cond sem", 0);
    } else {
        sem = new Semaphore("cond sem", 1);
    }
    numWaiting = 0;
}
Condition::~Condition() { 
    delete sem;
}

void Condition::Wait(Lock* conditionLock) {
    printf("%s is conWait...\n", currentThread->getName()); 
    ASSERT(conditionLock->isHeldByCurrentThread()); 
    numWaiting ++;
    conditionLock->Release();
    
    sem->P();
    conditionLock->Acquire();
}
void Condition::Signal(Lock* conditionLock) {
    ASSERT(conditionLock->isHeldByCurrentThread());
    if(numWaiting) {
        sem->V();
        numWaiting--;
    }
}
void Condition::Broadcast(Lock* conditionLock) {
    printf("%s is Signal, %s\n", currentThread->getName(), (conditionLock->lockThread)->getName());
    ASSERT(conditionLock->isHeldByCurrentThread());
    printf("%s is broadcasting.\n", currentThread->getName());
    while(numWaiting){
        sem->V();
        printf("%d\n", numWaiting);
        numWaiting--;
    }
}
