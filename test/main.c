#define F_CPU F_OSC

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <avr/sleep.h>


#include "global.h"

#include "uart.h"
#include "button.h"

void blink(void)
{
  unsigned char i = 50;
  for(; i > 0; --i)
  {
    sbi(PORTD, PD6);
    _delay_ms(50);
    cbi(PORTD, PD6);
    _delay_ms(50);
  }
}

ISR(BUTTON_ISR_NAME)
{
  unsigned char result;
  sbi(PORTD, PD6);
  result = button_begintest(1800);
  
  cbi(PORTD, PD6);
  
  if(result & _BV(0))
  {
  	PORTA &= ~(_BV(PA2) | _BV(PA3));
  }
  
  if(result & _BV(1))
  {
    PORTA |= _BV(PA3);
  } 
  
  if(result & _BV(2))
  {
    PORTA |= _BV(PA2);
  }
  
  _delay_ms(50);
  button_endtest();
}

int main(void)
{
  // all output ports
  DDRD = _BV(PD6);
  DDRA = (_BV(PA2) | _BV(PA3));
  PORTA = (_BV(PA2) | _BV(PA3));
  PORTD = (_BV(PD2) | _BV(PD6));
  
  _delay_ms(1000);
  
  PORTA &= ~(_BV(PA2) | _BV(PA3));
  PORTD &= ~(_BV(PD6));
  
  // low level generates interrupt
  //GICR |= _BV(INT0);   // enable INT0
  
  // ISC2 = 0 -> falling edge
  
  button_init();
  
  sei();
  
  while(1)
  {
    sleep_enable();
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    sleep_cpu();
    sleep_disable();
    blink();
  }
  
  return 0;
}
