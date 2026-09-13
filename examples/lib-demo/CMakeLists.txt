add_executable(lib-demo.elf
    lib-demo.c
    $<TARGET_OBJECTS:rpi_bm_boot>
)

target_link_libraries(lib-demo.elf 
    PRIVATE 
        rpi_bm
)

target_link_options(lib-demo.elf 
    PRIVATE
        -T${RPI_BM_LINKER_SCRIPT}
        -nostdlib
)
set_target_properties(lib-demo.elf 
    PROPERTIES 
        LINK_DEPENDS ${RPI_BM_LINKER_SCRIPT}
)

add_custom_command(TARGET lib-demo.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary lib-demo.elf lib-demo.img
    COMMENT "Converting ELF to raw binary image"
)