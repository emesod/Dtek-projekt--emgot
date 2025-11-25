#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#define HEIGHT 150
#define WIDTH 40
#define PIPE_GAP 5
#define PIPE_SPEED 1
#define GRAVITY 1
#define JUMP_STRENGTH -2
#define MAX_PIPES 3
#define PIPE_WIDTH 30
#define PIPE_SPACING 100 

/// Screen settings
#define VGA_DEFAULT_WIDTH 320
#define VGA_DEFAULT_HEIGHT 240
#define VGA_DEFAULT_BASE_ADDRESS 0x08000000UL


/// structs
struct rect {
    int x; // x coordinate (top left)
    int y; // y coordinate (top left)
    int width; 
    int height; 
    uint8_t color; 
};


