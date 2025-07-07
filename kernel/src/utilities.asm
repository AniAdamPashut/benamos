global hcf

global read_cr0
global read_cr2
global read_cr3
global read_cr4

global write_cr0
global write_cr2
global write_cr3
global write_cr4

global clear_interrupts
global set_interrupts

hcf:
    hlt
    jmp hcf


read_cr0:
    mov rax, cr0
    ret

read_cr2:
    mov rax, cr2
    ret

read_cr3:
    mov rax, cr3
    ret

read_cr4:
    mov rax, cr4
    ret


write_cr0:
    mov rax, rdi
    mov cr0, rax
    ret
    
write_cr2:
    mov rax, rdi
    mov cr2, rax
    ret

write_cr3:
    mov rax, rdi
    mov cr3, rax
    ret

write_cr4:
    mov rax, rdi
    mov cr4, rax
    ret

clear_interrupts:
    cli
    ret

set_interrupts:
    sti
    ret