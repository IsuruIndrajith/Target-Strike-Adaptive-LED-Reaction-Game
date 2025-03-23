// main/include/communication.h
#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "esp_now.h"
#include "esp_wifi.h"

// Initialize ESP-NOW and WiFi
void communication_init(void);

// Send a message to a specific receiver
void send_message(const uint8_t* receiver_addr, const char* message);

// Callback setup
void register_receive_callback(void);

#endif /* COMMUNICATION_H */