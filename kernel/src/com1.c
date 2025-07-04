#include <stdint.h>

#include "benami.h"
#include "printf.h"
#include "serial.h"

#define COM1 0x3F8

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