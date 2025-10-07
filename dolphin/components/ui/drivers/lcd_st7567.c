#include "lcd_st7567.h"
#include "driver/i2c.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <string.h>

#define TAG "ST7567"
#define LCD_I2C_ADDR 0x3F
#define LCD_PORT     I2C_NUM_0

static bool i2c_ready = false;

static esp_err_t st7567_write(uint8_t control, uint8_t data) {
    if (!i2c_ready) {
        ESP_LOGE(TAG, "I2C driver not installed!");
        return ESP_FAIL;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (LCD_I2C_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, control, true);
    i2c_master_write_byte(cmd, data, true);
    i2c_master_stop(cmd);
    esp_err_t ret = i2c_master_cmd_begin(LCD_PORT, cmd, 100 / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(cmd);
    return ret;
}

static inline void st7567_cmd(uint8_t cmd)   { st7567_write(0x00, cmd); }
static inline void st7567_data(uint8_t data) { st7567_write(0x40, data); }

void lcd_i2c_init(int sda, int scl, int freq_hz) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda,
        .scl_io_num = scl,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = freq_hz,
    };
    ESP_ERROR_CHECK(i2c_param_config(LCD_PORT, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(LCD_PORT, conf.mode, 0, 0, 0));
    i2c_ready = true;
    ESP_LOGI(TAG, "I2C initialized on SDA=%d SCL=%d", sda, scl);
}

void lcd_init(uint8_t bias, uint8_t ratio, uint8_t contrast) {
    vTaskDelay(pdMS_TO_TICKS(50));
    st7567_cmd(0xAE);
    st7567_cmd(bias);        // 0xA2=1/9, 0xA3=1/7
    st7567_cmd(0xA0);        // ADC normal
    st7567_cmd(0xC8);        // COM reverse
    st7567_cmd(0x2F);        // Power control
    st7567_cmd(ratio);       // Resistor ratio
    st7567_cmd(0x81);        // Contrast
    st7567_cmd(contrast & 0x3F);
    st7567_cmd(0xAF);        // Display ON
    ESP_LOGI(TAG, "ST7567 initialized");
}

void lcd_set_contrast(uint8_t contrast) {
    st7567_cmd(0x81);
    st7567_cmd(contrast & 0x3F);
}

void lcd_update(const uint8_t *buffer) {
    for (int page = 0; page < 8; page++) {
        st7567_cmd(0xB0 | page);
        st7567_cmd(0x10);  // col high
        st7567_cmd(0x00);  // col low
        for (int col = 0; col < LCD_WIDTH; col++) {
            st7567_data(buffer[page * LCD_WIDTH + col]);
        }
    }
}
