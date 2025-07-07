global get_max_phys_addr_bits
global disable_paging
global enable_paging

get_max_phys_addr_bits:
    push rbx

    mov rax, 0x80000000 
    cpuid

    cmp rax, 0x80000008 ; check if the leaf 0x8 is available
    jle .default

    mov rax, 0x80000008
    cpuid

    and rax, 0xFF

    pop rbx
    ret
    
.default:
    mov rax, 36
    pop rbx
    ret 


disable_paging:  
    ; Disable the PG bit in CR0
    mov rax, cr0
    btr rax, 31
    mov cr0, rax

    ret

enable_paging:
    ; Disable the PG bit in CR0
    mov rax, cr0
    bts rax, 31
    mov cr0, rax

    ret