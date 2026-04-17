#pragma once

#include "Integers.hpp"

enum theme_id_t {
    THEME_CRT_GREEN = 0,
    THEME_AMBER = 1,
    THEME_MATRIX = 2,
    THEME_NEON = 3,
    THEME_MONO = 4
};

typedef struct theme_t {
    u32 fg;
    u32 bg;
    u32 accent;
    u8 effect_id;
} theme_t;

void theme_set(theme_id_t id);
theme_id_t theme_get_id(void);
const theme_t* theme_get(void);
