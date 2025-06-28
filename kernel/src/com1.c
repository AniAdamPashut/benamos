#include <stdint.h>
#include "benami.h"

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

void serial_init() {
    outb(COM1 + 1, 0x00); // Disable interrupts
    outb(COM1 + 3, 0x80); // Enable DLAB
    outb(COM1 + 0, 0x03); // Set baud rate divisor to 3 (38400 baud)
    outb(COM1 + 1, 0x00);
    outb(COM1 + 3, 0x03); // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7); // Enable FIFO, clear them
    outb(COM1 + 4, 0x0B); // IRQs enabled, RTS/DSR set
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