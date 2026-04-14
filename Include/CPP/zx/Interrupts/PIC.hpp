// PIC.hpp
// Written by TechEverything

/*
Header for PIC.cpp
*/

/*
0x20 - master command
0x21 - master data

0xA0 - slave command
0xA1 - slave data
*/

#pragma once

#include "Integers.hpp"
 
namespace PIC {
    void SendEOI(u8 irq);
    void Remap(i32 offset_1, i32 offset_2);
    
    namespace IRQ {
        void SetMask(u8 IRQline);
        void ClearMask(u8 IRQline);
    }
}