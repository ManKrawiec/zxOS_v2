// GDT.hpp
// Written by TechEverything

/*
Header for GDT.cpp
*/

#pragma once

#include "Integers.hpp"

namespace GDT {
    struct Entry {
        u32 base;
        u32 limit;
        u8 flags;
        u8 access_byte;
    };
    
    struct __attribute__((packed)) GDTR {
        u16 limit;
        uptr base;
    };

    void DefaultInitialize();
}