section .bss

gdtr
     dw 0 ; for the limit storage
     dq 0 ; for the pointer


section .text

global set_gdt
global reload_segments

set_gdt:
    mov [gdtr], di ; move the limit to the first 2 bytes
    mov [gdtr + 2], rsi ; move the pointer to the other 8 bytes
    lgdt [gdtr]
    ret

reload_segments:
    lea rax, [.reload_cs]
    
    push 0x8 ; push cs segment selector (first in gdt)
    push rax ; push address of `.reload_cs`

    retfq

.reload_cs:
    mov ax, 0x10 ; push ds segment selector (second in gdt) 
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax

    ret
