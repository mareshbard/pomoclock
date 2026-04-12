#include "ssd1306.h"
#include <string.h>

// Função para enviar comandos I2C
void ssd1306_command(ssd1306_t *ssd, uint8_t command) {
    uint8_t buffer[2] = {0x00, command};
    i2c_write_blocking(ssd->i2c, ssd->addr, buffer, 2, false);
}

void ssd1306_init_bm(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t addr, i2c_inst_t *i2c) {
    ssd->width = width;
    ssd->height = height;
    ssd->addr = addr;
    ssd->i2c = i2c;
    memset(ssd->buffer, 0, sizeof(ssd->buffer));
}

void ssd1306_config(ssd1306_t *ssd) {
    ssd1306_command(ssd, 0xAE); // Display OFF
    ssd1306_command(ssd, 0x20); // Set Memory Addressing Mode
    ssd1306_command(ssd, 0x00); // Horizontal addressing mode
    
    ssd1306_command(ssd, 0x21); // Set Column Address
    ssd1306_command(ssd, 0);    // ANTES: faltava o 'ssd' aqui
    ssd1306_command(ssd, 127);  // ANTES: faltava o 'ssd' aqui
    
    ssd1306_command(ssd, 0x22); // Set Page Address
    ssd1306_command(ssd, 0);    // ANTES: faltava o 'ssd' aqui
    ssd1306_command(ssd, 7);    // ANTES: faltava o 'ssd' aqui
    
    ssd1306_command(ssd, 0xAF); // Display ON
}

void ssd1306_send_data(ssd1306_t *ssd) {
    uint8_t temp[1025];
    temp[0] = 0x40; // Coordenada de dados
    memcpy(&temp[1], ssd->buffer, 1024);
    i2c_write_blocking(ssd->i2c, ssd->addr, temp, 1025, false);
}

void ssd1306_draw_bitmap(ssd1306_t *ssd, const uint8_t *bitmap) {
    memcpy(ssd->buffer, bitmap, 1024);
    ssd1306_send_data(ssd);
}

void ssd1306_fill(ssd1306_t *ssd, bool set) {
    memset(ssd->buffer, set ? 0xFF : 0x00, 1024);
}

// Uma fonte 8x8 simplificada (apenas para exemplo, você pode expandir depois)
// Se sua biblioteca original tinha uma fonte, use a dela.
void ssd1306_draw_char(ssd1306_t *ssd, char c, uint8_t x, uint8_t y) {
    if (x > 120 || y > 56) return;
    
    // Este é um exemplo simplificado. Para uma fonte real, 
    // você precisaria de um array com os desenhos das letras.
    // Como queremos apenas compilar, vamos desenhar um quadrado no lugar da letra
    // ou você pode colar um array de fonte aqui.
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            ssd1306_draw_pixel(ssd, x + i, y + j, true);
        }
    }
}

void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y) {
    while (*str) {
        ssd1306_draw_char(ssd, *str++, x, y);
        x += 8; 
        if (x > 120) break;
    }
}

// Adicione também a função de pixel que o código acima pede:
void ssd1306_draw_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool set) {
    if (x >= ssd->width || y >= ssd->height) return;
    if (set)
        ssd->buffer[x + (y / 8) * ssd->width] |= (1 << (y % 8));
    else
        ssd->buffer[x + (y / 8) * ssd->width] &= ~(1 << (y % 8));
}