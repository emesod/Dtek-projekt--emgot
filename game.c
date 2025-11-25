#include "graphics.h"
#include "settings.h"
#include <stdlib.h>
#include <stdio.h>
int game_running = 1;
struct GameState gs;

// --------------init functions--------------
void initGame(struct GameState *gs) {
    gs->birdY = HEIGHT / 2;
    gs->velocity = 0;
    gs->pipeX = WIDTH - 1;
    gs->gapY = (HEIGHT - PIPE_GAP - 1);
    gs->score = 0;
}

void resetPipe(struct GameState *gs) {
    gs->pipeX = WIDTH - 1;
    gs->gapY = (HEIGHT - PIPE_GAP - 1);
}


/// -------------Game-----------



void initInterruptTimer(void)
{
    int timer_base = 0x04000020;
    int cycles = 300000-1;  // 30000 MHz, 10ms frame

    volatile unsigned short* period_l = (volatile unsigned short*)(timer_base + 0x08);
    volatile unsigned short* period_h = (volatile unsigned short*)(timer_base + 0x0C);

    *period_l = cycles & 0xFFFF;
    *period_h = (cycles >> 16) & 0xFFFF;

    volatile int* control =  (volatile int*)(timer_base + 0x04);
    volatile int* status  = (volatile int*)(timer_base + 0x00);

    *status = 0;     // clear pending
    *control = 6;    // enable timer + interrupt
}

/* 
applyGravity(GameState *gs){

}
*/

int checkCollision(struct GameState *gs) {
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
int get_btn(void){
    volatile int* button_status = (volatile int*) 0x040000d0;
    return *button_status;
  }
  


processInput(struct GameState *gs){
    int btn = get_btn();

    if (btn != 0) {
        gs->velocity = JUMP_STRENGTH;
    }
}


// ------------Score----------

void updateScore(struct GameState *gs) { // When pipe moves left past the bird
    if (gs->pipeX == 1) {   
        gs->score++;
    }
}

void showScore(GameState *gs) {
    printf("Score: %d\n", gs->score);
}

void handle_interrupt(unsigned cause)
{
    volatile unsigned int* status = (volatile unsigned int*) 0x04000020;

    if (*status & 1) {
        *status = 0;

        processInput(&gs);
        //applyGravity(&gs);
        //updatePipes(&gs);

        if (checkCollision(&gs)) {
            printf("Death");
        }

        

    }
}
//  ------------------main----------------
int main(int argc, char const *argv[]){

    initGame(&gs);
    initInterruptTimer();
    while (1) {
    }
}


