#include "settings.c"

GameState gs;
int game_running = 1;

typedef struct {
    int x;        // left side of the pipe
    int gapY;     // top of the gap
    int width;    // usually 3–5 chars wide
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


struct rect {
    int x; // x coordinate (top left)
    int y; // y coordinate (top left)
    int width; 
    int height; 
    int color; 
};


// --------------init functions--------------
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


/// -------------Game-----------

void handle_interrupt(unsigned cause)
{
    volatile unsigned int* status = (volatile unsigned int*) 0x04000020;

    if (*status & 1) {
        *status = 0;

        processInput(&gs);
        applyGravity(&gs);
        updatePipes(&gs);

        if (checkCollision(&gs)) {
            game_running = 0;
        }

        draw(&gs);
    }
}

void initInterruptTimer(void)
{
    int timer_base = 0x04000020;
    int cycles = 3000000-1;  // 30 MHz, 100ms frame

    volatile unsigned short* period_l = (volatile unsigned short*)(timer_base + 0x08);
    volatile unsigned short* period_h = (volatile unsigned short*)(timer_base + 0x0C);

    *period_l = cycles & 0xFFFF;
    *period_h = (cycles >> 16) & 0xFFFF;

    volatile int* control =  (volatile int*)(timer_base + 0x04);
    volatile int* status  = (volatile int*)(timer_base + 0x00);

    *status = 0;     // clear pending
    *control = 6;    // enable timer + interrupt
}

applyGravity(GameState *gs){

}


int checkCollision(GameState *gs) {
    int birdX = 5; // bird is always in the middle of the screen

    for (int i = 0; i < MAX_PIPES; i++) {
        Pipe *p = &gs->pipes[i];

        // compare x-coordinate for pipe and bird
        if (birdX >= p->x && birdX < p->x + p->width) {

            // top pipe collision
            if (gs->birdY < p->gapY) return 1;

            // bottom pipe collision
            if (gs->birdY > p->gapY + PIPE_GAP) return 1;
        }
    }

    return 0;
}

// ----input-----
processInput(GameState *gs){
int btn = get_btn();

    if (btn != 0) {
        gs->velocity = JUMP_STRENGTH;
    }
}

int get_btn(void){
  volatile int* button_status = (volatile int*) 0x040000d0;
  return *button_status;
}

// ------------Score----------

void updateScore(GameState *gs) { // When pipe moves left past the bird
    if (gs->pipeX == 1) {   
        gs->score++;
    }
}

void showScore(GameState *gs) {
    printf("Score: %d\n", gs->score);
}


//  ------------------main----------------
int main(int argc, char const *argv[]){

    initGame(&gs);
    initInterruptTimer();

    while (game is running){
    processInput()
    applyGravity()
    updatePipe()
    if (collision) stop
    draw()
    delay()
    }
}

