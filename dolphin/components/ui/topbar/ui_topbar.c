#include "ui_topbar.h"
#include "gfx/lcd_gfx.h"
#include "drivers/lcd_st7567.h"
#include "fonts/ui_icons.h"
#include <stdio.h>

#define TOPBAR_MARGIN_X   2
#define TOPBAR_MARGIN_Y   2
#define TOPBAR_HEIGHT     12

void ui_render_topbar(const topbar_model_t *m) {
    // 🧹 Clear only top bar area (not entire screen)
    for (int y = 0; y < TOPBAR_HEIGHT; y++) {
        for (int x = 0; x < LCD_WIDTH; x++) {
            gfx_draw_pixel(x, y, 0);
        }
    }

    // --- Draw time (centered with margins) ---
    char buf[8];
    snprintf(buf, sizeof(buf), "%02u:%02u", m->hour, m->minute);

    int text_width = 6 * 5; // "HH:MM"
    int x_time = (LCD_WIDTH - text_width) / 2;
    int y_time = TOPBAR_MARGIN_Y;

    gfx_draw_text(x_time, y_time, buf);

    // --- Battery icon (top-right) ---
    const uint8_t* batt = (m->batt_soc < 20) ? icon_batt_empty :
                          (m->batt_soc < 70) ? icon_batt_half : icon_batt_full;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            bool pixel = batt[row] & (1 << col);
            gfx_draw_pixel(LCD_WIDTH - TOPBAR_MARGIN_X - 8 + col, y_time + row, pixel);
        }
    }

    // --- Wi-Fi icon (left of battery) ---
    const uint8_t* wifi = (m->wifi_rssi < -80) ? icon_wifi_1 :
                          (m->wifi_rssi < -60) ? icon_wifi_2 : icon_wifi_3;

    for (int row = 0; row < 8; row++) {
        for (int col = 0; col < 8; col++) {
            bool pixel = wifi[row] & (1 << col);
            gfx_draw_pixel(LCD_WIDTH - TOPBAR_MARGIN_X - 16 + col, y_time + row, pixel);
        }
    }

    // Separator line
    for (int x = 0; x < LCD_WIDTH; x++) gfx_draw_pixel(x, 12, 1);

    // ✅ Do NOT call lcd_update() here
    // The caller (home screen or system) will push the final composed frame.
    (void)gfx_update();
}
