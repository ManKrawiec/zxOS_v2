// Keyboard.cpp
// Written by TechEverything

/*
Driver for keyboard (PS-2)
*/

#include "Integers.hpp"

#include "zx/Interrupts/PIC.hpp"

namespace Keyboard {
    void Initialize() {
        PIC::IRQ::ClearMask(1);
        return;
    }
}