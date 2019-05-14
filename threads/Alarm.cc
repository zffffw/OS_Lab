

#include "Alarm.h"
#include "system.h"
#include "synch.h"
Alarm :: Alarm(char* debugname) {
    name = debugname;
    queue = new List();
}

Alarm :: ~Alarm() {
    delete queue;
}


void test(int tmp) {
    if(myalarm->queue->IsEmpty()) {
        currentThread->Finish();
    } else {
        currentThread->Yield();
    }
}

void Alarm :: Pause(int howLong) {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    if(queue->IsEmpty()) {
        Thread* sauce = new Thread("sauce thread");
        sauce->Fork(test, 0);
    }
    queue->SortedInsert((void *) currentThread, stats->totalTicks + howLong);
    (void) interrupt->SetLevel(oldLevel);
    currentThread->Sleep();
}

void wakeup(int tmp) {
    ListElement* p = (ListElement *) tmp;
    if (p->key <= stats->totalTicks) {
        scheduler->ReadyToRun((Thread *)p->item);
    }
    (myalarm->queue)->Remove();
}

void Alarm :: searchAndWakeUp() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    queue->Mapcar(wakeup);
    (void) interrupt->SetLevel(oldLevel);
}
