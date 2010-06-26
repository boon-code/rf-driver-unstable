#include "spi.h"
#include <avr/io.h>

#define spi_chip_select()   (ISPI_PORT &= ~_BV(ISPI_CS))
#define spi_chip_release()  (ISPI_PORT |= _BV(ISPI_CS))

static ispi_callback spi_cbfunc = 0;
static volatile unsigned char spi_count = 2;
static unsigned char* spi_data = 0;

ISR(SPI_STC)
{
  if(spi_count < 2)
  {
  
  }
}

void spi_init_master(ispi_callback func)
{
	unsigned char tmp = 0;
	
	// MOSI, SCK and CS (always output) are outputs, MISO is input others remain unchanged
  ISPI_DDR |= (_BV(ISPI_MOSI) | _BV(ISPI_SCK) | _BV(ISPI_CS));
  
  ISPI_DDR &= ~(_BV(ISPI_MISO));
  
  spi_chip_release();
  
  spi_cbfunc = func;
  spi_data = buffer;
  
  // enable SPI, master, clock rate F_OSC/4, 
  // CPOL = 0 CPHA = 0
  SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPIE);
  
  // dummy read...
  tmp = SPSR;
  tmp = SPDR;
}

unsigned char spi_btransfer(const unsigned char data)
{
	spi_chip_select();
  
  //Start new transmission
  SPDR = data;
  
  // wait until finished
  while(!(SPSR & (1 << SPIF)));
  
  spi_chip_release();
  
  return SPDR;
}

unsigned short spi_wtransfer(const unsigned short data)
{
	unsigned char data_lo = (unsigned char)data;
	unsigned char data_hi = (unsigned char)(data >> 8);
	
	spi_chip_select();
	
	SPDR = data_hi;
	
	while(!(SPSR & (1 << SPIF)));
	
	data_hi = SPDR;
	
	SPDR = data_lo;
	
	while(!(SPSR & (1 << SPIF)));
	
	data_lo = SPDR;
	
	spi_chip_release();
	
	return (data_hi << 8) | (data_lo);
}

