/*
* TDQC5
* Bruce Cain
*
* Source code for packets.h
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Packets.h"

#define UDPHEADERSIZE 8

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

   printf("Name     : ");
   msgPayload(payloadLength - 12, pFile);
   printf("HP       : %u/%u\n", htobe24(zergStatus->hitPoints), 
           htobe24(zergStatus->maxHitPoints));
   printf("Type     : %s\n", breedID[zergStatus->type - 1]);
   printf("Armor    : %u\n", zergStatus->armor);

   singleFloat.hex = htobe32(zergStatus->speed);
   printf("MaxSpeed : %f m/s\n", singleFloat.typeFloat);

   free(zergStatus);
}

static void cmdInstruction(FILE *pFile)
{
    ZergCommand *commandPayload = calloc(sizeof(ZergCommand), 1);
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
        printf("Invalid command.\n");
        free(commandPayload);
        exit(1);
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
        commandParameters = calloc(sizeof(ZergCommandParameters), 1);
        fread(commandParameters, sizeof(commandParameters), 1, pFile);

        singleFloat.hex = htobe32(commandParameters->parameterTwo);
        switch(htobe16(commandPayload->command))
        {
            case 1:
            {
                printf("Bearing: %f\n", singleFloat.typeFloat);
                printf("Location: %u\n", htobe16(commandParameters->parameterOne));
                break;
            }
            case 5:
            {
                if(htobe16(commandParameters->parameterOne))
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
                printf("Re-send packet: %u\n", htobe32(commandParameters->parameterTwo));
                break;
            }
            default:
            {
                printf("Invalid command.\n");
                free(commandPayload);
                free(commandParameters);
                exit(1);
            }
        }
        free(commandParameters);
    }
        
    free(commandPayload);
}

void gpsData(FILE *pFile)
{
    ZergGPS *zergGPS = calloc(sizeof(ZergGPS), 1);
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
        printf("Invalid latitude %f\n", latitude.typeDouble);
        exit(5);
    }

    if(longitude.typeDouble < -180 || longitude.typeDouble > 180)
    {
        printf("Invalid longitude %f\n", longitude.typeDouble);
        exit(5);
    }

    printf("Latitude : %f ", latitude.typeDouble);
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

    printf("Altitude : %f fathoms\n", altitude.typeFloat);
    printf("Bearing  : %f deg.\n", bearing.typeFloat);
    printf("Speed    : %fm/s\n", speed.typeFloat);
    printf("Accuracy : %fm\n", accuracy.typeFloat);
}

void parseZergHeader(ZergHeader *zergHeader, FILE *pFile)
{
    EthernetHeader *ethHeader = calloc(sizeof(EthernetHeader), 1);
    IpHeader *ipHeader = calloc(sizeof(IpHeader), 1);

    fread(ethHeader, sizeof(EthernetHeader), 1, pFile);

    /* Check if Ethernet is 0800 */
    if(ethHeader->ethernetType == 0x08)
    {
        /* Move to IP header
         * Must move back because of EthernetHeader
         * size
         */
        fseek(pFile, -2, SEEK_CUR);
    }
    else
    {
        fseek(pFile, 2, SEEK_CUR);
    }

    /* Read in IP header */
    fread(ipHeader, sizeof(IpHeader), 1, pFile);

    /* Move to Zerg header based on IHL size */
    fseek(pFile, UDPHEADERSIZE + ((ipHeader->IHL - 5) * 4), SEEK_CUR);
    
    /* Start parsing Zerg header */
    fread(zergHeader, sizeof(*zergHeader), 1, pFile);

    if(zergHeader->version != 1)
    {
        printf("Invalid Zerg packet version %d\n", zergHeader->version);
        free(ethHeader);
        free(zergHeader);
        free(ipHeader);
        exit(4);
    }

    printf("Zerg---\nVersion  : %u\n", zergHeader->version);
    printf("Sequence : %u\n", htobe32(zergHeader->seqID));
    printf("From     : %u\n", htobe16(zergHeader->srcID));
    printf("To       : %u\n", htobe16(zergHeader->dstID));

    switch(zergHeader->type)
    {
        case 0:
        {
            printf("Message  : ");
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
            free(ethHeader);
            free(zergHeader);
            free(ipHeader);
            exit(2);
        }
    }

    free(ethHeader);
    free(ipHeader);
    
}


