#include <stdint.h>

#include "benami.h"
#include "printf.h"
#include "serial.h"

#define COM1 0x3F8

#include <stdint.h>

#include "benami.h"
#include "printf.h"
#include "serial.h"

#define COM1 0x3F8

// Initialize COM1 serial port
void initialize_com1(void) {
    outb(0x3F8 + 1, 0x00); // Disable interrupts
    outb(0x3F8 + 3, 0x80); // Enable DLAB
    outb(0x3F8 + 0, 0x01); // Set divisor (115200 baud)
    outb(0x3F8 + 1, 0x00);
    outb(0x3F8 + 3, 0x03); // 8N1
    outb(0x3F8 + 2, 0xC7); // Enable FIFO
    outb(0x3F8 + 4, 0x0B); // IRQs enabled, RTS/DSR set
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