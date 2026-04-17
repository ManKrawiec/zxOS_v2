// Debug.cpp
// Written by TechEverything

/*
Handles serial output
*/

#include "zx/Ports/IO.hpp"

namespace Debug {
    void Output(const char* src) {
        while (*src) {
            IO::Output::Byte(0x3F8, *src);
            src++;
        }
    }
}