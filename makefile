CC = gcc
CFLAGS = $(CF) -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline
DEPS = packets.h
SRCE = encode.c packets.c
SRCD = decode.c packets.c

all: packets.c decode.c encode.c packets.h
	$(CC) $(CFLAGS) -o decode $(SRCD)
	$(CC) $(CFLAGS) -o encode $(SRCE)

decode: packets.c decode.c packets.h
	$(CC) $(CFLAGS) -o decode $(SRCD)

encode: packets.c encode.c packets.h
	$(CC) $(CFLAGS) -o decode $(SRCE)

debug:packets.c decode.c
	$(CC) $(CFLAGS) -o decode $(SRCD)
#	$(CC) $(CFLAGS) -o encode $(SRCE)

clean:
	-@rm -rf *.o
	-@rm -rf *.c

