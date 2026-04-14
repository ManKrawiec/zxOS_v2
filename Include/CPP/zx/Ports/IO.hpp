// IO.hpp
// Written by TechEverything

/*
Port I/O for x86
*/

#pragma once

#include "Integers.hpp"

namespace IO {
    namespace Input {
        static inline u8 Byte(u16 port) {
            u8 ret;
            __asm__ volatile ( "inb %w1, %b0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }

        static inline u16 Word(u16 port) {
            u16 ret;
            __asm__ volatile ( "inw %w1, %w0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }

        static inline u32 DoubleWord(u16 port) {
            u32 ret;
            __asm__ volatile ( "inl %w1, %0"
                    : "=a"(ret)
                    : "Nd"(port)
                    : "memory");
            return ret;
        }
    }

    namespace Output {
        static inline void Byte(u16 port, u8 val) {
            __asm__ volatile ( "outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }

        static inline void Word(u16 port, u16 val) {
            __asm__ volatile ( "outb %w0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }

        static inline void DoubleWord(u16 port, u32 val) {
            __asm__ volatile ( "outb %0, %w1" : : "a"(val), "Nd"(port) : "memory");
        }
    }

    namespace Methods {
        static inline void Hiccup(void) {
            Output::Byte(0x80, 0);
        }
    }
}