#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "driver/gpio.h"
#include "ssd1306.h"


// Configuration - Update these based on your setup
#define BUTTON1_GPIO    GPIO_NUM_12
#define BUTTON2_GPIO    GPIO_NUM_13
#define BUTTON3_GPIO    GPIO_NUM_5
#define BUZZER_GPIO     GPIO_NUM_15

// 7-Segment Pins
#define SEG_A_GPIO      GPIO_NUM_19
#define SEG_B_GPIO      GPIO_NUM_18
#define SEG_C_GPIO      GPIO_NUM_26
#define SEG_D_GPIO      GPIO_NUM_25
#define SEG_E_GPIO      GPIO_NUM_33
#define SEG_F_GPIO      GPIO_NUM_23
#define SEG_G_GPIO      GPIO_NUM_27

// MAC Addresses
static uint8_t receiver2Address[] = {0xB0, 0xA7, 0x32, 0x28, 0xD4, 0x64};
static uint8_t receiver3Address[] = {0xE0, 0x5A, 0x1B, 0xCB, 0x37, 0x60};
static uint8_t receiver4Address[] = {0xB0, 0xA7, 0x32, 0x16, 0x2E, 0x6C};
static uint8_t receiver5Address[] = {0xB0, 0xA7, 0x32, 0x15, 0x44, 0xF4};

// Game State
static struct {
    bool onePlayer;
    bool twoPlayer;
    bool easy;
    bool med;
    bool hard;
    bool options;
    bool menuMusic;
    bool restart;
    bool roundTwo;
    bool messageRE;
    bool cUpdated;
    
    int score1;
    int s;
    int i;
    int n;
    int m;
    int b3;
    
    int sequence[9];
    uint8_t lastButtonState1;
    uint8_t lastButtonState2;
    uint8_t lastButtonState3;
} gameState;

SSD1306_t dev;

const uint8_t digitPatterns[10][7] = {
    {1,1,1,1,1,1,0}, // 0
    {0,1,1,0,0,0,0}, // 1
    {1,1,0,1,1,0,1}, // 2
    {1,1,1,1,0,0,1}, // 3
    {0,1,1,0,0,1,1}, // 4
    {1,0,1,1,0,1,1}, // 5
    {1,0,1,1,1,1,1}, // 6
    {1,1,1,0,0,0,0}, // 7
    {1,1,1,1,1,1,1}, // 8
    {1,1,1,1,0,1,1}  // 9
};

void displayDigit(uint8_t digit) {
    if(digit > 9) return;
    gpio_set_level(SEG_A_GPIO, digitPatterns[digit][0]);
    gpio_set_level(SEG_B_GPIO, digitPatterns[digit][1]);
    gpio_set_level(SEG_C_GPIO, digitPatterns[digit][2]);
    gpio_set_level(SEG_D_GPIO, digitPatterns[digit][3]);
    gpio_set_level(SEG_E_GPIO, digitPatterns[digit][4]);
    gpio_set_level(SEG_F_GPIO, digitPatterns[digit][5]);
    gpio_set_level(SEG_G_GPIO, digitPatterns[digit][6]);
}

void initialize_gpio() {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << BUTTON1_GPIO) | (1ULL << BUTTON2_GPIO) | (1ULL << BUTTON3_GPIO),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    gpio_config_t output_conf = {
        .pin_bit_mask = (1ULL << BUZZER_GPIO) | (1ULL << SEG_A_GPIO) | (1ULL << SEG_B_GPIO) |
                        (1ULL << SEG_C_GPIO) | (1ULL << SEG_D_GPIO) | (1ULL << SEG_E_GPIO) |
                        (1ULL << SEG_F_GPIO) | (1ULL << SEG_G_GPIO),
        .mode = GPIO_MODE_OUTPUT
    };
    gpio_config(&output_conf);
}

void initialize_display() {
    i2c_master_init(&dev, GPIO_NUM_21, GPIO_NUM_22, -1);
    ssd1306_init(&dev, 128, 64);
    ssd1306_clear_screen(&dev, false);
}

