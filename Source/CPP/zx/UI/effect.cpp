#include "zx/UI/effect.h"

namespace {
    const effect_t g_no_effect = {0, 0, 0};
    const effect_t* g_current_effect = &g_no_effect;
    u8 g_current_effect_id = EFFECT_NONE;
}

void effect_init(void) {
    effect_set(EFFECT_NONE);
}

void effect_set(u8 id) {
    g_current_effect_id = id;

    switch (id) {
        case EFFECT_MATRIX:
            g_current_effect = effect_matrix_get();
            break;
        default:
            g_current_effect = &g_no_effect;
            break;
    }

    if (g_current_effect->init != 0) {
        g_current_effect->init();
    }
}

void effect_tick(void) {
    if (g_current_effect->tick != 0) {
        g_current_effect->tick();
    }
}

void effect_render(void) {
    if (g_current_effect->render != 0) {
        g_current_effect->render();
    }
}
