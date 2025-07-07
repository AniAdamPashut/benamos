#include "cpu/gdt.h"
#include "cpu/idt.h"
#include "cpu/mm.h"
#include "com1.h"

void init() {
    initialize_com1();
    initialize_gdt();
    initialize_idt();

    initialize_paging();
}