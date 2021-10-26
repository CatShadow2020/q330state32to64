/*
 * util.h
 *
 *  Created on: Nov 29, 2020
 *      Author: aakimov
 */

#ifndef UTIL_H_
#define UTIL_H_
#include <stdint.h>
#include "datatype.h"



void gcrcinit (crc_table_type *crctable);
longint gcrccalc (crc_table_type *crctable, byte *p, longint len);
char *trim(char *s);


#endif /* UTIL_H_ */
