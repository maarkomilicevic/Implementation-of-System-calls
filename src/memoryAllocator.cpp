//
// Created by os on 8/21/
//

#include "../h/memoryAllocator.hpp"
#include "../lib/console.h"

memoryAllocator::MemFragment *memoryAllocator::head = nullptr;



void*  memoryAllocator::allocate(size_t size){

    if(!head){
        head = (memoryAllocator::MemFragment*) HEAP_START_ADDR;
        head->next = nullptr;
        head->prev = nullptr;
        head->size = ((uint64) ((uint64*) HEAP_END_ADDR - (uint64*) HEAP_START_ADDR))/MEM_BLOCK_SIZE;
    }
    void *returnValue = nullptr;
    for(MemFragment* cur = memoryAllocator::head; cur != nullptr; cur = cur->next) {
        if (cur->size < size) continue;
           returnValue = cur;
            if (cur->size > size) {

                MemFragment *newSeg = (MemFragment *) ((uint64 *) cur + size * MEM_BLOCK_SIZE);
                if (cur->prev) cur->prev->next = newSeg;
                else head = newSeg;
                if (cur->next) cur->next->prev = newSeg;
                newSeg->next = cur->next;
                newSeg->prev = cur->prev;
                newSeg->size = cur->size;
                cur->size = size;
          return (void*) ((uint64*) returnValue + sizeof(MemFragment));
            }
        if(cur->prev) {
            cur->prev->next = cur->next;
            if(cur->next) cur->next->prev = cur->prev;
            cur->size = size;
            return (void*) ((uint64*) returnValue + sizeof(MemFragment));
        }
        else {
            head = cur->next;
            if(cur->next) cur->next->prev = cur->prev;
            cur->size = size;
            return (void*) ((uint64*) returnValue + sizeof(MemFragment));
        }






    }
    return (void *) ((uint64 *) returnValue + sizeof(MemFragment));
}

void memoryAllocator::tryToJoin(MemFragment *cur) {
    if(!cur) return;
    if(cur->next && ((char*)cur+cur->size*MEM_BLOCK_SIZE) == (char*)(cur->next)){
        cur->size+= cur->next->size;
        cur->next= cur->next->next;
        if(cur->next) cur->next->prev=cur;

    }


}

int memoryAllocator::memFree(void *adr) {

    MemFragment* cur=nullptr;
    if(!head|| (char*) adr < (char*) head) cur = nullptr;
    else{
        for(cur = head; cur->next!=nullptr && (char*)adr>(char*)(cur->next);cur=cur->next);
    }
    MemFragment* newSeg = (MemFragment*) ((uint64*)adr - sizeof(MemFragment));
    newSeg->prev=cur;
    if(cur)newSeg->next=cur->next;
    else {
        newSeg->next = head;
    }
    if(newSeg->next)newSeg->next->prev=newSeg;
    if(cur)cur->next=newSeg;
    else head=newSeg;
    tryToJoin(newSeg);
    if(cur!=nullptr) tryToJoin(cur);
    return 0;
}