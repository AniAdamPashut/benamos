global outb
global inb

outb:
    mov dx, di
    mov rax, rsi
    and rax, 0xFF

    out dx, al
    ret


inb:
    mov ax, di
    in al, dx
    movzx rax, al
    ret