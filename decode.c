/*
* TDQC5
* Bruce Cain
*
* Decode Psychic CAPture and print in human readable format.
*/

#include <stdio.h>
#include <stdlib.h>
#include "Packets.h"

#define PCAPPACKETHEADSTART 24

int main(int argc, char *argv[])
{
    FILE *pFile = NULL;
    ZergHeader *zergHeader;
    PcapPacketHeader *packetHeader;

    if(argc != 2)
    {
        printf("To many arguments.\n");
        exit(1);
    }

    pFile = fopen(argv[1], "r");

    if(pFile == NULL)
    {
        printf("Couldn't open file %s\n", argv[1]);
        exit(1);
    }

    fseek(pFile, PCAPPACKETHEADSTART, SEEK_SET);
    packetHeader = calloc(sizeof(PcapPacketHeader), 1);
    fread(packetHeader, sizeof(PcapPacketHeader), 1, pFile);

    zergHeader = calloc(sizeof(ZergHeader), 1);

    do
    {
        /* Parse the Zerg header */
        parseZergHeader(zergHeader, pFile);

    /* Check for end of file */
    } while(fread(packetHeader, sizeof(PcapPacketHeader), 1, pFile) == 1);

    fclose(pFile);
    free(packetHeader);
    free(zergHeader);

    return 0;
}

