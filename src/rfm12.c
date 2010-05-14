#include "rfm12.h"

#define F_CPU F_OSC
#include <util/delay.h>

#ifdef SPI_HARDWARE
  #include "spi.h"
  
  #ifdef SPI_USE_CS
    #define rfm_bus_select()    
    #define rfm_bus_release()   
  #else
    #define rfm_bus_select() (RF_PORT &= ~_BV(RF_CS))
    #define rfm_bus_release() (RF_PORT |= _BV(RF_CS))
  #endif
#else
  #define rfm_bus_select() (RF_PORT &= ~_BV(RF_CS))
  #define rfm_bus_release() (RF_PORT |= _BV(RF_CS))
#endif

#ifdef RF_RESET
  #define _BV_RESET_PIN _BV(RF_RESET)
#else
  #define _BV_RESET_PIN 0x00
#endif

unsigned short rfm_command(unsigned short cmd)
{
  unsigned short ret = 0;

  rfm_bus_select();
  
#ifdef SPI_HARDWARE

  ret = spi_wtransfer(cmd);
  
#else
  
  _delay_us(1);
  for(unsigned char i = 0; i < 16; ++i)
	{
	  if (cmd & _BV(15))
	  {
	    // sbi(RF_PORT, RF_MOSI);
	    RF_PORT |= _BV(RF_MOSI);			
		}
		else
		{
			// cbi(RF_PORT, RF_MOSI);
			RF_PORT &= ~_BV(RF_MOSI);
		}
		
		ret <<= 1;
		
		if(RF_PIN & _BV(RF_MISO))
		{
			ret |= 1;
		}
		
		RF_PORT |= _BV(RF_SCK);
		
		cmd <<= 1;
		_delay_us(1);
		
		RF_PORT &= ~_BV(RF_SCK);
	}
  
#endif
  
  rfm_bus_release();
  
  return ret;
}

void rfm_init(void)
{
#ifdef SPI_HARDWARE
  #ifndef SPI_USE_CS
  
  RF_DDR |= (_BV(RF_CS) | _BV_RESET_PIN);
  RF_PORT |= _BV(RF_CS);
  
  #else
    #ifdef RF_RESET
    
  RF_DDR |= _BV_RESET_PIN;
  
    #endif
  #endif
  spi_init_master();
  
#else
  RF_DDR |= (_BV(RF_MOSI) | _BV(RF_SCK) | _BV(RF_CS) | _BV_RESET_PIN);
  RF_DDR &= ~(_BV(RF_MISO));
	RF_PORT |= _BV(RF_CS);
#endif

#ifdef RF_RESET
  asm volatile("nop");
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
	rfm_command(0x8209);    // !receiver, !basebandblock, !transmitter, !synthesizer, crystal, 
	                        // !wakeup-timer, !low-duty-cycle, !clk-output
	rfm_command(0xC4B7);    // keep offset when VDI hi, autotuning: +7,5kHz to -10kHz
	
}

void rfm_real_set_frequency(unsigned short param)
{
  if(param < RF_FREQUENCY_MIN)
  {
		param = RF_FREQUENCY_MIN;
	}
	else if(param > RF_FREQUENCY_MAX)
	{
		param = RF_FREQUENCY_MAX;
	}
	
	rfm_command(0xA000 | param);
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

#ifndef SPI_HARDWARE
void rfm_real_ready(void)
{
  RF_PORT &= ~_BV(RF_MOSI);

  asm volatile("nop");
  
  rfm_bus_select();

  asm volatile("nop");
  
	while (!(RF_PIN & _BV(RF_MISO)));
}

#else

void rfm_real_ready(void)
{
  unsigned short val = 0;
  
  while(!(val & 0x8000))
  {
  
    rfm_bus_select();
  
    val = spi_wtransfer(0x0000);
  
    rfm_bus_release();
  }
}

#endif

void rfm_simple_tx(unsigned char *data, unsigned char number)
{	
  unsigned char i;
  rfm_command(0xCA81);
	rfm_command(0x8238);			// TX on
	
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

// debug
#include "uart.h"

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

