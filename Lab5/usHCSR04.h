#ifndef USHCSR04_H
#define USHCSR04_H

#include <stdint.h>

void setupHCSR04(void);

void triggerHCSR04(void);

void getDistance(uint16_t *cm, uint16_t *in);

#endif