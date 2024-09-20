//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/syscall_c.h"
#include "../h/consoleBuffer.hpp"
#include "../h/Sem.hpp"
#include "../h/sleepThreads.hpp"
#include "../h/memoryAllocator.hpp"

extern void workerBodyG(void*);

#include "../lib/console.h"
#include "../lib/mem.h"


extern void printString(char const *string);

extern void printInteger(uint64 integer);



bool mode = false;


void Riscv::setMode(bool val) {
    mode = val;
}

void Riscv::popSppSpie()
{

    if (mode) mc_sstatus(Riscv::SSTATUS_SPP);
    else ms_sstatus(Riscv::SSTATUS_SPP);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

int numOfBlocks(size_t size){
    uint64 sum = MEM_BLOCK_SIZE, cnt = 1;

    while(sizeof(memoryAllocator::MemFragment) + size > sum){
        cnt++;
        sum += MEM_BLOCK_SIZE;
    }
    return cnt;
}


void* _mem_alloc(size_t size){
    return memoryAllocator::allocate(numOfBlocks(size));
}


int _mem_free(void* arg){
    return memoryAllocator::memFree(arg);
}


int _thread_create(thread_t* handle, void(*start_routine)(void*), void* arg){
    *handle = (thread_t) TCB::createThread(start_routine, arg);
    if(*handle == nullptr) return -1;
    return 0;
}

int _thread_exit(){
    if(! TCB::running) return -1;
    TCB::running->setFinished(true);
    TCB* t = TCB::running;
    TCB::dispatch();
    delete t;
    return 0;
}



void _thread_join(thread_t handle){
    if(handle == nullptr) return;
    if(((TCB*) handle)->isFinished()) return;
    ((TCB*) handle)->joined.addLast(TCB::running);
    TCB::running->setBlocked(true);
    TCB::dispatch();

}

int _sem_open(sem_t* handle, unsigned init){

    *handle = (sem_t) new Sem(init);
    if(*handle == nullptr) return -1;
    return 0;

}

int _sem_close(sem_t handle){

    if(handle == nullptr) return -1;
    while(!((Sem*)handle)->blocked.isEmpty()){
        ((Sem*) handle)->unblock();
    }
    mem_free(handle);

    return 0;
}

int  _sem_wait(sem_t id){
    if(!id) return -1;
    ((Sem*) id)->wait();

    return 0;
}

int _sem_signal(sem_t id){
    if(!id) return -1;
    ((Sem*) id)->signal();
    return 0;
}

int _sleep_time(time_t t){

    sleepThreads::addThread(TCB::running, t);
    TCB::dispatch();
    return 0;
}

char  _getc(){
    char c = consoleBuffer::getInstance().getIn();
    if(!c) return -1;
    return c;
}

void _putc(char c){
    consoleBuffer::getInstance().putOut(c);
    //workerBodyG(nullptr);
}

void Riscv::softwareTrap()  {
    uint64 volatile r0, r1, r2, r3;

    __asm__ volatile("mv %[r0], a0" : [r0] "=r"(r0));
    __asm__ volatile("mv %[r1], a1" : [r1] "=r"(r1));
    __asm__ volatile("mv %[r2], a2" : [r2] "=r"(r2));
    __asm__ volatile("mv %[r3], a3" : [r3] "=r"(r3));


    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();

    uint64 scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {

        // interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)
        sepc += 4;
        switch (r0) {
            case 0x01: {
                void *memBlok = _mem_alloc(r1);
                __asm__ volatile("sd %[value],10*8(fp)"::[value]"r"(memBlok));
            }
                break;
            case 0x02: {
                uint64 returnValue = _mem_free((void *) r1);
                __asm__ volatile("sd %[value],10*8(fp)"::[value]"r"(returnValue));
            }
                break;
            case 0x11: {
                int returnValue = _thread_create((thread_t *) r1, (void (*)(void *)) r2, (void *) r3);
                __asm__ volatile("sd %[value], 10*8(fp)"::[value]"r"(returnValue));
            }
                break;
            case 0x12: {
                int returnValue = _thread_exit();
                __asm__ volatile("sd %[value], 10*8(fp)"::[value]"r"(returnValue));
            }
                break;
            case 0x13: {
                TCB::dispatch();
            }
                break;
            case 0x14: {
                _thread_join((thread_t) r1);
            }
                break;
            case 0x21: {
                int returnValue = _sem_open((sem_t *) r1, r2);
                __asm__ volatile("sd %[value], 10*8(fp)"::[value]"r"(returnValue));
            }
                break;
            case 0x22: {
                int returnValue = _sem_close((sem_t) r1);
                __asm__ volatile("sd %[value], 10*8(fp)"::[value]"r"(returnValue));
            }
                break;
            case 0x23: {
                int returnValue = _sem_wait((sem_t) r1);
                __asm__ volatile("sd %[value], 10*8(fp)"::[value]"r"(returnValue));
            }
                break;
            case 0x24: {
                int returnValue = _sem_signal((sem_t) r1);
                __asm__ volatile("sd %[value], 10*8(fp)"::[value]"r"(returnValue));
            }
                break;

            case 0x31: {
                int returnValue = _sleep_time(r1);
                __asm__ volatile("sd %[value], 10*8(fp)"::[value]"r"(returnValue));
            }
                break;
            case 0x41: {
                int returnValue = _getc();
                __asm__ volatile("sd %[value], 10*8(fp)"::[value]"r"(returnValue));

            }
                break;
            case 0x42: {
                _putc((char) r1);
            }
                break;


            default:
                TCB::dispatch();
        }
    }else{
        putc('g');
    }
    w_sstatus(sstatus);
    w_sepc(sepc);
}

void Riscv::timerTrap() {
    uint64 volatile sepc = r_sepc();
    uint64 volatile sstatus = r_sstatus();




        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
        TCB::timeSliceCounter++;


        sleepThreads::check();
        sleepThreads::dec();

        if (TCB::running && TCB::timeSliceCounter >= TCB::running->getTimeSlice())
        {
            TCB::timeSliceCounter = 0;
            TCB::dispatch();
        }

    w_sstatus(sstatus);
    w_sepc(sepc);
}
void Riscv::consoleTrap()
    {
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();


        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)

     // console_handler();
        int pp = plic_claim();

        if(pp){
            while (CONSOLE_RX_STATUS_BIT & *((char*)CONSOLE_STATUS)) {
                char c = *((char*)CONSOLE_RX_DATA);
                consoleBuffer::getInstance().putIn(c);
            }
        }
       plic_complete(pp);




    w_sstatus(sstatus);
    w_sepc(sepc);

}


