#ifndef _SPI_MODULE_H_
#define _SPI_MODULE_H_

#include "config.h"

#define SPI_DDR DDRB

#define SPI_PORT	PORTB
#define SPI_PIN   PINB
#define SPI_CS		PB4
#define SPI_MOSI	PB5
#define SPI_MISO	PB6
#define SPI_SCK	  PB7


void spi_init_master(void);
unsigned char spi_btransfer(const unsigned char data);
unsigned short spi_wtransfer(const unsigned short data);

#endif
