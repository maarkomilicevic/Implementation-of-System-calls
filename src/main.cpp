//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.h"
#include "../h/consoleBuffer.hpp"


extern void printString(char const *string);

extern void printInteger(uint64 integer);

sem_t semaphore;

extern void userMain(void*);

void outputWorker(void* args){
    while(1){
        while(CONSOLE_TX_STATUS_BIT & *((char*) CONSOLE_STATUS) ){
            *((char*) CONSOLE_TX_DATA) = consoleBuffer::getInstance().getOut();
        }
    }
}


void idleWorker(void* arg){
    while(1) thread_dispatch();
}


void main()
{

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap | 1UL);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    Riscv::setMode(true);
    __asm__ volatile ("ecall");

    thread_t nit, nit5, nit3, nit2;

    thread_create(&nit, nullptr, nullptr);
    thread_create(&nit2, idleWorker, nullptr);
    thread_create(&nit3, outputWorker, nullptr);
    thread_create(&nit5, userMain, nullptr);

    thread_join(nit5);

}
