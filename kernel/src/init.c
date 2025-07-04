#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/mm.h"

void init() {
    initialize_gdt();
    initialize_idt();

    initialize_paging();
}