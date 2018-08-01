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

#define IPHEADERSTART 54
#define ZERGHEADERSTART 82

typedef union SinglePrecisionFloat
{
    uint32_t hex;
    float typeFloat;
} SinglePrecisionFloat;

typedef union DoublePrecisionFloat
{
    uint64_t hex;
    double typeDouble;
} DoublePrecisionFloat;

uint32_t htobe24(uint32_t num)
{
    uint32_t byte1, byte2, byte3, retVal;

    byte1 = (num & 0x0000ff) << 16;
    byte2 = num & 0x00ff00;
    byte3 = (num & 0xff0000) >> 16;

    retVal = byte1 | byte2 | byte3;

    return retVal;
}

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

static void statusZerg(int payloadLength, FILE *pFile)
{
   ZergStatus *zergStatus = calloc(sizeof(ZergStatus), 1);
   SinglePrecisionFloat singleFloat;
   char breedID[16][10] =
   {
       "OverMind", "Larva", "Cerebrate", "Overlord", 
       "Queen", "Drone", "Zergling", "Lurker",
       "Broodling", "Hydralist", "Guardian", "Scourge",
       "Ultralist", "Mutalist", "Defiler", "Devourer"
   };

   fread(zergStatus, sizeof(ZergStatus), 1, pFile);

   printf("Name: ");
   msgPayload(payloadLength - 12, pFile);
   printf("HP: %u/%u\n", htobe24(zergStatus->hitPoints), 
           htobe24(zergStatus->maxHitPoints));
   printf("Type: %s\n", breedID[zergStatus->type - 1]);
   printf("Armor: %u\n", zergStatus->armor);

   singleFloat.hex = htobe32(zergStatus->speed);
   printf("MaxSpeed: %f m/s\n", singleFloat.typeFloat);

   free(zergStatus);
}

static void cmdInstruction(FILE *pFile)
{
    ZergCommand *commandPayload = calloc(sizeof(ZergCommand), 1);
    SinglePrecisionFloat singleFloat;
    char command[8][11] = 
    {
        "GET STATUS", "GOTO", "GET GPS", "RESERVED",
        "RETURN", "SET GROUP", "STOP", "REPEAT"
    };

    fread(commandPayload, sizeof(ZergCommand), 1, pFile);

    if(htobe16(commandPayload->command) > 7)
    {
        printf("Invalid command.\n");
        free(commandPayload);
        exit(1);
    }

    printf("%s\n", command[htobe16(commandPayload->command)]);
    singleFloat.hex = htobe32(commandPayload->parameterTwo);

    if(htobe16(commandPayload->command) % 2 != 0)
    {
        switch(htobe16(commandPayload->command))
        {
            case 1:
            {
                printf("Bearing: %f\n", singleFloat.typeFloat);
                printf("Location: %u\n", htobe16(commandPayload->parameterOne));
                break;
            }
            case 5:
            {
                if(htobe16(commandPayload->parameterOne))
                {
                    printf("Remove from Group ID: ");
                }
                else
                {
                    printf("Add to Group ID: ");
                }
                        
                printf("%d\n", htobe32(commandPayload->parameterTwo));
                break;

            }
            case 7:
            {
                printf("Re-send packet: %d\n", htobe32(commandPayload->parameterTwo));
                break;
            }
            default:
            {
                printf("Invalid command.\n");
                free(commandPayload);
                exit(1);
            }
        }
    }
        
    free(commandPayload);
}

void gpsData(FILE *pFile)
{
    ZergGPS *zergGPS = calloc(sizeof(ZergGPS), 1);
    DoublePrecisionFloat longitude, latitude;
    SinglePrecisionFloat altitude, bearing, speed, accuracy;

    fread(zergGPS, sizeof(ZergGPS), 1, pFile);

    longitude.hex = htobe64(zergGPS->longitude);
    latitude.hex = htobe64(zergGPS->latitude);

    altitude.hex = htobe32(zergGPS->altitude);
    bearing.hex = htobe32(zergGPS->bearing);
    speed.hex = htobe32(zergGPS->speed);
    accuracy.hex = htobe32(zergGPS->accuracy);

    printf("Latitude: %f ", latitude.typeDouble);
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

    printf("Altitude: %f fathoms\n", altitude.typeFloat);
    printf("Bearing: %f deg.\n", bearing.typeFloat);
    printf("Speed: %fm/s\n", speed.typeFloat);
    printf("Accuracy: %fm\n", accuracy.typeFloat);

    free(zergGPS);
}

void parseZergHeader(ZergHeader *zergHeader, FILE *pFile)
{
    IpHeader *ipHeader = calloc(sizeof(IpHeader), 1);

    fseek(pFile, IPHEADERSTART, SEEK_SET);
    fread(ipHeader, sizeof(IpHeader), 1, pFile);

    fseek(pFile, (ZERGHEADERSTART + (ipHeader->IHL - 5) * 4), SEEK_SET);
    
    fread(zergHeader, sizeof(*zergHeader), 1, pFile);
    printf("Zerg---\nVersion: %u\n", zergHeader->version);
    printf("Sequence: %u\n", htobe32(zergHeader->seqID));
    printf("From: %u\n", htobe16(zergHeader->srcID));
    printf("To: %u\n", htobe16(zergHeader->dstID));

    switch(zergHeader->type)
    {
        case 0:
        {
            msgPayload(htobe24(zergHeader->totalLength) - 12, pFile);
            break;
        }
        case 1:
        {
            statusZerg(htobe24(zergHeader->totalLength) - 12, pFile);
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
            printf("Invalid type found: %u\n", zergHeader->type);
            free(zergHeader);
            free(ipHeader);
            exit(2);
        }
    }

    free(ipHeader);
    
}


