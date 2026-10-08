#ifndef PWM_H
#define PWM_H

#include <stdint.h>

/* PWM initialization */
void PWM_Init(void);

/* Set PWM duty cycle: 0 to 100 % */
void PWM_SetDuty(uint8_t duty);

/* Start PWM */
void PWM_Start(void);

/* Stop PWM */
void PWM_Stop(void);

#endif