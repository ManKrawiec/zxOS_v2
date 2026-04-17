#include "zx/UI/input.h"

#include "zx/Drivers/Keyboard.hpp"
#include "zx/UI/terminal.h"

namespace {
    constexpr u32 InputQueueSize = 128;

    u8 g_input_queue[InputQueueSize] = {0};
    u32 g_input_head = 0;
    u32 g_input_tail = 0;

    bool input_queue_pop(u8* scancode) {
        if (g_input_head == g_input_tail) {
            return false;
        }

        *scancode = g_input_queue[g_input_tail];
        g_input_tail = (g_input_tail + 1) % InputQueueSize;
        return true;
    }
}

void input_init(void) {
    g_input_head = 0;
    g_input_tail = 0;
}

void input_queue_push(u8 scancode) {
    u32 next = (g_input_head + 1) % InputQueueSize;
    if (next == g_input_tail) {
        return;
    }

    g_input_queue[g_input_head] = scancode;
    g_input_head = next;
}

void input_tick(void) {
    u8 scancode = 0;
    Keyboard::Event event;

    while (input_queue_pop(&scancode)) {
        if (!Keyboard::DecodeScancode(scancode, &event)) {
            continue;
        }

        if (event.release) {
            continue;
        }

        switch (event.type) {
            case Keyboard::EventType::Normal:
            case Keyboard::EventType::Keypad:
                terminal_input(event.ident);
                break;
            case Keyboard::EventType::Special:
                if (event.ident == Keyboard::SpecialKeyCode::Enter) {
                    terminal_input('\n');
                } else if (event.ident == Keyboard::SpecialKeyCode::Backspace) {
                    terminal_input('\b');
                } else if (event.ident == Keyboard::SpecialKeyCode::Tab) {
                    terminal_input('\t');
                }
                break;
            default:
                break;
        }
    }
}
