#include "spi.h"
#include <avr/io.h>

#ifdef SPI_USE_CS
  #define spi_chip_select()   (SPI_PORT &= ~_BV(SPI_CS))
  #define spi_chip_release()  (SPI_PORT |= _BV(SPI_CS))
#else
  #define spi_chip_select()
  #define spi_chip_release()
#endif

void spi_init_master(void)
{
	unsigned char tmp = 0;
	
	// MOSI, SCK and CS (always output) are outputs, MISO is input others remain unchanged
  SPI_DDR |= (_BV(SPI_MOSI) | _BV(SPI_SCK) | _BV(SPI_CS));
  SPI_DDR &= ~(_BV(SPI_MISO));
  
  spi_chip_release();
  
  // enable SPI, master, clock rate F_OSC/128, 
  // CPOL = 0 CPHA = 0
  SPCR = _BV(SPE) | _BV(MSTR);
  
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

