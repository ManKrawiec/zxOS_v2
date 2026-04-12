// Tags.cpp
// Written by TechEverything

/*
Tag parsing and identifiers for Multiboot2
*/

#include "Functions.hpp"
#include "Integers.hpp"
#include "zx/Multiboot2/Tags.hpp"

#include "zx/VGA/Color.hpp"
#include "zx/VGA/Output.hpp"

namespace Multiboot2 {
    namespace Container {
        const Multiboot2::Tags::CLI::Tag* cli;
        const Multiboot2::Tags::Bootloader::Tag* bootloader;
        const Multiboot2::Tags::Framebuffer::Tag* framebuffer;
        const Multiboot2::Tags::MemoryMap::Tag* mmap;
    }

    namespace Tags {
        namespace Types {
            constexpr u32 End = 0;
            constexpr u32 CLI = 1;
            constexpr u32 Bootloader = 2;
            constexpr u32 MemoryMap = 6;
            constexpr u32 Framebuffer = 8;
        }

        function return_type(void) Parse(Multiboot2::Tags::Tag* general_tag) {
            switch (general_tag->type) {
                case Multiboot2::Tags::Types::CLI:
                    Multiboot2::Container::cli 
                        = (const Multiboot2::Tags::CLI::Tag*)general_tag;
                    break;
                case Multiboot2::Tags::Types::Bootloader:
                    Multiboot2::Container::bootloader
                        = (const Multiboot2::Tags::Bootloader::Tag*)general_tag;
                    break;
                case Multiboot2::Tags::Types::MemoryMap:
                    Multiboot2::Container::mmap
                        = (const Multiboot2::Tags::MemoryMap::Tag*)general_tag;
                    break;
                case Multiboot2::Tags::Types::Framebuffer:
                    Multiboot2::Container::framebuffer
                        = (const Multiboot2::Tags::Framebuffer::Tag*)general_tag;
                    break;
                default:
                    break;
            }
        }
    }
}