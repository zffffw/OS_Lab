

#include "Alarm.h"
#include "system.h"
#include "synch.h"

int removeNum = 0;

Alarm :: Alarm(char* debugname) {
    name = debugname;
    queue = new List();
}

Alarm :: ~Alarm() {
    delete queue;
}


void test(int tmp) {
    while(true) {
        if(myalarm->queue->IsEmpty()) {
            // printf("1111\n");
            currentThread->Finish();
        } else {
            // printf("2222\n");
            currentThread->Yield();
        }
    }
}

void Alarm :: Pause(int howLong) {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    // if queue is empty, we need a sauce thread to keep the system active.
    if(queue->IsEmpty()) {
        // printf("sauce thread is been created\n");
        Thread* sauce = new Thread("sauce thread");
        sauce->Fork(test, 0);
    }
    // alarmItem is a item for queue
    alarmItem* tmp = new alarmItem;
    tmp->sleepThread = currentThread;
    tmp->wakeupTime = stats->totalTicks + howLong;
    printf("%s is sleeping...\n", currentThread->getName());
    printf("wakeup time : %d\n", stats->totalTicks + howLong);
    queue->SortedInsert((void *) tmp, stats->totalTicks + howLong);
    currentThread->Sleep();
}

void wakeup(int tmp) {
    alarmItem* p = (alarmItem *) tmp;
    if (p->wakeupTime <= stats->totalTicks) {
        scheduler->ReadyToRun(p->sleepThread);
        removeNum ++;
    }
}
void print(int tmp) {
    alarmItem* p = (alarmItem *) tmp;
    printf("%s->", p->sleepThread->getName());
}

void Alarm :: searchAndWakeUp() {
    IntStatus oldLevel = interrupt->SetLevel(IntOff);
    // queue->Mapcar((VoidFunctionPtr) print);
    // putchar('\n');
    queue->Mapcar((VoidFunctionPtr) wakeup);
    for(int i = 0; i < removeNum; ++i) {
        queue->Remove();
    }
    removeNum = 0;
    (void) interrupt->SetLevel(oldLevel);
}
