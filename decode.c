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
#include "packets.h"

int main(int argc, char *argv[])
{
    FILE *pFile = NULL;
    char byte;
    int byteCnt = 0, i = 0;
    char *pcap;

    pFile = fopen(argv[1], "r");

    if(pFile == NULL)
    {
        printf("Couldn't open file %s\n", argv[1]);
        exit(1);
    }

    while(fread(&byte, sizeof(char), 1, pFile) == 1)
    {
        byteCnt++;
    }

    pcap = calloc(byteCnt, sizeof(char *));

    fseek(pFile, 0, SEEK_SET);

    while(fread(&byte, sizeof(char), 1, pFile) == 1)
    {
        pcap[i++] = byte;
    }

    printf("Big endian read: ");
    printHex(pcap, 83, 86, 0);
    printf("\nLittle endian read: ");
    printHex(pcap, 83, 86, 1);
    printf("\n");

    printf("IHL = %d\n", hexToInt(pcap, 54, 55, 0) & 240);


    fclose(pFile);

    return 0;
}

