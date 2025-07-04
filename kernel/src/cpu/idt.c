#include "cpu/idt.h"
#include "printf.h"
#include "utilities.h"

static volatile idt_t interrupt_table = {0};

#define REGISTER_HANDLER(handler, index) isr_handler_table[index] = handler
#define REGISTER_ISR(isr, index) interrupt_table.entries[index] = create_interrupt_descriptor((u64)isr, 0x8, 0)

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};


interrupt_descriptor_t create_interrupt_descriptor(u64 handler, u16 segment_selector, u8 ist) {
    u8 attributes = 0x8F;
    u16 offset_low = handler & 0xFFFF;
    u16 offset_middle = (handler >> 16) & 0xFFFF;
    u32 offset_high = (handler >> 32);
    u16 options = (attributes << 8) | ist;

    return (interrupt_descriptor_t) {
        .offset_high = offset_high,
        .offset_low = offset_low,
        .offset_middle = offset_middle,
        .options = options,
        .segment_selector = segment_selector,
        ._reserved = 0
    };
}

static void* isr_handler_table[256] = { NULL };

void isr_handler(struct registers *r)
{
	void (*handler)(struct registers* r);
    handler = isr_handler_table[r->int_no];

    if  (handler != NULL)
    {
        handler(r);
        return;
    }
    printf("[ISRERR]\n");
    printf("\x1b[90m%02x:%02x:%02x\x1b[0m \x1b[91mPANIC\x1b[0m \x1b[90m%s:%s:%u\x1b[0m ", rtc_get_hours(), rtc_get_minutes(), rtc_get_seconds(), __FILE__, __func__, __LINE__);
    printf("%s [%lu]\n", exception_messages[r->int_no], r->error);

    printf("Faulting Address (RIP) = 0x%016lX\n", r->rip);
    printf("R15 = 0x%016lX R14 = 0x%016lX\n", r->r15, r->r14);
    printf("R13 = 0x%016lX R12 = 0x%016lX\n", r->r13, r->r12);
    printf("R11 = 0x%016lX R10 = 0x%016lX\n", r->r11, r->r10);
    printf("R9  = 0x%016lX R8  = 0x%016lX\n", r->r9 , r->r8 );
    printf("RBP = 0x%016lX RSP = 0x%016lX\n", r->rbp, r->rsp);
    printf("RSI = 0x%016lX RDI = 0x%016lX\n", r->rsi, r->rdi);
    printf("RDX = 0x%016lX RCX = 0x%016lX\n", r->rdx, r->rcx);
    printf("RBX = 0x%016lX RAX = 0x%016lX\n", r->rbx, r->rax);
    printf("CS  = 0x%016lX SS  = 0x%016lX\n", r->cs , r->ss );
    printf("CR4 = 0x%016lX CR3 = 0x%016lX\n", read_cr4(), read_cr3());
    printf("CR2 = 0x%016lX CR0 = 0x%016lX\n", read_cr2(), read_cr0());
    printf("RFLAGS = 0x%016lX\n", r->rflags);

    hcf();
}

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr128();


void set_interrupts_in_table() {
    REGISTER_ISR(isr0, 0);
    REGISTER_ISR(isr1, 1);
    REGISTER_ISR(isr2, 2);
    REGISTER_ISR(isr3, 3);
    REGISTER_ISR(isr4, 4);
    REGISTER_ISR(isr5, 5);
    REGISTER_ISR(isr6, 6);
    REGISTER_ISR(isr7, 7);
    REGISTER_ISR(isr8, 8);
    REGISTER_ISR(isr9, 9);
    REGISTER_ISR(isr10, 10);
    REGISTER_ISR(isr11, 11);
    REGISTER_ISR(isr12, 12);
    REGISTER_ISR(isr13, 13);
    REGISTER_ISR(isr14, 14);
    REGISTER_ISR(isr15, 15);
    REGISTER_ISR(isr16, 16);
    REGISTER_ISR(isr17, 17);
    REGISTER_ISR(isr18, 18);
    REGISTER_ISR(isr19, 19);
    REGISTER_ISR(isr20, 20);
    REGISTER_ISR(isr21, 21);
    REGISTER_ISR(isr22, 22);
    REGISTER_ISR(isr23, 23);
    REGISTER_ISR(isr24, 24);
    REGISTER_ISR(isr25, 25);
    REGISTER_ISR(isr26, 26);
    REGISTER_ISR(isr27, 27);
    REGISTER_ISR(isr28, 28);
    REGISTER_ISR(isr29, 29);
    REGISTER_ISR(isr30, 30);
    REGISTER_ISR(isr31, 31);
    REGISTER_ISR(isr128, 128);
}

void breakpoint(struct registers* r) {
    printf("CUSTOM BREAKPOINT\n");
}

void load_custom_interrupts() {
    REGISTER_HANDLER(breakpoint, 0x03);
}

extern void load_idt();

void initialize_idt() { 
    set_interrupts_in_table();
    load_custom_interrupts();

    load_idt();
}