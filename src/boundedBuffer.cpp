//
// Created by os on 8/22/23.
//

#include "../h/boundedBuffer.hpp"
#include "../h/tcb.hpp"


boundedBuffer::boundedBuffer(): head(0), tail(0), count (0){
    sem_open(&inSem, 1);
    sem_open(&outSem, 1);
    sem_open(&itemAvl, 0);
    sem_open(&spaceAvl, N);
}

void boundedBuffer::append(char c){
    sem_wait(inSem);
    sem_wait(spaceAvl);
    buffer[tail] = c;
    tail = (tail+1)%N;
    count++;
    sem_signal(itemAvl);
    sem_signal(inSem);

}

char boundedBuffer::take(){

   sem_wait(outSem);
   sem_wait(itemAvl);
   char c = buffer[head];
   head = (head+1)%N;
   count--;
   sem_signal(spaceAvl);
   sem_signal(outSem);
   return c;
}