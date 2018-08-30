#include "adc.h"


void ADC_Init(void)
{
	ADCSRA |= (1<<ADEN) // Разрешение использования АЦП
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//Делитель 128 = 64 кГц
	ADMUX |= (1<<REFS1)|(1<<REFS0); //Внутренний Источник ОН 2,56в, вход ADC0

}
//----------------------------------------
unsigned int ADC_convert (void)
{
	ADCSRA |= (1<<ADSC); //Начинаем преобразование
	while((ADCSRA & (1<<ADSC))); //проверим закончилось ли аналого-цифровое преобразование
	return (unsigned int) ADC;
}
//---------------------------------------
void adc_task(void)
{
	total_adc = 1023;
	adc_value = ADC_convert(); //Вызовем преобразование
	setpos(0,0);
	percentage =  (100.0f * adc_value)/total_adc;
	snprintf(adc_dec, 4, "%d%%  ", percentage);
	str_lcd(adc_dec);
}