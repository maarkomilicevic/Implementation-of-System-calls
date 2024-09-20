//
// Created by os on 8/21/23.
//

#ifndef PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
#define PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP

#include "../lib/hw.h"

class memoryAllocator {

private:
    memoryAllocator(){};

public:
    struct MemFragment {
        MemFragment *next = nullptr;
        MemFragment *prev = nullptr;
        size_t size = ((uint64) ((uint64*) HEAP_END_ADDR - (uint64*) HEAP_START_ADDR))/MEM_BLOCK_SIZE;
    };



    static void* allocate(size_t);
    static MemFragment *head;
    static void tryToJoin(MemFragment *cur);
    static int memFree(void*);
};



#endif //PROJECT_BASE_V1_1_MEMORYALLOCATOR_HPP
