

    .section .text.startup, "ax", @progbits
    .set noreorder
    .align 2
    .global _main
    .global _start
    .type _start, @function

_start:
    la $gp, _gp
    j     _main
    li    $a0, 1

