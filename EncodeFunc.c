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
#include "EncodeFunc.h"

/* Header sizes */
#define UDPHEADERSIZE    8
#define HEADERSIZETOTAL  42
#define IPHEADERSIZE     20
#define ZERGHEADERSIZE   12
#define STATUSHEADERSIZE 12
#define GPSHEADERSIZE    32

/* Exit statuses */
#define INVALIDVERSION      11
#define INVALIDTYPE         12
#define INVALIDMESSAGE      17
#define INVALIDHITPOINTS    21
#define INVALIDBREED        24
#define INVALIDNAME         26
#define INVALIDSTATUSFORMAT 29
#define INVALIDCOMMAND      31
#define INVALIDRESENDFORMAT 37
#define INVALIDSETGRPFORMAT 38
#define INVALIDGOTOFORMAT   39
#define INVALIDLONGITUDE    41
#define INVALIDLATITUDE     42
#define INVALIDGPSFORMAT    49
#define INVALIDREADSTRING   50
#define INVALIDFORMAT       51

/**
 * @brief StatusInputCheck Checks the status input fields.
 *
 * @param fields The fields being checked from the input file.
 * 
 * @return A r if the check succeded or 0 if it failed.
 */
static int statusInputCheck(char fields[][17])
{
    int returnValue = 1;
    char field[4][17] =
    {
        "HP:", "Type:", "Armor:", "MaxSpeed:"  
    };

    for(int i = 0; i < 4; i++)
    {
        if(strcmp(fields[i], field[i]))
        {
            returnValue = 0;
            break;
        }
    }

    return returnValue;
}

/**
 * @brief getTypeNumber This gets the value associated with the breed.
 *
 * @param type A pointer to a character (string) that has the name of
 * the breed.
 *
 * @return The value associated with the give type is returned as a
 * int or 0 if the breed was invalid.
 */
static int getTypeNumber(char *type)
{
    int returnValue = 0;
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
            returnValue = i + 1;
        }
    }

    return returnValue;
}

/**
 * @brief getCmdNumber Gets the number associated with the command.
 *
 * @param type A point to a character (string) that contains the
 * command.
 *
 * @return The value associated with the command as a int or
 * 10 if the command was invalid.
 */
static int getCmdNumber(char *type)
{
    int returnValue = 10;
    char command[8][11] = 
    {
        "GET STATUS", "GOTO", "GET GPS", "RESERVED",
        "RETURN", "SET GROUP", "STOP", "REPEAT"
    };

    for(int i = 0; i < 8; i++)
    {
        if(!(strcmp(type, command[i])))
        {
            returnValue = i;
        }
    }
    
    return returnValue;
}

/**
 * @brief moveToNestLine Moves the file points to the next line.
 *
 * @param inFile The stream that is being manipulated.
 */
static void moveToNextLine(FILE *inFile)
{
    char ch = fgetc(inFile);
    while(ch != '\n' && ch != EOF)
    {
        ch = fgetc(inFile);
    }
}

/**
 * @brief checkInputGPS Checks the fields in the GPS header.
 *
 * @param fields Are the fields captured form the file being
 * encoded.
 *
 * @return The value returned is 0 if no error was found or 1
 * if a invalid field is found.
 */
static int checkInputGPS(char fields[][16])
{
    int returnValue = 0;
    char field[6][11] =
    {
        "Latitude:", "Longitude:", "Altitude:",
        "Bearing:", "Speed:", "Accuracy:"
    };

    for(int i = 0; i < 6; i++)
    {
        if(strcmp(field[i], fields[i]))
        {
            returnValue = 1;
            break;
        }
    }

    return returnValue;
}

/**
 * @brief fprintPcapPacketHeader Encodes the pcap packet header.
 *
 * @param zergLength Length of Zerg header and payload.
 *
 * @param outFile File being written too.
 */
static void fprintPcapPacketHeader(int zergLength, FILE *outFile)
{
    PcapPacketHeader *pcapPacketHeader = calloc(sizeof(PcapPacketHeader), 1);
    pcapPacketHeader->uinxEpoch               = 0x00;
    pcapPacketHeader->microSecEpoch           = 0x00;
    pcapPacketHeader->lengthOfDataCaptured    = zergLength + HEADERSIZETOTAL;
    pcapPacketHeader->untruncatedPacketLength = 0x00;

    fwrite(pcapPacketHeader, sizeof(PcapPacketHeader), 1, outFile);

    free(pcapPacketHeader);
}

