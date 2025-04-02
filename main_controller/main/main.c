#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_now.h"
#include "esp_wifi.h"
#include "nvs_flash.h"
#include "esp_log.h"

// Logging tag
static const char *TAG = "MAIN_CTRL";

// --- Pin Definitions ---
// Buttons & buzzer
#define BUTTON1_GPIO    12
#define BUTTON2_GPIO    13
#define BUTTON3_GPIO    5
#define BUZZER_GPIO     15

// Seven-segment display pins
#define SEG_A_GPIO      19
#define SEG_B_GPIO      18
#define SEG_C_GPIO      26
#define SEG_D_GPIO      25
#define SEG_E_GPIO      33
#define SEG_F_GPIO      23
#define SEG_G_GPIO      27

// --- Global Variables for Game State ---
volatile int lastButtonState1 = 1;
volatile int lastButtonState2 = 1;
volatile int lastButtonState3 = 1;
volatile bool onePlayer = false;
volatile bool twoPlayer = false;
volatile bool easy = false;
volatile bool med = false;
volatile bool hard = false;
volatile bool options = true;
volatile bool menuMusic = true;
volatile bool restart = false;
volatile int score1 = 0;
volatile int s = 0;
volatile int i_counter = 0;
volatile int n = 0;
volatile int m = 0;
volatile int b3 = 0;

// Communication flags
volatile bool roundTwo = false;
volatile bool messageRE = false;
volatile bool cUpdated = false;

// --- Random Sequence Data ---
#define SEQ_SIZE 9
int sequence[SEQ_SIZE] = {0};

// --- Function: Generate Random Sequence ---
void randomNumbers(int array[], int size, int min, int max)
{
    for (int r = 0; r < size; r++) {
        array[r] = (rand() % (max - min + 1)) + min;
        while (r > 0 && array[r] == array[r - 1]) {
            array[r] = (rand() % (max - min + 1)) + min;
        }
    }
}

// --- Peer MAC Addresses ---
// (Change these addresses as needed.)
uint8_t receiver2Address[6] = {0xAC, 0x15, 0x18, 0xEC, 0x56, 0xC4};
uint8_t receiver3Address[6] = {0xE0, 0x5A, 0x1B, 0xCB, 0x37, 0x60};
uint8_t receiver4Address[6] = {0xB0, 0xA7, 0x32, 0x16, 0x2E, 0x6C};
uint8_t receiver5Address[6] = {0xB0, 0xA7, 0x32, 0x15, 0x44, 0xF4};

// --- ESP-NOW Receive Callback ---
void receiveCallback(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
    char message[50];
    if (data_len < sizeof(message))
        strncpy(message, (const char *)data, data_len);
    message[data_len] = '\0';
    
    ESP_LOGI(TAG, "Received: %s", message);
    
    if (strcmp(message, "newCycle") == 0) {
        cUpdated = false;
    }
    
    if (!cUpdated && i_counter < SEQ_SIZE) {
        if (strcmp(message, "sensor:Triggered") == 0) {
            gpio_set_level(BUZZER_GPIO, 1);
            vTaskDelay(pdMS_TO_TICKS(200));
            gpio_set_level(BUZZER_GPIO, 0);
            s++;
            messageRE = true;
            i_counter++;
            cUpdated = true;
        } else if (strcmp(message, "timeFailed") == 0) {
            i_counter++;
            messageRE = true;
            cUpdated = true;
        }
    }
}

