#ifndef ULTRA_H
#define ULTRA_H

#include <stdint.h>

/* Initialize ultrasonic sensor */
void ULTRA_Init(void);

/* Return distance in centimeters */
unsigned int ULTRA_GetDistanceCm(void);

#endif