#include "Table.h"

Table::Table(int size):Size(size) {
    tableLock = new Lock("tableLock");
    currentSize = -1;
    array = new void*[Size];
};

Table::~Table() {
    //输出表中最后存放的东西
    // char *t;
    printf("size: %d\n", currentSize);
    for(int i = 0; i <= currentSize; ++i) {
        printf("%s ", (char*)array[i]);
    }
    putchar('\n');
    delete [] array;
    delete tableLock;
}

int Table::Alloc(void* object) {
    tableLock->Acquire();
    printf("thread %c insert %s\n", currentThread->getName()[1], (char*) object);
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
    printf("thread %c release %s\n", currentThread->getName()[1], (char*) array[index]);
    currentThread->Yield();
    for(int i = index; i < currentSize; ++i) {
        
        array[i] = array[i + 1];
    }
    currentSize --;
    tableLock->Release();
}


