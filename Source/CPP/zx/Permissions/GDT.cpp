// GDT.cpp
// Written by TechEverything

/*
Permissions (This code sets Ring0 to the whole kernel and OS!)
*/

#include "zx/Permissions/GDT.hpp"

#include "Integers.hpp"

#include "zx/VGA/Output.hpp"
#include "zx/VGA/Color.hpp"

namespace GDT {
    u64 gdt_entries[3];

    void Encode(u8* target, Entry source) {
        /*
        Entry:
        bits 0..15 : Limit (bytes 0 - 1)
        bits 16..39 : Base (bytes 2 - 4)
        bits 40..47 : Access Byte (byte 5)
        bits 48..51 : Limit (byte 6)
        bits 52..55 : Flags (byte 6)
        bits 56..63 : Base (byte 7)
        */
        
        // Limit
        target[0] = source.limit & 0xFF;
        target[1] = (source.limit >> 8) & 0xFF;
        target[6] = (source.limit >> 16) & 0x0F;

        // Base
        target[2] = source.base & 0xFF;
        target[3] = (source.base >> 8) & 0xFF;
        target[4] = (source.base >> 16) & 0xFF;
        target[7] = (source.base >> 24) & 0xFF;

        // Access byte
        target[5] = source.access_byte;

        // Flags
        target[6] |= (source.flags << 4);
    }

    void DefaultInitialize() {
        /*
        Flags (4 bits in 1 byte):

        Granularity
        Size
        Long_Mode
        Reserved
        */

        /*
        Access Byte:

        Present (1 bit)
        Ring (2 bits)
        Descriptor Type (1 bit)
        Executable (1 bit)
        Direction/Conforming (1 bit)
        RW (1 bit)
        Accessed (1 bit)
        */

        Entry Null = {
            0,  // base
            0,  // limit
            0,  // flags
            0   // access
        };
        Entry KernelCode = {
            0,          // base
            0xFFFFF,    // limit
            0b1100,     // flags
            0b10011111  // access byte
        };
        Entry KernelData = {
            0,          // base
            0xFFFFF,    // limit
            0b1100,     // flags
            0b10010011  // access byte
        };

        Encode((u8*)&gdt_entries[0], Null);
        Encode((u8*)&gdt_entries[1], KernelCode);
        Encode((u8*)&gdt_entries[2], KernelData);

        GDTR gdtr;
        gdtr.limit = sizeof(gdt_entries) - 1;
        gdtr.base = (uptr)&gdt_entries[0];

        asm volatile("lgdt %0" : : "m"(gdtr));
        asm volatile(
            "mov $0x10, %%ax \n"
            "mov %%ax, %%ds \n"
            "mov %%ax, %%es \n"
            "mov %%ax, %%fs \n"
            "mov %%ax, %%gs \n"
            "mov %%ax, %%ss \n"
            "ljmp $0x08, $.flush \n"
            ".flush: \n"
            :
            :
            : "ax"
        );
    }
}