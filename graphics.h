// This code was written by Algot
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "settings.h"
#include <stdint.h>

void clear_screen(uint8_t color);
void draw_pixel(int x, int y, uint8_t color);
void fill_rect(const struct rect *r);
void draw_sprite(const uint8_t *spritemap, int sprite_x, int sprite_y, int sprite_width, int sprite_height);
uint8_t rgb332(uint8_t r, uint8_t g, uint8_t b);

#endif /* GRAPHICS_H */

