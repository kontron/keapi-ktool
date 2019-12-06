TOPDIR := $(shell pwd)

CC = gcc
CFLAGS ?= -Wall -Wextra -pedantic -g -O0
LDFLAGS ?= -lkeapi

PKG_CONFIG ?= pkg-config
INSTALL ?= install

# install directories
PREFIX ?= /usr
SBINDIR ?= $(PREFIX)/sbin

cflags_for_lib = $(shell $(PKG_CONFIG) --cflags $(1))
ldflags_for_lib = $(shell $(PKG_CONFIG) --libs $(1))

KEAPI_CFLAGS += -I$(TOPDIR)/include
KEAPI_CFLAGS += $(call cflags_for_lib,keapi)

KEAPI_LIBS += $(call ldflags_for_lib,keapi)

OUT_DIR = bin

SRCS = src/keapi-battery.c src/keapi-cpu.c src/keapi-eeprom.c \
	src/keapi-gpio.c src/keapi-pcidev.c src/keapi-voltage.c \
	src/keapi-disk.c src/keapi-fan.c src/keapi-i2c.c \
	src/keapi-memory.c src/keapi-smbus.c src/keapi-watchdog.c \
	src/keapi-display.c src/keapi-general.c src/keapi-netdev.c \
	src/keapi-temp.c src/keapi-spi.c src/keapi-perf.c src/main.c

OBJECTS=$(SRCS:.c=.o)

DEPS := $(shell find $(TOPDIR) -name '*.d')

all: $(OUT_DIR)/ktool

src/%.o: src/%.c
	$(CC) -MD -MT $@ -MF $(@:.o=.d) $(CFLAGS) $(KEAPI_CFLAGS) -c -o $@ $<

$(OUT_DIR)/ktool: $(OBJECTS)
	mkdir -p $(OUT_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@ $(KEAPI_LIBS)

install: $(OUT_DIR)/ktool
	$(INSTALL) -d -m 0755 $(DESTDIR)$(SBINDIR)
	$(INSTALL) -m 0755 $(OUT_DIR)/ktool $(DESTDIR)/$(SBINDIR)

.PHONY: clean
clean:
	rm -rf $(OUT_DIR)/* $(SRCS:.c=.o)
	rm -f $(DEPS)

-include $(DEPS)
