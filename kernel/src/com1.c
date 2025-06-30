#include <stdint.h>

#include "benami.h"
#include "printf.h"

#define COM1 0x3F8

void outb(u16 port, u8 value) {
    __asm__ volatile (
        "outb %0, %1"
        :
        : "a"(value), "d"(port)
    );
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__ volatile (
        "inb %1, %0"
        : "=a"(ret)
        : "d"(port)
    );
    return ret;
}

int serial_is_transmit_ready() {
    return inb(COM1 + 5) & 0x20;
}

void serial_write_char(char c) {
    while (!serial_is_transmit_ready());
    outb(COM1, c);
}

void serial_write_string(const char* s) {
    while (*s) serial_write_char(*s++);
}

void _putchar(char c) {
    serial_write_char(c);
}