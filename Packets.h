#ifndef PACKETS_H
#define PACKETS_H

#include <stdint.h>

typedef struct ZergHeader
{
    uint8_t type: 4;
    uint8_t version: 4;
    uint32_t totalLength: 24;
    uint16_t srcID;
    uint16_t  dstID;
    uint32_t seqID;
} ZergHeader;

typedef struct ZergStatus
{
    uint32_t hitPoints: 24;
    uint8_t armor;
    uint32_t maxHitPoints: 24;
    uint8_t type;
    uint32_t speed;
} ZergStatus;

typedef struct ZergCommand
{
    uint16_t command;

} ZergCommand;

typedef struct ZergCommandParameters
{
    uint16_t parameterOne;
    uint32_t parameterTwo;
} ZergCommandParameters;

typedef struct ZergGPS
{
    uint64_t longitude;
    uint64_t latitude;
    uint32_t altitude;
    uint32_t bearing;
    uint32_t speed;
    uint32_t accuracy;
} ZergGPS;

typedef struct PcapPacketHeader
{
    uint32_t uinxEpoch;
    uint32_t microSecEpoch;
    uint32_t lengthOfDataCaptured;
    uint32_t untruncatedPacketLength;
} PcapPacketHeader;

typedef struct EthernetHeader
{
    uint32_t macDestinationFirst;
    uint16_t macDestinationSec;
    uint16_t macSourceFirst;
    uint32_t macSourceSec;
    uint8_t ethernetType;
} EthernetHeader;

typedef struct IpHeader
{
    uint8_t IHL: 4;
    uint8_t version: 4;
    uint8_t ECN: 2;
    uint8_t DSCP: 6;
    uint16_t totalLength;
    uint16_t ID;
    uint16_t fragmentOffset: 13;
    uint16_t flags: 3;
    uint8_t TTL;
    uint8_t protocol;
    uint16_t checksum;
    uint32_t srcIP;
    uint32_t dstIP;
} IpHeader; 

uint32_t htobe24(uint32_t num);
void parseZergHeader(ZergHeader *zergHeader, FILE *pFile);

#endif
