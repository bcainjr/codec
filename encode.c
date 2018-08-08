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

#define ZERGHEADERSIZE 12

int main(int argc, char *argv[])
{
    FILE *pFile, *pOutFile;
    ZergHeader *zergHeader;
    char fields[5][16] = {0}, header[33] = {0};
    uint32_t seqID = 0;
    uint16_t srcID = 0, dstID = 0;
    unsigned int version = 0, type = 0, totalLength = 0;

    if(argc != 3)
    {
        printf("To many or to little arguments.\n");
        exit(2);
    }

    pFile = fopen(argv[1], "r");
    if(pFile == NULL)
    {
        printf("Failed to open file %s.\n", argv[1]);
        exit(3);
    }

    pOutFile = fopen(argv[2], "wb+");
    if(pOutFile == NULL)
    {
        printf("Failed to open %s.\n", argv[2]);
        exit(3);
    }

    fprintPcapFileHeader(pOutFile);

    /* Move pass header banner */
    while(fread(header, sizeof(char), 32, pFile) == 32)
    { 
        zergHeader= calloc(sizeof(ZergHeader), 1);
        fscanf(pFile, "%15s %u", fields[0], &version);
        zergHeader->version = 0x1;

        if(strcmp(fields[0], "Version:") && zergHeader->version != version)
        {
            printf("Invalid Version.\n");
            free(zergHeader);
            exit(3);
        }

        /* Read in main Zerg header */   
        fscanf(pFile, "%15s %u", fields[0], &seqID);
        zergHeader->seqID = htobe32(seqID);

        fscanf(pFile, "%15s %hu", fields[1], &srcID);
        zergHeader->srcID = htobe16(srcID);

        fscanf(pFile, "%15s %hu", fields[2], &dstID);
        zergHeader->dstID = htobe16(dstID);

        fscanf(pFile, "%15s %u", fields[3], &type);
        zergHeader->type = type;

        fscanf(pFile, "%15s %u", fields[4], &totalLength);
        zergHeader->totalLength = htobe24(ZERGHEADERSIZE);

        if(checkInputMain(fields))
        {
            printf("Invalid main format.\n");
            free(zergHeader);
            fclose(pFile);
            exit(4);
        }
     
        fprintZergHeader(zergHeader, pFile, pOutFile); 
        free(zergHeader);
    }

    fclose(pFile);
    fclose(pOutFile);

    return 0;
}
