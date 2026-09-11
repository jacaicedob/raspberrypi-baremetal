add_library(rpi_bm 
    STATIC
        gpio.c
        # uart.c    # add once built
        # timer.c   # add once built
)

target_include_directories(rpi_bm 
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)