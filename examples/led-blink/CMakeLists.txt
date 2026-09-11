add_executable(blink.elf ../boot/startup.s blink.c)

add_custom_command(TARGET blink.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary blink.elf blink.img
    COMMENT "Converting ELF to raw binary image"
)
