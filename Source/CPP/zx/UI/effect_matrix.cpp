#include "zx/UI/effect.h"

#include "zx/UI/renderer.h"
#include "zx/UI/theme.h"

namespace {
    constexpr u32 MatrixMaxColumns = 100;

    u32 g_matrix_heads[MatrixMaxColumns] = {0};
    u32 g_matrix_seed = 0x13572468;
    u32 g_matrix_tick = 0;

    u32 matrix_rand(void) {
        g_matrix_seed = g_matrix_seed * 1664525u + 1013904223u;
        return g_matrix_seed;
    }

    char matrix_glyph(u32 column, u32 row) {
        static const char glyphs[] = "01ZX/\\|*-+.";
        return glyphs[(column + row + g_matrix_tick) % (sizeof(glyphs) - 1)];
    }

    void matrix_init(void) {
        u32 rows = renderer_get_rows();

        for (u32 i = 0; i < MatrixMaxColumns; i++) {
            g_matrix_heads[i] = rows == 0 ? 0 : matrix_rand() % rows;
        }
    }

    void matrix_tick_impl(void) {
        u32 columns = renderer_get_columns();
        u32 rows = renderer_get_rows();

        if (rows == 0) {
            return;
        }

        g_matrix_tick++;

        for (u32 x = 0; x < columns && x < MatrixMaxColumns; x++) {
            if ((matrix_rand() & 3u) == 0) {
                g_matrix_heads[x]++;
            }

            if (g_matrix_heads[x] >= rows + 6) {
                g_matrix_heads[x] = matrix_rand() % rows;
            }
        }
    }

    void matrix_render(void) {
        const theme_t* theme = theme_get();
        u32 columns = renderer_get_columns();
        u32 rows = renderer_get_rows();

        for (u32 x = 0; x < columns && x < MatrixMaxColumns; x++) {
            u32 head = g_matrix_heads[x];

            if (head < rows) {
                renderer_put_overlay_char(x, head, matrix_glyph(x, head), theme->accent, theme->bg);
            }

            if (head > 0 && head - 1 < rows) {
                renderer_put_overlay_char(x, head - 1, '.', theme->fg, theme->bg);
            }
        }
    }

    const effect_t g_matrix_effect = {
        matrix_init,
        matrix_tick_impl,
        matrix_render
    };
}

const effect_t* effect_matrix_get(void) {
    return &g_matrix_effect;
}
