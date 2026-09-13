add_executable(mini-uart.elf
    mini-uart.c
    $<TARGET_OBJECTS:rpi_bm_boot>
)

target_link_libraries(mini-uart.elf 
    PRIVATE 
        rpi_bm
)

target_link_options(mini-uart.elf 
    PRIVATE
        -T${RPI_BM_LINKER_SCRIPT}
        -nostdlib
)
set_target_properties(mini-uart.elf 
    PROPERTIES 
        LINK_DEPENDS ${RPI_BM_LINKER_SCRIPT}
)

add_custom_command(TARGET mini-uart.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary mini-uart.elf mini-uart.img
    COMMENT "Converting ELF to raw binary image"
)