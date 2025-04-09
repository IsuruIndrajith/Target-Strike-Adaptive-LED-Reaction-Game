#include "game_logic.h"
#include <stdio.h>

// In this simple stub, we maintain a score and a round counter.
static int current_score = 0;
static int current_round = 0;

void game_logic_start(void)
{
    current_score = 0;
    current_round = 1;
    printf("Game Logic: Starting game - Round %d, Score %d (stub).\n", current_round, current_score);
}

void game_logic_update(void)
{
    // For demonstration, we simply increment score and round.
    current_score += 10;
    current_round += 1;
    printf("Game Logic: Updated game - Round %d, Score %d (stub).\n", current_round, current_score);

    // Here you might also call firebase_update_score(current_score)
    // or update the displays.
}
