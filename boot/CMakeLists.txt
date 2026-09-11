add_library(rpi_bm_boot 
    OBJECT 
        startup.s
)

# Exposed so executable targets can reference the linker script explicitly.
set(RPI_BM_LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld CACHE FILEPATH
    "Linker script for rpi_bm bare-metal executables")