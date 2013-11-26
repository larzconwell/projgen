global _start

section .data
sys_exit: equ 1

section .text
_start:
        xor ebx, ebx
        mov eax, sys_exit
        int 0x80
