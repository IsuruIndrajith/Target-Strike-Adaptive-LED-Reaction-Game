idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES firebase oled sev_seg espnow_comm game_logic
)
