#define F_CPU F_OSC
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define sbi(reg, bit) reg |= (1<<bit)
#define cbi(reg, bit) reg &= ~(1<<bit)

#define NUM_BUTTONS 3
#define READ_DELAY_US 5


unsigned char buttons_test(unsigned int times)
{
  unsigned int but[BUTTON_COUNT];
  unsigned char result = 0x0;
  
  for(unsigned char i = 0; i < BUTTON_COUNT; ++i)
  {
    but[i] = 0;
  }
  
  for(unsigned int i = 0; i < times; ++i)
  {
    for(unsigned char j = 0; j < BUTTON_COUNT; ++j)
    {
      BUTTON_DDR = _BV(j);
      _delay_us(READ_DELAY_US);
      if((BUTTON_PIN & _BV(PD2)) == 0)
        ++but[j];
    }
  }
  
  for(unsigned char i = 0; i < BUTTON_COUNT; ++i)
  {
    if(but[i] > times/2)
      result |= _BV(i);
  }
  return result;
}

ISR(INT0_vect)
{
  unsigned char result;
  sbi(PORTD, PD6);
  result = buttons_test(2500);
  
  BUTTON_DDR = _BV(PC0) | _BV(PC1) | _BV(PC2);
  
  cbi(PORTD, PD6);
  
  if(result & _BV(PC0))
  {
  }
  
  if(result & _BV(PC1))
  {
  } 
  
  if(result & _BV(PC2))
  {
  }
  
  _delay_ms(200);
  
  sbi(GIFR, INTF2);
}

