/**
* TDQC5
* @file Packets.c
* @author Bruce Cain
* 
* @brief Source code for Packets.h to parse and create pcaps.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "Packets.h"

#define UDPHEADERSIZE 8
#define HEADERSIZETOTAL 42
#define IPHEADERSIZE 20
#define ZERGHEADERSIZE 12
#define STATUSHEADERSIZE 12

/**
 * @brief Host to big endian with 24 bits.
 *
 * htobe24 takes a integer that will flip the lower 24 bits.
 * @param num The number that is wanted to flip the lower 24 of its bits.
 * @return The returned value is num's lower 24 bits flipped.
 */
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
        fread(&commandParameters->parameterOne,
                sizeof(commandParameters->parameterOne), 1, pFile);
        fread(&commandParameters->parameterTwo,
                sizeof(commandParameters->parameterTwo), 1, pFile);

        singleFloat.hex = htobe32(commandParameters->parameterTwo);
        switch(htobe16(commandPayload->command))
        {
            case 1:
            {
                printf("Bearing: %f deg.\n", singleFloat.typeFloat);
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

void parseZergHeader(ZergHeader *zergHeader, FILE *pFile)
{
    EthernetHeader *ethHeader = calloc(sizeof(EthernetHeader), 1);
    IpHeader *ipHeader = calloc(sizeof(IpHeader), 1);

    fread(ethHeader, sizeof(EthernetHeader), 1, pFile);

    /* Check if Ethernet is 0800 */
    if(htobe16(ethHeader->ethernetType) == 0x8100)
    {
         /*
          * Move forward because of extra 4 bytes
          * for 802.1Q ethernet type.
         */
        fseek(pFile, 4, SEEK_CUR);
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

    printf("----------Zerg Header----------\n");
    printf("Version:   %u\n", zergHeader->version);
    printf("Sequence:  %u\n", htobe32(zergHeader->seqID));
    printf("From:      %u\n", htobe16(zergHeader->srcID));
    printf("To:        %u\n", htobe16(zergHeader->dstID));
    printf("Type:      %u\n", zergHeader->type);
    printf("Length:    %u\n", htobe24(zergHeader->totalLength));

    switch(zergHeader->type)
    {
        case 0:
        {
            printf("Message:   ");
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

void fprintPcapFileHeader(FILE *outFile)
{
    PcapFileHeader *pcapFileHeader = calloc(sizeof(PcapFileHeader), 1);
    pcapFileHeader->fileTypeID         = 0xA1B2C3D4;
    pcapFileHeader->majorVersion       = 0x0002;
    pcapFileHeader->minorVersion       = 0x0004;
    pcapFileHeader->gmtOffset          = 0x00000000;
    pcapFileHeader->accuracyDelta      = 0x00000000;
    pcapFileHeader->maxLengthOfCapture = 0x00000000;
    pcapFileHeader->linkLayerType      = 0x00000001;

    fwrite(pcapFileHeader, sizeof(PcapFileHeader), 1, outFile);
    free(pcapFileHeader);
}

void fprintPcapPacketHeader(int zergLength, FILE *outFile)
{
    PcapPacketHeader *pcapPacketHeader = calloc(sizeof(PcapPacketHeader), 1);
    pcapPacketHeader->uinxEpoch               = 0x00000000;
    pcapPacketHeader->microSecEpoch           = 0x00000000;
    pcapPacketHeader->lengthOfDataCaptured    = zergLength + HEADERSIZETOTAL;
    pcapPacketHeader->untruncatedPacketLength = 0x00000000;

    fwrite(pcapPacketHeader, sizeof(PcapPacketHeader), 1, outFile);

    free(pcapPacketHeader);
}

void fprintEthernetHeader(FILE *outFile)
{
    EthernetHeader *ethernetHeader = calloc(sizeof(EthernetHeader), 1);
    ethernetHeader->macDestinationFirst = 0x00000000;
    ethernetHeader->macDestinationSec   = 0x0000;
    ethernetHeader->macSourceFirst      = 0x0000;
    ethernetHeader->macSourceSec        = 0x00000000;
    ethernetHeader->ethernetType        = 0x0008;

    fwrite(ethernetHeader, sizeof(EthernetHeader), 1, outFile);

    free(ethernetHeader);
}

void fprintIpHeader(int zergLength, FILE *outFile)
{
    IpHeader *ipHeader = calloc(sizeof(IpHeader), 1);
    ipHeader->IHL            = 0x5;
    ipHeader->version        = 0x4;
    ipHeader->ECN            = 0x0;
    ipHeader->DSCP           = 0x0;
    ipHeader->totalLength    = htobe16(zergLength + IPHEADERSIZE + UDPHEADERSIZE);
    ipHeader->ID             = 0x0000;
    ipHeader->flags          = 0x0;
    ipHeader->fragmentOffset = 0x0;
    ipHeader->TTL            = 0x0;
    ipHeader->protocol       = 0x11;
    ipHeader->checksum       = 0x0;
    ipHeader->srcIP          = 0x0;
    ipHeader->dstIP          = 0x0;

    fwrite(ipHeader, sizeof(IpHeader), 1, outFile);

    free(ipHeader);
}

void fprintUdpHeader(int zergLength, FILE *outFile)
{
    UdpHeader *udpHeader = calloc(sizeof(UdpHeader), 1);
    udpHeader->srcPort  = 0x0;
    udpHeader->dstPort  = 0x0;
    udpHeader->length   = htobe16(zergLength + UDPHEADERSIZE);
    udpHeader->checksum = 0x0;

    fwrite(udpHeader, sizeof(UdpHeader), 1, outFile);

    free(udpHeader);
}

void getZergString(int type, char **str, FILE *pFile)
{ 
    char *word = NULL, sep[3] = " ";
    size_t size = 0, strLength = 0, startStr = 0; 

    /* Remove tag and capture message */
    if(getdelim(&word, &size, *sep, pFile) != -1)
    {
        free(word);
    }

    /* Clear leading white space from string */
    while(isspace(fgetc(pFile)))
    {
        continue;
    }

    /* Move back to begining of first word */
    fseek(pFile, -1, SEEK_CUR);
    startStr = ftell(pFile);

    if(type == 0)
    {
        while(fgetc(pFile) != EOF)
        {
            strLength++;
        }
    }
    else if(type == 1)
    {
        while(fgetc(pFile) != '\n')
        {
            strLength++;
        }

    }
    fseek(pFile, startStr - ftell(pFile), SEEK_CUR);
    *str = calloc(strLength, 1);
    fgets(*str, strLength, pFile);
}

void fprintZergHeader(ZergHeader *zergHeader, FILE *inFile, FILE *outFile)
{
    switch(zergHeader->type)
    {
        case 0:
        {
            char *printLine;

            /* Skip new line character */
            fseek(inFile, 1, SEEK_CUR);
            getZergString(zergHeader->type, 
                    &printLine, inFile);
            fwrite(printLine, htobe24(zergHeader->totalLength) - ZERGHEADERSIZE,
                    1, outFile);

            free(printLine);
            break;
        }
        case 1:
        {
            fprintZergStatus(htobe24(zergHeader->totalLength), inFile, outFile);
            break;
        }
        case 2:
        {
            fprintZergCommand(inFile, outFile);
            break;
        }
        case 3:
        {
            fprintZergGPS(inFile, outFile);
            break;
        }
        default:
        {
            printf("Invalid type.\n");
            break;
        }
    }

    free(zergHeader);
}

void fprintZergStatus(int totalLength, FILE *inFile, FILE *outFile)
{
    ZergStatus *zergStatus = calloc(sizeof(ZergStatus), 1);
    char *printLine, type[11];
    char fields[1][16];
    SinglePrecisionFloat speed;
    uint32_t hitPoints, maxHitPoints;

    /* Skip new line character */
    fseek(inFile, 1, SEEK_CUR);

    /* Start filling Zerg Status header */
    getZergString(
            1,
            &printLine, inFile);

    fscanf(inFile, "%15s %u/%u", fields[0], &hitPoints, &maxHitPoints);
    zergStatus->hitPoints = htobe24(hitPoints);
    zergStatus->maxHitPoints = htobe24(maxHitPoints);

    if(hitPoints > maxHitPoints)
    {
        printf("Hit points can't be higher the Max Hit Points.\n");
        exit(14);
    }

    fscanf(inFile, "%15s %10s", fields[0], type);
    zergStatus->type = getTypeNumber(type);
    fscanf(inFile, "%15s %hhu", fields[0], &zergStatus->armor);
    fscanf(inFile, "%15s %f", fields[0], &(speed.typeFloat));
    zergStatus->speed = htobe32(speed.hex);

    fwrite(zergStatus, sizeof(ZergStatus), 1, outFile);
    fwrite(printLine, totalLength - ZERGHEADERSIZE - STATUSHEADERSIZE,
            1, outFile);

    free(printLine);
    free(zergStatus);
    
}

void fprintZergCommand(FILE *inFile, FILE *outFile)
{
     ZergCommand *zergCommand = calloc(sizeof(ZergCommand), 1);
    char *command = NULL, padding = 0;
    char fields[1][16];
    size_t size = 0;

    /* Skip new line character */
    fseek(inFile, 1, SEEK_CUR);

    getline(&command, &size, inFile);
    command[strlen(command) - 1] = '\0';
    zergCommand->command = htobe16(getCmdNumber(command));
    fwrite(zergCommand, sizeof(ZergCommand), 1, outFile);

    if(htobe16(zergCommand->command) % 2 == 0 && 
            htobe16(zergCommand->command) < 7)
    {
        fwrite(&padding, sizeof(char), 4, outFile);
    }
    else
    {
        ZergCommandParameters *commandParameters = 
            calloc(sizeof(ZergCommandParameters), 1);
        SinglePrecisionFloat paramTwo;

        switch(htobe16(zergCommand->command))
        {
            case 1:
            {
                uint16_t paramOne;
                fscanf(inFile, "%15s %f", fields[1], &(paramTwo.typeFloat));
                commandParameters->parameterTwo = htobe32(paramTwo.hex);
                fscanf(inFile, "%15s %hu", fields[1], &paramOne);
                commandParameters->parameterOne = htobe16(paramOne);
                break;
            }
            case 3:
            {
                fwrite(&padding, sizeof(char), 4, outFile);
                break;
            }
            case 5:
            {
                char *word = NULL, *line = NULL, *nextWord = NULL;
                size_t size = 0;
                getline(&line, &size, inFile);
                word = strtok_r(line, " ", &nextWord);

                if(!strcmp(word, "Add"))
                {
                   commandParameters->parameterOne = 1;
                }
                else if(!strcmp(word, "Remove"))
                {
                   commandParameters->parameterOne = 0;
                }
                else
                {
                    printf("Invalid SET GROUP command.\n");
                    free(line);
                    free(commandParameters);
                    exit(4);
                }
                
                do
                {
                    word = strtok_r(NULL, " ", &nextWord);
                    if(word == 0)
                    {
                        break;
                    }

                } while(!strtol(word, NULL, 10));
                commandParameters->parameterTwo = htobe32(strtol(word, NULL, 10));
                free(line);
                break;
            }
            case 7:
            {
                char *word = NULL, *line = NULL, *nextWord = NULL;
                size_t size = 0;
                getline(&line, &size, inFile);
                word = strtok_r(line, " ", &nextWord);

                if(strcmp(word, "Re-send"))
                {
                    printf("Invalid input, must have Re-send line.\n");
                    free(line);
                    exit(15);
                }

                do
                {
                    word = strtok_r(NULL, " ", &nextWord);
                } while(!strtol(word, NULL, 10) && strtol(word, NULL, 10) != 0);
                word = strtok_r(NULL, " ", &nextWord);
                commandParameters->parameterTwo = htobe32(strtol(word, NULL, 10));
                free(line);
                break;
            }
            default:
            {
                printf("Invalid Command.\n");
                free(commandParameters);
                free(command);
                exit(16);
            }
        }

        fwrite(commandParameters, sizeof(ZergCommandParameters), 1, outFile);
        free(commandParameters);
        free(command);
    }
    
    free(zergCommand);
}

void fprintZergGPS(FILE *inFile, FILE *outFile)
{
    ZergGPS *zergGPS = calloc(sizeof(ZergGPS), 1);
    char field[1][16];
    DoublePrecisionFloat longitude, latitude;
    SinglePrecisionFloat altitude, bearing, speed, accuracy;

    fscanf(inFile, "%15s %lf", field[0], &(latitude.typeDouble));
    moveToNextLine(inFile);
    zergGPS->latitude = htobe64(latitude.hex);
    fscanf(inFile, "%15s %lf", field[0], &(longitude.typeDouble));
    moveToNextLine(inFile);
    zergGPS->longitude = htobe64(longitude.hex);

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

    /* Reading in GPS header */
    fscanf(inFile, "%15s %f", field[0], &(altitude.typeFloat));
    moveToNextLine(inFile);
    zergGPS->altitude = htobe32(altitude.hex);

    fscanf(inFile, "%15s %f", field[0], &(bearing.typeFloat));
    moveToNextLine(inFile);
    zergGPS->bearing = htobe32(bearing.hex);

    fscanf(inFile, "%15s %f", field[0], &(speed.typeFloat));
    moveToNextLine(inFile);
    zergGPS->speed = htobe32(speed.hex);

    fscanf(inFile, "%15s %f", field[0], &(accuracy.typeFloat));
    moveToNextLine(inFile);
    zergGPS->accuracy = htobe32(accuracy.hex);

    /* Writing GPS header to specified pcap file. */
    fwrite(zergGPS, sizeof(ZergGPS), 1, outFile);
    free(zergGPS);
}

int getTypeNumber(char *type)
{
    char statusType[16][11] = 
    {
        "Overmind", "Larva", "Cerebrate", "Overlord",
        "Queen", "Drone", "Zergling", "Lurker", "Broodling",
        "Hydralist", "Guardian", "Scourage", "Ultralisk",
        "Mutalisk", "Defiler", "Devourer"
    };

    for(int i = 0; i < 16; i++)
    {
        if(!(strcmp(type, statusType[i])))
        {
            return i + 1;
        }
    }

    printf("Invalid type.\n");
    exit(12);
}

int getCmdNumber(char *type)
{
    char command[8][11] = 
    {
        "GET STATUS", "GOTO", "GET GPS", "RESERVED",
        "RETURN", "SET GROUP", "STOP", "REPEAT"
    };

    for(int i = 0; i < 8; i++)
    {
        if(!(strcmp(type, command[i])))
        {
            return i;
        }
    }

    printf("Invalid Command.\n");
    exit(13);
}

void moveToNextLine(FILE *inFile)
{
    while(fgetc(inFile) != '\n')
    {
        continue;
    }
}

int checkInputMain(char fields[][16])
{
    int returnValue = 0;
    char field[5][11] =
    {
        "Sequence:", "From:", "To:",
        "Type:", "Length:"
    };

    for(int i = 0; i < 5; i++)
    {
        if(strcmp(field[i], fields[i]))
        {
            returnValue = 1;
        }
    }

    return returnValue;
}
