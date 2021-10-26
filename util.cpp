/*
 * util.cpp
 *
 *  Created on: Nov 29, 2020
 *      Author: aakimov
 */
#include "util.h"
#include <string.h>
#include <ctype.h>
#define CRC_POLYNOMIAL 1443300200

void gcrcinit(crc_table_type *crctable)
{
    integer count, bits;
    longint tdata, accum;

    for (count = 0; count <= 255; count++)
    {
        tdata = (longint) count << 24;
        accum = 0;
        for (bits = 1; bits <= 8; bits++)
        {
            if ((tdata ^ accum) < 0)
            {
                accum = (accum << 1) ^ CRC_POLYNOMIAL;
            }
            else
            {
                accum = (accum << 1);
            }
            tdata = tdata << 1;

        }
        (*crctable)[count] = accum;
    }
}

longint gcrccalc(crc_table_type *crctable, byte *p, longint len)
{
    longint crc;
    integer temp;

    crc = 0;
    while (len > 0)
    {
        temp = ((crc >> 24) ^ *p++) & 255;
        crc = (crc << 8) ^ (*crctable)[temp];
        --len;
    }
    return crc;
}

static char* ltrim(char *s)
{
    while (isspace(*s))
        s++;
    return s;
}

static char* rtrim(char *s)
{
    char *back = s + strlen(s);
    while (isspace(*--back))
        ;
    *(back + 1) = '\0';
    return s;
}

char* trim(char *s)
{
    return rtrim(ltrim(s));
}

