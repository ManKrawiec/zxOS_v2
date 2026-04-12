// Utilities.hpp
// Written by TechEverything

/*
Header for Utilities.cpp
*/

#pragma once

#include "Functions.hpp"
#include "Integers.hpp"

namespace Memory {
    function return_type(void) Move(
        void* dest,
        const void* src,
        size n
    );

    function return_type(void) Copy(
        void* dest,
        void* src,
        size n
    );
}