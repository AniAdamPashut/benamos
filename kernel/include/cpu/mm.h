#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "benami.h"

static volatile u64 MAXPHYSADDR;

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

void initialize_paging();

#define PAGE_SIZE               0x1000ULL
#define TABLE_SIZE              512
#define SELF_REF_ADDRESS        511

typedef struct paging_entry page_mapping_level4[TABLE_SIZE];
typedef struct paging_entry page_directory_pointer_table[TABLE_SIZE];
typedef struct paging_entry page_directory[TABLE_SIZE];
typedef struct paging_entry page_table[TABLE_SIZE];

#endif