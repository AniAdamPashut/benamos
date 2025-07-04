#include "cpu/mm.h"

extern u64 get_max_phys_addr_bits();

void initialize_paging() {
    MAXPHYSADDR = get_max_phys_addr_bits();

    
    int x = 1;
}