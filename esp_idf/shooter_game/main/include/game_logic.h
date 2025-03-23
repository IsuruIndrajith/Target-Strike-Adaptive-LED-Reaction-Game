// main/include/game_logic.h
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>

// Game state
typedef struct {
    bool one_player;
    bool two_player;
    bool easy;
    bool med;
    bool hard;
    bool options;
    bool menu_music;
    bool restart;
    int score1;
    int score;
    int sequence_index;
    int button1_count;
    int button2_count;
    int button3_count;
    bool cycle_updated;
    bool message_received;
    int sequence[9];
} game_state_t;

// Initialize game logic
void game_logic_init(void);

// Update game state
void game_logic_update(void);

// Process sensor trigger event
void sensor_triggered(void);

// Process time failure event
void time_failed(void);

// Set cycle updated flag
void set_cycle_updated(bool updated);

// Get current game state
game_state_t* get_game_state(void);

#endif /* GAME_LOGIC_H */