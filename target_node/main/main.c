#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"

// Logging tag
static const char *TAG = "NODE_TARGET";

// --- Pin Definitions ---
#define LED1_GPIO    13
#define LED2_GPIO    12
#define TRIG_GPIO    5
#define ECHO_GPIO    18

// --- Timing and Flags ---
volatile unsigned long last_time = 0;
volatile unsigned long current_time = 0;
volatile unsigned long interval = 2000;  // Default interval
volatile bool timeUpdated = false;
volatile bool sensorTrig = false;
volatile bool timeFail = false;
volatile bool cUpdated = false;
volatile bool nodeoff = false;

// --- Main Controller MAC Address ---
// Replace with the MAC address of your main controller ESP32.
uint8_t mainAddress[6] = {0xAC, 0x15, 0x18, 0xED, 0x86, 0x58};

// --- Function: Read Sensor ---
// This is a simplified version. In a real application, you should measure the pulse width.
float readSensor(void) {
    gpio_set_level(TRIG_GPIO, 0);
    vTaskDelay(pdMS_TO_TICKS(2));
    gpio_set_level(TRIG_GPIO, 1);
    vTaskDelay(pdMS_TO_TICKS(10));
    gpio_set_level(TRIG_GPIO, 0);
    
    // Simulated pulse duration; replace with a proper measurement method.
    uint32_t duration = 100;
    float distance = (duration / 2.0) * 0.0344;
    return distance;
}

// --- ESP-NOW Receive Callback for Node ---
void receive_callback_target(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    char message[50];
    if(data_len < sizeof(message))
        strncpy(message, (const char *)data, data_len);
    message[data_len] = '\0';
    ESP_LOGI(TAG, "Received: %s", message);
    
    if ((strcmp(message, "nodeonEasy") == 0 || strcmp(message, "nodeonMed") == 0 ||
         strcmp(message, "nodeonHard") == 0) && !timeUpdated) {
        last_time = current_time;
        timeUpdated = true;
    }
    
    if ((strcmp(message, "nodeonEasy") == 0) && !sensorTrig && !timeFail && cUpdated) {
        interval = 6000;
        float distance = readSensor();
        ESP_LOGI(TAG, "Distance: %f", distance);
        gpio_set_level(LED1_GPIO, 1);
        gpio_set_level(LED2_GPIO, 1);
        if (distance < 35 && distance > 0) {
            sensorTrig = true;
            gpio_set_level(LED1_GPIO, 0);
            gpio_set_level(LED2_GPIO, 0);
            cUpdated = false;
        } else if (current_time - last_time >= interval) {
            timeFail = true;
            gpio_set_level(LED1_GPIO, 0);
            gpio_set_level(LED2_GPIO, 0);
            cUpdated = false;
        }
    }
    
    if ((strcmp(message, "nodeonMed") == 0) && !sensorTrig && !timeFail && cUpdated) {
        interval = 4000;
        float distance = readSensor();
        ESP_LOGI(TAG, "Distance: %f", distance);
        gpio_set_level(LED1_GPIO, 1);
        gpio_set_level(LED2_GPIO, 1);
        if (distance < 35 && distance > 0) {
            sensorTrig = true;
            gpio_set_level(LED1_GPIO, 0);
            gpio_set_level(LED2_GPIO, 0);
            cUpdated = false;
        } else if (current_time - last_time >= interval) {
            timeFail = true;
            gpio_set_level(LED1_GPIO, 0);
            gpio_set_level(LED2_GPIO, 0);
            cUpdated = false;
        }
    }
    
    if ((strcmp(message, "nodeonHard") == 0) && !sensorTrig && !timeFail && cUpdated) {
        interval = 2000;
        float distance = readSensor();
        ESP_LOGI(TAG, "Distance: %f", distance);
        gpio_set_level(LED1_GPIO, 1);
        gpio_set_level(LED2_GPIO, 1);
        if (distance < 35 && distance > 0) {
            sensorTrig = true;
            gpio_set_level(LED1_GPIO, 0);
            gpio_set_level(LED2_GPIO, 0);
            cUpdated = false;
        } else if (current_time - last_time >= interval) {
            timeFail = true;
            gpio_set_level(LED1_GPIO, 0);
            gpio_set_level(LED2_GPIO, 0);
            cUpdated = false;
        }
    }
    
    if (strcmp(message, "nodeoff") == 0) {
        ESP_LOGI(TAG, "NODE OFF RECEIVED");
        gpio_set_level(LED1_GPIO, 0);
        gpio_set_level(LED2_GPIO, 0);
        timeUpdated = false;
        cUpdated = true;
        nodeoff = true;
    }
}

// --- Node Target Task ---
void node_target_task(void *pvParameters) {
    while(1) {
        current_time = xTaskGetTickCount() * portTICK_PERIOD_MS;
        
        if (sensorTrig) {
            char sensorTriggered[] = "sensor:Triggered";
            ESP_ERROR_CHECK(esp_now_send(mainAddress, (uint8_t *)sensorTriggered, strlen(sensorTriggered)));
            sensorTrig = false;
        }
        
        if (timeFail) {
            char timeFailed[] = "timeFailed";
            ESP_ERROR_CHECK(esp_now_send(mainAddress, (uint8_t *)timeFailed, strlen(timeFailed)));
            timeFail = false;
        }
        
        if (nodeoff) {
            ESP_LOGI(TAG, "NODE OFF RECEIVED");
            char newCycle[] = "newCycle";
            ESP_ERROR_CHECK(esp_now_send(mainAddress, (uint8_t *)newCycle, strlen(newCycle)));
            nodeoff = false;
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// --- GPIO Initialization for Node ---
void init_gpio_target(void) {
    gpio_config_t io_conf;
    // Configure LED pins as outputs
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << LED1_GPIO) | (1ULL << LED2_GPIO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
    
    // Configure TRIG pin as output
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << TRIG_GPIO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
    
    // Configure ECHO pin as input
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << ECHO_GPIO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

// --- Wi-Fi and ESP-NOW Initialization for Node ---
void init_wifi_espnow_target(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(receive_callback_target));
    
    // Add main controller as peer
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, mainAddress, 6);
    peerInfo.channel = 0;
    peerInfo.ifidx = ESP_IF_WIFI_STA;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}

// --- app_main Entry Point for Node ---
void app_main(void)
{
    init_gpio_target();
    init_wifi_espnow_target();
    
    xTaskCreate(node_target_task, "node_target_task", 4096, NULL, 5, NULL);
}
