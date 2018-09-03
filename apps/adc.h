#ifndef ADC_H_
#define ADC_H_
#include <avr/io.h>


void ADC_Init(void);
char adc_dec[4];
unsigned int percentage;
unsigned int total_adc;
unsigned int adc_value;

#endif /* ADC_H_ */