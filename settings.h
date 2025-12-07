// Algot and Emelie worked together on this code 
#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

#define HEIGHT 240
#define WIDTH 320
#define PIPE_GAP 100
#define PIPE_SPEED 3
#define GRAVITY 1
#define JUMP_STRENGTH 5
#define MAX_PIPES 3
#define PIPE_WIDTH 30
#define PIPE_SPACING 100
#define GRAVITY 1
#define MAX_FALL_SPEED 10

#define VGA_DEFAULT_WIDTH 320
#define VGA_DEFAULT_HEIGHT 240
#define VGA_DEFAULT_BASE_ADDRESS 0x08000000UL
#define VGA_BUFFER_SIZE (VGA_DEFAULT_WIDTH * VGA_DEFAULT_HEIGHT)

#define VGA_DMA_BASE 0x04000100UL

struct rect {
    int x;
    int y;
    int width;
    int height;
    uint8_t color;
};

typedef struct {
    int x;
    int gapY;
    int scored;
} Pipe;

typedef struct {
    int birdY;
    int velocity;
    Pipe pipes[MAX_PIPES];
    int score;
    int isDead;
} GameState;

#endif /* SETTINGS_H */

