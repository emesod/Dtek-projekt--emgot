#include "settings.c"


typedef struct {
    int x;        // left side of the pipe
    int gapY;     // top of the gap
    int width;    // usually 3–5 chars wide
} Pipe;

/// Game state struct
typedef struct {
    int birdY;
    int velocity;
    int pipeX;
    int gapY;
    Pipe pipes[MAX_PIPES];
    int score;
} GameState;


struct rect {
    int x; // x coordinate (top left)
    int y; // y coordinate (top left)
    int width; 
    int height; 
    int color; 
};


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