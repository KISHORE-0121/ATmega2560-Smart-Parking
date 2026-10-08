#ifndef ADC_H
#define ADC_H

#include <stdint.h>

/* ADC initialization */
void ADC_Init(void);

/* Read 10-bit ADC value from selected channel */
uint16_t ADC_Read(uint8_t channel);

#endif