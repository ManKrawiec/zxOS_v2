#pragma once

#include "Integers.hpp"

typedef struct effect_t {
    void (*init)(void);
    void (*tick)(void);
    void (*render)(void);
} effect_t;

enum effect_id_t {
    EFFECT_NONE = 0,
    EFFECT_MATRIX = 1
};

void effect_init(void);
void effect_set(u8 id);
void effect_tick(void);
void effect_render(void);
const effect_t* effect_matrix_get(void);
