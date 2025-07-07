#ifndef MEMORY_MANAGEMENT_H
#define MEMORY_MANAGEMENT_H

#include "benami.h"

static volatile u64 MAXPHYSADDR;

void initialize_paging();

typedef u64 page_mapping_level4[512];
typedef u64 page_directory_pointer_table[512];
typedef u64 page_directory[512];
typedef u64 page_table[512];

#endif