#include "synch-sem.h"
class BoundedBuffer {
   public:
     // create a bounded buffer with a limit of 'maxsize' bytes
     BoundedBuffer(int maxsize);
     ~BoundedBuffer();
     // read 'size' bytes from the bounded buffer, storing into 'data'.
     // ('size' may be greater than 'maxsize')
     void Read(void *data, int size);
     
     // write 'size' bytes from 'data' into the bounded buffer.
     // ('size' may be greater than 'maxsize')
     void Write(void *data, int size);
   private:
     int maxSize;
     int count;
     Lock* boundLock;
     Condition* notFull;
     Condition* notEmpty;
     void *buffer;
     int nextRead;
     int nextWrite;
};

