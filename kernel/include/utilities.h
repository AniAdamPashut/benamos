#ifndef UTILS_H
#define UTILS_H

#include "benami.h"

inline void hcf(void);

inline u64 read_cr0();
inline u64 read_cr2();
inline u64 read_cr3();
inline u64 read_cr4();

uint8_t rtc_get_seconds();
uint8_t rtc_get_minutes();
uint8_t rtc_get_hours();
uint8_t rtc_get_weekday();

#endif 