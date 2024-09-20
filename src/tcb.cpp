//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"


TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* args)
{
    if(body == nullptr) {
        TCB::running = new TCB(body, args, TIME_SLICE);
        return running;
    }
    return new TCB(body, args, TIME_SLICE);

}

void TCB::yield()
{
    thread_dispatch();
}

void TCB::setFinished(bool value){
    finished = value;
    if(!value) return;
    while(!joined.isEmpty()){
        TCB* thread = joined.removeFirst();
        thread->setBlocked(false);
        Scheduler::put(thread);
    }
}

void TCB::dispatch() {
    TCB::timeSliceCounter = 0;
    TCB *old = TCB::running;
    if (old && !old->isFinished() && !old->isBlocked()) { Scheduler::put(old); }
    TCB::running = Scheduler::get();
    if (running) TCB::contextSwitch(&old->context, &running->context);
}

void TCB::threadWrapper()
{

    Riscv::popSppSpie();
    running->body(running->args);
    TCB::running->setFinished(true);
    thread_exit();


}
