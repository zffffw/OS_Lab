

#ifndef EVENTBARRIER_H
#define EVENTBARRIER_H
#include "synch.h"
#include "thread.h"
#include "system.h"
enum BarrierState{UNSIGNALED, SIGNALED};

class EventBarrier {
    public:
        EventBarrier(char* name);
        ~EventBarrier();
        void Wait();
        void Signal();
        void Complete();
        int Waiters();
        int getStats() { return curStats; };
    private:
        int waitNum;
        char* debugName;
        Condition* signalCon;
        Condition* completeCon;
        BarrierState curStats;
        Lock* signalConLock;
        Lock* completeConLock;
        int n;
};

#endif
