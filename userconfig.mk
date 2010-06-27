
MCU = atmega16
F_OSC=8000000

AVRDUDE_PORT = /dev/ttyACM0

BUILD_SYSTEM = ./bs.sh

#SRC = $(wildcard *.c)
SRC = 	src/rfm12.c \
	src/spi/spi.c \
	src/timer0.c \
	test/uart.c \
	test/button/button.c \
	test/main.c
HEADER = src/rfm12.h.in \
	src/spi/spi.h.in \
	src/timer0.h.in \
	test/button/button.h.in

INCLUDES = -I"./src" -I"./test" -I"./test/button" -I"./src/spi"
TARGET = main

# List Assembler source files here.
ASRC = 
