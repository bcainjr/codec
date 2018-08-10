/*
 * Bruce Cain
 *
 * Encode function header file.
 */

#ifndef ENCODEFUNC_H
#define ENCODEFUNC_H

#include <stdint.h>

void fprintPcapFileHeader(FILE *outFile);
int checkInputMain(char fields[][16]);
void fprintZergHeader(ZergHeader *zergHeader, FILE *inFile, FILE *outFile);

#endif
