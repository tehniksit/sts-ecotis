# define F_CPU 1000000UL
#include "Ecotis.h"
#include <avr/io.h>
#include <util/delay.h>


void ecotis_port_init(void)
{
	//port init for lcd
	ADCSRA &= ~(1<<ADEN); 
	PORTA = 0x00;
	DDRA = 0xFF;
	
	///port init for buttons
	//DDRC = 0b10000000;
	//PORTC = 0b10000000;
    //
	//PORTE |=(1<<PE4)|(1<<PE5)|(1<<PE6)|(1<<PE7);
	//DDRE &=~((1<<PE4)|(1<<PE5)|(1<<PE6)|(1<<PE7));
	//*///
	
	
	//инициализация порта для соленоидов
	
	DRIVER_PORT_DIRECT1 |= ((1<<DRIVER_PIN5) | (1<<DRIVER_PIN4) | (1<<DRIVER_PIN3) | (1<<DRIVER_PIN2) | (1<<DRIVER_PIN1) | (1<<DRIVER_PIN0));
	DRIVER_PORT_DIRECT2 |= ((1<<DRIVER_PIN9) | (1<<DRIVER_PIN8));
	DRIVER_PORT1 &= ~((1<<DRIVER_PIN5) | (1<<DRIVER_PIN4) | (1<<DRIVER_PIN3) | (1<<DRIVER_PIN2) | (1<<DRIVER_PIN1) | (1<<DRIVER_PIN0));
	DRIVER_PORT2 &= ~((1<<DRIVER_PIN9) | (1<<DRIVER_PIN8));

	//инициализация порта для моторов
	
	DRIVER_PORT_DIRECT1 |= ((1<<DRIVER_PIN7) | (1<<DRIVER_PIN6));
	DRIVER_PORT1 &= ~((1<<DRIVER_PIN7) | (1<<DRIVER_PIN6));

	
}