// --- Seven-Segment Display Function ---
// This function sets the segments for a given number (0-9).
void display_number(int num) {
    // Turn off all segments first
    gpio_set_level(SEG_A_GPIO, 0);
    gpio_set_level(SEG_B_GPIO, 0);
    gpio_set_level(SEG_C_GPIO, 0);
    gpio_set_level(SEG_D_GPIO, 0);
    gpio_set_level(SEG_E_GPIO, 0);
    gpio_set_level(SEG_F_GPIO, 0);
    gpio_set_level(SEG_G_GPIO, 0);
    
    switch(num) {
        case 0:
            gpio_set_level(SEG_A_GPIO, 1);
            gpio_set_level(SEG_B_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            gpio_set_level(SEG_D_GPIO, 1);
            gpio_set_level(SEG_E_GPIO, 1);
            gpio_set_level(SEG_F_GPIO, 1);
            break;
        case 1:
            gpio_set_level(SEG_B_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            break;
        case 2:
            gpio_set_level(SEG_A_GPIO, 1);
            gpio_set_level(SEG_B_GPIO, 1);
            gpio_set_level(SEG_D_GPIO, 1);
            gpio_set_level(SEG_E_GPIO, 1);
            gpio_set_level(SEG_G_GPIO, 1);
            break;
        case 3:
            gpio_set_level(SEG_A_GPIO, 1);
            gpio_set_level(SEG_B_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            gpio_set_level(SEG_D_GPIO, 1);
            gpio_set_level(SEG_G_GPIO, 1);
            break;
        case 4:
            gpio_set_level(SEG_B_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            gpio_set_level(SEG_F_GPIO, 1);
            gpio_set_level(SEG_G_GPIO, 1);
            break;
        case 5:
            gpio_set_level(SEG_A_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            gpio_set_level(SEG_D_GPIO, 1);
            gpio_set_level(SEG_F_GPIO, 1);
            gpio_set_level(SEG_G_GPIO, 1);
            break;
        case 6:
            gpio_set_level(SEG_A_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            gpio_set_level(SEG_D_GPIO, 1);
            gpio_set_level(SEG_E_GPIO, 1);
            gpio_set_level(SEG_F_GPIO, 1);
            gpio_set_level(SEG_G_GPIO, 1);
            break;
        case 7:
            gpio_set_level(SEG_A_GPIO, 1);
            gpio_set_level(SEG_B_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            break;
        case 8:
            gpio_set_level(SEG_A_GPIO, 1);
            gpio_set_level(SEG_B_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            gpio_set_level(SEG_D_GPIO, 1);
            gpio_set_level(SEG_E_GPIO, 1);
            gpio_set_level(SEG_F_GPIO, 1);
            gpio_set_level(SEG_G_GPIO, 1);
            break;
        case 9:
            gpio_set_level(SEG_A_GPIO, 1);
            gpio_set_level(SEG_B_GPIO, 1);
            gpio_set_level(SEG_C_GPIO, 1);
            gpio_set_level(SEG_F_GPIO, 1);
            gpio_set_level(SEG_G_GPIO, 1);
            break;
        default:
            break;
    }
}

// --- Display Update (Placeholder) ---
// In a real project, you might use an SSD1306 driver.
// Here we log the messages.
void updateDisplay(const char *line1, const char *line2) {
    ESP_LOGI(TAG, "Display: %s | %s", line1, line2);
}

// --- GPIO Initialization ---
void init_gpio(void) {
    gpio_config_t io_conf;
    // Configure buttons as inputs with pull-up resistors
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << BUTTON1_GPIO) | (1ULL << BUTTON2_GPIO) | (1ULL << BUTTON3_GPIO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    
    // Configure buzzer and seven-segment pins as outputs
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << BUZZER_GPIO) | (1ULL << SEG_A_GPIO) | (1ULL << SEG_B_GPIO) |
                           (1ULL << SEG_C_GPIO) | (1ULL << SEG_D_GPIO) | (1ULL << SEG_E_GPIO) |
                           (1ULL << SEG_F_GPIO) | (1ULL << SEG_G_GPIO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);
}

// --- Wi-Fi and ESP-NOW Initialization ---
void init_wifi_espnow(void) {
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    
    ESP_ERROR_CHECK(esp_now_init());
    ESP_ERROR_CHECK(esp_now_register_recv_cb(receiveCallback));
    
    // Add peers (the MAC addresses must match your network setup)
    esp_now_peer_info_t peerInfo;
    memset(&peerInfo, 0, sizeof(peerInfo));
    
    memcpy(peerInfo.peer_addr, receiver2Address, 6);
    peerInfo.channel = 0;
    peerInfo.ifidx = ESP_IF_WIFI_STA;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
    
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, receiver3Address, 6);
    peerInfo.channel = 0;
    peerInfo.ifidx = ESP_IF_WIFI_STA;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
    
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, receiver4Address, 6);
    peerInfo.channel = 0;
    peerInfo.ifidx = ESP_IF_WIFI_STA;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
    
    memset(&peerInfo, 0, sizeof(peerInfo));
    memcpy(peerInfo.peer_addr, receiver5Address, 6);
    peerInfo.channel = 0;
    peerInfo.ifidx = ESP_IF_WIFI_STA;
    peerInfo.encrypt = false;
    ESP_ERROR_CHECK(esp_now_add_peer(&peerInfo));
}

// --- Main Controller Task ---
void main_controller_task(void *pvParameters) {
    // Generate the random sequence
    randomNumbers(sequence, SEQ_SIZE, 1, 3);
    
    // Initial display prompt
    updateDisplay("Press to start", "");
    
    while (1) {
        // Read button states (active low)
        int button1state = gpio_get_level(BUTTON1_GPIO);
        if (button1state == 0 && lastButtonState1 == 1) {
            n++;
            gpio_set_level(BUZZER_GPIO, 1);
            vTaskDelay(pdMS_TO_TICKS(200));
            gpio_set_level(BUZZER_GPIO, 0);
        }
        lastButtonState1 = button1state;
        
        int button2state = gpio_get_level(BUTTON2_GPIO);
        if (button2state == 0 && lastButtonState2 == 1 && n > 1) {
            m++;
            gpio_set_level(BUZZER_GPIO, 1);
            vTaskDelay(pdMS_TO_TICKS(200));
            gpio_set_level(BUZZER_GPIO, 0);
        }
        lastButtonState2 = button2state;
        
        // Menu logic simulation
        if (n == 1) {
            updateDisplay("One player", "Two Players?");
        } else if (n == 2) {
            updateDisplay("One player", "");
        } else if (n == 3) {
            updateDisplay("Two players", "");
        }
        
        if (n > 3 && m == 0) {
            n = 2;
        }
        
        if (n == 2 && m == 1) {
            onePlayer = true;
            updateDisplay("Easy Med or Hard", "");
            n = 4;
        }
        
        if (n == 3 && m == 1) {
            onePlayer = true;
            twoPlayer = true;
            updateDisplay("Easy Med or Hard", "");
        }
        
        if (m > 1 && n < 1) {
            m = 1;
        }
        
        if (n == 4) {
            updateDisplay("Easy", "");
        } else if (n == 5) {
            updateDisplay("Med", "");
        } else if (n == 6) {
            updateDisplay("Hard", "");
        }
        
        if (n > 6 && m == 1) {
            n = 4;
        }
        
        if (n == 4 && m == 2) {
            easy = true;
        } else if (n == 5 && m == 2) {
            med = true;
        } else if (n == 6 && m == 2) {
            hard = true;
        }
        
        if (onePlayer && (easy || med || hard)) {
            if (options) {
                updateDisplay("Game starting...", "");
                vTaskDelay(pdMS_TO_TICKS(1050));
                
                // Countdown simulation
                updateDisplay("3", "");
                vTaskDelay(pdMS_TO_TICKS(500));
                updateDisplay("2", "");
                vTaskDelay(pdMS_TO_TICKS(500));
                updateDisplay("1", "");
                vTaskDelay(pdMS_TO_TICKS(500));
                options = false;
            }
            
            updateDisplay("SHOOT!", "");
            ESP_LOGI(TAG, "Score: %d", s);
            display_number(s);
        }
        
        // Determine difficulty string
        char difficulty[10];
        if (easy) {
            strcpy(difficulty, "Easy");
        } else if (med) {
            strcpy(difficulty, "Med");
        } else if (hard) {
            strcpy(difficulty, "Hard");
        } else {
            strcpy(difficulty, "");
        }
        
        // Send messages based on the random sequence
        if (sequence[i_counter] == 1 && !cUpdated) {
            char message[20];
            snprintf(message, sizeof(message), "nodeon%s", difficulty);
            ESP_ERROR_CHECK(esp_now_send(receiver2Address, (uint8_t *)message, strlen(message)));
        } else if (sequence[i_counter] == 2 && !cUpdated) {
            char message[20];
            snprintf(message, sizeof(message), "nodeon%s", difficulty);
            ESP_ERROR_CHECK(esp_now_send(receiver3Address, (uint8_t *)message, strlen(message)));
        } else if (sequence[i_counter] == 3 && !cUpdated) {
            char message[20];
            snprintf(message, sizeof(message), "nodeon%s", difficulty);
            ESP_ERROR_CHECK(esp_now_send(receiver4Address, (uint8_t *)message, strlen(message)));
        }
        
        if (messageRE && i_counter < SEQ_SIZE) {
            char nodeoff[] = "nodeoff";
            ESP_ERROR_CHECK(esp_now_send(receiver2Address, (uint8_t *)nodeoff, strlen(nodeoff)));
            ESP_ERROR_CHECK(esp_now_send(receiver3Address, (uint8_t *)nodeoff, strlen(nodeoff)));
            ESP_ERROR_CHECK(esp_now_send(receiver4Address, (uint8_t *)nodeoff, strlen(nodeoff)));
            ESP_LOGI(TAG, "Node OFF message SENT");
            messageRE = false;
        }
        
        if (i_counter == SEQ_SIZE && !twoPlayer) {
            char menuMusicOff[] = "menuMusicOff";
            ESP_ERROR_CHECK(esp_now_send(receiver5Address, (uint8_t *)menuMusicOff, strlen(menuMusicOff)));
            updateDisplay("Game over", "Try again!");
        }
        
        if (i_counter == SEQ_SIZE && twoPlayer && !restart) {
            int button3state = gpio_get_level(BUTTON3_GPIO);
            if (button3state == 0 && lastButtonState3 == 1) {
                b3++;
                gpio_set_level(BUZZER_GPIO, 1);
                vTaskDelay(pdMS_TO_TICKS(200));
                gpio_set_level(BUZZER_GPIO, 0);
            }
            lastButtonState3 = button3state;
            
            score1 = s;
            updateDisplay("Player 2's turn!", "Press to start!");
            
            if (b3 == 1) {
                restart = true;
                i_counter = 0;
                s = 0;
                updateDisplay("SHOOT!!!", "");
            }
        }
        
        if (i_counter == SEQ_SIZE && restart) {
            char menuMusicOff[] = "menuMusicOff";
            ESP_ERROR_CHECK(esp_now_send(receiver5Address, (uint8_t *)menuMusicOff, strlen(menuMusicOff)));
            char scoreStr[10];
            if (score1 > s) {
                snprintf(scoreStr, sizeof(scoreStr), "Score: %d", score1);
                updateDisplay("Player 1 wins", scoreStr);
            } else if (score1 < s) {
                snprintf(scoreStr, sizeof(scoreStr), "Score: %d", s);
                updateDisplay("Player 2 wins", scoreStr);
            } else {
                updateDisplay("Its a tie!", "Play again");
            }
        }
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

// --- app_main Entry Point ---
void app_main(void)
{
    // Initialize GPIO, Wi-Fi and ESP-NOW
    init_gpio();
    init_wifi_espnow();
    
    // Initialize random seed
    srand((unsigned) esp_random());
    
    // Create the main controller task
    xTaskCreate(main_controller_task, "main_controller_task", 8192, NULL, 5, NULL);
}
