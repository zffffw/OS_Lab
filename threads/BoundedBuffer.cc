
#include "BoundedBuffer.h"
#include <stdlib.h>

BoundedBuffer::BoundedBuffer(int maxsize):maxSize(maxsize) {
    boundLock = new Lock("boundLock");
    notFull = new Condition("notFull");
    notEmpty = new Condition("notEmpty");
    count = 0;
    nextRead = 0;
    nextWrite = 0;
    buffer = (void *)malloc(maxSize * sizeof(void));
}

BoundedBuffer::~BoundedBuffer() {
    delete boundLock;
    delete notFull;
    delete notEmpty;
}

void BoundedBuffer::Read(void *data, int size) {
    boundLock->Acquire();
    //no things to read
    while(count == 0) {
        notEmpty->Wait(boundLock);
    }

    for(int i = 0; i < size; ++i) {
        memcpy(data + i, buffer + nextRead, sizeof(void));
        nextRead = (nextRead++) % maxSize;
    }
    count += size;
    notFull->Signal(boundLock);
    boundLock->Release();
}

void BoundedBuffer::Write(void *data, int size) {
    boundLock->Acquire();
    //no enough size to write
    while(count + size > maxSize) {
        notFull->Wait(boundLock);
    }
    for(int i = 0; i < size; ++i) {
        memcpy(buffer + nextWrite, data + i, sizeof(void));
        nextWrite = (nextWrite++) % maxSize;
    }
    count += size;
    notEmpty->Signal(boundLock);
    boundLock->Release();
}

