#include "zx/UI/renderer.h"

#include "zx/Assets/Fonts/BitFont.hpp"
#include "zx/Multiboot2/Tags.hpp"
#include "zx/Ports/IO.hpp"
#include "zx/UI/terminal.h"
#include "zx/UI/theme.h"
#include "zx/VGA/Info.hpp"

namespace {
    constexpr u32 FontWidth = 8;
    constexpr u32 FontHeight = 8;

    render_backend_ops_t g_vga_backend;
    render_backend_ops_t g_framebuffer_backend;

    u8 framebuffer_map_component(u32 color, u8 shift) {
        return (u8)((color >> shift) & 0xFF);
    }

    u8 vga_distance(u8 a, u8 b) {
        return a > b ? (u8)(a - b) : (u8)(b - a);
    }

    u8 vga_map_color(u32 color) {
        static const u32 palette[16] = {
            0x000000, 0x0000AA, 0x00AA00, 0x00AAAA,
            0xAA0000, 0xAA00AA, 0xAA5500, 0xAAAAAA,
            0x555555, 0x5555FF, 0x55FF55, 0x55FFFF,
            0xFF5555, 0xFF55FF, 0xFFFF55, 0xFFFFFF
        };

        u8 best = 0;
        u32 best_score = 0xFFFFFFFF;
        u8 r = framebuffer_map_component(color, 16);
        u8 g = framebuffer_map_component(color, 8);
        u8 b = framebuffer_map_component(color, 0);

        for (u8 i = 0; i < 16; i++) {
            u8 pr = framebuffer_map_component(palette[i], 16);
            u8 pg = framebuffer_map_component(palette[i], 8);
            u8 pb = framebuffer_map_component(palette[i], 0);
            u32 score = vga_distance(r, pr) + vga_distance(g, pg) + vga_distance(b, pb);

            if (score < best_score) {
                best = i;
                best_score = score;
            }
        }

        return best;
    }

    void vga_move_cursor(u32 x, u32 y) {
        u16 pos = (u16)(y * VGA::Width + x);

        IO::Output::Byte(0x3D4, 0x0F);
        IO::Output::Byte(0x3D5, (u8)(pos & 0xFF));
        IO::Output::Byte(0x3D4, 0x0E);
        IO::Output::Byte(0x3D5, (u8)((pos >> 8) & 0xFF));
    }

    void vga_begin_frame(void) {
    }

    void vga_put_char(u32 x, u32 y, char c, u32 fg, u32 bg) {
        volatile u8* video = (volatile u8*)(VGA::Address);
        u32 offset = (y * VGA::Width + x) * 2;
        u8 attr = (u8)(vga_map_color(fg) | (vga_map_color(bg) << 4));

        video[offset] = (u8)c;
        video[offset + 1] = attr;
    }

    void vga_clear(u32 bg) {
        for (u32 y = 0; y < VGA::Height; y++) {
            for (u32 x = 0; x < VGA::Width; x++) {
                vga_put_char(x, y, ' ', 0xFFFFFF, bg);
            }
        }
    }

    void vga_scroll(void) {
    }

    void vga_end_frame(void) {
        vga_move_cursor(terminal_get_cursor_x(), terminal_get_cursor_y());
    }

    bool framebuffer_available(void) {
        return Multiboot2::Container::framebuffer != 0;
    }

    void framebuffer_plot(u32 x, u32 y, u32 color) {
        const Multiboot2::Tags::Framebuffer::Tag* fb = Multiboot2::Container::framebuffer;
        u8* base = (u8*)(uptr)fb->framebuffer_addr;
        u32 bytes_per_pixel = fb->framebuffer_bpp / 8;
        u32 offset = y * fb->framebuffer_pitch + x * bytes_per_pixel;
        u8 r = framebuffer_map_component(color, 16);
        u8 g = framebuffer_map_component(color, 8);
        u8 b = framebuffer_map_component(color, 0);

        if (bytes_per_pixel >= 3) {
            base[offset] = b;
            base[offset + 1] = g;
            base[offset + 2] = r;
        }

        if (bytes_per_pixel == 4) {
            base[offset + 3] = 0;
        }
    }

    void framebuffer_fill_cell(u32 cell_x, u32 cell_y, u32 color) {
        u32 start_x = cell_x * FontWidth;
        u32 start_y = cell_y * FontHeight;

        for (u32 y = 0; y < FontHeight; y++) {
            for (u32 x = 0; x < FontWidth; x++) {
                framebuffer_plot(start_x + x, start_y + y, color);
            }
        }
    }

