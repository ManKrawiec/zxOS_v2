// Macros.hpp
// Written by TechEverything

/*
Macros for VGA output
*/

#pragma once

#include "Integers.hpp"

namespace VGA {
    constexpr uptr Address = 0xB8000;
    constexpr u32 Width = 80;
    constexpr u32 Height = 25; 
}