#include "cpu/gdt.h"
#include "utilities.h"

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

extern void set_gdt(u16 limit, u64 table);

extern void reload_segments(void);

void initialize_gdt() {
    global_descriptor_table.length = 1;
    segment_descriptor code_descriptor = create_kernel_code_segment();
    segment_descriptor data_descriptor = create_kernel_data_segment();
    segment_descriptor user_code_descriptor = create_user_code_segment();
    segment_descriptor user_data_descriptor = create_user_data_segment();

    clear_interrupts();
    
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

    set_interrupts();
}