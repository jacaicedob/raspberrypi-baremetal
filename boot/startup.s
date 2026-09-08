.section .text.boot

.global _start

_start:
    // Check which core we are on (bits 0-1 of MPIDR give core ID)
    // _start is run by all cores in parallel
    mrc p15, 0, r0, c0, c0, 5 // Read core ID
    and r0, r0, #3   // Mask the lower 2 bits
    cmp r0, #0   // Is core ID == 0?
    bne park_core  // branch if not core 0

    // Core 0: set up the stack pointer just below our loaded address
    ldr sp, =0x8000

    // Zero out the .bss section
    ldr r0, =__bss_start // Load start address
    ldr r1, =__bss_end   // Load end address
    mov r2, #0            // Load 0 to r2
bss_zero_loop:
    cmp r0, r1            // Have we reached bss_end?
    bge bss_zero_done     // If r0 >= r1, we are done zeroing 
    str r2, [r0]          // Store 0 (from r2) into the memory address pointed to by r0
    add r0, r0, #4        // Increment r0 address by 4 bytes (next 32-bit word)
    b bss_zero_loop       // Go to top of loop
bss_zero_done:

    // Jump into C code
    bl main

    // main() should never return on bare-metal, but if it does:
hang:
    b hang

park_core:
    // Cores 1-3: park indefinitely in low-power wait
    wfe
    b park_core

