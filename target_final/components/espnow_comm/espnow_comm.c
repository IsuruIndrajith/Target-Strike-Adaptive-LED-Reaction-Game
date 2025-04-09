#include "espnow_comm.h"
#include <string.h>

static const char *TAG = "espnow_comm";
static uint8_t peer_addr[6] = BROADCAST_ADDR;
static espnow_recv_cb_t user_recv_cb = NULL;

static void on_data_sent(const uint8_t *mac, esp_now_send_status_t status) {
    ESP_LOGI(TAG, "Send status: %s", status==ESP_NOW_SEND_SUCCESS?"OK":"FAIL");
}

static void on_data_recv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
    if (user_recv_cb) {
        user_recv_cb(data, len);
    }
}

void espnow_init(espnow_recv_cb_t on_recv) {
    user_recv_cb = on_recv;
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t wcfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&wcfg));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_ERROR_CHECK(esp_now_init());
    esp_now_register_send_cb(on_data_sent);
    esp_now_register_recv_cb(on_data_recv);
    esp_now_peer_info_t peer = {
        .channel = 0,
        .encrypt = false
    };
    memcpy(peer.peer_addr, peer_addr, 6);
    ESP_ERROR_CHECK(esp_now_add_peer(&peer));
    ESP_LOGI(TAG, "ESP-NOW initialized");
}

esp_err_t espnow_send(const char *msg) {
    return esp_now_send(peer_addr, (const uint8_t *)msg, strlen(msg));
}
