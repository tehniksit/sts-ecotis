#include "adc.h"


void ADC_Init(void)
{
	ADCSRA |= (1<<ADEN) // ���������� ������������� ���
	|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);//�������� 128 = 64 ���
	ADMUX |= (1<<REFS1)|(1<<REFS0); //���������� �������� �� 2,56�, ���� ADC0

}
//----------------------------------------
unsigned int ADC_convert (void)
{
	ADCSRA |= (1<<ADSC); //�������� ��������������
	while((ADCSRA & (1<<ADSC))); //�������� ����������� �� �������-�������� ��������������
	return (unsigned int) ADC;
}
//---------------------------------------
void adc_task(void)
{
	total_adc = 1023;
	adc_value = ADC_convert(); //������� ��������������
	setpos(0,0);
	percentage =  (100.0f * adc_value)/total_adc;
	snprintf(adc_dec, 4, "%d%%  ", percentage);
	str_lcd(adc_dec);
}