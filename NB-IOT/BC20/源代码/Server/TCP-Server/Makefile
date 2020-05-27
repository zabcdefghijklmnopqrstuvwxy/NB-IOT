CC=gcc
LD=

SRCS=$(wildcard *.c)
OBJS=$(patsubst %c,%o,$(SRCS))

INCLUDE= -I .
CFLAGS= -w -g -O0

TARGET=a.out

.PHONY:all clean

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CC) -o $@ $^

%o:%c
	$(CC) -c $^ $(INCLUDE) $(CFLAGS)

clean:
	rm -fr $(OBJS) $(TARGET)

