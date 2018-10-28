//---------------------------------------------------------
//
// Project #1: TinyFP Representation
//
// March 22, 2018
//
// Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
// Systems Software Laboratory
// Dept. of Computer Science and Engineering
// Seoul National University
//
//---------------------------------------------------------


#include <stdlib.h>
#include "pa1.h"


typedef unsigned char tinyfp;

// changes tf's nth bit to 1
// assumes that tf's nth bit is 0
tinyfp nthBitToOne(tinyfp tf, int n) {

    switch (n) {
        case 0 :
            tf = tf | 0b00000001;
            break;
        case 1:
            tf = tf | 0b00000010;
            break;
        case 2:
            tf = tf | 0b00000100;
            break;
        case 3:
            tf = tf | 0b00001000;
            break;
        case 4:
            tf = tf | 0b00010000;
            break;
        case 5:
            tf = tf | 0b00100000;
            break;
        case 6:
            tf = tf | 0b01000000;
            break;
        case 7:
            tf = tf | 0b10000000;
            break;
    }
    return tf;

}

int* decToBin(int n) {
    int *result = (int*) malloc(sizeof(int) * 4);
    for (int i = 0; i < 4; ++i) {
        result[i] = (n % 2);
        n = n / 2;
    }
    return result;
}


tinyfp int2tinyfp(int x){

    //check 0
    if (x == 0)
        return 0b00000000;

    // check +inf
    if (x > 240)
        return 0b01111000;
    else if (x < -240)
        return 0b11111000;

    // x is in denormalizable range
    tinyfp tf = 0b00000000;

    // change the sign bit
    if (x < 0) {
        tf = nthBitToOne(tf, 7);
        x = -x;
    }

    // find out exp and frac
    int f[] = {0, 0, 0};
    int temp = x;
    int cnt = 7; // due to bias

    while (temp != 1) {
        cnt++;
        f[0] = f[1];
        f[1] = f[2];
        if (temp % 2)
            f[2] = 1;
        else
            f[2] = 0;
        temp = temp / 2;
    }

    int* exp = decToBin(cnt);

    for (int i = 0; i < 4; ++i) {
        if (exp[i])
            tf = nthBitToOne(tf, i + 3);
    }


    for (int i = 0; i < 3; ++i) {
        if (f[i])
            tf = nthBitToOne(tf, i);
    }


    return tf;
}

float power(float n , int m) {

    if (m == 0)
        return 1;

    if (m < 0) {
        m = -m;
        n = 1 / n;
    }
    float result = n;

    for (int i = 0; i < m - 1; ++i)
        result *= n;

    return result;
}


int getSign(tinyfp tf) {

    if (tf == (tf | 0b10000000))
        return 1;
    return 0;
}

int* getExp(tinyfp tf) {

    int * exp = (int*)malloc(sizeof(int) * 4);
    exp[3] = 0, exp[2] = 0, exp[1] = 0, exp[0] = 0;

    if (tf == (tf | 0b01000000))
        exp[3] = 1;
    if (tf == (tf | 0b00100000))
        exp[2] = 1;
    if (tf == (tf | 0b00010000))
        exp[1] = 1;
    if (tf == (tf | 0b00001000))
        exp[0] = 1;
    return exp;
}

int* getFrac(tinyfp tf) {

    int * frac = (int*)malloc(sizeof(int) * 3);
    frac[2] = 0, frac[1] = 0, frac[0] = 0;

    if (tf == (tf | 0b00000100))
        frac[2] = 1;
    if (tf == (tf | 0b00000010))
        frac[1] = 1;
    if (tf == (tf | 0b00000001))
        frac[0] = 1;
    return frac;
}


int tinyfp2int(tinyfp x) {
    int sign = getSign(x);
    int *exp = getExp(x);
    int *frac = getFrac(x);

    // +-inf or +-nan
    if (exp[3] == 1 && exp[2] == 1 && exp[1] == 1 && exp[0] == 1)
        return -2147483648;

    // calculate int
    int newExp = -7;
    for (int i = 0; i < 4; ++i) {
        if (exp[i])
            newExp += power(2, i);
    }

    float newFrac = 1;
    for (int i = 0; i < 3; ++i) {
        if (frac[i])
            newFrac += power(2, i - 3);
    }

    int result = (int) (newFrac * power(2, newExp));

    if(sign)
        result = -result;

    return result;
}



union Data {
    float f;
    unsigned int i;
    tinyfp tf;
};

tinyfp float2tinyfp(float x) {

    // check out of range or +-inf
    if (x > 240 || (x == 1.0 / 0)) {
        return 0b01111000;
    }
    else if (x < -240 || (x == -1.0 / 0)) {
        return 0b11111000;
    }

    // check +-nan
    if (x != x) {
        union Data d;
        d.f = x;
        d.i = d.i >> 31;
        if (d.i == 1)
            return 0b11111100;
        return 0b01111100;
    }

    // check 0
    if (x == 0) {
        union Data d;
        d.f = x;
        d.i = d.i >> 31;
        if (d.i == 1)
            return 0b10000000;
        return 0b00000000;
    }

    // check sign
    tinyfp result;

    if (x >= 0)
        result = 0b00000000;
    else {
        result = 0b10000000;
        x = -x;
    }


    // check denormalized
    if (x < 0.015625) {
        float sd = 0.001953125;
        if (x < sd)
            result = result | 0b00000000;
        else if (x < 2 * sd)
            result = result | 0b00000001;
        else if (x < 3 * sd)
            result = result | 0b00000010;
        else if (x < 4 * sd)
            result = result | 0b00000011;
        else if (x < 5 * sd)
            result = result | 0b00000100;
        else if (x < 6 * sd)
            result = result | 0b00000101;
        else if (x < 7 * sd)
            result = result | 0b00000110;
        else
            result = result | 0b00000111;
    }
    else {
        union Data d;
        d.f = x;
        union Data exp;
        unsigned int bias_diff = 127 - 7;
        exp.i = d.i;
        exp.i = (exp.i << 1) >> 24;
        exp.i = exp.i - bias_diff;
        exp.i = exp.i << 3;
        result = result | exp.tf;

        union Data frac;
        frac.f = x;
        frac.i = (frac.i << 9) >> 29;
        result = result | frac.tf;

    }
    return result;
}

float tinyfp2float(tinyfp x) {

    int sign = getSign(x);
    int* exp = getExp(x);
    int* frac = getFrac(x);

    if (exp[0] == 1 && exp[1] == 1 && exp[2] == 1 && exp[3] == 1) { // check nan
        if (sign) {
            if (x == 0b11111000)
                return -1.0 / 0.0;
            return 0.0 / 0.0;
        }
        if (x == 0b01111000)
            return 1.0 / 0.0;
        return -(0.0 / 0.0);   //wrong result
    }


    int newExp = -7;
    for (int i = 0; i < 4; ++i) {
        if (exp[i])
            newExp += power(2, i);
    }

    float newFrac = 1;

    if ((!exp[3]) && (!exp[2]) && (!exp[1]) && (!exp[0])) {
        newExp = -6;
        newFrac = 0;
    }

    for (int i = 0; i < 3; ++i) {
        if (frac[i])
            newFrac += power(2, i - 3);
    }

    float result = newFrac * power(2, newExp);

    if(sign)
        result = -result;

    return result;
}
