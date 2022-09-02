cmake_minimum_required(VERSION 3.22)
set(BOARD esp32)

find_package(Zephyr)
project(hello_world_blinky)

target_sources(app PRIVATE src/main.c)
