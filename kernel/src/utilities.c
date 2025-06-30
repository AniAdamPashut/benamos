#include "benami.h"
#include "serial.h"

void hcf(void) {
    for (;;) {
        asm ("hlt");
    }
}

u64 read_cr0()
{
    u64 cr;
    __asm__ __volatile__ ( "mov %%cr0, %0" : "=r"(cr) );
    return cr;
}

u64 read_cr2()
{
    u64 cr;
    __asm__ __volatile__ ( "mov %%cr2, %0" : "=r"(cr) );
    return cr;
}

u64 read_cr3()
{
    u64 cr;
    __asm__ __volatile__ ( "mov %%cr3, %0" : "=r"(cr) );
    return cr;
}

u64 read_cr4()
{
    u64 cr;
    __asm__ __volatile__ ( "mov %%cr4, %0" : "=r"(cr) );
    return cr;
}

uint8_t rtc_get_seconds()
{
    outb(0x70, 0x00);
    return inb(0x71);
}

uint8_t rtc_get_minutes()
{
    outb(0x70, 0x02);
    return inb(0x71);
}

uint8_t rtc_get_hours()
{
    outb(0x70, 0x04);
    return inb(0x71);
}

uint8_t rtc_get_weekday()
{
    outb(0x70, 0x06);
    return inb(0x71);
}