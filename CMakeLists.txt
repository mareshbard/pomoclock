cmake_minimum_required(VERSION 3.13)
set(PICO_BOARD pico_w)
# Força o caminho correto do SDK (estamos usando o 1.5.1 conforme seu comando)
set(PICO_SDK_PATH "C:/Users/SAMSUNG/.pico-sdk/sdk/1.5.1")

include(pico_sdk_import.cmake)

project(pomoclock C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(pomoclock pomoclock.c)

target_include_directories(pomoclock PRIVATE ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(pomoclock
    pico_stdlib
    pico_cyw43_arch_lwip_threadsafe_background
    hardware_pwm
    hardware_adc
)
# Habilita o Monitor Serial via USB
pico_enable_stdio_usb(pomoclock 1)
pico_enable_stdio_uart(pomoclock 0)

pico_add_extra_outputs(pomoclock)