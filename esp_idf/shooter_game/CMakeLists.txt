# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(shooter_game)

idf_component_register(
    SRCS "main.c" "display.c" "buttons.c" "buzzer.c" "seven_segment.c" "game_logic.c" "communication.c"
    INCLUDE_DIRS "include"
)
