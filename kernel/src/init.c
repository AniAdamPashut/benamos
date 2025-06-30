#include "cpu/gdt.h"
#include "cpu/idt.h"

void init() {
    initialize_gdt();
    initialize_idt();
}