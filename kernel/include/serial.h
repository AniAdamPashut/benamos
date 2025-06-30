#ifndef SERIAL_H
#define SERIAL_H

#include "benami.h"

void outb(u16 port, u8 value);

u8 inb(u16 port);

#endif