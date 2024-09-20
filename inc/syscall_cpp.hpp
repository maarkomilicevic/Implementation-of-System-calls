//
// Created by os on 8/23/23.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
#define PROJECT_BASE_V1_1_SYSCALL_CPP_HPP


#ifndef _syscall_cpp
#define _syscall_cpp
#include "syscall_c.h"



class Thread {
public:
    Thread (void (*body)(void*), void* arg);
    virtual ~Thread () ;
    int start ();
    void join();
    static void dispatch ();
    static int sleep (time_t);
protected:
    Thread ();
    virtual void run () {}

private:
    static void threadWraper(void*);
    thread_t myHandle;
    void (*body)(void*); void* arg;
};
class Semaphore {
public:
    Semaphore (unsigned init = 1);
    virtual ~Semaphore ();
    int wait ();
    int signal ();
private:
    sem_t myHandle;
};
class PeriodicThread : public Thread {
public:
    void terminate ();
protected:
    PeriodicThread (time_t period);
    virtual void periodicActivation () {}
private:
    time_t period;
    bool cond;
    static void pWrapper(void*);
};

class Console {
public:
    static char getc ();

    static void putc (char);
};
#endif


#endif //PROJECT_BASE_V1_1_SYSCALL_CPP_HPP
