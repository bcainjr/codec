/**
* TDQC5
* @file Packets.c
* @author Bruce Cain
* 
* @brief Source code for Packets.h to parse and create pcaps.
*/

#include <stdlib.h>
#include <ctype.h>
#include "Packets.h"

/**
 * @brief Host to big endian with 24 bits.
 *
 * htobe24 takes a integer that will flip the lower 24 bits.
 *
 * @param num The number that is wanted to flip the lower 24 of its bits.
 *
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


