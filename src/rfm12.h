#ifndef _RFM_12_H_
#define _RFM_12_H_

#include <avr/io.h>

// user options...
#include "config.h"

#ifdef RF_FREQUENCY_433
  #define RF_FREQUENCY_MAX 3903
  #define RF_FREQUENCY_MIN 96
  #define FREQUENCY_TO_USHORT(f) ((unsigned short)((f-430.0)/0.0025))
#endif

unsigned short rfm_command(unsigned short cmd);

void rfm_init(void);
#define rfm_frequency(f) (rfm_real_set_frequency(FREQUENCY_TO_USHORT(f)))
void rfm_bandwidth(unsigned char bandwidth, unsigned char gain, unsigned char drssi);
unsigned char rfm_baudrate(unsigned short baud);
void rfm_power(unsigned char power, unsigned char mod);

void rfm_simple_tx(unsigned char *data, unsigned char number);
void rfm_simple_rx(unsigned char *data, unsigned char number);


// don't use directly unless you can't use rfm_frequency makro
void rfm_real_set_frequency(unsigned short param);


#endif
