#ifndef ULTRA_H
#define ULTRA_H

/* Initialize the ultrasonic sensor and Timer4 */
void ULTRA_Init(void);

/* Measure distance in centimeters
   Returns 0 if no valid echo is received */
unsigned int ULTRA_GetDistanceCm(void);

#endif
