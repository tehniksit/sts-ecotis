//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _DELAY_H
#define _DELAY_H
#define F_CPU 1000000UL
#define BAUD 9600
#include <avr/io.h>
#include <util/delay.h>
//#include "intrinsics.h"


#define delay_us(us)          __delay_cycles((F_CPU * us)/1000000uL)

// Процедура программной задержки ~1 мс
void delay_ms(unsigned int ms);

#endif

/*//------------------------------------------------------------------------------
// This is Open source software. You can place this code on your site, but don't
// forget a link to my YouTube-channel: https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Это программное обеспечение распространяется свободно. Вы можете размещать
// его на вашем сайте, но не забудьте указать ссылку на мой YouTube-канал 
// "Электроника в объектике" https://www.youtube.com/channel/UChButpZaL5kUUl_zTyIDFkQ
// Автор: Надыршин Руслан / Nadyrshin Ruslan
//------------------------------------------------------------------------------
#ifndef _DELAY_H
#define _DELAY_H

//#include <intrinsics.h>


#define delay_us(tick)          __delay_cycles(tick * F_CPU)

// Процедура программной задержки ~1 мс
void delay_ms(unsigned int ms);

#endif*/
