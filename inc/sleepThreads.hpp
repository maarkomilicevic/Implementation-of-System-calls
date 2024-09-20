//
// Created by os on 8/20/23.
//

#ifndef PROJECT_BASE_V1_1_SLEEPTHREADS_HPP
#define PROJECT_BASE_V1_1_SLEEPTHREADS_HPP
#include "list.hpp"
#include "tcb.hpp"

class sleepThreads {

public:
    static void addThread(TCB* thread, int sleepTime);

    static void check();

    static void dec();

private:

   struct sleepThread{
        TCB* tcb;
        int sleepTime;
        struct sleepThread *next;
    };

    static struct sleepThread *head;
};


#endif //PROJECT_BASE_V1_1_SLEEPTHREADS_HPP
