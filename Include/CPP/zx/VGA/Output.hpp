// Output.hpp
// Written by TechEverything

/*
Header for Output.cpp
*/

#include "zx/VGA/Info.hpp"
#include "Integers.hpp"
#include "Functions.hpp"

#pragma once

namespace VGA {
    function return_type(void) Output(const char* src, u8 color);

    namespace Complex {
        enum class Status {
            Ok,
            Warn,
            Failed,
            Info
        };

        function return_type(void) OutputStatusMessage(
            Status status, const char* message
        );

        function return_type(void) ClearScreen(u8 color);
    }
}