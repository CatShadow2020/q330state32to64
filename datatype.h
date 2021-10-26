/*
 * datatype.h
 *
 *  Created on: Nov 29, 2020
 *      Author: aakimov
 */

#ifndef DATATYPE_H_
#define DATATYPE_H_
#include <stdint.h>

typedef char tseed_net[2];
#define longint int32_t
#define word uint16_t
#define byte uint8_t
#define boolean uint8_t
#define integer int32_t
#define longword uint32_t
typedef char tseed_stn[5];
typedef longword t64[2];
typedef char string9[10];
#define single float
typedef void *pointer;
typedef longint taccminutes[60];
typedef byte tip_v6[16];
typedef char tlocation[2];
typedef char tseed_name[3];
typedef longint crc_table_type[256];

#endif /* DATATYPE_H_ */