    void framebuffer_begin_frame(void) {
        const theme_t* theme = theme_get();
        g_framebuffer_backend.clear(theme->bg);
    }

    void framebuffer_put_char(u32 x, u32 y, char c, u32 fg, u32 bg) {
        unsigned char glyph_index = (unsigned char)c;
        if (glyph_index > 127) {
            glyph_index = '?';
        }

        framebuffer_fill_cell(x, y, bg);

        for (u32 row = 0; row < FontHeight; row++) {
            u8 line = Assets::Fonts::BitFont::Font[glyph_index][row];
            for (u32 col = 0; col < FontWidth; col++) {
                if (line & (1 << (7 - col))) {
                    framebuffer_plot(x * FontWidth + col, y * FontHeight + row, fg);
                }
            }
        }
    }

    void framebuffer_clear(u32 bg) {
        if (!framebuffer_available()) {
            return;
        }

        const Multiboot2::Tags::Framebuffer::Tag* fb = Multiboot2::Container::framebuffer;
        for (u32 y = 0; y < fb->framebuffer_height; y++) {
            for (u32 x = 0; x < fb->framebuffer_width; x++) {
                framebuffer_plot(x, y, bg);
            }
        }
    }

    void framebuffer_scroll(void) {
    }

    void framebuffer_end_frame(void) {
    }

    void renderer_configure_backends(void) {
        g_vga_backend.begin_frame = vga_begin_frame;
        g_vga_backend.put_char = vga_put_char;
        g_vga_backend.clear = vga_clear;
        g_vga_backend.scroll = vga_scroll;
        g_vga_backend.end_frame = vga_end_frame;
        g_vga_backend.columns = VGA::Width;
        g_vga_backend.rows = VGA::Height;

        g_framebuffer_backend.begin_frame = framebuffer_begin_frame;
        g_framebuffer_backend.put_char = framebuffer_put_char;
        g_framebuffer_backend.clear = framebuffer_clear;
        g_framebuffer_backend.scroll = framebuffer_scroll;
        g_framebuffer_backend.end_frame = framebuffer_end_frame;

        if (framebuffer_available()) {
            g_framebuffer_backend.columns = Multiboot2::Container::framebuffer->framebuffer_width / FontWidth;
            g_framebuffer_backend.rows = Multiboot2::Container::framebuffer->framebuffer_height / FontHeight;
        } else {
            g_framebuffer_backend.columns = 0;
            g_framebuffer_backend.rows = 0;
        }
    }
}

render_backend_ops_t* g_backend = 0;

bool render_set_backend(render_backend_id_t id) {
    renderer_configure_backends();

    if (id == RENDER_BACKEND_FRAMEBUFFER && framebuffer_available()) {
        g_backend = &g_framebuffer_backend;
    } else {
        g_backend = &g_vga_backend;
    }

    if (terminal_is_initialized()) {
        terminal_resize(g_backend->columns, g_backend->rows);
    }

    return g_backend != 0;
}

void renderer_begin_frame(void) {
    if (g_backend != 0 && g_backend->begin_frame != 0) {
        g_backend->begin_frame();
    }
}

void renderer_end_frame(void) {
    if (g_backend != 0 && g_backend->end_frame != 0) {
        g_backend->end_frame();
    }
}

void renderer_flush_terminal(void) {
    if (g_backend == 0) {
        return;
    }

    const theme_t* theme = theme_get();
    u32 columns = terminal_get_columns();
    u32 rows = terminal_get_rows();

    for (u32 y = 0; y < rows; y++) {
        for (u32 x = 0; x < columns; x++) {
            g_backend->put_char(x, y, terminal_get_char(x, y), theme->fg, theme->bg);
        }
    }

    if (terminal_cursor_visible()) {
        u32 cursor_x = terminal_get_cursor_x();
        u32 cursor_y = terminal_get_cursor_y();
        char cursor_char = terminal_get_char(cursor_x, cursor_y);

        if (cursor_char == ' ') {
            cursor_char = '_';
        }

        renderer_put_overlay_char(cursor_x, cursor_y, cursor_char, theme->bg, theme->accent);
    }
}

void renderer_put_overlay_char(u32 x, u32 y, char c, u32 fg, u32 bg) {
    if (g_backend != 0 && g_backend->put_char != 0) {
        g_backend->put_char(x, y, c, fg, bg);
    }
}

u32 renderer_get_columns(void) {
    return g_backend != 0 ? g_backend->columns : 0;
}

u32 renderer_get_rows(void) {
    return g_backend != 0 ? g_backend->rows : 0;
}
