#ifndef _GLOBAL_CONFIG_
#define _GLOBAL_CONFIG_

// your options

#define SPI_HARDWARE 
#define SPI_USE_CS

// if RF_RESET is defined, it is used!!
// #define RF_RESET  PC7

#define RF_FREQUENCY_433


#define RF_PORT   PORTC
#define RF_PIN    PINC
#define RF_DDR    DDRC

#define RF_MISO   PC0
#define RF_CS     PC1
#define RF_SCK    PC2
#define RF_MOSI   PC3

/*
#define RF_PORT   PORTB
#define RF_PIN    PINB
#define RF_DDR    DDRB

#define RF_MISO   PB6
#define RF_MOSI   PB5
#define RF_CS     PB4
#define RF_SCK    PB7
*/


#endif
