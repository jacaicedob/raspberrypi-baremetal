add_executable(blink-led.elf
    blink-led.c
    $<TARGET_OBJECTS:rpi_bm_boot>
)

target_link_libraries(blink-led.elf 
    PRIVATE 
        rpi_bm
)

target_link_options(blink-led.elf 
    PRIVATE
        -T${RPI_BM_LINKER_SCRIPT}
        -nostdlib
)
set_target_properties(blink-led.elf 
    PROPERTIES 
        LINK_DEPENDS ${RPI_BM_LINKER_SCRIPT}
)

add_custom_command(TARGET blink-led.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary blink-led.elf blink-led.img
    COMMENT "Converting ELF to raw binary image"
)