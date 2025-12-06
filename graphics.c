#include "graphics.h"
#include <stdint.h>
#include <stddef.h>

static inline int min_int(int a, int b)
{
    return (a < b) ? a : b;
}

static inline int max_int(int a, int b)
{
    return (a > b) ? a : b;
}

// Double buffering setup
static volatile uint8_t *buffer0 = (volatile uint8_t *)(VGA_DEFAULT_BASE_ADDRESS);
static volatile uint8_t *buffer1 = (volatile uint8_t *)(VGA_DEFAULT_BASE_ADDRESS + VGA_BUFFER_SIZE);
static volatile uint8_t *current_draw_buffer = (volatile uint8_t *)(VGA_DEFAULT_BASE_ADDRESS);  // Which buffer we're drawing to
static int current_buffer_index = 0;

// DMA Controller registers (using byte offsets for clarity)
static volatile uint8_t *dma_controller_base = (volatile uint8_t *)(VGA_DMA_BASE);

static int screen_buffer_width = VGA_DEFAULT_WIDTH;
static int screen_buffer_height = VGA_DEFAULT_HEIGHT;


void swap_buffers(void) {

    volatile uint32_t *status_reg = (volatile uint32_t *)(dma_controller_base + 0x0C);
    while (*status_reg & 0x01) {
      
    }
    


    volatile uint32_t *backbuffer_reg = (volatile uint32_t *)(dma_controller_base + 0x04);
    *backbuffer_reg = (uint32_t)current_draw_buffer;
    
    
    volatile uint32_t *buffer_reg = (volatile uint32_t *)(dma_controller_base);
    *buffer_reg = 1;
    
   
    if (current_buffer_index == 0) {
        current_draw_buffer = buffer1;
        current_buffer_index = 1;
    } else {
        current_draw_buffer = buffer0;
        current_buffer_index = 0;
    }
}

void clear_screen(uint8_t color) {
    int total_pixels = screen_buffer_width * screen_buffer_height;
    for (int i = 0; i < total_pixels; ++i) {
        current_draw_buffer[i] = color;
    }
}

void draw_pixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= screen_buffer_width || y < 0 || y >= screen_buffer_height) {
        return;
    }
    current_draw_buffer[y * screen_buffer_width + x] = color;
}

void fill_rect(const struct rect *r) {

    int x_start = max_int(0, min_int(r->x, screen_buffer_width - 1));
    int y_start = max_int(0, min_int(r->y, screen_buffer_height - 1));
    int x_end = max_int(0, min_int(r->x + r->width, screen_buffer_width));
    int y_end = max_int(0, min_int(r->y + r->height, screen_buffer_height));

    for (int y = y_start; y < y_end; ++y) {
        volatile uint8_t *row = current_draw_buffer + y * screen_buffer_width;
        for (int x = x_start; x < x_end; ++x) {
            row[x] = r->color;
        }
    }
}

void draw_sprite(const uint8_t *spritemap, int sprite_x, int sprite_y, int sprite_width, int sprite_height) {
    for (int y = 0; y < sprite_height; ++y) {
        for (int x = 0; x < sprite_width; ++x) {
            int index = y * sprite_width + x;
            uint8_t color = spritemap[index];
            if (color != 0) {
                draw_pixel(sprite_x + x, sprite_y + y, color);
            }
        }
    }
}

uint8_t rgb332(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0x07) << 5) | ((g & 0x07) << 2) | (b & 0x03);
}