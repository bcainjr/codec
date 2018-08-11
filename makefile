CC = gcc
CFLAGS = $(CF) -Wall -Wextra -Wpedantic -Wwrite-strings -Wstack-usage=1024 -Wfloat-equal -Waggregate-return -Winline
DEPSD = DecodeFunc.h Packets.h
DEPSE = EncodeFunc.h Packets.h
SRCE = encode.c EncodeFunc.c Packets.c
SRCD = decode.c DecodeFunc.c Packets.c

all: Packets.c decode.c encode.c Packets.h
	$(CC) $(CFLAGS) -o decode $(SRCD)
	$(CC) $(CFLAGS) -o encode $(SRCE)

decode: $(SRCD) $(DEPSD)
	$(CC) $(CFLAGS) -o decode $(SRCD)

encode: $(SRCE) $(DEPSE)
	$(CC) $(CFLAGS) -o encode $(SRCE)

debug: Packets.c decode.c encode.c
	@$(CC) $(CFLAGS) -o decode $(SRCD)
	@$(CC) $(CFLAGS) -o encode $(SRCE)

clean:
	-@rm -rf *.o
	-@rm -rf encode decode

