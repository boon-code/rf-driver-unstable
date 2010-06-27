#include "timer0.h"
#include <avr/io.h>

#define CS_SETTINGS (_BV(CS02) | _BV(CS00))
#define CS_MASK 0x7
#define RFM12_OCR 39

void timer0_init(void)
{
	// WGM: CTC, COM: Normal, 
	TCCR0 = _BV(WGM01);
	OCR0 = RFM12_OCR;
	TCNT0 = 0;
	TIMSK |= _BV(TOIE0);
}

void timer0_stop(void)
{
	TCCR0 &= ~(CS_MASK);
}

void timer0_restart(void)
{
	TCNT0 = 0;
	TCCR0 |= CS_SETTINGS;
}
