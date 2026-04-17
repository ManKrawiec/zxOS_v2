// Color.hpp
// Written by TechEverything

/*
Header for Color.cpp and usage of colors
*/

#pragma once

#include "Integers.hpp"

namespace VGA {
    namespace Color {
        enum class Colors {
            Black,
            Blue,
            Green,
            Cyan,
            Red,
            Magenta,
            Brow,
            Gray,
            DarkGray,
            LightBlue,
            LightGreen,
            LightCyan,
            LightRed,
            LightMagenta,
            Yellow,
            White
        };
        
        u8 ProcessColor(Colors foreground, Colors background);
    }
}