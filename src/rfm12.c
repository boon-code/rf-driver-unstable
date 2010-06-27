#include "rfm12.h"

#define F_CPU F_OSC
#include <util/delay.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "timer0.h"
#include "hope_rf.h"

#define RFM12_FREQUENCY_MAX 3903
#define RFM12_FREQUENCY_MIN 96

/*
#ifdef RF_RESET
  #define _BV_RESET_PIN _BV(RF_RESET)
#else
  #define _BV_RESET_PIN 0x00
#endif
*/

#define ST_IDLE 0
#define ST_TX 1
#define ST_RX 2

unsigned char rfm_arq_count = 0;
unsigned char* rfm_packet;
unsigned char rfm_count = 0;
static volatile unsigned char g_status = ST_IDLE;

/*
ISR(INT2_vect)
{
	switch(g_status)
	{
		case ST_IDLE:
			break;
		
		case ST_TX:
			if(spi_wtransfer(0x0000) & 0x8000)
			{
				spi_wtransfer(HFC_TX | rfm_packet[rfm_count++]);
			}
			
			if(rfm_count >= 38)
			{
				rfm_command(0x8208);			// TX off
				g_status = ST_IDLE;
			}
				
			break;
		
		case ST_RX:
			if(spi_wtransfer(0x0000) & 0x8000)
			{
				rfm_packet[rfm_count++] = spi_wtransfer(0xB000);
			}
			
			if(rfm_count >= 32)
			{
				rfm_command(0x8208);			// RX off
				g_status = ST_IDLE;
			}
			
			break;
	}
}
*/

void rfm_dotx(unsigned char* pack)
{
	while(g_status != ST_IDLE);
	rfm_packet = pack;
	rfm_count = 1;
	g_status = ST_TX;
	rfm_command(0x8238);
	//rfm_command(0xB8AA);
	
}

ISR(TIMER0_COMP_vect)
{
	++rfm_arq_count;
	rfm_dotx(rfm_packet);
	timer0_stop();
	timer0_restart();
}

unsigned short rfm_command(unsigned short cmd)
{
  return spi_wtransfer(cmd);
}

void rfm_init(void)
{
#ifdef RFM12_RESET
  RFM12_DDR |= _BV(RFM12_RESET_P);
#endif
  spi_init_master();

#ifdef RFM12_RESET
  RF_PORT &= ~ _BV_RESET_PIN;
  _delay_ms(1);
  RF_PORT |= _BV_RESET_PIN;
#endif

  // Power On Reset..
  for (unsigned char i = 0; i < 150; ++i)
		_delay_ms(10);

	rfm_command(0xC0E0);	  // clk: 10MHz, low voltage = 2.2V
	rfm_command(0x80D7);	  // enable rx fifo, tx buffer, 433MHz, crystal cap: 12pF
	rfm_command(0xC2AB);	  // internal (digital) filter, autolock, !fastmode, DQD: 3
	rfm_command(0xCA81);	  // fifo int level: 8bit, !fifo fill, !sensitiv reset
	rfm_command(0xE000);	  // wakeup time: 0 (disabled)
	rfm_command(0xC800);		// low duty cycle: 0 (disabled)
	rfm_command(HFC_POWER | HF_ENABLE_CRYSTAL | HF_DISABLE_CLKOUT);
	//rfm_command(0x8209);    // !receiver, !basebandblock, !transmitter, !synthesizer, crystal, 
	                        // !wakeup-timer, !low-duty-cycle, !clk-output
	rfm_command(0xC4B7);    // keep offset when VDI hi, autotuning: +7,5kHz to -10kHz
	timer0_init();
	
	MCUCSR &= ~(_BV(ISC2));
  GICR |= _BV(INT2);
}

void rfm_real_set_frequency(unsigned short param)
{
  if(param < RFM12_FREQUENCY_MIN)
  {
		param = RFM12_FREQUENCY_MIN;
	}
	else if(param > RFM12_FREQUENCY_MAX)
	{
		param = RFM12_FREQUENCY_MAX;
	}
	
	rfm_command(HFC_FREQUENCY | param);
}

void rfm_bandwidth(unsigned char bandwidth, unsigned char gain, unsigned char drssi)
{
	rfm_command(0x9400 | ((bandwidth & 7) << 5) |((gain & 3) << 3) | (drssi & 7));
}

unsigned char rfm_baudrate(unsigned short baud)
{
	if (baud < 663)
		return 0;
	
	if (baud < 5400)					// Baudrate= 344827,58621/(R+1)/(1+CS*7)
		rfm_command(0xC680 | ((43104 / baud) - 1));
	else
		rfm_command(0xC600 | ((344828UL / baud) - 1));
	return 1;
}

void rfm_power(unsigned char power, unsigned char mod)
{	
	rfm_command(0x9800 | (power & 7) | ((mod & 15) << 4));
}

void rfm_real_ready(void)
{
  unsigned short val = 0;
  
  while(!(val & 0x8000))
  {
    val = spi_wtransfer(0x0000);
  }
}

void rfm_simple_tx(unsigned char *data, unsigned char number)
{	
  unsigned char i;
  //rfm_command(0xCA81);
	//rfm_command(0x8238);			// TX on
	
	_delay_us(1);
	rfm_real_ready();
	rfm_command(0xB8AA);
	rfm_real_ready();
	rfm_command(0xB8AA);
	rfm_real_ready();
	rfm_command(0xB8AA);
	rfm_real_ready();
	rfm_command(0xB82D);
	rfm_real_ready();
	rfm_command(0xB8D4);
	
	for(i = 0; i < number; ++i)
	{
	  rfm_real_ready();
		rfm_command(0xB800 | (*data++));
	}
	rfm_real_ready();
	rfm_command(0x8208);			// TX off
}

void rfm_simple_rx(unsigned char *data, unsigned char number)
{	
  unsigned char i;
  //rfm_command(0xCA81);			// set FIFO mode
	//rfm_command(0x82C8);			// RX on
	//rfm_command(0xCA83);			// enable FIFO
	
	for (i = 0; i < number; i++)
	{
	  rfm_real_ready();
		*data++=rfm_command(0xB000);
	}
	rfm_command(0x8208);			// RX off
}