void ecotis_loading(void)
{
		setpos(0,0);
		str_lcd("     ECOTIS     ");
		setpos(0,0);
		str_lcd("SmartTonerSystem");
		_delay_ms(1000);
		str_lcd("     ECOTIS     ");
		setpos(0,1);
		str_lcd("Loading System..");
		_delay_ms(1500);
		setpos(0,0);
		str_lcd("     ECOTIS     ");
		setpos(0,1);
		str_lcd(" K:F C:F M:F Y:F");
		// turn off supplying K toner
		//	PORTG &=~((1<<0)|(1<<1)); //123
		//	PORTD&=~((1<<7)|(1<<6));
		//text
		
		
}
void system_state(void)
{
	
	//show_state_lcd();
	setpos(0,1);
	str_lcd(" K:F C:F M:F Y:F");
	
	PORTD = 0;
	PORTG = 0;
	//_delay_ms(200);
	//show_ip_lcd();
	
}
void load_bk_toner(void)
{
	//function for loading K toner
	setpos(0,1);
	str_lcd(" K:L C:F M:F Y:F");
	_delay_ms(1500);
	setpos(0,0);
	str_lcd(" Low toner Black ");
	setpos(0,1);
	str_lcd("Loading toner...");
	_delay_ms(100);
	_delay_ms(1000);
	DRIVER_PORT1 |=((1<<DRIVER_PIN5)|(1<<DRIVER_PIN4)); // turn on the clutches
	DRIVER_PORT1 |=(1<<DRIVER_PIN7); //run motor 1
	_delay_ms(1300); //supply time
	//PORTG |=(1<<1); //run auxiliary motor 2
	DRIVER_PORT1 |=(1<<DRIVER_PIN6);
	_delay_ms(50); //
	DRIVER_PORT1 &=~((1<<DRIVER_PIN7)|(1<<DRIVER_PIN6)); //turn off motors
	//PORTD &=~((1<<6));
	_delay_ms(2000);
	DRIVER_PORT1 &=~((1<<DRIVER_PIN5)|(1<<DRIVER_PIN4));
	pub_toner_sup_bk();
}
void load_c_toner(void)
{
	setpos(0,1);
	str_lcd(" K:F C:L M:F Y:F");
	_delay_ms(1500);
	setpos(0,0);
	str_lcd(" Low toner Cyan ");
	setpos(0,1);
	str_lcd("Loading toner...");
	_delay_ms(100);
	_delay_ms(1000);
	PORTD |=((1<<5)|(1<<4)); // turn on the clutches
	PORTG |=(1<<0); //run motor 1
	_delay_ms(1300); //supply time
	//PORTG |=(1<<1); //run auxiliary motor 2
	PORTG |=(1<<1);
	_delay_ms(50); //
	PORTG &=~((1<<0)|(1<<1)); //turn off motors
	//PORTD &=~((1<<6));				 //switch off clutches
	_delay_ms(2000);
	PORTD &=~((1<<6)|(1<<7));
	pub_toner_sup_c();
}
void load_m_toner(void)
{
	setpos(0,1);
	str_lcd(" K:F C:F M:L Y:F");
	_delay_ms(1500);
	setpos(0,0);
	str_lcd("Low toner Magenta");
	setpos(0,1);
	str_lcd("Loading toner...");
	_delay_ms(100);
	_delay_ms(1000);
	PORTD |=((1<<3)|(1<<2)); // turn on the clutches
	PORTG |=(1<<0); //run motor 1
	_delay_ms(1300); //supply time
	//PORTG |=(1<<1); //run auxiliary motor 2
	PORTG |=(1<<1);
	_delay_ms(50); //
	PORTG &=~((1<<0)|(1<<1)); //turn off motors
	//PORTD &=~((1<<6));				 //switch off clutches
	_delay_ms(2000);
	PORTD &=~((1<<6)|(1<<7));
	pub_toner_sup_m();
}
void load_y_toner(void)
{
	setpos(0,1);
	str_lcd(" K:F C:F M:F Y:L");
	_delay_ms(1500);
	setpos(0,0);
	str_lcd("Low toner Yelow");
	setpos(0,1);
	str_lcd("Loading toner...");
	_delay_ms(100);
	PORTD |=((1<<1)|(1<<0)); // turn on the clutches
	PORTG |=(1<<0); //run motor 1
	_delay_ms(1300); //supply time
	//PORTG |=(1<<1); //run auxiliary motor 2
	PORTG |=(1<<1);
	_delay_ms(50); //
	PORTG &=~((1<<0)|(1<<1)); //turn off motors
	//PORTD &=~((1<<6));				 //switch off clutches
	_delay_ms(2000);
	PORTD &=~((1<<6)|(1<<7));
	pub_toner_sup_y();
}
void ecotis_task(void)
{
	
	
	
	int but_count = 0;
	
	if((PINC & (1<<PC0)))
	{
		
		
		load_bk_toner();
		if (but_count < 5)
		{
			but_count++;

		}
		else
		{
			//function for loading K toner
			setpos(0,1);
			str_lcd(" K:L C:F M:F Y:F");
			_delay_ms(1500);
			setpos(0,0);
			str_lcd(" Low toner Black ");
			setpos(0,1);
			str_lcd("Loading toner...");
			_delay_ms(100);
			_delay_ms(1000);
			PORTD |=((1<<7)|(1<<6)); // turn on the clutches
			PORTG |=(1<<0); //run motor 1
			_delay_ms(1300); //supply time
			//PORTG |=(1<<1); //run auxiliary motor 2
			PORTG |=(1<<1);
			_delay_ms(50); //
			PORTG &=~((1<<0)|(1<<1)); //turn off motors
			//PORTD &=~((1<<6));
			_delay_ms(2000);
			PORTD &=~((1<<6)|(1<<7));
			
			

		}
	}
	
	
	else if((PINC & (1<<PC2)))
	{
		load_c_toner();
	}
	
	/*if (but_count < 5)
	{
		but_count++;

	}
	else
	{
		//function for loading K toner
		setpos(0,1);
		str_lcd(" K:F C:L M:F Y:F");
		_delay_ms(1500);
		setpos(0,0);
		str_lcd(" Low toner Cyan ");
		setpos(0,1);
		str_lcd("Loading toner...");
		_delay_ms(100);
		_delay_ms(1000);
		PORTD |=((1<<5)|(1<<4)); // turn on the clutches
		PORTG |=(1<<0); //run motor 1
		_delay_ms(1300); //supply time
		//PORTG |=(1<<1); //run auxiliary motor 2
		PORTG |=(1<<1);
		_delay_ms(50); //
		PORTG &=~((1<<0)|(1<<1)); //turn off motors
		//PORTD &=~((1<<6));				 //switch off clutches
		_delay_ms(2000);
		PORTD &=~((1<<6)|(1<<7));

	}*/
	else if ((PINC & (1<<PC1)))
	{
		load_m_toner();
		
		/*if (but_count < 5)
		{
			but_count++;

		}
		else
		{
			//function for loading K toner
			setpos(0,1);
			str_lcd(" K:F C:F M:L Y:F");
			_delay_ms(1500);
			setpos(0,0);
			str_lcd("Low toner Magenta");
			setpos(0,1);
			str_lcd("Loading toner...");
			_delay_ms(100);
			_delay_ms(1000);
			PORTD |=((1<<3)|(1<<2)); // turn on the clutches
			PORTG |=(1<<0); //run motor 1
			_delay_ms(1300); //supply time
			//PORTG |=(1<<1); //run auxiliary motor 2
			PORTG |=(1<<1);
			_delay_ms(50); //
			PORTG &=~((1<<0)|(1<<1)); //turn off motors
			//PORTD &=~((1<<6));				 //switch off clutches
			_delay_ms(2000);
			PORTD &=~((1<<6)|(1<<7));

		}*/
	}
	else if ((PINC & (1<<PC3)))
	{
		
		load_y_toner();
		/*if (but_count < 5)
		{
			but_count++;

		}
		else
		{
			//function for loading K toner
			setpos(0,1);
			str_lcd(" K:F C:F M:F Y:L");
			_delay_ms(1500);
			setpos(0,0);
			str_lcd("Low toner Yelow");
			setpos(0,1);
			str_lcd("Loading toner...");
			_delay_ms(100);
			PORTD |=((1<<1)|(1<<0)); // turn on the clutches
			PORTG |=(1<<0); //run motor 1
			_delay_ms(1300); //supply time
			//PORTG |=(1<<1); //run auxiliary motor 2
			PORTG |=(1<<1);
			_delay_ms(50); //
			PORTG &=~((1<<0)|(1<<1)); //turn off motors
			//PORTD &=~((1<<6));				 //switch off clutches
			_delay_ms(2000);
			PORTD &=~((1<<6)|(1<<7));
		}*/
	}
	
	
	/*else
	{
		setpos(0,0);
		str_lcd("     EcoTis     ");
		setpos(0,1);
		str_lcd(" K:F C:F M:F Y:F");
		PORTD = 0;
		PORTG = 0;
		
		if (but_count > 0)
		{
			but_count--;

		}
		else
		{
			
			setpos(0,0);
			str_lcd("     EcoTis     ");
			_delay_ms(1000);
			setpos(0,1);
			str_lcd(" K:F C:F M:F Y:F");
			PORTD = 0;
			PORTG = 0;
			// turn off supplying K toner
			
			
			
			
			
		}
		
		
	}*/
	
	
	

}