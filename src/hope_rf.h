#ifndef _HOPE_RF_H_
#define _HOPE_RF_H_

#define HFC_POWER (_BV(15) | _BV(9))

#define HF_ENABLE_RX (_BV(7))
#define HF_RX_BASEBAND (_BV(6))
#define HF_START_TX (_BV(5))
#define HF_ENABLE_SYNTH (_BV(4))
#define HF_ENABLE_CRYSTAL (_BV(3))
#define HF_ENABLE_LOWBAT (_BV(2))
#define HF_ENABLE_WAKE (_BV(1))
#define HF_DISABLE_CLKOUT (_BV(0))


#define HFC_FREQUENCY (_BV(15) | _BV(13))


#define HFC_DATA_RATE (_BV(15) | _BV(14) | _BV(10) | _BV(9))


#define HFC_FIFO 0xca00

#define HF_DISABLE_SENITIV_RESET 0x1
#define HF_FFA_SYNC (_BV(1))
#define HF_FILL_MODE_ALWAYS (_BV(2))


#define HFC_RX (_BV(15) | _BV(13) | _BV(12))


#define HFC_TX (_BV(15) | _BV(13) | _BV(12) | _BV(11))


#define HFC_STATUS 0

#endif
