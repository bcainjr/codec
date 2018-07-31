#ifndef PACKETS_H
#define PACKETS_H

typedef struct ZergHeader
{
    char version: 1;
    char type: 3;
    int totalLength: 24;
    int srcID: 16;
    int dstID: 16;
    int seqID;
    void (*paylod)(int totalLength);
} ZergHeader;

typedef struct PcapFileHeader
{
    int fileType;
    int majorVer: 16;
    int minorVer: 16;
    int gmtOffset;
    int accuracyDelta;
    int maxLengthCapture;
    int linkLayer;
} PcapFileHeader;

typedef struct IpHeader
{

} IpHeader; 

int hexToInt(char *pcap, int start, int end, char endian);
void printHex(char *pcap, int start, int end, char endian);
void msgPayload(int totalLength);
void statusZerg(int totalLength);
void cmdInstruction(int totalLength);
void gpsData(int totalLength);

#endif
