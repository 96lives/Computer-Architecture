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
#include <stdbool.h>

tinyfp TFMAX = 0b01111000;
tinyfp TFMIN = 0b11111000;
tinyfp MASK0 = 0b00000001;
tinyfp MASK1 = 0b00000010;
tinyfp MASK2 = 0b00000100;
tinyfp MASK3 = 0b00001000;
tinyfp MASK4 = 0b00010000;
tinyfp MASK5 = 0b00100000;
tinyfp MASK6 = 0b01000000;
tinyfp MASK7 = 0b10000000;

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

    if (x > 240)
        return TFMAX;
    else if (x < -240)
        return TFMIN;
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


int tinyfp2int(tinyfp x){



	    return 2;
}


tinyfp float2tinyfp(float x){




	    return 2;
}


float tinyfp2float(tinyfp x){



	    return 2.0;
}

