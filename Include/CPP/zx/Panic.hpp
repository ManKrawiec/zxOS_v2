// Panic.cpp
// Written by TechEverything

/*
Header for Panic.cpp
*/

#pragma once

#include "Functions.hpp"
#include "Integers.hpp"

#include "zx/VGA/Color.hpp"
#include "zx/VGA/Output.hpp"

namespace Panic {
    void Common(
        const char* title,
        const char* description
    );
}