void esp_now_send_callback(const uint8_t *mac_addr, esp_now_send_status_t status) {
    // Handle send completion if needed
}

void esp_now_receive_callback(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    if(data_len > 0) {
        char message[32];
        snprintf(message, sizeof(message), "%.*s", data_len, data);
        
        if(strstr(message, "newCycle")) {
            gameState.cUpdated = false;
        }
        else if(!gameState.cUpdated && gameState.i < 9) {
            if(strstr(message, "sensor:Triggered")) {
                gpio_set_level(BUZZER_GPIO, 1);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(BUZZER_GPIO, 0);
                
                gameState.s++;
                gameState.messageRE = true;
                gameState.i++;
                gameState.cUpdated = true;
            }
            else if(strstr(message, "timeFailed")) {
                gameState.i++;
                gameState.messageRE = true;
                gameState.cUpdated = true;
            }
        }
    }
}

void initialize_esp_now() {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_send_cb(esp_now_send_callback));
    ESP_ERROR_CHECK(esp_now_register_recv_cb(esp_now_receive_callback));

    // Add peers
    esp_now_peer_info_t peerInfo = {};
    peerInfo.channel = 0;
    peerInfo.encrypt = false;
    
    memcpy(peerInfo.peer_addr, receiver2Address, 6);
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
    
    memcpy(peerInfo.peer_addr, receiver3Address, 6);
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
    
    memcpy(peerInfo.peer_addr, receiver4Address, 6);
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
    
    memcpy(peerInfo.peer_addr, receiver5Address, 6);
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}

void update_display(const char* line1, const char* line2) {
    ssd1306_clear_screen(&dev, false);
    ssd1306_display_text(&dev, 0, line1, strlen(line1), false);
    if(line2 != NULL) {
        ssd1306_display_text(&dev, 1, line2, strlen(line2), false);
    }
}

void game_loop() {
    while(1) {
        // Handle button inputs
        int button1state = gpio_get_level(BUTTON1_GPIO);
        if(button1state == 0 && gameState.lastButtonState1 == 1) {
            gameState.n++;
            gpio_set_level(BUZZER_GPIO, 1);
            vTaskDelay(pdMS_TO_TICKS(200));
            gpio_set_level(BUZZER_GPIO, 0);
        }
        gameState.lastButtonState1 = button1state;

        // Handle game state transitions
        if(gameState.onePlayer && (gameState.easy || gameState.med || gameState.hard)) {
            if(gameState.options) {
                update_display("Game starting...", NULL);
                vTaskDelay(pdMS_TO_TICKS(1050));
                
                // Countdown animation
                const char* countdown[] = {"3", "2", "1"};
                for(int i=0; i<3; i++) {
                    ssd1306_clear_screen(&dev, false);
                    ssd1306_display_text(&dev, 3, countdown[i], 1, false);
                    vTaskDelay(pdMS_TO_TICKS(500));
                }
                gameState.options = false;
            }
            
            // Update 7-segment display
            displayDigit(gameState.s);
            
            // Handle ESP-NOW communication
            if(gameState.messageRE && gameState.i < 9) {
                const char *msg = "nodeoff";
                esp_now_send(receiver2Address, (uint8_t *)msg, strlen(msg));
                esp_now_send(receiver3Address, (uint8_t *)msg, strlen(msg));
                esp_now_send(receiver4Address, (uint8_t *)msg, strlen(msg));
                gameState.messageRE = false;
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void app_main() {
    // Initialize components
    initialize_gpio();
    initialize_display();
    initialize_esp_now();

    // Initialize game state
    memset(&gameState, 0, sizeof(gameState));
    gameState.lastButtonState1 = 1;
    gameState.lastButtonState2 = 1;
    gameState.lastButtonState3 = 1;
    gameState.options = true;
    gameState.menuMusic = true;

    // Create main game task
    xTaskCreate(game_loop, "game_loop", 4096, NULL, 5, NULL);
}