/*
* TDQC5
* Bruce Cain
*
* Encode Zerg header
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Packets.h"
#pragma pack(1)


int main(int argc, char *argv[])
{
    FILE *pFile, *pOutFile;
    ZergHeader *zergHeader = calloc(sizeof(ZergHeader), 1);
    char fields[1][16];
    uint32_t bigE32;
    uint16_t bigE16;
    int version, type, totalLength;

    if(argc != 2)
    {
        printf("To many or to little arguments.\n");
        exit(2);
    }

    pFile = fopen(argv[1], "r");

    if(pFile == NULL)
    {
        printf("Failed to open file.\n");
        exit(3);
    }

    /* Move one line down */
    fseek(pFile, 32, SEEK_CUR);

    fscanf(pFile, "%15s %d", fields[0], &version);
    zergHeader->version = 0x1;

    if(strcmp(fields[0], "Version:") && zergHeader->version != version)
    {
        printf("Invalid Version.\n");
        exit(3);
    }

    pOutFile = fopen("test.pcap", "wb+");
    
    fscanf(pFile, "%15s %u", fields[0], &bigE32);
    zergHeader->seqID = htobe32(bigE32);
    fscanf(pFile, "%15s %hu", fields[0], &bigE16);
    zergHeader->srcID = htobe16(bigE16);
    fscanf(pFile, "%15s %hu", fields[0], &bigE16);
    zergHeader->dstID = htobe16(bigE16);
    fscanf(pFile, "%15s %d", fields[0], &type);
    zergHeader->type = type;
    fscanf(pFile, "%15s %d", fields[0], &totalLength);
    zergHeader->totalLength = htobe24(totalLength);

   


    /* Write headers before Zerg header */
    fprintPcapFileHeader(pOutFile);
    fprintPcapPacketHeader(zergHeader->totalLength, pOutFile);
    fprintEthernetHeader(pOutFile);
    fprintIpHeader(zergHeader->totalLength, pOutFile);
    fprintUdpHeader(zergHeader->totalLength, pOutFile);
    fwrite(zergHeader, sizeof(ZergHeader), 1, pOutFile);
    fprintZergHeader(zergHeader, pFile, pOutFile); 

    fclose(pFile);
    fclose(pOutFile);

    return 0;
}
