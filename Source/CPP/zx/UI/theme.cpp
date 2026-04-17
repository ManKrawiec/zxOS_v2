#include "zx/UI/theme.h"

#include "zx/UI/effect.h"

namespace {
    const theme_t g_themes[] = {
        {0x55FF55, 0x001100, 0xA0FFA0, EFFECT_NONE},
        {0xFFB000, 0x1A0D00, 0xFFD36A, EFFECT_NONE},
        {0x70FF70, 0x000000, 0xC8FFC8, EFFECT_MATRIX},
        {0x66F0FF, 0x050014, 0xFF4FD8, EFFECT_NONE},
        {0xD0D0D0, 0x111111, 0xFFFFFF, EFFECT_NONE}
    };

    theme_id_t g_theme_id = THEME_CRT_GREEN;
}

void theme_set(theme_id_t id) {
    if ((u32)id > (u32)THEME_MONO) {
        id = THEME_CRT_GREEN;
    }

    g_theme_id = id;
    effect_set(g_themes[id].effect_id);
}

theme_id_t theme_get_id(void) {
    return g_theme_id;
}

const theme_t* theme_get(void) {
    return &g_themes[g_theme_id];
}
