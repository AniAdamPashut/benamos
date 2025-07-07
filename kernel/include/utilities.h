#ifndef UTILS_H
#define UTILS_H

#include "benami.h"

extern void hcf();

extern u64 read_cr0();
extern u64 read_cr2();
extern u64 read_cr3();
extern u64 read_cr4();

extern void write_cr0(u64 value);
extern void write_cr2(u64 value);
extern void write_cr3(u64 value);
extern void write_cr4(u64 value);

extern void clear_interrupts();
extern void set_interrupts();

uint8_t rtc_get_seconds();
uint8_t rtc_get_minutes();
uint8_t rtc_get_hours();
uint8_t rtc_get_weekday();



#endif 