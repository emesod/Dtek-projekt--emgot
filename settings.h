#ifndef SETTINGS_H
#define SETTINGS_H

#include <stdint.h>

#define HEIGHT 240
#define WIDTH 320
#define PIPE_GAP 50
#define PIPE_SPEED 1
#define GRAVITY 1
#define JUMP_STRENGTH 10
#define MAX_PIPES 3
#define PIPE_WIDTH 30
#define PIPE_SPACING 100
#define GRAVITY 1
#define MAX_FALL_SPEED 10

#define VGA_DEFAULT_WIDTH 320
#define VGA_DEFAULT_HEIGHT 240
#define VGA_DEFAULT_BASE_ADDRESS 0x08000000UL

struct rect {
    int x;
    int y;
    int width;
    int height;
    uint8_t color;
};

typedef struct {
    int x;
    int y;
    int width;
    int height; 
} Pipe;

/// ---------Game state struct------------------
typedef struct {
    int birdY;
    int velocity;
    int pipeX;
    int gapY;
    Pipe pipes[MAX_PIPES];
    int score;
} GameState;

#endif /* SETTINGS_H */

