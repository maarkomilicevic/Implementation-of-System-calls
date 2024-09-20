//
// Created by marko on 20.4.22..
//

#ifndef OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
#define OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"
#include "syscall_c.h"
#include "list.hpp"

// Thread Control Block
class TCB
{
public:
    ~TCB() { delete[] stack; }

    bool isFinished() const { return finished; }

    void setFinished(bool value);

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body, void* args);

    static void yield();

    void setBlocked(bool value) {blocked = value;}

    bool isBlocked() { return blocked;}

    static TCB *running;

    static void dispatch();




private:
    List<TCB> joined;

    TCB(Body body, void* args, uint64 timeSlice) :
            body(body),
            stack(body != nullptr ? new uint64[STACK_SIZE] : nullptr),
            context({(uint64) &threadWrapper,
                     stack != nullptr ? (uint64) &stack[STACK_SIZE] : 0
                    }),
            timeSlice(timeSlice),
            finished(false), blocked(false), args(args)
    {
        if (body != nullptr) { Scheduler::put(this);}
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    struct Sem{
        bool isBlocked = false;
        sem_t semaphore = nullptr;
    };

    Body body;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool finished;
    bool blocked;
    void* args;

    friend class Sem;
    friend class Riscv;
    friend void _thread_join(thread_t);

    static void threadWrapper();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static uint64 timeSliceCounter;

    static uint64 constexpr STACK_SIZE = 4096;
    static uint64 constexpr TIME_SLICE = 2;
};

#endif //OS1_VEZBE07_RISCV_CONTEXT_SWITCH_2_INTERRUPT_TCB_HPP
