global get_max_phys_addr_bits

get_max_phys_addr_bits:
    push rbx

    mov rax, 0x80000000
    cpuid

    cmp rax, 0x80000008
    jle .default


    mov rax, 0x80000008
    cpuid

    and rax, 0xFF

    pop rbx
    ret
    
.default:
    mov rax, 36
    ret 
