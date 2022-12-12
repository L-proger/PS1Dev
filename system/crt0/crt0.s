

    .section .text.startup, "ax", @progbits
    .set noreorder
    .align 2
    .global main
    .global _start
    .type _start, @function

_start:
    j     main
    li    $a0, 1

