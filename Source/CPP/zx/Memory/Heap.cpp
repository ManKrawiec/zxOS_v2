// Heap.cpp
// Written by TechEverything

/*
Handles heap memory allocation
*/

#include "Integers.hpp"
#include "zx/Panic.hpp"

#include "zx/Memory/Heap.hpp"

u32 Space[Memory::BlockAmount] = {0}; // 256 * 4 = 1KB
bool Used[Memory::BlockAmount] = {false};

namespace Memory {
    MemoryBlock __attribute__((nodiscard)) Allocate(size n) {
        // get 4-byte aligned 
        size amount = (n + 3) / 4;

        if (amount > BlockAmount) {
            Panic::Common(
                "Allocation too large",
                "Requested allocation exceeds heap capacity."
            );
            return {nullptr, 0, 0};
        }

        for (size i = 0; i <= BlockAmount - amount; i++) {
            bool can_use = true;
            for (size j = 0; j < amount; j++) {
                if (Used[j + i] == true) {
                    can_use = false;
                    break;
                }
            }

            if (can_use) {
                for (size j = 0; j < amount; j++) {
                    Used[i + j] = true;
                }
                MemoryBlock block;
                block.ptr = &Space[i];
                block.amount = amount;
                block.start = i;
                return block;
            }
        }

        Panic::Common("Out of memory", "The system has run out of memory.\nThe heap allocator can not find any free memory to write to.");
        MemoryBlock block;
        block.ptr = nullptr;
        block.amount = amount;
        block.start = 0;
        return block;
    }

    void Free(MemoryBlock block) {
        if (block.ptr == nullptr) {
            Panic::Common("Invalid free", "Can not free an invalid block.");
        };
        if (block.start + block.amount > BlockAmount) {
            Panic::Common("Heap corruption", "Invalid free detected.");
            return;
        }
        for (size i = 0; i < block.amount; i++) {
            Used[block.start + i] = false;
        }
    }

    void Defragment() {
        // just mark everything clear lol
        for (size i = 0; i < BlockAmount; i++) {
            Used[i] = false;
        }
    }
}