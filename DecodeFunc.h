/*
 * Bruce Cain
 *
 * Header file for decode functions
 */

#ifndef DECODEFUNC_H
#define DECODEFUNC_H

#include <stdint.h>

int checkFileHeader(FILE *inFile);
void parseZergHeader(ZergHeader *zergHeader, FILE *pFile);
#endif
