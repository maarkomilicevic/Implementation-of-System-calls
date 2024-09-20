//
// Created by os on 8/19/23.
//

#include "../h/Sem.hpp"
#include "../h/tcb.hpp"

void Sem::wait() {
    if(--val < 0) block();
}

void Sem::signal(){
    if(++val <= 0) unblock();
}

void Sem::block() {
    if(TCB::running == nullptr) return;
    blocked.addLast(TCB::running);
    TCB::running->setBlocked(true);
    TCB::dispatch();
}

void Sem::unblock() {
    TCB* t = blocked.removeFirst();
    t->setBlocked(false);
    Scheduler::put(t);
}