#include "settings.c"
#include <stdint.h>
#include <stddef.h>


static volatile uint8_t *screen_buffer = (volatile uint8_t *)VGA_DEFAULT_BASE_ADDRESS;
static int screen_buffer_width = VGA_DEFAULT_WIDTH;
static int screen_buffer_height = VGA_DEFAULT_HEIGHT;


void clear_screen(uint8_t color) {
    int total_pixels = screen_buffer_width * screen_buffer_height;
    for (int i = 0; i < total_pixels; ++i) {
        screen_buffer[i] = color;
    }
}

void draw_pixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= screen_buffer_width || y < 0 || y >= screen_buffer_height) {
        return;
    }
    screen_buffer[y * screen_buffer_width + x] = color;
}

void fill_rect(const struct rect *r) {

    int x_start = max(0, min(r->x, screen_buffer_width - 1));
    int y_start = max(0, min(r->y, screen_buffer_height - 1));
    int x_end = max(0, min(r->x + r->width, screen_buffer_width));
    int y_end = max(0, min(r->y + r->height, screen_buffer_height));

    for (int y = y_start; y < y_end; ++y) {
        volatile uint8_t *row = screen_buffer + y * screen_buffer_width;
        for (int x = x_start; x < x_end; ++x) {
            row[x] = r->color;
        }
    }
}

uint8_t rgb332(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0x07) << 5) | ((g & 0x07) << 2) | (b & 0x03);
}