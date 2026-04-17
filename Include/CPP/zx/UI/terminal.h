#pragma once

#include "Integers.hpp"

void terminal_init(u32 columns, u32 rows);
void terminal_resize(u32 columns, u32 rows);
void terminal_clear(void);
void terminal_putc(char c);
void terminal_write(const char* text);
void terminal_input(char c);
void terminal_tick(void);

bool terminal_is_initialized(void);
u32 terminal_get_columns(void);
u32 terminal_get_rows(void);
u32 terminal_get_cursor_x(void);
u32 terminal_get_cursor_y(void);
bool terminal_cursor_visible(void);
char terminal_get_char(u32 x, u32 y);
