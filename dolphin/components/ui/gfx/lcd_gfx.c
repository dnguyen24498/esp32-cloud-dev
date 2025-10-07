#include "lcd_gfx.h"
#include "drivers/lcd_st7567.h"

#include <string.h>

// framebuffer, 1 bit per pixel, 8 rows per page
uint8_t lcd_buffer[LCD_WIDTH * (LCD_HEIGHT/8)];

extern const uint8_t font6x8[][6]; // simple 6x8 font

void gfx_clear(void) {
    memset(lcd_buffer, 0x00, sizeof(lcd_buffer));
}

void gfx_draw_pixel(int x, int y, bool color) {
    if (x<0 || x>=LCD_WIDTH || y<0 || y>=LCD_HEIGHT) return;
    int page = y / 8;
    int bit = y % 8;
    uint8_t *byte = &lcd_buffer[page*LCD_WIDTH + x];
    if (color) *byte |= (1<<bit);
    else       *byte &= ~(1<<bit);
}

void gfx_draw_char(int x, int y, char c) {
    if (c < 32 || c > 127) c = '?';
    const uint8_t *glyph = font6x8[c-32];
    for (int col=0; col<6; col++) {
        uint8_t line = glyph[col];
        for (int row=0; row<8; row++) {
            bool pixel = line & (1<<row);
            gfx_draw_pixel(x+col, y+row, pixel);
        }
    }
}

void gfx_draw_text(int x, int y, const char* str) {
    while (*str) {
        gfx_draw_char(x, y, *str++);
        x += 6;
    }
}

void gfx_update(void) {
    lcd_update(lcd_buffer);
}