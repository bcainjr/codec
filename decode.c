/*
* TDQC5
* Bruce Cain
*
* Decode Psychic CAPture and print in human readable format.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <endian.h>
#include "packets.h"

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

    /* Parse the Zerg header */
    zergHeader = calloc(sizeof(ZergHeader), 1);
    parseZergHeader(zergHeader, pFile);

    /* Check for end of file */
    packetHeader = calloc(sizeof(PcapPacketHeader), 1);
    if(fread(packetHeader, sizeof(PcapPacketHeader), 1, pFile) != 
            sizeof(PcapPacketHeader))
    {
        printf("No more packets\n");
    }

    fclose(pFile);
    free(packetHeader);
    free(zergHeader);

    return 0;
}

