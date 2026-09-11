add_executable(blink.elf
    blink.c
    $<TARGET_OBJECTS:rpi_bm_boot>
)

target_link_libraries(blink.elf 
    PRIVATE 
        rpi_bm
)

target_link_options(blink.elf 
    PRIVATE
        -T${RPI_BM_LINKER_SCRIPT}
        -nostdlib
)
set_target_properties(blink.elf 
    PROPERTIES 
        LINK_DEPENDS ${RPI_BM_LINKER_SCRIPT}
)

add_custom_command(TARGET blink.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary blink.elf blink.img
    COMMENT "Converting ELF to raw binary image"
)