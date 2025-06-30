#ifndef IDT_H
#define IDT_H

#include "benami.h"

#define MISSING_ERROR_CODE_MAGIC 0xb36b9b8e1a7d7d63

typedef struct  __attribute__((packed)) interrupt_descriptor {
    u16 offset_low;
    u16 segment_selector;
    u16 options;
    u16 offset_middle;
    u32 offset_high;
    u32 _reserved;
} interrupt_descriptor_t;

typedef struct idt {
    interrupt_descriptor_t entries[256];
} idt_t;

typedef struct registers {
    uint64_t r15;
	uint64_t r14;
	uint64_t r13;
	uint64_t r12;
	uint64_t r11;
	uint64_t r10;
	uint64_t r9;
	uint64_t r8;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rbp;
	uint64_t rdx;
	uint64_t rcx;
	uint64_t rbx;
	uint64_t rax;
    uint64_t int_no;
	uint64_t error;
	uint64_t rip;
	uint64_t cs;
	uint64_t rflags;
	uint64_t rsp;
	uint64_t ss;
} __attribute__((packed)) registers_t;

void initialize_idt();

#endif