//
// Created by ds-ubuntu on 18. 4. 6.
//
#include <stdio.h>
#include "pa2.h"
#define BYTE_FORMAT "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BITS(byte)  (byte & 0x80 ? '1' : '0'), \
  							(byte & 0x40 ? '1' : '0'), \
  							(byte & 0x20 ? '1' : '0'), \
  							(byte & 0x10 ? '1' : '0'), \
  							(byte & 0x08 ? '1' : '0'), \
  							(byte & 0x04 ? '1' : '0'), \
  							(byte & 0x02 ? '1' : '0'), \
  							(byte & 0x01 ? '1' : '0')

#define PRINT_TINYFP(t)		printf("tinyfp("BYTE_FORMAT"), ", BYTE_TO_BITS(t))


int main (){
 	//
    tinyfp a = 0b01110110;
    tinyfp b = 0b01010000;
	tinyfp c = add(a, b);
	PRINT_TINYFP(c);


}