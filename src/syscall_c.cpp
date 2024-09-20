//
// Created by os on 8/17/23.
//

#include "../h/syscall_c.h"
#include "../lib/console.h"





void* mem_alloc (size_t size){
    __asm__ volatile("mv a1, %[size]" :: [size] "r" (size));
    uint64 val = 0x01;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));

    __asm__ volatile("ecall");
    uint64 *memblok;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (memblok));
    return memblok;
}
int mem_free (void* element){
    __asm__ volatile("mv a1, %[size]" :: [size] "r" (element));
    uint64 val = 0x02;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;

}

int thread_create (
        thread_t* handle,
        void(*start_routine)(void*),
        void* arg
){
    __asm__ volatile("mv a3, %[size]" :: [size] "r" (arg));
    __asm__ volatile("mv a2, %[size]" :: [size] "r" (start_routine));
    __asm__ volatile("mv a1, %[size]" :: [size] "r" (handle));

    uint64 val = 0x11;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;

}

int thread_exit (){
    uint64 val = 0x12;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));

    __asm__ volatile("ecall");

    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;


}

void thread_dispatch(){
    uint64 val = 0x13;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));

    __asm__ volatile("ecall");
}

void thread_join (
        thread_t handle
){

    __asm__ volatile("mv a1, %[size]" :: [size] "r" (handle));
    uint64 val = 0x14;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");

}

int sem_open (
        sem_t* handle,
        unsigned init
){

    __asm__ volatile("mv a2, %[size]" :: [size] "r" (init));
    __asm__ volatile("mv a1, %[size]" :: [size] "r" (handle));
    uint64 val = 0x21;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;
}

int sem_close (sem_t handle){

    __asm__ volatile("mv a1, %[size]" :: [size] "r" (handle));
    uint64 val = 0x22;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;
}

int sem_wait (sem_t id){
    __asm__ volatile("mv a1, %[size]" :: [size] "r" (id));
    uint64 val = 0x23;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;
}

int sem_signal (sem_t id){
    __asm__ volatile("mv a1, %[size]" :: [size] "r" (id));
    uint64 val = 0x24;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;

}

int time_sleep (time_t t){
    __asm__ volatile("mv a1, %[size]" :: [size] "r" (t));
    uint64 val = 0x31;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;
}

char getc (){

  // return  __getc();
    uint64 val = 0x41;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
    uint64 returnValue;
    __asm__ volatile("mv %[size], a0" : [size] "=r" (returnValue));
    return returnValue;
}

void putc(char c){
    //__putc(c);
    __asm__ volatile("mv a1, %[size]" :: [size] "r" (c));
    uint64 val = 0x42;
    __asm__ volatile("mv a0, %[size]" :: [size] "r" (val));
    __asm__ volatile("ecall");
}


