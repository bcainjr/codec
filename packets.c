/*
* TDQC5
* Bruce Cain
*
* Source code for packets.h
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "packets.h"

void printHex(char *pcap, int start, int end, char endian)
{
    int size = (end - start) * 2;
    char *byte, *pbyte;


    byte = calloc(size, sizeof(char));

    if(byte == NULL)
    {
        printf("In printHex byte failed to allocate.\n");
        exit(2);
    }

    pbyte = calloc(2, sizeof(char));

    if(pbyte == NULL)
    {
        printf("In printHex pbyte failed to allocate.\n");
        exit(2);
    }

    if(endian)
    {   
        for(int i = (end - 1); i >= start; i--)
        {
            sprintf(pbyte, "%02hhx", pcap[i]);
            strcat(byte, pbyte);
        }
    }
    else
    {
        for(int i = start; i < end; i++)
        {
            sprintf(pbyte, "%02hhx", pcap[i]);
            strcat(byte, pbyte);
        }
    }


    printf("0x%s", byte);

    free(pbyte);
    free(byte);
}

int hexToInt(char *pcap, int start, int end, char endian)
{
    int iByte = 0, size = (end - start) * 2;
    char *byte, *pbyte;


    byte = calloc(size, sizeof(char));
    pbyte = calloc(2, sizeof(char));

    if(endian)
    {   
        for(int i = end; i > start; i--)
        {
            sprintf(pbyte, "%02hhx", pcap[i]);
            strcat(byte, pbyte);
        }
    }
    else
    {
        for(int i = start; i < end; i++)
        {
            sprintf(pbyte, "%02hhx", pcap[i]);
            strcat(byte, pbyte);
        }
    }

    iByte = (int) strtol(byte, NULL, 16);
    free(pbyte);
    free(byte);
    
    return iByte;
}
