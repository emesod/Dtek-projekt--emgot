# Dtek-projekt--emgot


## Description
This is a small flappy chicken game that runs on a DE10-LITE board, created for the course IS1500 (DTEK) at KTH.
The main game loop runs on interuppts from the internal timer.
On each interuppt game logic is executed and then drawn to the screen through sending pixels through the VGA.


## How to run
Connect the board to a monitor through VGA. 
Compile with "make" and then run "dtekv-run main.bin"

Using a button (key1) you can make the chicken jump and accumulate score. Pressing the jump button after death simply resets the game and the score.