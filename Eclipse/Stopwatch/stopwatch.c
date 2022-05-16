/*
 * stopwatch.c
 *
 *  Created on: Sep 19, 2021
 *      Author: Muhammad Al-Barbary
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void timer();
void BUTTONS();

char seconds1=0,minutes1=0,hours1=0;
char seconds2=0,minutes2=0,hours2=0;

int main()
{
	DDRC |= 0x0F;
	DDRA |= 0b00111111;
	BUTTONS();
	timer();
	while(1)
	{
		PORTA = (PORTA & 0xC0) | (1<<0);
		PORTC = (PORTC & 0xF0) | (seconds1 & 0x0F);
		_delay_ms(5);
		if(seconds1>=10)
		{
			seconds1=0;
			seconds2++;
		}
		PORTA = (PORTA & 0xC0) | (1<<1);
		PORTC = (PORTC & 0xF0) | (seconds2 & 0x0F);
		if(seconds2>=6)
		{
			seconds2=0;
			minutes1++;
		}
		_delay_ms(5);



		PORTA = (PORTA & 0xC0) | (1<<2);
		PORTC = (PORTC & 0xF0) | (minutes1 & 0x0F);
		_delay_ms(5);
		if(minutes1>=10)
		{
			minutes1=0;
			minutes2++;
		}
		PORTA = (PORTA & 0xC0) | (1<<3);
		PORTC = (PORTC & 0xF0) | (minutes2 & 0x0F);
		if(minutes2>=6)
		{
			minutes2=0;
			hours1++;
		}
		_delay_ms(5);



		PORTA = (PORTA & 0xC0) | (1<<4);
		PORTC = (PORTC & 0xF0) | (hours1 & 0x0F);
		_delay_ms(5);
		if(hours1>=10)
		{
			hours1=0;
			hours2++;
		}
		PORTA = (PORTA & 0xC0) | (1<<5);
		PORTC = (PORTC & 0xF0) | (hours2 & 0x0F);
		if((hours2>=2) && (hours1>=4))
		{
			seconds1=0,minutes1=0,hours1=0;
			seconds2=0,minutes2=0,hours2=0;
		}
		_delay_ms(5);

	}
}

void timer(void)
{
	SREG &= ~(1<<7);
	TCNT1 = 0; //timer initiation
	OCR1A = 15624; // interrupt every 1s when freq= freq_processor/64
	TIMSK |= (1<<OCIE1A); //enable interrupt
	TCCR1A &= ~( (1<<COM1A1) | (1<<COM1A0) | (1<<COM1B1) | (1<<COM1B0) | (1<<WGM10) | (1<<WGM11) | (1<<FOC1B));
	TCCR1A |=  (1<<FOC1A);
	TCCR1B |=  ( (1<<WGM12) | (1<<CS11) | (1<<CS10) );
	TCCR1B &= ~( (1<<WGM13) | (1<<CS12) );
	SREG |= (1<<7);
}


ISR(TIMER1_COMPA_vect)
{
	seconds1++;
}

void BUTTONS (void)
{
	DDRB &= ~( (1<<2) | (1<<3) );
	DDRD &= ~(1<<2);
	PORTB |= ( (1<<2) | (1<<3) );
	PORTD |= (1<<2);
	SREG &= ~(1<<7);
	GICR |= ( (1<<7) | (1<<6) | (1<<5) );
	MCUCR |= ( (1<<ISC01) | (1<<ISC11) | (1<<ISC10) );
	MCUCR &= ~(1<<ISC00);
	MCUCSR &= ~(1<<ISC2);
	GIFR |= ( (1<<INTF1) | (INTF0) | (INTF2) );
	SREG |= (1<<7);
}

ISR(INT0_vect)
{
	 seconds1=0,minutes1=0,hours1=0;
	 seconds2=0,minutes2=0,hours2=0;


}

ISR(INT1_vect)
{

	while (! (GIFR &(1<<INTF2)) )
	{
		PORTA = (PORTA & 0xC0) | (1<<0);
		PORTC = (PORTC & 0xF0) | (seconds1 & 0x0F);
		_delay_ms(5);
		PORTA = (PORTA & 0xC0) | (1<<1);
		PORTC = (PORTC & 0xF0) | (seconds2 & 0x0F);
		_delay_ms(5);
		PORTA = (PORTA & 0xC0) | (1<<2);
		PORTC = (PORTC & 0xF0) | (minutes1 & 0x0F);
		_delay_ms(5);
		PORTA = (PORTA & 0xC0) | (1<<3);
		PORTC = (PORTC & 0xF0) | (minutes2 & 0x0F);
		_delay_ms(5);
		PORTA = (PORTA & 0xC0) | (1<<4);
		PORTC = (PORTC & 0xF0) | (hours1 & 0x0F);
		_delay_ms(5);
		PORTA = (PORTA & 0xC0) | (1<<5);
		PORTC = (PORTC & 0xF0) | (hours2 & 0x0F);
		_delay_ms(5);

	}
}

ISR(INT2_vect)
{
}
