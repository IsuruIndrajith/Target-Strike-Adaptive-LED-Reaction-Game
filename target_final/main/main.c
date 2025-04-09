#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "espnow_comm.h"
#include "ultrasonic_sensor.h"
#include "driver/gpio.h"
#include "esp_timer.h"      // For esp_timer_get_time()
#include "driver/gpio.h"    // For GPIO functions
#include "ultrasonic_sensor.h" // For ultrasonic_init()

static const char *TAG = "app_main";
static bool targetActive = false;
static uint32_t activationTime = 0;
static uint32_t timeLimit = 10000; // easy default

// Receive handler
static void on_recv(const uint8_t *data, size_t len) {
    char msg[32];
    memcpy(msg, data, len);
    msg[len] = '\0';
    ESP_LOGI(TAG, "Recv: %s", msg);

    if (strncmp(msg, "nodeon", 6)==0) {
        gpio_set_level(GPIO_NUM_2, 1);
        targetActive = true;
        activationTime = esp_timer_get_time()/1000;
        if (strcmp(msg, "nodeonMed")==0) timeLimit = 7000;
        else if (strcmp(msg, "nodeonHard")==0) timeLimit = 5000;
        else timeLimit = 10000;
    } else if (strcmp(msg, "nodeoff")==0) {
        gpio_set_level(GPIO_NUM_2, 0);
        targetActive = false;
    }
    // handle other commands similarly...
}

void app_main(void) {
    ESP_LOGI(TAG, "Starting...");
    // LED
    gpio_pad_select_gpio(GPIO_NUM_2);
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_NUM_2, 0);

    ultrasonic_init();
    espnow_init(on_recv);

    while (1) {
        vTaskDelay(pdMS_TO_TICKS(500));
        float dist = ultrasonic_measure_cm();
        if (dist < 0) continue;

        static uint32_t lastSend = 0;
        uint32_t now = esp_timer_get_time()/1000;

        // example: periodic distance send
        if (now - lastSend > 500 && !targetActive) {
            char buf[32];
            snprintf(buf, sizeof(buf), "distance:%.1f", dist);
            espnow_send(buf);
            lastSend = now;
        }

        // active mode
        if (targetActive) {
            if (now - activationTime > timeLimit) {
                espnow_send("timeFailed");
                targetActive = false;
                gpio_set_level(GPIO_NUM_2, 0);
            } else if (dist < 30) {
                espnow_send("sensor:Triggered");
                // flash LED
                gpio_set_level(GPIO_NUM_2, 0);
                vTaskDelay(pdMS_TO_TICKS(100));
                gpio_set_level(GPIO_NUM_2, 1);
                // cooldown
                vTaskDelay(pdMS_TO_TICKS(1000));
            }
        }
    }
}
