CC = gcc
CFLAGS = -I./include -I/usr/include/keapi -Wall -Wextra -pedantic -g -O0
LDFLAGS = -lkeapi

OUT_DIR = bin
FILE_EXT =

SRCS = src/keapi-battery.c src/keapi-cpu.c src/keapi-eeprom.c \
	src/keapi-gpio.c src/keapi-pcidev.c src/keapi-voltage.c \
	src/keapi-disk.c src/keapi-fan.c src/keapi-i2c.c \
	src/keapi-memory.c src/keapi-smbus.c src/keapi-watchdog.c \
	src/keapi-display.c src/keapi-general.c src/keapi-netdev.c \
	src/keapi-temp.c src/keapi-spi.c src/keapi-perf.c src/main.c

OBJECTS=$(SRCS:.c=.o)

all: clean $(OUT_DIR) $(OBJECTS) ktool

ktool: $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) $(LDFLAGS) -o $(OUT_DIR)/$@

clean:
	rm -rf $(OUT_DIR)/* $(SRCS:.c=.o)

$(OUT_DIR):
	mkdir -p $(OUT_DIR)
