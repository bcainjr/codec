/*
* TDQC5
* Bruce Cain
*
* Decode Psychic CAPture and print in human readable format.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Packets.h"
#include "DecodeFunc.h"

#pragma pack(1)

/* Exit codes */
#define INVALIDARGUMENTS 2
#define FAILEDFILEOPEN   4
#define INVALIDPCAP      56

int main(int argc, char *argv[])
{
    FILE *pFile = NULL;
    ZergHeader *zergHeader;
    PcapPacketHeader *packetHeader;

    if(argc != 2)
    {
        printf("To many arguments.\n");
        exit(INVALIDARGUMENTS);
    }

    pFile = fopen(argv[1], "r");

    if(pFile == NULL)
    {
        printf("Couldn't open file %s\n", argv[1]);
        exit(FAILEDFILEOPEN);
    }

    if(checkFileHeader(pFile))
    {
        printf("Invalid pcap.\n");
        fclose(pFile);
        exit(INVALIDPCAP);
    }

    packetHeader = calloc(sizeof(PcapPacketHeader), 1);

    while(fread(packetHeader, sizeof(PcapPacketHeader), 1, pFile) == 1)
    {


        /* Parse the Zerg header */
        zergHeader = calloc(sizeof(ZergHeader), 1);
        parseZergHeader(zergHeader, packetHeader, pFile);
        packetHeader = calloc(sizeof(PcapPacketHeader), 1);
    }

    fclose(pFile);
    free(packetHeader);

    return 0;
}
