// Color.cpp
// Written by TechEverything

/*
Process colors for VGA
*/

#include "Integers.hpp"

#include "zx/VGA/Color.hpp"

namespace VGA {
    namespace Color {
        u8 ProcessColor(Colors foreground, Colors background) {
            u8 color = 0;
            color = ((u32)foreground & 0x0F) | (((u32)background & 0x0F)<< 4);
            return color;
        }
    }
}