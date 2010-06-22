#define F_CPU F_OSC
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "button.h"

#define sbi(reg, bit) reg |= (1<<bit)
#define cbi(reg, bit) reg &= ~(1<<bit)

<?py:
def create_mask():
    echo(",".join(["_BV(%s)" % i for i in BUTTON_PINS]))
?>

volatile unsigned char g_button = 0x00;

static void read_delay(void)
{
  _delay_us(BUTTON_READ_DELAY);
}

unsigned char buttons_test(unsigned int times)
{
  unsigned char masks[BUTTON_COUNT] = {<?py:create_mask()?>};
  unsigned int but[BUTTON_COUNT] = {<?py:echo(",".join(['0' for i in range(BUTTON_COUNT)]))?>};
  unsigned char result = 0x0;
  BUTTON_DDR |= BUTTON_MASK;
  
  for(unsigned int i = 0; i < times; ++i)
  {

    for(unsigned char j = 0; j < BUTTON_COUNT; ++j)
    {
      BUTTON_DDR &= ~(BUTTON_MASK);
      BUTTON_DDR |= masks[j];
      read_delay();
      if((BUTTON_INT_PIN & _BV(BUTTON_PINT)) == 0)
        ++but[j];
    }
  }
  
  for(unsigned char i = 0; i < BUTTON_COUNT; ++i)
  {
    if(but[i] > times/2)
      result |= _BV(i);
  }
  
  g_button = result;
  
  return result;
}

void button_init(void)
{
  BUTTON_DDR |= BUTTON_MASK;
  cbi(BUTTON_INT_DDR, _BV(BUTTON_PINT));
  sbi(BUTTON_INT_PORT, _BV(BUTTON_PINT));
  BUTTON_PORT &= ~(BUTTON_MASK);
  
  BUTTON_SET_INT_SENSE
  
  sbi(GIFR, BUTTON_INT_FLAG);
  sbi(GICR, BUTTON_INTERRUPT);
}

