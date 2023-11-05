TARGET=a
CC=gcc
CSTD=c99

IDIR=include
ODIR=obj
SDIR=src

CFLAGS=-I$(IDIR) -Wall -Wextra -Werror -pedantic -g -std=$(CSTD)

.PHONY: default all clean

default: $(TARGET)
all: default

OBJECTS=$(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(wildcard $(SDIR)/*.c))
HEADERS=$(wildcard $(IDIR)/*.h)

$(ODIR)/%.o: $(SDIR)/%.c $(HEADERS)
	@mkdir -p $(ODIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(CFLAGS) -o $@

clean:
	-rm -f $(ODIR)/*.o
	-rm -f $(TARGET)