#include "Table.h"

Table::Table(int size):Size(size) {
    tableLock = new Lock("tableLock");
    currentSize = -1;
    array = new void*[Size];
};

Table::~Table() {
    //输出表中最后存放的东西
    char *t;
    printf("%d\n", currentSize);
    while((t = (char*)Get(0))) {
        Release(0);
        printf("%s ", t);
    }
    putchar('\n');
    delete [] array;
}

int Table::Alloc(void* object) {
    tableLock->Acquire();
    if(currentSize == Size - 1) {
        tableLock->Release();
        return -1;
    }
    currentSize++;
    currentThread->Yield();
    array[currentSize] = object;
    tableLock->Release();
    return currentSize;
}

int Table::getCurSize() {
    return currentSize;
}

void* Table::Get(int index) {
    return currentSize >= index ? array[index] : NULL;
}

void Table::Release(int index) {
    tableLock->Acquire();
    if(index > currentSize || currentSize == -1) {
        tableLock->Release();
        return ;
    }
    currentThread->Yield();
    for(int i = index; i < currentSize; ++i) {
        
        array[i] = array[i + 1];
    }
    currentSize --;
    tableLock->Release();
}


