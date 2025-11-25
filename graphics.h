#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "settings.h"
#include <stdint.h>

void clear_screen(uint8_t color);
void draw_pixel(int x, int y, uint8_t color);
void fill_rect(const struct rect *r);
uint8_t rgb332(uint8_t r, uint8_t g, uint8_t b);

#endif /* GRAPHICS_H */

