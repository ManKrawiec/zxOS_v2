// GDT.hpp
// Written by TechEverything

/*
Header for GDT.cpp
*/

#pragma once

#include "Functions.hpp"
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

    function return_type(void) DefaultInitialize();
}