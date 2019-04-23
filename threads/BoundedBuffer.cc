#include "BoundedBuffer.h"

BoundedBuffer:: BoundedBuffer(int maxsize) {
    maxSize = maxsize;
    buffer = new char[maxSize + 1];
    buffer[maxSize] = '\0'; // set rear to zero for ouput
    // buffer = (char *)malloc(maxsize * sizeof(char));
    lock = new Lock("Lock in bounded buffer");
    notFull = new Condition("Condition for not full");
    notEmpty = new Condition("Condition for empty");
    nextRead = 0;
    nextWrite = 0;
}

BoundedBuffer:: ~BoundedBuffer() {
    delete [] buffer;
    delete lock;
    delete notFull;
    delete notEmpty;
}
     
void BoundedBuffer:: Read(void *data, int size) {
    lock->Acquire();
    while (count - size < 0){
        notEmpty->Wait(lock);
    }
    char *cdata;
    cdata = (char *)data;
    int secondPart = nextRead + size > maxSize ? (nextRead + size) % maxSize : 0;
    int firstPart = size - secondPart;
    memcpy(cdata, buffer + nextRead, firstPart);
    nextRead += firstPart;
    count -= firstPart;
    if (secondPart) {
        nextRead = 0;
        memcpy(cdata + firstPart, buffer + nextRead, secondPart);
        nextRead += secondPart;
        count -= secondPart;
    }
    cdata[size] = '\0';
    data = (void *)cdata;
    notFull->Signal(lock);
    lock->Release();
    printf("\t(Read \"%s\" from buffer) (Next read[%d])\n", cdata, nextRead);
}

void BoundedBuffer:: Write(void *data, int size) {
    lock->Acquire();
    char *cData = (char *) data;
    while (count + size > maxSize){
        notFull->Wait(lock);
    }

    int secondPart = nextWrite + size > maxSize ? (nextWrite + size) % maxSize : 0;
    int firstPart = size - secondPart;
    memcpy(buffer + nextWrite, cData, firstPart);
    count += firstPart;
    nextWrite += firstPart;
    if (secondPart) {
        nextWrite = 0;
        memcpy(buffer + nextWrite, cData + firstPart, secondPart);
        nextWrite += secondPart;
        count += secondPart;
    }
    notEmpty->Signal(lock);
    lock->Release();
    printf("\t(Current Buffer \"%s\") (Next write[%d])\n", buffer, nextWrite);
}
