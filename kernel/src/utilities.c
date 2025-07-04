#include "benami.h"
#include "serial.h"


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