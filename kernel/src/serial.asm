global outb
outb:
    mov dx, di        ; port = low 16 bits of RDI
    mov al, sil       ; value = low 8 bits of RSI
    out dx, al
    ret

global inb
inb:
    mov dx, di        ; port = low 16 bits of RDI
    in al, dx
    movzx rax, al     ; return result in RAX (System V ABI)
    ret