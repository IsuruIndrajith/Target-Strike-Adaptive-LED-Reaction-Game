#include "ultrasonic_sensor.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


#define TRIG_GPIO 5
#define ECHO_GPIO 18
static const char *TAG = "ultrasonic";

void ultrasonic_init(void) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL<<TRIG_GPIO),
        .mode = GPIO_MODE_OUTPUT,
    };
    gpio_config(&io_conf);
    io_conf.pin_bit_mask = (1ULL<<ECHO_GPIO);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    io_conf.pull_down_en = 0;
    gpio_config(&io_conf);
}

float ultrasonic_measure_cm(void) {
    // Trigger pulse
    gpio_set_level(TRIG_GPIO, 0);
    ets_delay_us(2);
    gpio_set_level(TRIG_GPIO, 1);
    ets_delay_us(10);
    gpio_set_level(TRIG_GPIO, 0);
    // Wait for echo start
    uint32_t start = esp_timer_get_time();
    while (gpio_get_level(ECHO_GPIO)==0) {
        if (esp_timer_get_time() - start > 30000) {
            ESP_LOGW(TAG, "No echo");
            return -1;
        }
    }
    uint32_t echo_start = esp_timer_get_time();
    while (gpio_get_level(ECHO_GPIO)==1);
    uint32_t echo_end = esp_timer_get_time();
    float dt = (echo_end - echo_start) * 1e-6; // seconds
    float distance = dt * 343.0f / 2.0f * 100.0f; // cm
    if (distance<2 || distance>400) {
        ESP_LOGW(TAG, "Out of range: %.1f cm", distance);
        return -1;
    }
    return distance;
}

