#include "zx/UI/terminal.h"

namespace {
    constexpr u32 TerminalMaxColumns = 100;
    constexpr u32 TerminalMaxRows = 60;

    char g_terminal_buffer[TerminalMaxRows][TerminalMaxColumns];
    u32 g_terminal_columns = 0;
    u32 g_terminal_rows = 0;
    u32 g_terminal_cursor_x = 0;
    u32 g_terminal_cursor_y = 0;
    u32 g_terminal_blink_ticks = 0;
    bool g_terminal_cursor_state = true;
    bool g_terminal_initialized = false;

    void terminal_blank_row(u32 row) {
        for (u32 x = 0; x < g_terminal_columns; x++) {
            g_terminal_buffer[row][x] = ' ';
        }
    }

    void terminal_scroll_internal(void) {
        if (g_terminal_rows == 0) {
            return;
        }

        for (u32 y = 1; y < g_terminal_rows; y++) {
            for (u32 x = 0; x < g_terminal_columns; x++) {
                g_terminal_buffer[y - 1][x] = g_terminal_buffer[y][x];
            }
        }

        terminal_blank_row(g_terminal_rows - 1);
        g_terminal_cursor_y = g_terminal_rows - 1;
    }
}

void terminal_init(u32 columns, u32 rows) {
    terminal_resize(columns, rows);
}

void terminal_resize(u32 columns, u32 rows) {
    g_terminal_columns = columns > TerminalMaxColumns ? TerminalMaxColumns : columns;
    g_terminal_rows = rows > TerminalMaxRows ? TerminalMaxRows : rows;
    g_terminal_cursor_x = 0;
    g_terminal_cursor_y = 0;
    g_terminal_blink_ticks = 0;
    g_terminal_cursor_state = true;
    g_terminal_initialized = true;
    terminal_clear();
}

void terminal_clear(void) {
    if (!g_terminal_initialized) {
        return;
    }

    for (u32 y = 0; y < g_terminal_rows; y++) {
        terminal_blank_row(y);
    }

    g_terminal_cursor_x = 0;
    g_terminal_cursor_y = 0;
}

void terminal_putc(char c) {
    if (!g_terminal_initialized || g_terminal_columns == 0 || g_terminal_rows == 0) {
        return;
    }

    if (c == '\r') {
        g_terminal_cursor_x = 0;
        return;
    }

    if (c == '\n') {
        g_terminal_cursor_x = 0;
        g_terminal_cursor_y++;
        if (g_terminal_cursor_y >= g_terminal_rows) {
            terminal_scroll_internal();
        }
        return;
    }

    if (c == '\b') {
        if (g_terminal_cursor_x > 0) {
            g_terminal_cursor_x--;
            g_terminal_buffer[g_terminal_cursor_y][g_terminal_cursor_x] = ' ';
        }
        return;
    }

    if (c == '\t') {
        for (u32 i = 0; i < 4; i++) {
            terminal_putc(' ');
        }
        return;
    }

    if ((unsigned char)c < 32 || (unsigned char)c > 126) {
        return;
    }

    g_terminal_buffer[g_terminal_cursor_y][g_terminal_cursor_x] = c;
    g_terminal_cursor_x++;

    if (g_terminal_cursor_x >= g_terminal_columns) {
        g_terminal_cursor_x = 0;
        g_terminal_cursor_y++;
        if (g_terminal_cursor_y >= g_terminal_rows) {
            terminal_scroll_internal();
        }
    }
}

void terminal_write(const char* text) {
    while (*text) {
        terminal_putc(*text);
        text++;
    }
}

void terminal_input(char c) {
    terminal_putc(c);
}

void terminal_tick(void) {
    g_terminal_blink_ticks++;
    if (g_terminal_blink_ticks >= 24) {
        g_terminal_blink_ticks = 0;
        g_terminal_cursor_state = !g_terminal_cursor_state;
    }
}

bool terminal_is_initialized(void) {
    return g_terminal_initialized;
}

u32 terminal_get_columns(void) {
    return g_terminal_columns;
}

u32 terminal_get_rows(void) {
    return g_terminal_rows;
}

u32 terminal_get_cursor_x(void) {
    return g_terminal_cursor_x;
}

u32 terminal_get_cursor_y(void) {
    return g_terminal_cursor_y;
}

bool terminal_cursor_visible(void) {
    return g_terminal_cursor_state;
}

char terminal_get_char(u32 x, u32 y) {
    if (!g_terminal_initialized || x >= g_terminal_columns || y >= g_terminal_rows) {
        return ' ';
    }

    return g_terminal_buffer[y][x];
}
