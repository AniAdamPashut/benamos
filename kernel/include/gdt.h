#ifndef GLOBAL_DESCRIPTOR_TABLE_H
#define GLOBAL_DESCRIPTOR_TABLE_H

#include "benami.h"

#define GDT_SIZE 12

typedef struct gdt_t {
  u64 entries[GDT_SIZE];
  usize length;
} gdt;

typedef struct __attribute__((packed)) segment_descriptor_t { 
  u16 limit;
  u16 base_low;
  u8 base_middle;
  u8 access;
  u8 options;
  u8 base_high;
} segment_descriptor;

segment_descriptor create_segment(u32 base, u32 limit, u8 access, u8 flags) {
  u8 options = ((limit >> 16) & 0x0F) | (flags & 0xF0);
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

#endif