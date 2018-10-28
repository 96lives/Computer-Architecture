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

tinyfp EXP_MASK = 0b01111000;
tinyfp FRAC_MASK = 0b00000111;
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
    if (n >= 0)
        return res << ((unsigned int) n);
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
        if ((frac & (1 << i)) != 0)
            res += pow2( ((int)exponent) - 10 + i);
    }
    if ((x >> 7) == 1)
        res *= -1;
    return res;

}

union Data {
    float f;
    unsigned int u;
    tinyfp tf;
};

tinyfp float2tinyfp(float x){

    if ( (x >= 248) || (x == 1.0/0.0) )
        return TF_INF;
    else if ( (x <= -248) || (x == -1.0/0.0) )
      return TF_NINF;

   // check +-nan
    if (x != x) {
        union Data d;
        d.f = x;
        if (d.u >> 31 == 1)
            return TF_INF + 7 + (1 << 7);
        return TF_INF + 7;
    }

   // check sign
    tinyfp res;
    if (x >= 0)
        res = 0;
    else {
        res = 1 << 7;
        x = -x;
    }

    // check denormalized
    if (x < 0.015625) {
        float sd = 0.001953125;
        if (x < sd)
            res = res | 0b00000000;
        else if (x < 2 * sd)
            res = res | 0b00000001;
        else if (x < 3 * sd)
            res = res | 0b00000010;
        else if (x < 4 * sd)
            res = res | 0b00000011;
        else if (x < 5 * sd)
            res = res | 0b00000100;
        else if (x < 6 * sd)
            res = res | 0b00000101;
        else if (x < 7 * sd)
            res = res | 0b00000110;
        else
            res = res | 0b00000111;
        return res;
    }

    union Data d;
    d.f = x;
    union Data exp;
    unsigned int bias_diff = 127 - 7;
    exp.u = d.u;
    exp.u = (exp.u << 1) >> 24;
    exp.u = exp.u - bias_diff;
    exp.u = exp.u << 3;
    res = res | exp.tf;

    union Data frac;
    frac.f = x;
    frac.u = (frac.u << 9) >> 29;
    res = res | frac.tf;

    return res;
}

float frac_pow2(int n) {
    unsigned int pos_res = 1;
    float res = 1;
    int i = 0;
    if (n >= 0)
        return (float)(pos_res << ((unsigned int) n) );
    n = -n;
    for (i = 0; i < n; ++i)
        res /= 2;
    return res;
}


float tinyfp2float(tinyfp x){
    int exponent = (int)((x & EXP_MASK)>>3);
    tinyfp frac = x & FRAC_MASK;
    float f = 0;
    int i = 0;

    // check INF and NAN
    if ((x & EXP_MASK) == 0b01111000){
        if ((x & FRAC_MASK) != 0)
            return ((x >> 7) == 1) ? -0.0/0.0 : 0.0/0.0;
        if (x == TF_INF)
            return 1.0/0.0;
        else
            return -1.0/0.0;
    }

    // check normalized
    if (exponent == 0)
        exponent = -6;
    else {
        exponent -= 7;
        frac += 0b00001000;
    }

    for (i = 0; i < 4; ++i){
        if ((frac & (1 << i)) != 0)
            f += frac_pow2(exponent - 3 + i);
    }
    if ((x >> 7) == 1)
        f *= -1;
	return f;
}

