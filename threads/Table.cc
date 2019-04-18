#include "Table.h"

Table::Table(int size):Size(size) {
    tableLock = new Lock("tableLock");
    currentSize = -1;
    array = new void*[Size];
};

Table::~Table() {
    delete [] array;
}

int Table::Alloc(void* object) {
    if(currentSize == Size) {
        return -1;
    }
    tableLock->Acquire();
    array[++currentSize] = object;
    tableLock->Release();
    return currentSize;
    
}

void* Table::Get(int index) {
    return currentSize >= index ? array[index] : NULL;
}

void Table::Release(int index) {
    tableLock->Acquire();
    for(int i = index; i < currentSize; ++i) {
        array[i] = array[i + 1];
    }
    currentSize --;
    tableLock->Release();
}


