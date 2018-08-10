/*
 * Bruce Cain
 *
 * Encode function header file.
 */

#ifndef ENCODEFUNC_H
#define ENCODEFUNC_H

#include <stdint.h>

/*
 * Prints the pcap file header to outFile.
 */
void fprintPcapFileHeader(FILE *outFile);

/*
 * Validates the main zerg header captured in fields.
 */
int checkInputMain(char fields[][16]);

/*
 * Prints the zerg header to outFile using the data from inFile with
 * it saved in the zergHeader.
 */
void fprintZergHeader(ZergHeader *zergHeader, FILE *inFile, FILE *outFile);

#endif
