#pragma once

#include "Integers.hpp"

enum render_backend_id_t {
    RENDER_BACKEND_VGA_TEXT = 0,
    RENDER_BACKEND_FRAMEBUFFER = 1
};

typedef struct render_backend_ops_t {
    void (*begin_frame)(void);
    void (*put_char)(u32 x, u32 y, char c, u32 fg, u32 bg);
    void (*clear)(u32 bg);
    void (*scroll)(void);
    void (*end_frame)(void);
    u32 columns;
    u32 rows;
} render_backend_ops_t;

extern render_backend_ops_t* g_backend;

bool render_set_backend(render_backend_id_t id);
void renderer_begin_frame(void);
void renderer_end_frame(void);
void renderer_flush_terminal(void);
void renderer_put_overlay_char(u32 x, u32 y, char c, u32 fg, u32 bg);
u32 renderer_get_columns(void);
u32 renderer_get_rows(void);
