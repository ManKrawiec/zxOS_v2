// Heap.hpp
// Written by TechEverything

/*
Header for heap memory allocation
*/

#pragma once

#include "Integers.hpp"

namespace Memory {
    constexpr u32 BlockAmount = 4096;

    struct MemoryBlock {
        void* ptr;
        size amount;
        size start;
    };

    MemoryBlock Allocate(size n);
    void Free(MemoryBlock block);
    void Defragment();
}