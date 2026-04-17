// Kernel.cpp
// Written by TechEverything

/*
Handles initialization, connects all other code together
*/

#include "Integers.hpp"

#include "zx/Multiboot2/Magic.hpp"
#include "zx/Multiboot2/Tags.hpp"

#include "zx/Permissions/GDT.hpp"

#include "zx/Interrupts/IDT.hpp"
#include "zx/Interrupts/PIC.hpp"

#include "zx/Debug.hpp"
#include "zx/Panic.hpp"

#include "zx/Memory/String.hpp"

#include "zx/Drivers/Keyboard.hpp"

#include "zx/UI/effect.h"
#include "zx/UI/input.h"
#include "zx/UI/kernel_tick.h"
#include "zx/UI/renderer.h"
#include "zx/UI/terminal.h"
#include "zx/UI/theme.h"

extern "C" void __attribute__((stdcall)) KernelMain(
    uptr stack_size,
    uptr multiboot2_ident,
    uptr multiboot2_address
) {
    Debug::Output("Initializing zxOS\n");

    static char stack_buf[12];
    String::Convert::IntegerToASCII(stack_buf, stack_size);

    if (multiboot2_ident != Multiboot2::Magic) {
        Panic::Common("Invalid Multiboot2 identifier", "The Multiboot2 identifier is invalid");
        return;
    }

    if (multiboot2_address & 7) {
        Panic::Common("Unaligned Multiboot2 address", "The Multiboot2 address is not aligned properly");
        return;
    }

    multiboot2_address += 8;

    Multiboot2::Tags::Tag* tag = (Multiboot2::Tags::Tag*)(multiboot2_address);

    for (
        ; 
        tag->type != Multiboot2::Tags::Types::End;
        tag = (Multiboot2::Tags::Tag*)((u8*)tag + ((tag->size + 7) & ~7))
    ) {
        Multiboot2::Tags::Parse(tag);
    }

    GDT::DefaultInitialize();
    IDT::DefaultInitialize();
    PIC::Remap(0x20, 0x28);
    
    for (size irq = 0; irq < 16; irq++) {
        PIC::IRQ::SetMask(irq);
    }

    PIC::IRQ::ClearMask(2);

    Keyboard::Initialize();
    input_init();

    if (Multiboot2::Container::framebuffer != 0) {
        render_set_backend(RENDER_BACKEND_FRAMEBUFFER);
    } else {
        render_set_backend(RENDER_BACKEND_VGA_TEXT);
    }

    terminal_init(renderer_get_columns(), renderer_get_rows());
    effect_init();
    theme_set(THEME_MATRIX);

    terminal_write("zxOS UI subsystem online\n");
    terminal_write("backend: ");
    if (Multiboot2::Container::framebuffer != 0) {
        terminal_write("framebuffer\n");
    } else {
        terminal_write("vga_text\n");
    }

    terminal_write("stack: ");
    terminal_write(stack_buf);
    terminal_write("B\n");
    terminal_write("path: input -> terminal -> renderer -> effect\n");
    terminal_write("type on keyboard\n");

    asm volatile("sti");

    for (;;) {
        kernel_tick();
    }
}
