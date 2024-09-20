//
// Created by os on 8/22/23.
//

#ifndef PROJECT_BASE_V1_1_BOUNDEDBUFFER_HPP
#define PROJECT_BASE_V1_1_BOUNDEDBUFFER_HPP
#include "../h/syscall_c.h"
const int N = 10000;
class boundedBuffer {
public:
    boundedBuffer();

    void append(char );
    char take();

private:
    char buffer[N];
    int head, tail, count;
    sem_t inSem, outSem, itemAvl, spaceAvl;
};


#endif //PROJECT_BASE_V1_1_BOUNDEDBUFFER_HPP
