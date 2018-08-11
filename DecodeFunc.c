/*
* TDQC5
* Bruce Cain
*
* Decode functions.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Packets.h"
#include "DecodeFunc.h"

#define UDPHEADERSIZE    8

/* Exit statuses */
#define INVALIDVERSION     11
#define INVALIDTYPE        12
#define INVALIDCOMMAND     31
#define INVALIDLONGITUDE   41
#define INVALIDLATITUDE    42
#define INVALIDPROTOCOL    54
#define INVALIDIPSIZE      55
#define INVALIDMAGICNUMBER 56
#define BADPCAPHEADERSIZE  57

/**
 * @brief udpCheck checks if the next protocol is UDP
 *
 * @param ipHeader A struct that contains IP data.
 */
static void udpCheck(IpHeader *ipHeader)
{
    char flag = 0;
    if(ipHeader->version == 6)
    {
        if(ipHeader->fragmentOffset != 0x11)
        {
            flag = 1;    
        }
    }

    if(ipHeader->version == 4)
    {
        if(ipHeader->protocol != 0x11)
        {
            flag = 1;
        }
    }

    if(flag)
    {
        fprintf(stderr, "Invalid protocol \n");
        free(ipHeader);
        exit(INVALIDPROTOCOL);
    }
}

/**
 * @brief MsgPayload Prints out the end of Zerg payloads
 *
 * MsgPayload (Message Payload) will can print out either the message
 * or name in the Zerg headers.
 *
 * @param payloadLength The length of the message or name.
 *
 * @param pFile The file that is being decoded.
 */
static void msgPayload(int payloadLength, FILE *pFile)
{
    char *byte = calloc(payloadLength + 1, sizeof(char));
    fread(byte, payloadLength, sizeof(char), pFile);

    if(byte == NULL)
    {
        return;
    }

    printf("%s\n", byte);
    free(byte);
}

/**
 * @brief statusZerg Builds the status payload from the pcap.
 *
 * statusZerg parses pFile for the status payload.
 *
 * @param payloadLength The length of the name field.
 *
 * @param pFile The file that is being decoded.
 */
static void statusZerg(int payloadLength, FILE *pFile)
{
   ZergStatus *zergStatus = calloc(1, sizeof(ZergStatus));
   SinglePrecisionFloat singleFloat;
   char breedID[16][10] =
   {
       "OverMind", "Larva", "Cerebrate", "Overlord", 
       "Queen", "Drone", "Zergling", "Lurker",
       "Broodling", "Hydralist", "Guardian", "Scourge",
       "Ultralist", "Mutalist", "Defiler", "Devourer"
   };

   fread(zergStatus, sizeof(ZergStatus), 1, pFile);

   printf("Name:      ");
   msgPayload(payloadLength - 12, pFile);
   printf("HP:        %u/%u\n", htobe24(zergStatus->hitPoints), 
           htobe24(zergStatus->maxHitPoints));
   printf("Type:      %s\n", breedID[zergStatus->type - 1]);
   printf("Armor:     %u\n", zergStatus->armor);

   singleFloat.hex = htobe32(zergStatus->speed);
   printf("MaxSpeed:  %f u/s\n", singleFloat.typeFloat);

   free(zergStatus);
}

/**
 * @brief cmdInstruction Builds the command payload.
 *
 * cmdInstruction (Command Instruction) takes in pFile and will parse
 * out the command instructions.
 *
 * @param pFile The pcap file that is being decoded.
 */
static void cmdInstruction(FILE *pFile)
{
    ZergCommand *commandPayload = calloc(1, sizeof(ZergCommand));
    ZergCommandParameters *commandParameters;
    SinglePrecisionFloat singleFloat;
    char command[8][11] = 
    {
        "GET STATUS", "GOTO", "GET GPS", "RESERVED",
        "RETURN", "SET GROUP", "STOP", "REPEAT"
    };

    fread(commandPayload, sizeof(ZergCommand), 1, pFile);

    if(htobe16(commandPayload->command) > 7)
    {
        fprintf(stderr, "Invalid command.\n");
        free(commandPayload);
        fclose(pFile);
        exit(INVALIDCOMMAND);
    }

    printf("%s\n", command[htobe16(commandPayload->command)]);

    if(htobe16(commandPayload->command) % 2 == 0)
    {
        /* Account for padding */
        fseek(pFile, 4, SEEK_CUR);
    }
    else
    {
        /* Read in parameters */
        commandParameters = calloc(1, sizeof(ZergCommandParameters));
        fread(&commandParameters->parameterOne,
                sizeof(commandParameters->parameterOne), 1, pFile);
        fread(&commandParameters->parameterTwo,
                sizeof(commandParameters->parameterTwo), 1, pFile);

        singleFloat.hex = htobe32(commandParameters->parameterTwo);
        switch(htobe16(commandPayload->command))
        {
            case 1:
            {
                printf("Bearing:  %.4f deg.\n", singleFloat.typeFloat);
                printf("Location: %u m\n", 
                        htobe16(commandParameters->parameterOne));
                break;
            }
            case 5:
            {
                if(!htobe16(commandParameters->parameterOne))
                {
                    printf("Remove from Group ID: ");
                }
                else
                {
                    printf("Add to Group ID: ");
                }
                        
                printf("%d\n", htobe32(commandParameters->parameterTwo));
                break;

            }
            case 7:
            {
                printf("Re-send packet: %u\n",
                        htobe32(commandParameters->parameterTwo));
                break;
            }
            default:
            {
                fprintf(stderr, "Invalid command.\n");
                free(commandPayload);
                free(commandParameters);
                fclose(pFile);
                exit(INVALIDCOMMAND);
            }
        }
        free(commandParameters);
    }
        
    free(commandPayload);
}

