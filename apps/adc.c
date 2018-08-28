#include "adc.h"
#include <avr/io.h>

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
//----------------------------------------
