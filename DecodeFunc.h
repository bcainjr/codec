/*
 * Bruce Cain
 *
 * Header file for decode functions
 */

#ifndef DECODEFUNC_H
#define DECODEFUNC_H

/*
 * Validates the pcap file header reading from inFile and
 * returns 0 if valid or 1 if invalid.
 */
int checkFileHeader(FILE *inFile);

/*
 * Parses pFile for the zerg header data and saves it into
 * zergHeader.
 */
void parseZergHeader(ZergHeader *zergHeader, PcapPacketHeader *packetHeader, FILE *pFile);

#endif