/**
 * @brief gpsData Parsing GPS header.
 *
 * gpsData begans to read in all the data form the given pcap and
 * convert all the values to its respective single and double
 * precision.
 *
 * @param pFile The pcap file that is being decoded.
 */
static void gpsData(FILE *pFile)
{
    ZergGPS *zergGPS = calloc(1, sizeof(ZergGPS));
    DoublePrecisionFloat longitude, latitude;
    SinglePrecisionFloat altitude, bearing, speed, accuracy;

    fread(zergGPS, sizeof(ZergGPS), 1, pFile);

    /* Convert hex to double and single precision floats */
    longitude.hex = htobe64(zergGPS->longitude);
    latitude.hex = htobe64(zergGPS->latitude);

    altitude.hex = htobe32(zergGPS->altitude);
    bearing.hex = htobe32(zergGPS->bearing);
    speed.hex = htobe32(zergGPS->speed);
    accuracy.hex = htobe32(zergGPS->accuracy);

    free(zergGPS);

    if(latitude.typeDouble < -90 || latitude.typeDouble > 90)
    {
        fprintf(stderr, "Invalid latitude %f\n", latitude.typeDouble);
        fclose(pFile);
        exit(INVALIDLATITUDE);
    }

    if(longitude.typeDouble < -180 || longitude.typeDouble > 180)
    {
        fprintf(stderr, "Invalid longitude %f\n", longitude.typeDouble);
        fclose(pFile);
        exit(INVALIDLONGITUDE);
    }

    printf("Latitude:  %f ", latitude.typeDouble);
    if(latitude.typeDouble < 0)
    {
        printf("deg. S\n");
    }
    else
    {
        printf("deg. N\n");
    }
    
    printf("Longitude: %f ", longitude.typeDouble);
    if(longitude.typeDouble < 0)
    {
        printf("deg. W\n");
    }
    else
    {
        printf("deg. E\n");
    }

    printf("Altitude:  %f fathoms\n", altitude.typeFloat);
    printf("Bearing:   %f deg.\n", bearing.typeFloat);
    printf("Speed:     %f m/s\n", speed.typeFloat);
    printf("Accuracy:  %f m\n", accuracy.typeFloat);
}

/**
 * @brief parseZergHeader begans parsing the initial Zerg header.
 *
 * parseZergHeader starts the initial parsing of the main Zerg header
 * thats right after the UDP header.
 *
 * @param zergHeader The struct that holds the Zerg header data.
 *
 * @param pFile The pcap file that is being decoded.
 */
