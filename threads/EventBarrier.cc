#include "EventBarrier.h"


EventBarrier :: EventBarrier(char* name) {
    waitNum = 0;
    curStats = UNSIGNALED;
    debugName = name;
    signalCon = new Condition("signal con", true);
    completeCon = new Condition("complete con", true);
    signalConLock = new Lock("signalCon lock");
    completeConLock = new Lock("completeCon lock");
}

EventBarrier :: ~EventBarrier() {
    delete signalCon;
    delete completeCon;
    delete signalConLock;
    delete completeConLock;
}

void EventBarrier :: Wait() {
    signalConLock->Acquire();
    waitNum ++;
    if (curStats == SIGNALED) {
        signalConLock->Release();
        return ;
    }
    printf("%s is Wait\n", currentThread->getName());
    signalCon->Wait(signalConLock);
    signalConLock->Release();
    Complete();
}

void EventBarrier :: Signal() {
    signalConLock->Acquire();

    curStats = SIGNALED;
    printf("%s is Signal\n", currentThread->getName());
    signalCon->Broadcast(signalConLock);
    
    completeConLock->Acquire();
    signalConLock->Release();
    completeCon->Wait(completeConLock);
    curStats = UNSIGNALED;
    completeConLock->Release();
}

int EventBarrier :: Waiters() {
    return waitNum;
}

void EventBarrier :: Complete() {
    completeConLock->Acquire();
    waitNum --;
    printf("%s is Complete, %d left.\n", currentThread->getName(), waitNum);
    if(waitNum == 0) {
        printf("111111\n");
        completeCon->Broadcast(completeConLock);
        completeConLock->Release();
        return ;
    }
    completeCon->Wait(completeConLock);
    completeConLock->Release();
}

