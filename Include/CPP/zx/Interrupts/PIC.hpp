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

#include "Functions.hpp"
#include "Integers.hpp"
 
namespace PIC {
    function return_type(void) SendEOI(u8 irq);
    function return_type(void) Remap(i32 offset_1, i32 offset_2);
    
    namespace IRQ {
        function return_type(void) SetMask(u8 IRQline);
        function return_type(void) ClearMask(u8 IRQline);
    }
}