void parseZergHeader(
        ZergHeader *zergHeader, 
        PcapPacketHeader *packetHeader, 
        FILE *pFile)
{
    EthernetHeader *ethHeader = calloc(1, sizeof(EthernetHeader));
    IpHeader *ipHeader;
    UdpHeader *udpHeader;
    unsigned int type = 0, zergLength = 0, ipLength = 0, 
                 packetLength = 0, ethLength = 14;

    fread(ethHeader, sizeof(EthernetHeader), 1, pFile);

    /* Check if Ethernet is 8100 */
    if(htobe16(ethHeader->ethernetType) == 0x8100)
    {
         /*
          * Move forward because of extra 4 bytes
          * for 802.1Q ethernet type.
         */
        fseek(pFile, 4, SEEK_CUR);
        ethLength += 4;
    }

    ipHeader = calloc(sizeof(IpHeader), 1);
    /* Read in IP header */
    fread(ipHeader, sizeof(IpHeader), 1, pFile);
    if(ipHeader->version == 6 && htobe16(ethHeader->ethernetType) == 0x86dd)
    {
        udpCheck(ipHeader);
        /* Move to UDP header based on IPv6 */
        fseek(pFile, 20, SEEK_CUR);
        ipLength = 40;
    }
    else if(ipHeader->version == 4 && 
            (htobe16(ethHeader->ethernetType) == 0x0800 || 
             htobe16(ethHeader->ethernetType) == 0x8100))
    {
        udpCheck(ipHeader);
        /* Move to UDP header based on IHL size */
        if(ipHeader->IHL < 5)
        {
            fprintf(stderr, "Invalid IP size.\n");
            free(ethHeader);
            free(ipHeader);
            free(packetHeader);
            free(zergHeader);
            fclose(pFile);
            exit(INVALIDIPSIZE);
        }

        ipLength = ipHeader->IHL * 4;
        fseek(pFile, (ipHeader->IHL - 5) * 4, SEEK_CUR);
    }
    else
    {
        fprintf(stderr, "Invalid IP version.\n");
        free(zergHeader);
        free(ipHeader);
        free(packetHeader);
        free(ethHeader);
        fclose(pFile);
        exit(INVALIDVERSION);
    }


    /* Read udp header */
    udpHeader = calloc(sizeof(UdpHeader), 1);
    fread(udpHeader, sizeof(UdpHeader), 1, pFile);

    /* Start parsing Zerg header */
    fread(zergHeader, sizeof(*zergHeader), 1, pFile);
    type = zergHeader->type;
    zergLength = htobe24(zergHeader->totalLength);

    if(zergLength == 12)
    {
        zergLength += 6;
    }
    else if(zergLength == 14)
    {
        zergLength += 4;
    }

    /* Verifty packet length */
    packetLength = ethLength + ipLength + zergLength + UDPHEADERSIZE; 
    if(!(packetHeader->lengthOfDataCaptured == packetLength ||
            htobe32(packetHeader->lengthOfDataCaptured) == packetLength))
    {
        printf("Invalid pcap packet header size.\n");
        printf("packet length: %d\n", packetHeader->lengthOfDataCaptured);
        printf("added length: %d\n", ethLength + ipLength + zergLength + UDPHEADERSIZE);
        free(zergHeader);
        free(packetHeader);
        free(ethHeader);
        free(udpHeader);
        free(ipHeader);
        fclose(pFile);
        exit(BADPCAPHEADERSIZE);
    }


    if(zergHeader->version != 1)
    {
        fprintf(stderr, "Invalid Zerg packet version %d\n", zergHeader->version);
        free(zergHeader);
        free(packetHeader);
        free(ethHeader);
        free(udpHeader);
        free(ipHeader);
        fclose(pFile);
        exit(INVALIDVERSION);
    }

    printf("----------Zerg Header----------\n");
    printf("Version:   %u\n", zergHeader->version);
    printf("Sequence:  %u\n", htobe32(zergHeader->seqID));
    printf("From:      %u\n", htobe16(zergHeader->srcID));
    printf("To:        %u\n", htobe16(zergHeader->dstID));
    printf("Type:      %u\n", zergHeader->type);
    printf("Length:    %u\n", htobe24(zergHeader->totalLength));

    free(packetHeader);
    free(ethHeader);
    free(ipHeader);
    free(udpHeader);
    free(zergHeader);

    switch(type)
    {
        case 0:
        {
            printf("Message:   ");
            msgPayload(zergLength - 12, pFile);
            break;
        }
        case 1:
        {
            statusZerg(zergLength - 12, pFile);
            break;
        }
        case 2:
        {
            cmdInstruction(pFile);
            break;
        }
        case 3:
        {
            gpsData(pFile);
            break;
        }
        default: 
        {
            fprintf(stderr, "Invalid type found: %u\n", zergHeader->type);
            free(zergHeader);
            fclose(pFile);
            exit(INVALIDTYPE);
        }
    }
}

/**
 * @breif checkFileHeader validates the pcap file header.
 *
 * @param inFile Is the file being encoded.
 *
 * @return IF 0 the file header is correct if 1 it is false.
 */
int checkFileHeader(FILE *inFile)
{
    PcapFileHeader *fileHeader = calloc(1, sizeof(PcapFileHeader));
    int linkLayer = 0, majorVersion = 0, minorVersion = 0;
    char endian = 0, returnValue = 0;

    fread(fileHeader, sizeof(PcapFileHeader), 1, inFile);

    if(fileHeader->fileTypeID == 0xd4c3b2a1)
    {
        endian = 1;
    }
    else if(fileHeader->fileTypeID != 0xa1b2c3d4)
    {
        fprintf(stderr, "Invalid magic number.\n");
        returnValue = 1;
    }

    if(!returnValue)
    {
        if(!endian)
        {
            linkLayer = fileHeader->linkLayerType;
            majorVersion = fileHeader->majorVersion;
            minorVersion = fileHeader->minorVersion;
        }
        else
        {
            linkLayer = htobe32(fileHeader->linkLayerType);
            majorVersion = htobe16(fileHeader->majorVersion);
            minorVersion = htobe16(fileHeader->minorVersion);

        }
    }

    if(!returnValue)
    {
        if(linkLayer != 1 || majorVersion != 2 ||
            minorVersion != 4)
        {
            returnValue = 1;
        }
    }
     
    free(fileHeader);
    return returnValue;
}
