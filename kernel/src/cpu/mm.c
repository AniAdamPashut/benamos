#include <limine.h>
#include "cpu/mm.h"
#include "printf.h"
#include "utilities.h"
#include "mandatory.h"

extern u64 get_max_phys_addr_bits();
extern void disable_paging();
extern void enable_paging();

#define PAGE_SIZE         0x1000ULL
#define PML4_ENTRIES      512
#define PDP_ENTRIES       512
#define PD_ENTRIES        512
#define PT_ENTRIES        512

#define KERNEL_VIRT_BASE  0xFFFFFFFF80000000ULL
#define KERNEL_PHYS_BASE  0x100000ULL  // Physical load address of kernel (1 MiB)

#define PAGE_PRESENT      (1ULL << 0)
#define PAGE_WRITABLE     (1ULL << 1)
#define PAGE_USER         (1ULL << 2)
#define PAGE_WRITE_THROUGH (1ULL << 3)
#define PAGE_CACHE_DISABLE (1ULL << 4)
#define PAGE_NO_EXECUTE   (1ULL << 63)

#define PAGE_FLAGS        (PAGE_PRESENT | PAGE_WRITABLE)

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0
};

static u8 *heap = (u8 *)NULL;

void find_heap_address() {
    struct limine_memmap_response *response = memmap_request.response;
    struct limine_memmap_entry **entries = response->entries;

    u64 amount_of_entries = response->entry_count;

    for (u64 i = 0; i < amount_of_entries; i++) {
        struct limine_memmap_entry *entry = entries[i];
        
        if (entry->type != LIMINE_MEMMAP_USABLE)
            continue;
        
        if (entry->length < 0x5000)
            continue;

        heap = (u8 *)entry->base;
        break;
    }

}

// void *alloc_page() {
//     void *page = heap;
//     heap += PAGE_SIZE;
//     memset(page, 0, PAGE_SIZE);
//     return page;
// }

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

struct pml4_entry {
    u8 present: 0;
    u8 readable: 1;
    u8 supervisor: 2;
    u8 write_through: 3;
    u8 cache_disable: 4;
    u8 accessed: 5;
    u8 dirty: 6;
    u8 page_size: 7;
    u8 global: 8;

    u64 address: 62;
    u8 execute_disable: 63;
};

u64 *setup_tables(void) {
    u64 *pml4 = alloc_page();
    *(pml4 + 511) = pml4 | 0b11;
    
    // Identity map: 0x0 - 0x1000000 (16 MiB)
    for (u64 addr = 0; addr < 0x1000000; addr += PAGE_SIZE) {
        map_page(pml4, addr, addr, PAGE_FLAGS);
    }

    // Higher-half map for kernel: map 16 MiB kernel area
    for (u64 offset = 0; offset < 0x1000000; offset += PAGE_SIZE) {
        u64 vaddr = KERNEL_VIRT_BASE + offset;
        u64 paddr = KERNEL_PHYS_BASE + offset;
        map_page(pml4, vaddr, paddr, PAGE_FLAGS);
    }

    return pml4;

}

void initialize_paging() {
    MAXPHYSADDR = get_max_phys_addr_bits();
    printf("got max physical address: %x\n", MAXPHYSADDR);
    
    find_heap_address();
    u64 *pml4 = setup_tables(); 
    write_cr3((u64)pml4);
}