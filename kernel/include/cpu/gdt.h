#ifndef GLOBAL_DESCRIPTOR_TABLE_H
#define GLOBAL_DESCRIPTOR_TABLE_H

#include "benami.h"

#define GDT_SIZE 12

typedef struct __attribute__((packed)) segment_descriptor_t { 
  u16 limit;
  u16 base_low;
  u8 base_middle;
  u8 access;
  u8 options;
  u8 base_high;
} segment_descriptor;

typedef struct gdt_t {
  segment_descriptor entries[GDT_SIZE];
  usize length;
} gdt;

void initialize_gdt();

#endif