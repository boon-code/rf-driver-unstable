

MCU = atmega16
F_OSC=8000000

AVRDUDE_PORT = /dev/ttyACM0

#SRC = $(wildcard *.c)
SRC = src/rfm12.c \
      src/spi.c \
      test/uart.c \
      test/main.c

INCLUDES = -I"./src" -I"./test"
TARGET = main

# List Assembler source files here.
ASRC = 
