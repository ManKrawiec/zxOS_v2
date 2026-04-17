#include "zx/UI/kernel_tick.h"

#include "zx/UI/effect.h"
#include "zx/UI/input.h"
#include "zx/UI/renderer.h"
#include "zx/UI/terminal.h"

void kernel_tick(void) {
    input_tick();
    terminal_tick();
    effect_tick();

    renderer_begin_frame();
    renderer_flush_terminal();
    effect_render();
    renderer_end_frame();
}
