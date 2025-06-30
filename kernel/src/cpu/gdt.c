#include "cpu/gdt.h"

static volatile gdt global_descriptor_table = {0}; 

segment_descriptor create_segment(u32 base, u32 limit, u8 access, u8 flags) {
  u8 options = ((limit >> 16) & 0x0F) | ((flags << 4) & 0xF0);
  u16 limit_low = limit & 0xFFFF;
  u16 base_low = base & 0xFFFF;
  u8 base_middle = (base >> 16) & 0xFF;
  u8 base_high = (base >> 24) & 0xFF;
  
  return  (segment_descriptor){
    .limit = limit_low,
    .base_low = base_low,
    .base_middle = base_middle,
    .access = access,
    .options = options,
    .base_high = base_high
  };
}

segment_descriptor create_kernel_code_segment() {
    return create_segment(0, 0xFFFFF, 0b10011010, 0x2);
}

segment_descriptor create_kernel_data_segment() {
    return create_segment(0, 0xFFFFF, 0b10010010, 0xC);
}

segment_descriptor create_user_code_segment() {
    return create_segment(0, 0xFFFFF, 0b11111010, 0x2);
}

segment_descriptor create_user_data_segment() {
    return create_segment(0, 0xFFFFF, 0b11110010, 0x2);
}

void set_gdt(u16 limit, u64 table) {
    static struct __attribute__((packed)) {
        u16 limit;
        u64 base; // using 64-bit to be safe on x86_64
    } gdtr;

    gdtr.limit = limit;
    gdtr.base = table;

    __asm__ volatile (
        "lgdt %0"
        :
        : "m"(gdtr)
        : "memory"
    );
}

__attribute__((naked)) void reload_segments(void) {
    __asm__ __volatile__ (
        "leaq 1f(%%rip), %%rax\n\t"
        "pushq $0x08\n\t"      // Code segment selector
        "pushq %%rax\n\t"
        "retfq\n\t"            // Far return to reload CS
        "1:\n\t"
        
        "movw $0x10, %%ax\n\t" // Data segment selector
        "movw %%ax, %%ds\n\t"
        "movw %%ax, %%es\n\t"
        "movw %%ax, %%fs\n\t"
        "movw %%ax, %%gs\n\t"
        "movw %%ax, %%ss\n\t"
        
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
    
    global_descriptor_table.entries[global_descriptor_table.length] = code_descriptor;
    global_descriptor_table.length++;

    usize code_segement_selector = global_descriptor_table.length;

    global_descriptor_table.entries[global_descriptor_table.length] = data_descriptor;
    global_descriptor_table.length++;

    usize data_segement_selector = global_descriptor_table.length;
    
    global_descriptor_table.entries[global_descriptor_table.length] = user_code_descriptor;
    global_descriptor_table.length++;

    usize user_code_segement_selector = global_descriptor_table.length;

    global_descriptor_table.entries[global_descriptor_table.length] = user_data_descriptor;
    global_descriptor_table.length++;

    usize user_data_segement_selector = global_descriptor_table.length;

    set_gdt((global_descriptor_table.length * sizeof(u64)) - 1, (u64)global_descriptor_table.entries);
    
    reload_segments();

    __asm__ ("sti");
}