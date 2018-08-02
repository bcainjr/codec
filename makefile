CC = gcc
CFLAGS = $(CF) -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline
DEPS = Packets.h
SRCE = encode.c Packets.c
SRCD = decode.c Packets.c

all: Packets.c decode.c encode.c Packets.h
	$(CC) $(CFLAGS) -o decode $(SRCD)
	$(CC) $(CFLAGS) -o encode $(SRCE)

decode: $(SRCD) $(DEPS)
	$(CC) $(CFLAGS) -o decode $(SRCD)

encode: $(SRCE) $(DEPS)
	$(CC) $(CFLAGS) -o decode $(SRCE)

debug: Packets.c decode.c
	$(CC) $(CFLAGS) -o decode $(SRCD)
#	$(CC) $(CFLAGS) -o encode $(SRCE)

clean:
	-@rm -rf *.o
	-@rm -rf *.c

