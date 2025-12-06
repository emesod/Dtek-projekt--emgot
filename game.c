#include "graphics.h"
#include "settings.h"
#include "dtekv-lib.h"
#include "sprites.h"
#include <stdlib.h>

int game_running = 1;
GameState gs;

extern void enable_interrupt(void);
static unsigned int seed = 1234567;

int my_rand(void)
{
    seed = seed * 1103515245 + 12345;
    return (seed >> 16) & 0x7FFF;
}

// --------------init functions--------------
void initGame(GameState *gs)
{
    gs->birdY = HEIGHT / 2;
    gs->velocity = 0;
    gs->score = 0;

    for (int i = 0; i < MAX_PIPES; i++)
    {
        gs->pipes[i].x = WIDTH + i * (WIDTH / MAX_PIPES); // spread out
        gs->pipes[i].gapY = my_rand() % (HEIGHT - PIPE_GAP - 1);
    }
}

void restartGame(GameState *gs)
{
    initGame(gs);       // reset everything
    gs->isDead = 0;     // bird lives again
}

void updatePipes(GameState *gs)
{
    for (int i = 0; i < MAX_PIPES; i++)
    {
        gs->pipes[i].x -= PIPE_SPEED;

        // if pipe moves off screen, reset it
        if (gs->pipes[i].x + PIPE_WIDTH < 0)
        {
            gs->pipes[i].x = WIDTH;
            gs->pipes[i].gapY = my_rand() % (HEIGHT - PIPE_GAP - 1);
            gs->pipes[i].scored = 0;
        }
    }
}

/// -------------Game-----------

void initInterruptTimer(void)
{
    int timer_base = 0x04000020;
    int cycles = 3000000 - 1; // 30000 MHz, 10ms frame

    volatile unsigned short *period_l = (volatile unsigned short *)(timer_base + 0x08);
    volatile unsigned short *period_h = (volatile unsigned short *)(timer_base + 0x0C);

    *period_l = cycles & 0xFFFF;
    *period_h = (cycles >> 16) & 0xFFFF;

    volatile int *control = (volatile int *)(timer_base + 0x04);
    volatile int *status = (volatile int *)(timer_base + 0x00);

    *status = 0;  // clear pending
    *control = 7; // enable timer + interrupt

    // Enable timer interrupt in platform interrupt controller
    volatile unsigned int *interruptmask = (volatile unsigned int *)0x04000018;
    *interruptmask = 8;

    enable_interrupt();
    print("Interrupt timer initialized\n");
}

void applyGravity(GameState *gs)
{
    gs->velocity += GRAVITY;
    if (gs->velocity > MAX_FALL_SPEED)
        gs->velocity = MAX_FALL_SPEED;

    gs->birdY += gs->velocity;
}

void deadBird(GameState *gs){
    gs->isDead = 1; // mark the bird as dead
    gs->velocity = 2;
}

int checkDeath(GameState *gs)
{
    int birdX = 50;
    int birdY = gs->birdY;
    int birdW = 30;
    int birdH = 30;

    for (int i = 0; i < MAX_PIPES; i++)
    {
        Pipe *p = &gs->pipes[i];

        int pipeLeft = p->x;
        int pipeRight = p->x + PIPE_WIDTH;

        // Check if bird is horizontally inside pipe
        if (birdX + birdW > pipeLeft && birdX < pipeRight)
        {

            // If bird is above gap or below gap there is a collision
            if (birdY < p->gapY ||
                birdY + birdH > p->gapY + PIPE_GAP)
            {
                return 1;
            }
        }
    }
    if (gs->birdY < 0 || gs->birdY >= HEIGHT)
    {
        return 1;
    }
    return 0;
}

// ----input-----

int get_btn(void)
{
    volatile int *button_status = (volatile int *)0x040000d0;
    return *button_status;
}

void processInput(GameState *gs){
    int btn = get_btn();
    if (gs->isDead) {
        if (btn != 0) {
            print("Restarting game.\n");
            restartGame(gs);
        }
        return;
    }    

    if (btn != 0)
    {
        gs->velocity = -JUMP_STRENGTH;
    }
}

// ------------Score----------

void updateScore(GameState *gs)
{ // When bird moves left past the pipe
    int birdX = 50;

    for (int i = 0; i < MAX_PIPES; i++)
    {
        Pipe *p = &gs->pipes[i];

        // If bird has passed pipe and hasn't been scored yet
        if (!p->scored && p->x + PIPE_WIDTH < birdX)
        {
            gs->score++;
            p->scored = 1; // mark as scored
            print("Score: ");
            print_dec(gs->score);
            print("\n");
        }
    }
}

void showScore(GameState *gs)
{
    // printf("Score: %d\n", gs->score);
}

void handle_interrupt(unsigned cause)
{
    volatile unsigned short *timer_status = (volatile unsigned short *)0x04000020;

    if (*timer_status & 1)
    {
        *timer_status = 0;

        processInput(&gs);
         // Check for death **before** applying gravity
        if (!gs.isDead && checkDeath(&gs)){
            print("Death\n");
            deadBird(&gs);
        }

        // Apply gravity
        if (!gs.isDead){
            applyGravity(&gs);
            updatePipes(&gs);
            updateScore(&gs);
        } else {
            // for dead bird
            gs.velocity += GRAVITY / 2;
            if (gs.velocity > MAX_FALL_SPEED / 2)
                gs.velocity = MAX_FALL_SPEED / 2;
            gs.birdY += gs.velocity;

            // Stop bird at bottom
            if (gs.birdY > HEIGHT - 30){
                gs.birdY = HEIGHT - 30;
                gs.velocity = 0;
            }
        }


        clear_screen(rgb332(0, 1, 3));


        for (int i = 0; i < MAX_PIPES; i++)
        {
            Pipe *p = &gs.pipes[i];

            // top pipe
            struct rect top;
            top.x = p->x;
            top.y = 0;
            top.width = PIPE_WIDTH;
            top.height = p->gapY;
            top.color = rgb332(5, 0, 0);
            fill_rect(&top);

            // bottom pipe
            struct rect bottom;
            bottom.x = p->x;
            bottom.y = p->gapY + PIPE_GAP;
            bottom.width = PIPE_WIDTH;
            bottom.height = HEIGHT - bottom.y;
            bottom.color = rgb332(0, 5, 0);
            fill_rect(&bottom);
        }
         // bird
        draw_sprite(test_sprite, 50, gs.birdY, 30, 30);
    }
}
//  ------------------main----------------
int main(int argc, char const *argv[])
{

    initGame(&gs);
    initInterruptTimer();

    while (1)
    {
        __asm__ volatile("nop");
    }
}
