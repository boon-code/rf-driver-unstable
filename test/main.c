#define F_CPU F_OSC

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/sleep.h>


#include "global.h"

#include "uart.h"
#include "rfm12.h"

void send(void);
void receive(void);

void blink(void)
{
  unsigned char i = 20;
  for(; i > 0; --i)
  {
    sbi(PORTD, PD6);
    _delay_ms(50);
    cbi(PORTD, PD6);
    _delay_ms(50);
  }
}

ISR(INT2_vect)
{
	sbi(PORTA, PA2);
	sbi(PORTD, PD6);
  send();
	sbi(GIFR, INTF2);
}

int main(void)
{
	DDRD = _BV(PD6);
	DDRA = (_BV(PA2) | _BV(PA3));
	PORTD = _BV(PD6);
	
	uart_init(9600);
	uart_write_str("s \n");
	
	blink();
	
	rfm_init();
	uart_write_str("init\n");
	rfm_frequency(433.92);
	rfm_bandwidth(4, 1, 4);
	rfm_baudrate(19200);
	rfm_power(4, 0);
	
  uart_transmit('\n');
  uart_write_shex(rfm_command(0x0000));
  uart_transmit('\n');
  
  blink();
  
  MCUCSR &= ~(_BV(ISC2));
  GICR |= _BV(INT2);
  // rising edge
  
  sei();

	while(1)
	{
		sbi(PORTD, PD6);
    _delay_ms(1000);
    cbi(PORTD, PD6);
    rfm_command(0xCA81);
    rfm_command(0x8238);
    rfm_command(0xB8AA);
    _delay_ms(1000);
	}
}

void receive(void)
{	
  unsigned char test[32];
  
	rfm_simple_rx(test, 32);
	
	//sbi(PORTD, PD6);
	
	for(unsigned char i = 0; i < 32; ++i)
	{
	  uart_transmit(test[i]);
	}
	uart_transmit('\n');
	
	//cbi(PORTD, PD6);
}

void send(void)
{
  unsigned char test[]="Neuer Test!\0";	
	rfm_simple_tx(test, 11);
}

