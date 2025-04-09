#pragma once
#include <esp_now.h>
#include <esp_wifi.h>
#include <esp_log.h>

#define BROADCAST_ADDR {0xAC,0x15,0x18,0xEC,0x56,0xC4}

typedef void (*espnow_recv_cb_t)(const uint8_t *data, size_t len);
void espnow_init(espnow_recv_cb_t on_recv);
esp_err_t espnow_send(const char *msg);
