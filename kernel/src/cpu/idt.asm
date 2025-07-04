section .bss

idtr dw 0
     dq 0

section .text

global load_idt

load_idt:
    mov [idtr], di ; move the limit to the first 2 bytes
    mov [idtr + 2], rsi ; move the pointer to the other 8 bytes
    lidt [idtr]
    ret