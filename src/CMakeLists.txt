add_library(rpi_bm 
    STATIC
        fmt.c
        gpio.c
        gpu_mbox.c
        mini_uart.c  
        timer.c
        display.c
)

target_include_directories(rpi_bm 
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)