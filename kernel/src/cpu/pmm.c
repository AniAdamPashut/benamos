#include <limine.h>
#include "cpu/pmm.h"
#include "printf.h"
#include "utilities.h"
#include "mandatory.h"

extern u64 get_max_phys_addr_bits();
extern void disable_paging();
extern void enable_paging();

#define KERNEL_VIRT_BASE  0xFFFFFFFF80000000ULL

static struct buddy_allocator allocator = {0};

void set_buddy_present_upwars_rec(usize order, u8 position, u64 runs) {
    if (order < 0) {
        return;
    }
    
    struct buddy *runner = &allocator.buddies[order];
    for (u64 i = 0; i < runs; i++)
        runner = runner->next;
    
    runner->bitmap = runner->bitmap | (1 << position);
    set_buddy_present_upwars_rec(order - 1, position * 2, runs * 2);
}

void set_buddy_present_downwards_rec(usize order, u8 position, u64 runs) {
    if (order < 0) {
        return;
    }
    
    struct buddy *runner = &allocator.buddies[order];
    for (u64 i = 0; i < runs; i++)
        runner = runner->next;
    
    runner->bitmap = runner->bitmap | (1 << position);
    set_buddy_present_downwards_rec(order + 1, position / 2, runs / 2);
}

u64 convert_position_to_addr(usize order, u8 position, u64 runs) {
    return (order * BASE_REVISION) * position * runs;
}

void *alloc_page(usize size) {
    usize minimal_buddy_order = (size - 1) / BASE_REVISION;

    if (minimal_buddy_order > BUDDY_COUNT) {
        printf("Large memory requested: 0x%x. cannot fullfil request", size);
        return NULL;
    }
    
    struct buddy *runner = &allocator.buddies[minimal_buddy_order];
    u64 runs = 0;

    while (runner != NULL) {
        for (u8 i = 0; i < BITMAP_SIZE; i++) {
            if (!((runner->bitmap >> i) & 0x1)) {
                set_buddy_present_upwars_rec(minimal_buddy_order, i, runs);
                set_buddy_present_downwards_rec(minimal_buddy_order, i, runs);
                return (void *)convert_position_to_addr(minimal_buddy_order, i, runs);
            }
        }

        runner = runner->next;
        runs++;
    }

    printf("Out of memory in kernel. NOT GOOD!!!");
    return NULL;
}

// void map_page(u64 *pml4, u64 vaddr, u64 paddr, u64 flags) {
//     usize pml4_idx = (vaddr >> 39) & 0x1FF;
//     usize pdpt_idx = (vaddr >> 30) & 0x1FF;
//     usize pd_idx   = (vaddr >> 21) & 0x1FF;
//     usize pt_idx   = (vaddr >> 12) & 0x1FF;

//     u64 *pdpt, *pd, *pt;

//     if (!(pml4[pml4_idx] & PAGE_PRESENT)) {
//         pdpt = alloc_page();
//         pml4[pml4_idx] = (u64)pdpt | flags;
//     } else {
//         pdpt = (u64 *)(pml4[pml4_idx] & ~0xFFFULL);
//     }

//     if (!(pdpt[pdpt_idx] & PAGE_PRESENT)) {
//         pd = alloc_page();
//         pdpt[pdpt_idx] = (u64)pd | flags;
//     } else {
//         pd = (u64 *)(pdpt[pdpt_idx] & ~0xFFFULL);
//     }

//     if (!(pd[pd_idx] & PAGE_PRESENT)) {
//         pt = alloc_page();
//         pd[pd_idx] = (u64)pt | flags;
//     } else {
//         pt = (u64 *)(pd[pd_idx] & ~0xFFFULL);
//     }

//     pt[pt_idx] = (paddr & ~0xFFFULL) | flags;
// }


// u64 *setup_tables(void) {
//     u64 *pml4 = alloc_page();
//     *(pml4 + 511) = pml4 | 0b11;
    
//     // Identity map: 0x0 - 0x1000000 (16 MiB)
//     for (u64 addr = 0; addr < 0x1000000; addr += PAGE_SIZE) {
//         map_page(pml4, addr, addr, PAGE_FLAGS);
//     }

//     // Higher-half map for kernel: map 16 MiB kernel area
//     for (u64 offset = 0; offset < 0x1000000; offset += PAGE_SIZE) {
//         u64 vaddr = KERNEL_VIRT_BASE + offset;
//         u64 paddr = KERNEL_PHYS_BASE + offset;
//         map_page(pml4, vaddr, paddr, PAGE_FLAGS);
//     }

//     return pml4;

// }

void initialize_paging() {
    MAXPHYSADDR = get_max_phys_addr_bits();
    printf("got max physical address: %x\n", MAXPHYSADDR);
    
    // find_heap_address();
    // u64 *pml4 = setup_tables(); 
    // write_cr3((u64)pml4);
}