/**
 * @brief fprintEthernetHeader Encodes the ethernet header.
 *
 * @param outFile File being written too.
 */
static void fprintEthernetHeader(FILE *outFile)
{
    EthernetHeader *ethernetHeader = calloc(1, sizeof(EthernetHeader));
    ethernetHeader->macDestinationFirst = 0x00000000;
    ethernetHeader->macDestinationSec   = 0x0000;
    ethernetHeader->macSourceFirst      = 0x0000;
    ethernetHeader->macSourceSec        = 0x00000000;
    ethernetHeader->ethernetType        = 0x0008;

    fwrite(ethernetHeader, sizeof(EthernetHeader), 1, outFile);

    free(ethernetHeader);
}

/**
 * @brief fprintIpHeader Encodes the IP header.
 *
 * @param zergLength The length of Zerg header and payload.
 *
 * @param outFile The file being written too.
 */
static void fprintIpHeader(int zergLength, FILE *outFile)
{
    IpHeader *ipHeader = calloc(1, sizeof(IpHeader));
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

/**
 * @brief fprintUdpHeader Encodes the UDP header.
 *
 * @param zergLength The length of Zerg header and payload.
 *
 * @param outFile The file being written too.
 */
static void fprintUdpHeader(int zergLength, FILE *outFile)
{
    UdpHeader *udpHeader = calloc(1, sizeof(UdpHeader));
    udpHeader->srcPort  = 0x0;
    udpHeader->dstPort  = 0x0;
    udpHeader->length   = htobe16(zergLength + UDPHEADERSIZE);
    udpHeader->checksum = 0x0;

    fwrite(udpHeader, sizeof(UdpHeader), 1, outFile);

    free(udpHeader);
}

/**
 * @brief fprintHeaders Calls all the more static headers to be encoded.
 *
 * @param zergHeader A struct that contains all Zerg header data.
 *
 * @param outFile The file being written too.
 */
static void fprintHeaders(ZergHeader *zergHeader, FILE *outFile)
{
    int zergLen = htobe24(zergHeader->totalLength), padding = 0;
    if(zergHeader->type == 0 && zergLen < 18)
    {
        padding = 18 - zergLen;
    }

    if(zergHeader->type == 2 && zergLen == 14)
    {
        padding = 4;
    }
    /* Write headers before Zerg header */
    fprintPcapPacketHeader(zergLen + padding, outFile);
    fprintEthernetHeader(outFile);
    fprintIpHeader(zergLen, outFile);
    fprintUdpHeader(zergLen, outFile);
    fwrite(zergHeader, sizeof(ZergHeader), 1, outFile);
}

/**
 * @brief getZergString This will get either a name or message.
 *
 * getZergString based on type will get either a name or message from
 * the pFile. This will then copy the string to str and return the
 * size of the string.
 *
 * @param zergHeader The struct that holds the Zerg header data.
 *
 * @param str Pointer to a charater pointer that holds the name/message.
 *
 * @param pFile The file being encoded.
 *
 * @return A size_t (unsigned long) thats the length of str.
 */
static size_t getZergString(ZergHeader *zergHeader, char **str, FILE *inFile, FILE *outFile)
{ 
    char *word = NULL, sep[1] = " ";
    size_t size = 0, strLen = 0, padding = 0; 

    /* Remove tag and capture message */
    if(getdelim(&word, &size, *sep, inFile) != -1)
    {
        if(zergHeader->type == 0 && strcmp("Message: ", word))
        {
            fprintf(stderr, "Failed message.\n");
            free(zergHeader);
            free(word);
            fclose(outFile);
            fclose(inFile);
            exit(INVALIDMESSAGE);
        }
        else if(zergHeader->type == 1 && strcmp("Name: ", word))
        {
            fprintf(stderr, "Failed name.\n");
            free(zergHeader);
            fclose(inFile);
            fclose(outFile);
            free(word);
            exit(INVALIDNAME);
        }
        free(word);
    }
    else
    {
        fprintf(stderr, "Failed string read.\n");
        free(zergHeader);
        fclose(inFile);
        fclose(outFile);
        free(word);
        exit(INVALIDREADSTRING);
    }

    /* Clear leading white space from string */
    while(isblank(fgetc(inFile)))
    {
        continue;
    }

    /* Move back to begining of first word */
    fseek(inFile, -1, SEEK_CUR);
    
    if(zergHeader->type == 1)
    {
        strLen = htobe24(zergHeader->totalLength) - 24;
    }
    else
    {
        strLen = htobe24(zergHeader->totalLength) - 12;
        /* Account for padding */
        if(strLen < 6)
        {
            padding = 7 - strLen;
        }
    }


    *str = calloc(strLen + padding + 1, sizeof(char));
    word = calloc(strLen + padding + 1, sizeof(char));

    fgets(word, strLen + 1, inFile);
    strcpy(*str, word);

    free(word);

    return strLen + padding;
}

/**
 * @brief fprintZergStatus Encodes the Zerg status payload.
 *
 * @param zergHeader A struct that contains Zerg header data.
 *
 * @param inFile The file being encoded.
 *
 * @param outFile The file being written too.
 */
static void fprintZergStatus(ZergHeader *zergHeader, FILE *inFile, FILE *outFile)
{
    ZergStatus *zergStatus = calloc(1, sizeof(ZergStatus));
    char *printLine = NULL, type[12] = {0};
    char fields[4][17] = {0};
    SinglePrecisionFloat speed;
    int exitStatus = 0;
    uint8_t armor = 0;
    uint32_t hitPoints, maxHitPoints;
    size_t nameLength = 0;

    /* Skip new line character */
    fseek(inFile, 1, SEEK_CUR);

    /* Start filling Zerg Status header */
    nameLength = getZergString(zergHeader, &printLine, inFile, outFile);
    fscanf(inFile, "%15s %u/%u", fields[0], &hitPoints, &maxHitPoints);
    fscanf(inFile, "%15s %10s", fields[1], type);
    fscanf(inFile, "%15s %hhu", fields[2], &armor);
    fscanf(inFile, "%15s %f", fields[3], &(speed.typeFloat));

    /* Check for invalid fields */
    if(!statusInputCheck(fields))
    {
        fprintf(stderr, "Invalid Status format.\n");
        exitStatus = INVALIDSTATUSFORMAT;
    }
    /* Check for invalid hit points */
    else if(hitPoints > maxHitPoints)
    {
        fprintf(stderr, "Hit points can't be higher the Max Hit Points.\n");
        exitStatus = INVALIDHITPOINTS;
    }
    /* Check if breed if invalid */
    else if(getTypeNumber(type) == 0)
    {
        fprintf(stderr, "Invalid breed.");
        exitStatus = INVALIDBREED;
    }
    
    if(exitStatus)
    {
        free(zergHeader);
        free(zergStatus);
        free(printLine);
        free(inFile);
        free(outFile);
        exit(exitStatus);
    }


    zergStatus->hitPoints = htobe24(hitPoints);
    zergStatus->maxHitPoints = htobe24(maxHitPoints);
    zergStatus->type = getTypeNumber(type);
    zergStatus->armor = armor;
    zergStatus->speed = htobe32(speed.hex);

    fprintHeaders(zergHeader, outFile);

    fwrite(zergStatus, sizeof(ZergStatus), 1, outFile);
    fwrite(printLine, nameLength, 1, outFile);

    free(printLine);
    free(zergStatus);
    
}

/**
 * @brief fprintZergCommand Encodes the Zerg command payload.
 *
 * @param zergHeader A struct that contains Zerg header data.
 *
 * @param inFile The file being encoded.
 *
 * @param outFile The file being written too.
 */
static void fprintZergCommand(ZergHeader *zergHeader, FILE *inFile, FILE *outFile)
{
    ZergCommand *zergCommand = calloc(1, sizeof(ZergCommand));
    SinglePrecisionFloat paramTwo;
    uint16_t paramOne = 0;
    char *command = NULL, padding = 0, fields[2][16];
    size_t size = 0;


    /* Skip new line character */
    fseek(inFile, 1, SEEK_CUR);

    getline(&command, &size, inFile);
    command[strlen(command) - 1] = '\0';
    zergCommand->command = htobe16(getCmdNumber(command));
    free(command);

    if(htobe16(zergCommand->command) == 10)
    {
        fprintf(stderr, "Invalid command.\n");
        free(zergCommand);
        free(zergHeader);
        fclose(inFile);
        fclose(outFile);
        exit(INVALIDCOMMAND);
    }


    if(htobe16(zergCommand->command) % 2 == 0 && 
            htobe16(zergCommand->command) < 7)
    {
        fprintHeaders(zergHeader, outFile);
        fwrite(zergCommand, sizeof(ZergCommand), 1, outFile);
        fwrite(&padding, sizeof(char), 4, outFile);
    }
    else
    {
        ZergCommandParameters *commandParameters = 
            calloc(1, sizeof(ZergCommandParameters));

        switch(htobe16(zergCommand->command))
        {
            /* GOTO command */
            case 1:
            {
                fscanf(inFile, "%15s %f", fields[0], &(paramTwo.typeFloat));
                moveToNextLine(inFile);

                fscanf(inFile, "%15s %hu", fields[1], &paramOne);
                moveToNextLine(inFile);

                if(strcmp("Bearing:", fields[0]) || strcmp("Location:", fields[1]))
                {
                    fprintf(stderr, "Invalid GOTO command format.\n");
                    free(commandParameters);
                    free(zergCommand);
                    free(zergHeader);
                    fclose(inFile);
                    fclose(outFile);
                    exit(INVALIDGOTOFORMAT);
                }

                commandParameters->parameterTwo = htobe32(paramTwo.hex);
                commandParameters->parameterOne = htobe16(paramOne);
                break;
            }
            /* Reserved command */
            case 3:
            {
                fwrite(&padding, sizeof(char), 4, outFile);
                break;
            }
            /* Set Group command */
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
                    fprintf(stderr, "Invalid SET GROUP command.\n");
                    free(line);
                    free(zergCommand);
                    free(zergHeader);
                    free(commandParameters);
                    fclose(inFile);
                    fclose(outFile);
                    exit(INVALIDSETGRPFORMAT);
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
            /* Repeat command */
            case 7:
            {
                char *word = NULL, *line = NULL, *nextWord = NULL;
                size_t size = 0;
                getline(&line, &size, inFile);
                word = strtok_r(line, " ", &nextWord);

                if(strcmp(word, "Re-send"))
                {
                    fprintf(stderr, "Invalid input, must have Re-send line.\n");
                    free(zergCommand);
                    free(zergHeader);
                    free(commandParameters);
                    free(line);
                    fclose(inFile);
                    fclose(outFile);
                    exit(INVALIDRESENDFORMAT);
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
                fprintf(stderr, "Invalid Command.\n");
                free(zergCommand);
                free(zergHeader);
                free(commandParameters);
                fclose(inFile);
                fclose(outFile);
                exit(INVALIDCOMMAND);
            }
        }

        fprintHeaders(zergHeader, outFile);
        fwrite(zergCommand, sizeof(ZergCommand), 1, outFile);
        fwrite(commandParameters, sizeof(ZergCommandParameters), 1, outFile);
        free(commandParameters);
    }
    
    free(zergCommand);
}

/**
 * @brief fprintZergGPS Encodes the Zerg GPS payload.
 *
 * @param zergHeader A struct that conatins the Zerg header data.
 *
 * @param inFile The file being encoded.
 *
 * @param outFile The file being written too.
 */
static void fprintZergGPS(ZergHeader *zergHeader, FILE *inFile, FILE *outFile)
{
    ZergGPS *zergGPS = calloc(1, sizeof(ZergGPS));
    char fields[6][16];
    DoublePrecisionFloat longitude, latitude;
    SinglePrecisionFloat altitude, bearing, speed, accuracy;


    fscanf(inFile, "%15s %lf", fields[0], &(latitude.typeDouble));
    moveToNextLine(inFile);

    fscanf(inFile, "%15s %lf", fields[1], &(longitude.typeDouble));
    moveToNextLine(inFile);

    if(latitude.typeDouble < -90 || latitude.typeDouble > 90)
    {
        fprintf(stderr, "Invalid latitude %f\n", latitude.typeDouble);
        free(zergGPS);
        free(zergHeader);
        fclose(inFile);
        fclose(outFile);
        exit(INVALIDLATITUDE);
    }

    if(longitude.typeDouble < -180 || longitude.typeDouble > 180)
    {
        fprintf(stderr, "Invalid longitude %f\n", longitude.typeDouble);
        free(zergGPS);
        free(zergHeader);
        fclose(inFile);
        fclose(outFile);
        exit(INVALIDLONGITUDE);
    }

    /* Reading in GPS header */
    fscanf(inFile, "%15s %f", fields[2], &(altitude.typeFloat));
    moveToNextLine(inFile);

    fscanf(inFile, "%15s %f", fields[3], &(bearing.typeFloat));
    moveToNextLine(inFile);

    fscanf(inFile, "%15s %f", fields[4], &(speed.typeFloat));
    moveToNextLine(inFile);

    fscanf(inFile, "%15s %f", fields[5], &(accuracy.typeFloat));

    if(checkInputGPS(fields))
    {
        fprintf(stderr, "Invalid GPS field.\n");
        free(zergGPS);
        free(zergHeader);
        fclose(inFile);
        fclose(outFile);
        exit(INVALIDGPSFORMAT);
    }

    zergGPS->latitude = htobe64(latitude.hex);
    zergGPS->longitude = htobe64(longitude.hex);
    zergGPS->altitude = htobe32(altitude.hex);
    zergGPS->bearing = htobe32(bearing.hex);
    zergGPS->speed = htobe32(speed.hex);
    zergGPS->accuracy = htobe32(accuracy.hex);

    fprintHeaders(zergHeader, outFile);

    /* Writing GPS header to specified pcap file. */
    fwrite(zergGPS, sizeof(ZergGPS), 1, outFile);
    free(zergGPS);
}
/**
 * @brief checkInputMain Checks the fields in the main Zerg header.
 *
 * @param fields Is the fields collected from the file to be encoded.
 *
 * @return The value returned is the 0 for no error or 1 if a
 * invalid field is found.
 */
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


/**
 * @brief fprintPcapFileHeader Encodes the pcap file header.
 *
 * @param outFile File being written too.
 */
void fprintPcapFileHeader(FILE *outFile)
{
    PcapFileHeader *pcapFileHeader = calloc(1, sizeof(PcapFileHeader));
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

/**
 * @brief fprintZergHeader Encodes the Zerg header.
 *
 * fprintZergHeader takes a struct ZergHeader and using the data will
 * then determine which payload to encode  based on the type found in
 * inFile. Then all of the encoded data is sent to outFile in the 
 * layout of a "outFile".pcap.
 *
 * @param zergHeader A struct that contains Zerg header data.
 *
 * @param inFile The file being encoded.
 *
 * @param outFile The file being written too.
 */
void fprintZergHeader(ZergHeader *zergHeader, FILE *inFile, FILE *outFile)
{
    switch(zergHeader->type)
    {
        case 0:
        {
            char *printLine;
            size_t payloadLength = 0;

            /* Skip new line character */
            fseek(inFile, 1, SEEK_CUR);
            payloadLength = getZergString(zergHeader, &printLine, inFile, outFile);
            fprintHeaders(zergHeader, outFile);
            fwrite(printLine, payloadLength, 1, outFile);

            free(printLine);
            break;
        }
        case 1:
        {
            fprintZergStatus(zergHeader, inFile, outFile);
            break;
        }
        case 2:
        {
            fprintZergCommand(zergHeader, inFile, outFile);
            break;
        }
        case 3:
        {
            fprintZergGPS(zergHeader, inFile, outFile);
            break;
        }
        default:
        {
            fprintf(stderr, "Invalid type.\n");
            free(zergHeader);
            fclose(inFile);
            fclose(outFile);
            exit(INVALIDTYPE);
        }
    }
    moveToNextLine(inFile);
}
