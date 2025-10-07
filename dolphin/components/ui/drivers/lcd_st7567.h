#pragma once
#include <stdint.h>
#include <stdbool.h>

#define LCD_WIDTH   128
#define LCD_HEIGHT   64

void lcd_i2c_init(int sda, int scl, int freq_hz);
void lcd_init(uint8_t bias, uint8_t ratio, uint8_t contrast);
void lcd_set_contrast(uint8_t contrast);
void lcd_update(const uint8_t *buffer);   // upload framebuffer
