#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/* Initialize Timer3 for 8-bit Fast PWM mode */
void PWM_Init(void);

/* Set PWM duty cycle from 0% to 100% */
void PWM_SetDuty(uint8_t duty);

/* Start Timer3 with prescaler 64 */
void PWM_Start(void);

/* Stop Timer3 and stop PWM generation */
void PWM_Stop(void);

#endif
