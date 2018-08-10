#ifndef PACKETS_H
#define PACKETS_H

#include <stdint.h>

/*
 * A union to convert a int to a float or vice versa
 * bit for bit.
 */
typedef union SinglePrecisionFloat
{
    uint32_t hex;
    float typeFloat;
}__attribute__((packed)) SinglePrecisionFloat;

/*
 * A union to conver a int to a double or vice versa
 * bit for bit.
 */
typedef union DoublePrecisionFloat
{
    uint64_t hex;
    double typeDouble;
}__attribute__((packed)) DoublePrecisionFloat;

/*
 * ZergHeader contains all fields for the main zerg header.
 */
typedef struct ZergHeader
{
    uint8_t type: 4;
    uint8_t version: 4;
    uint32_t totalLength: 24;
    uint16_t srcID;
    uint16_t  dstID;
    uint32_t seqID;
}__attribute__((packed)) ZergHeader;

/*
 * ZergStatus contains all fields for the zerg status payload.
 */
typedef struct ZergStatus
{
    uint32_t hitPoints: 24;
    uint8_t armor;
    uint32_t maxHitPoints: 24;
    uint8_t type;
    uint32_t speed;
}__attribute__((packed)) ZergStatus;

/*
 * ZergCommand conatains the initial command type.
 */
typedef struct ZergCommand
{
    uint16_t command;

}__attribute__((packed)) ZergCommand;

/*
 * ZergCommandParameters contains the two parameters in the
 * command payload.
 */
typedef struct ZergCommandParameters
{
    uint16_t parameterOne;
    uint32_t parameterTwo;
}__attribute__((packed)) ZergCommandParameters;

/*
 * ZergGPS contains the fields in the zerg gps payload.
 */
typedef struct ZergGPS
{
    uint64_t longitude;
    uint64_t latitude;
    uint32_t altitude;
    uint32_t bearing;
    uint32_t speed;
    uint32_t accuracy;
}__attribute__((packed)) ZergGPS;

/*
 * PcapFileHeader contains all pcap file header fields.
 */
typedef struct PcapFileHeader
{
    uint32_t fileTypeID;
    uint16_t majorVersion;
    uint16_t minorVersion;
    uint32_t gmtOffset;
    uint32_t accuracyDelta;
    uint32_t maxLengthOfCapture;
    uint32_t linkLayerType;
}__attribute__((packed)) PcapFileHeader;

/*
 * PcapPacketHeader contains all pcap packet header fields.
 */
typedef struct PcapPacketHeader
{
    uint32_t uinxEpoch;
    uint32_t microSecEpoch;
    uint32_t lengthOfDataCaptured;
    uint32_t untruncatedPacketLength;
}__attribute__((packed)) PcapPacketHeader;

/*
 * Ethernet header contains all ethernet header fields.
 */
typedef struct EthernetHeader
{
    uint32_t macDestinationFirst;
    uint16_t macDestinationSec;
    uint16_t macSourceFirst;
    uint32_t macSourceSec;
    uint16_t ethernetType;
}__attribute__((packed)) EthernetHeader;

/*
 * IpHeader contains all ipv4 header fields
 */
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
}__attribute__((packed)) IpHeader; 

/*
 * UdpHeader contains all udp header fields.
 */
typedef struct UdpHeader
{
    uint16_t srcPort;
    uint16_t dstPort;
    uint16_t length;
    uint16_t checksum;
}__attribute__((packed)) UdpHeader;

/*
 * htobe24 will flip the first 24 bits of num.
 */
uint32_t htobe24(uint32_t num);

#endif
