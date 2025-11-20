#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>

#define HEIGHT 20
#define WIDTH 40
#define PIPE_GAP 5
#define PIPE_SPEED 1
#define GRAVITY 1
#define JUMP_STRENGTH -2

struct termios oldt, newt;

/// Game state struct
typedef struct {
    int birdY;
    int velocity;
    int pipeX;
    int gapY;
    int score;
} GameState;


// init functions
void initGame(GameState *gs) {
    gs->birdY = HEIGHT / 2;
    gs->velocity = 0;
    gs->pipeX = WIDTH - 1;
    gs->gapY = rand() % (HEIGHT - PIPE_GAP - 1);
    gs->score = 0;
}

void resetPipe(GameState *gs) {
    gs->pipeX = WIDTH - 1;
    gs->gapY = rand() % (HEIGHT - PIPE_GAP - 1);
}


/// game
applyGravity(GameState *gs){

}


processInput(GameState *gs){

}

checkCollision(GameState *gs){

}

// Score

void updateScore(GameState *gs) { // When pipe moves left past the bird
    if (gs->pipeX == 1) {   
        gs->score++;
    }
}

void showScore(GameState *gs) {
    printf("Score: %d\n", gs->score);
}


// main
while (game is running)
{
    processInput()
    applyGravity()
    updatePipe()
    if (collision) stop
    draw()
    delay()
}