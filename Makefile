CC ?= gcc
CFLAGS = -Os -Wall -Wextra -s -fno-asynchronous-unwind-tables -fno-ident
LDLIBS = -lX11

TARGET = hmwm
SRC = hmwm.c

all: $(TARGET)

$(TARGET): $(SRC) config.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) /usr/local/bin/$(TARGET)

.PHONY: all clean install
