//--------------------------------------------------------------
//
//  4190.308 Computer Architecture (Fall 2018)
//
//  Project #2: TinyFP (8-bit floating point) Representation
//
//  October 16, 2018
//
//  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//--------------------------------------------------------------

#include <stdio.h>
#include "pa2.h"
#include <limits.h>

tinyfp TF_INF = 0b01111000;
tinyfp TF_NINF = 0b11111000;
int TMIN = INT_MIN;
int TMAX = INT_MAX;

// poisitve integer binary to tinyfp
tinyfp posint2tinyfp(int x){
    const int shift = 7;
    unsigned int bin = x << shift;
    tinyfp bbg = 0;
    tinyfp guard;
    tinyfp exponent = 7;
    int i;
    tinyfp first_one;
    unsigned int round_sticky;

    for (i = 7 + shift; i >= shift; --i) {
        first_one = bin >> i;
        if (first_one != 0) {
            round_sticky = (bin << (35 - i)) >> (35 - i);
            bbg = (bin >> (i-3)) & 0b111;
            // round up
            if (round_sticky > (1 << (i-4))){
                bbg += 1;
            }
            //round to even when 0.5
            else if (round_sticky == (1 << (i-4))) {
                guard = bbg & 1;
                if (guard == 1)
                    bbg +=1;
            }
            if (bbg == 0b1000) {
                bbg = 0;
                exponent += 1;
            }
            exponent += i - shift;
            exponent = exponent << 3;
            break;
        }
    }
    return exponent | bbg;
}


tinyfp int2tinyfp(int x){
    // check overflow
    tinyfp res = 0;
    unsigned int bin;
    int i;
    tinyfp temp;
    tinyfp shift = 7;

    if (x > 247)
        return TF_INF;
    else if (x < -247)
        return TF_NINF;
    else if (x == 0)
        return 0;
    // now always normalized value
    if (x < 0) {
        x = -x;
        res = 1 << 7;
    }

    res = res | posint2tinyfp(x);

    return res;

}

int pow2(int n) {
    unsigned int res = 1;
    if (n > 0)
        return res << ((unsigned int) n);
    else if (n == 0)
        return 1;
    return 0;
}


int tinyfp2int(tinyfp x){
    tinyfp exponent;
    tinyfp frac;
    int res = 0;
    int i;
    int exponent_int = 0;
    // check INF
    if ((x & 0b01111000) == 0b01111000)
        return INT_MIN;
    exponent = (x & 0b01111000) >> 3;
    frac = (x & 0b00000111) | 0b00001000;
    if (exponent < 7)
        return 0;
    for (i = 0; i <= 3; ++i) {
        if ((frac & (1 << i)) != 0){
            res += pow2( ((int)exponent) - 10 + i);
        }
    }
    if ((x >> 7) == 1)
        res *= -1;
    return res;

}


tinyfp float2tinyfp(float x){




	    return 2;
}


float tinyfp2float(tinyfp x){



	    return 2.0;
}

