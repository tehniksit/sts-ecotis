#ifndef LCD_H_
#define LCD_H_

# define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>

//----------------------------------------
void LCD_ini(void);
void setpos(unsigned char x, unsigned y);
void str_lcd (char str1[]);
//----------------------------------------
#define e1    PORTA|=0b00000001 // установка линии E в 1
#define e0    PORTA&=0b11111110 // установка линии E в 0
#define rs1    PORTA|=0b00000010 // установка линии RS в 1 (данные)
#define rs0    PORTA&=0b11111101 // установка линии RS в 0 (команда)
//----------------------------------------


#endif /* LCD_H_ */