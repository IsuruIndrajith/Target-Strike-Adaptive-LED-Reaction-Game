#include <stdio.h>

// Include headers from our components
#include "firebase/firebase.h"
#include "oled/oled.h"
#include "sev_seg/sev_seg.h"
#include "espnow_comm/espnow_comm.h"
#include "game_logic/game_logic.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void app_main(void)
{
    // Initialize all components
    printf("Initializing Firebase...\n");
    firebase_init();

    printf("Initializing OLED...\n");
    oled_init();

    printf("Initializing Seven Segment...\n");
    sev_seg_init();

    printf("Initializing ESP-NOW Communication...\n");
    espnow_comm_init();

    printf("Starting Game Logic...\n");
    game_logic_start();

    // Main loop: periodically update game state
    while (1) {
        game_logic_update();
        // Here you might also update the display or update Firebase with new score:
        // firebase_update_score(new_score);
        // oled_display_text("Game update!");
        // sev_seg_display_digit(new_digit);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
