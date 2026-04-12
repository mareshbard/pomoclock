#ifndef SSD1306_H
#define SSD1306_H

#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t addr;
    i2c_inst_t *i2c;
    uint8_t buffer[1024];
} ssd1306_t;

void ssd1306_init_bm(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t addr, i2c_inst_t *i2c);
void ssd1306_config(ssd1306_t *ssd);
void ssd1306_send_data(ssd1306_t *ssd);
void ssd1306_draw_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool set);
void ssd1306_fill(ssd1306_t *ssd, bool set);
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y);
void ssd1306_draw_bitmap(ssd1306_t *ssd, const uint8_t *bitmap);

#endif