#ifndef PHYSICAL_MEMORY_MANAGEMENT_H
#define PHYSICAL_MEMORY_MANAGEMENT_H

#include "benami.h"

static volatile u64 MAXPHYSADDR;

/* Paging */
#define PAGE_SIZE               0x1000ULL
#define TABLE_SIZE              512
#define SELF_REF_ADDRESS        511
#define PAGE_PRESENT            0b1

/* Allocation */
#define BUDDY_COUNT             4
#define BASE_REVISION           4096 // 4K base allocation
#define BITMAP_SIZE             64

struct paging_entry {
    // flags
    u8 present: 1;
    u8 readable: 1;
    u8 supervisor: 1;
    u8 write_through: 1;
    u8 cache_disable: 1;
    u8 accessed: 1;
    u8 dirty: 1;
    u8 page_size: 1;
    u8 global: 1;
    u8 _ignored1: 2;
    u8 _reserved1: 1;

    u64 physical_address: 52;

    u16 _ignored2: 10;
    u8 execute_disable: 1;
};


// 0 -> 4K
// 1 -> 8K
// 2 -> 16K
// 3 -> 32K
struct buddy {
    u64 bitmap;
    struct buddy *next;
}

struct buddy_allocator {
    struct buddy buddies[BUDDY_COUNT];
};

void initialize_paging();

void *alloc_page(usize size);
void free_page(void *);

#endif