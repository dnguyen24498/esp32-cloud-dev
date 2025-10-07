#pragma once
#include <stdint.h>
#include <stdbool.h>
#include "drivers/lcd_st7567.h"

extern uint8_t lcd_buffer[LCD_WIDTH * (LCD_HEIGHT/8)];

void gfx_clear(void);
void gfx_draw_pixel(int x, int y, bool color);
void gfx_draw_char(int x, int y, char c);
void gfx_draw_text(int x, int y, const char* str);
void gfx_update(void);
