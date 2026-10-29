CC ?= gcc
CFLAGS = -Os -Wall -Wextra -s -fno-asynchronous-unwind-tables -fno-ident -I/usr/local/include
LDLIBS = -L/usr/local/lib -lX11

# BSD Users,uncomment these lines
# CFLAGS = -Os -Wall -Wextra -s -fno-asynchronous-unwind-tables -fno-ident -I/usr/local/include
# LDLIBS = -L /usr/local/lib -lX11

TARGET = bare
# BSD Users will have to change it to barex.
# TARGET = barex
SRC = bare.c

all: $(TARGET)

$(TARGET): $(SRC) config.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LDLIBS)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	install -Dm755 $(TARGET) /usr/local/bin/$(TARGET)

.PHONY: all clean install
