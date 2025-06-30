#include "gdt.h"

static volatile gdt global_descriptor_table = {0}; 

segment_descriptor create_kernel_code_segment() {
    return create_segment(0xC0000000, 0xFFFFF, 0x9A, 0xA);
}

segment_descriptor create_kernel_data_segment() {
    return create_segment(0xC0000000, 0xFFFFF, 0x92, 0xC);
}

segment_descriptor create_user_code_segment() {
    return create_segment(0, 0xFFFFF, 0xFA, 0xA);
}

segment_descriptor create_user_data_segment() {
    return create_segment(0, 0xFFFFF, 0xF2, 0xC);
}

void set_gdt(uint16_t limit, void *table) {
    struct __attribute__((packed)) {
        uint16_t limit;
        uint64_t base; // using 64-bit to be safe on x86_64
    } gdtr;

    gdtr.limit = limit;
    gdtr.base = (uint64_t)table;

    __asm__ volatile (
        "lgdt %0"
        :
        : "m"(gdtr)
        : "memory"
    );
}

__attribute__((naked)) void reload_segments(void) {
    __asm__ __volatile__ (
        "movw $0x10, %%ax\n\t" // Data segment selector
        "movw %%ax, %%ds\n\t"
        "movw %%ax, %%es\n\t"
        "movw %%ax, %%fs\n\t"
        "movw %%ax, %%gs\n\t"
        "movw %%ax, %%ss\n\t"
        "leaq 1f(%%rip), %%rax\n\t"
        "pushq $0x08\n\t"      // Code segment selector
        "pushq %%rax\n\t"
        "retfq\n\t"            // Far return to reload CS
        "1:\n\t"
        "ret\n\t"
        :
        :
        : "rax"
    );
}

void initialize_gdt() {
    global_descriptor_table.length = 1;
    segment_descriptor code_descriptor = create_kernel_code_segment();
    segment_descriptor data_descriptor = create_kernel_data_segment();
    segment_descriptor user_code_descriptor = create_user_code_segment();
    segment_descriptor user_data_descriptor = create_user_data_segment();

    __asm__ ("cli");
    
    global_descriptor_table.entries[global_descriptor_table.length] = *(u64 *)&code_descriptor;
    global_descriptor_table.length++;

    usize code_segement_selector = global_descriptor_table.length;

    global_descriptor_table.entries[global_descriptor_table.length] = *(u64 *)&data_descriptor;
    global_descriptor_table.length++;

    usize data_segement_selector = global_descriptor_table.length;
    
    global_descriptor_table.entries[global_descriptor_table.length] = *(u64 *)&user_code_descriptor;
    global_descriptor_table.length++;

    usize user_code_segement_selector = global_descriptor_table.length;

    global_descriptor_table.entries[global_descriptor_table.length] = *(u64 *)&user_data_descriptor;
    global_descriptor_table.length++;

    usize user_data_segement_selector = global_descriptor_table.length;

    set_gdt((global_descriptor_table.length * sizeof(u64)) - 1, global_descriptor_table.entries);
    
    reload_segments();

    __asm__ ("sti");
}

void init() {
    initialize_gdt();
}