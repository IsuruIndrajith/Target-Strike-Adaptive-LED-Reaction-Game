#include "espnow_comm.h"
#include <stdio.h>

// Stub for ESP-NOW initialization
void espnow_comm_init(void)
{
    // In a complete implementation, initialize Wi-Fi, esp_now, and set up callbacks.
    printf("ESP-NOW communication initialized (stub).\n");
}

// Stub for sending a message via ESP-NOW
void espnow_send_message(const char *message)
{
    // In a complete implementation, call esp_now_send with the target MAC address.
    printf("ESP-NOW: Sent message '%s' (stub).\n", message);
}
