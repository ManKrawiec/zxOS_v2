// IO.hpp
// Written by TechEverything

/*
Port I/O for x86
*/

#pragma once

#include "Functions.hpp"
#include "Integers.hpp"

namespace IO {
    namespace Input {
        function private replace return_type(u8) Byte(u16 port) {
            u8 ret;
            __asm__ volatile ( "inb %w1, %b0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }

        function private replace return_type(u16) Word(u16 port) {
            u16 ret;
            __asm__ volatile ( "inw %w1, %w0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }

        function private replace return_type(u32) DoubleWord(u16 port) {
            u32 ret;
            __asm__ volatile ( "inl %w1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }
    }

    namespace Output {
        function private replace return_type(void) Byte(u16 port, u8 val) {
            __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }

        function private replace return_type(void) Word(u16 port, u16 val) {
            __asm__ volatile ( "outb %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }

        function private replace return_type(void) DoubleWord(u16 port, u32 val) {
            __asm__ volatile ( "outb %0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }
    }

    namespace Methods {
        function private replace return_type(void) Hiccup(void) {
            Output::Byte(0x80, 0);
        }
    }
}