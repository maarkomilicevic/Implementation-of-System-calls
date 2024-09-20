//
// Created by os on 8/20/23.
//

#include "../h/sleepThreads.hpp"

sleepThreads::sleepThread* sleepThreads::head = nullptr;

void sleepThreads::addThread(TCB* thread, int sleepTime){

    sleepThread *tr = (sleepThread*) mem_alloc(sizeof(sleepThread)), *cur, *pr = nullptr;

    if(!head){
        tr->sleepTime = sleepTime;
        tr->tcb = thread;
        thread->setBlocked(true);
        head = tr;
        head->next = nullptr;
        return;
    }
    int time = 0;
    cur = head;
    thread->setBlocked(true);
    tr->tcb = thread;
    while(cur){
        time += cur->sleepTime;
        if(time >= sleepTime) {
            if(!pr) {
                head = tr;
                head->next = cur;
                head->sleepTime = sleepTime;

            } else{
                pr->next = tr;
                tr->next = cur;
                tr->sleepTime = time-sleepTime;
            }

            break;
        }
        pr = cur;
        cur = cur->next;
    }
    if(!cur){
    pr->next = tr;
    tr->next = nullptr;
    tr->sleepTime = sleepTime - time;

    }else{
        cur->sleepTime -= tr->sleepTime;
    }




}

void sleepThreads::check(){
    if(!head) return;
    sleepThread *cur;
    while(head && !head->sleepTime){
        head->tcb->setBlocked(false);
        Scheduler::put(head->tcb);
        cur = head;
        head = head->next;
        mem_free(cur);
        cur->tcb = nullptr;
    }

}

void sleepThreads::dec() {
    if(!head) return;
    head->sleepTime--;

}