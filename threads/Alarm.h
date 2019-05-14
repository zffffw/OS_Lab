#ifndef ALARM_H
#define ALARM_H
#include "copyright.h"
#include "utility.h"
#include "list.h"
void test(int);
void wakeup(ListElement*);
class Alarm {
    public:
        Alarm(char* debugname);
        ~Alarm();
        void Pause(int howLong);
        void searchAndWakeUp();
        List* queue;
    private:
        char* name;
};
#endif
