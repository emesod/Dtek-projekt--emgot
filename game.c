#include "graphics.h"
#include "settings.h"
#include "dtekv-lib.h"
#include <stdlib.h>

int game_running = 1;
GameState gs;

extern void enable_interrupt(void);

// --------------init functions--------------
void initGame(GameState *gs) {
    gs->birdY = HEIGHT / 2;
    gs->velocity = 0;
    gs->pipeX = WIDTH - 1;
    gs->gapY = (HEIGHT - PIPE_GAP - 1);
    gs->score = 0;
}

void resetPipe(GameState *gs) {
    gs->pipeX = WIDTH - 1;
    gs->gapY = (HEIGHT - PIPE_GAP - 1);
}


/// -------------Game-----------



void initInterruptTimer(void)
{
    int timer_base = 0x04000020;
    int cycles = 3000000-1;  // 30000 MHz, 10ms frame

    volatile unsigned short* period_l = (volatile unsigned short*)(timer_base + 0x08);
    volatile unsigned short* period_h = (volatile unsigned short*)(timer_base + 0x0C);

    *period_l = cycles & 0xFFFF;
    *period_h = (cycles >> 16) & 0xFFFF;

    volatile int* control =  (volatile int*)(timer_base + 0x04);
    volatile int* status  = (volatile int*)(timer_base + 0x00);

    *status = 0;     // clear pending
    *control = 7;    // enable timer + interrupt
    
    // Enable timer interrupt in platform interrupt controller
    volatile unsigned int* interruptmask = (volatile unsigned int*) 0x04000018;
    *interruptmask = 8;
    
    enable_interrupt();
    print("Interrupt timer initialized\n");
}


void applyGravity(GameState *gs){
      gs->velocity += GRAVITY;
    if (gs->velocity > MAX_FALL_SPEED)
        gs->velocity = MAX_FALL_SPEED;

    gs->birdY += gs->velocity;

    // --- death check ---
    if (gs->birdY < 0 || gs->birdY >= HEIGHT) {
        printf("you died :(\n");
        game_running = 0;
    }
}


int checkCollision(GameState *gs) {
    int birdX = 5; // bird is always in the middle of the screen

    for (int i = 0; i < MAX_PIPES; i++) {
        Pipe *p = &gs->pipes[i];

        // horizontal collision
        if (birdX >= p->x && birdX < p->x + p->width) {

            // vertical collision
            if (birdY >= p->y && birdY < p->y + p->height) {
                return 1; 
            }
        }
    }

    return 0;
}

// ----input-----
void processInput(GameState *gs){
int btn = get_btn();

    if (btn != 0) {
        gs->velocity = -JUMP_STRENGTH;
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

void handle_interrupt(unsigned cause)
{
    volatile unsigned short* timer_status = (volatile unsigned short*) 0x04000020;
    
    if (*timer_status & 1) {
        *timer_status = 0;

        processInput(&gs);
        applyGravity(&gs);
        //updatePipes(&gs);
        

        if (checkCollision(&gs)) {
            print("Death\n");
        }

        struct rect r;
        r.x = 100;
        r.y = 60;
        r.width = 120;
        r.height = 60;
        r.color = rgb332(7, 7, 0); // Bright yellow in rgb332

        rect.x += 1;
        clear_screen(rgb332(0,0,0)); // Clear to black
        fill_rect(&r);

    }
}
//  ------------------main----------------
int main(int argc, char const *argv[]){

    initGame(&gs);
    initInterruptTimer();

    while (1) {
        __asm__ volatile("nop");
    }
}


