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
  static unsigned char tmp = 0;
  unsigned char test = 0;
  rfm_real_ready();
	test = rfm_command(0xB000);
  //receive();
  rfm_command(0xCA81);			// set FIFO mode
	rfm_command(0x82C8);			// RX on
	rfm_command(0xCA83);
	
	if(tmp & 1)
	  sbi(PORTD, PD6);
	else
	  cbi(PORTD, PD6);
	
	++tmp;
	sbi(GIFR, INTF2);
}

int main(void)
{
	DDRD = _BV(PD6);
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
  
  // rising edge
  MCUCSR &= ~(_BV(ISC2));
  GICR |= _BV(INT2);
  
  rfm_command(0xCA81);			// set FIFO mode
	rfm_command(0x82C8);			// RX on
	rfm_command(0xCA83);
	
	sbi(GIFR, INTF2);
  
  sei();

	while(1)
	{
	  sleep_enable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_cpu();
    sleep_disable();
    
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
  unsigned char test[]="Dies ist ein 433MHz Test !!!\n   ";	
	rfm_simple_tx(test, 32);
}

