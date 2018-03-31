//
// Created by ds-ubuntu on 18. 3. 23.
//
#include <stdio.h>
#include <stdlib.h>
#define BYTE_FORMAT "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BITS(byte)  (byte & 0x80 ? '1' : '0'), \
  							(byte & 0x40 ? '1' : '0'), \
  							(byte & 0x20 ? '1' : '0'), \
  							(byte & 0x10 ? '1' : '0'), \
  							(byte & 0x08 ? '1' : '0'), \
  							(byte & 0x04 ? '1' : '0'), \
  							(byte & 0x02 ? '1' : '0'), \
  							(byte & 0x01 ? '1' : '0')
#define PRINT_TINYFP(t)		printf("tinyfp("BYTE_FORMAT"), ", BYTE_TO_BITS(t));

typedef unsigned char tinyfp;

union Data {
    float f;
    int i;
    tinyfp tf[4];
};

/*
tinyfp float2tinyfp(float x) {

    // check out of range or +-inf
    if (x > 480 || (x == 1.0 / 0)) {
        return 0b01111000;
    }
    else if (x < -480 || (x == -1.0 / 0)) {
        return 0b11111000;
    }

    // check +-nan
    if (x == 0.0/0.0) {
        return 0b01111100;
    }
    else if (x == -(0.0/0.0)) {
        return 0b11111100;
    }

    // check sign
    tinyfp result;
    union Data d;

    if (x >= 0) {
        result = 0b00000000;
        d.f = x;
    }
    else {
        result = 0b10000000;
        d.f = -x;
    }



    // check denormalized
    if (x < 0.015625) {
        printf("denorm\n");
        union Data frac;
        frac.i = d.i;
        //get frac
        frac.i = frac.i << 9; // 23 -> 32
        printf("%d\n", frac.i);
        frac.i = frac.i >> 29; // 32 -> 3
        printf("%d\n", frac.i);
        result = result | frac.tf;
    }
    else {
        printf("norm\n");

        //get exp
        unsigned int exp = d.i;
        unsigned int frac = d.i;
        exp = ((exp << 1) >> 28) << 3;
        frac = (frac << 9) >> 29;
        result = result | exp;
        result = result | frac;
    }
    return result;

}

 */

int main(void) {
    union Data d;
    d.f = 0.001953125;
    PRINT_TINYFP(d.tf[3]);

};
