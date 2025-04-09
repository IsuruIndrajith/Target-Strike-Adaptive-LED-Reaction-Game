idf_component_register(
    SRCS "espnow_comm.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_wifi esp_netif
)
