#include "ui_home.h"
#include "drivers/lcd_st7567.h"
#include "gfx/lcd_gfx.h"
#include "topbar/ui_topbar.h"
#include "system_service.h"
#include <string.h>
#include <stdio.h>

static uint8_t current_index = 0;

// ──────────────────────────────
// Simple helper shapes
// ──────────────────────────────
static void draw_hline(int x, int y, int w, bool color) {
    for (int i = 0; i < w; i++) {
        gfx_draw_pixel(x + i, y, color);
    }
}

static void fill_rect(int x, int y, int w, int h, bool color) {
    for (int j = 0; j < h; j++) {
        for (int i = 0; i < w; i++) {
            gfx_draw_pixel(x + i, y + j, color);
        }
    }
}

// ──────────────────────────────
// Main rendering
// ──────────────────────────────
void ui_render_home(void)
{
    const char* MENU_ITEMS[] = {"Apps", "Tools", "Settings", "About"};
    int y = 20;
    for (int i = 0; i < 4; i++) {
        gfx_draw_text(8, y, MENU_ITEMS[i]);
        y += 10;
    }

    (void)gfx_update();
}

void ui_home_next(void)
{
    current_index = (current_index + 1) % 5;
    ui_render_home();
}

void ui_home_prev(void)
{
    current_index = (current_index + 4) % 5;
    ui_render_home();
}
