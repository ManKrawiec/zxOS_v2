// String.hpp
// Written by TechEverything

/*
Header for String.cpp
*/

#pragma once

#include "Functions.hpp"
#include "Integers.hpp"

namespace String {
    function return_type(void) Copy(
        char* dest, const char* src
    );

    function return_type(bool) Equals(
        const char* src1, const char* src2
    );

    function return_type(void) Reverse(char* dest, const char* src, size len);

    namespace Check {
        function return_type(bool) IsNumber(char c);
    }

    namespace Convert {
        function return_type(u8) CharToNumber(char c);

        function return_type(i32) ASCIIToInteger(
            const char* src
        );

        function return_type(void) IntegerToASCII(
            char* dest, i32 src
        );
    }
}