add_library(rpi_bm 
    STATIC
        gpio.c
        timer.c
        mini_uart.c  
)

target_include_directories(rpi_bm 
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)