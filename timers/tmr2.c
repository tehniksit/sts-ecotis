//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// ��� ����������� ����������� ���������������� ��������. �� ������ ���������
// ��� �� ����� �����, �� �� �������� ������� ������ �� ��� YouTube-�����
// "����������� � ���������" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// �����: �������� ������ / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#include <avr/io.h>
#define F_CPU 1000000UL
#include <avr/interrupt.h>
#include "timers.h"
#include "tmr2.h"

tmr_handler tmr2_handler;
uint8_t CSreg2 = (1 << CS20);


// ��������� �������� �������2
const uint16_t tmr2_divs[] =
{
	1,
	8,
	32,
	64,
	128,
	256,
	1024
};


//==============================================================================
//
//==============================================================================
uint8_t timer2_getCSregbits(uint16_t Divider)
{
	switch (Divider)
	{
		case 1:
		return (1 << CS20);
		case 8:
		return (1 << CS21);
		case 32:
		return (1 << CS20) | (1 << CS21);
		case 64:
		return (1 << CS22);
		case 128:
		return (1 << CS20) | (1 << CS22);
		case 256:
		return (1 << CS21) | (1 << CS22);
		case 1024:
		return (1 << CS20) | (1 << CS21) | (1 << CS22);
		default:
		return 0x00;
	}
}
//==============================================================================


//==============================================================================
// ��������� ����������� ������
//==============================================================================
void tmr2_init(uint32_t Freq, void (*func)(void))
{
	// ��������� ���������� ���������
	cli();
	
	// ���������� �������, ������� ����� �������� � ����������� ���������� �������
	tmr2_handler = func;
	
	// ����������� ����� ������ �������
	// ����� CTC, ������ �� OC2A, OC2B �� ����������
	// ���������� �� ��������� TCNT2 � OCR2A
	
	TCCR2 = 0x00;
	TCCR2 = 0x00;
	TCCR2 |= (1 << WGM21);
	TCCR2 |= (1 << FOC2);
	
	// ������� ����������� �������� ������� � �������� ��� ���������
	uint32_t Coef = F_CPU * 1000000 / Freq;
	uint32_t OC2_value = 256;
	
	// ���������� �������� ������� ������� � �����������
	for (uint8_t i = 0; i < (sizeof(tmr2_divs) / sizeof(tmr2_divs[0])); i++)
	{
		OC2_value = Coef / tmr2_divs[i];
		
		if (OC2_value < 256)        // �������� ������
		{
			CSreg2 = timer2_getCSregbits(tmr2_divs[i]);
			break;
		}
	}
	
	// �������� ��� ���������
	OCR2 = (uint8_t) (OC2_value - 1);
	// ���������� ����� ���������� �������
	TIFR = (1 << TOV2) | (1 << OCF2) | (1 << OCF2);
	// �������� ���������� �� ��������� TCNT2 � OCR2A
	TIMSK = (1 << OCIE2);
	
	// ��������� ���������� ���������
	sei();
}
//==============================================================================


//==============================================================================
// ��������� �������� ������
//==============================================================================
void tmr2_start(void)
{
	TCNT2 = 0;
	TCCR2 = CSreg2;
}
//==============================================================================


//==============================================================================
// ��������� ������������� ������
//==============================================================================
void tmr2_stop(void)
{
	TCCR2 = 0x00;
}
//==============================================================================


//==============================================================================
// ���������� ���������� �������2
//==============================================================================
ISR(TIMER2_COMP_vect)
{
	tmr2_handler();
}
//==============================================================================
