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

// get fraction values for 0 <= floats < 1
int* getFrac01(float x) {
    int* result = (int*) malloc(sizeof(int) * 3);
    result[2] = 0, result[1] = 0, result[0] = 0;

    if (0.5 <= x) {
        result[2] = 1;
        x = x - 0.5;
    }
    if (0.25 <= x) {
        result[1] = 1;
        x = x - 0.25;
    }
    if (0.125 <= x)
        result[0] = 1;

    return result;
}



tinyfp float2tinyfp(float x) {

    // check nan
    if (x != x) {
        if (x > 0)
            return 0b01111001;
        return 0b11111001;
    }

    int sign = 0;
    if (x < 0) {
        sign = 1;
        x = -x;
    }

    // check if is in range
    if (x > 240) {
        if (sign)
            return 0b11111000;
        return 0b01111000;
    }

    int cnt = 7;

    if (x >= 1) {
        while (x > 2) {
            x = x / 2;
            cnt++;
        }
    }
    else if (x >= 0.017578125) {
        while (x >= 1) {
            x = x * 2;
            cnt--;
        }
    }
    else {
        // denormalized form
        cnt = 0;
        x = x * power(2, 6);
    }

    // 1 <= x < 2
    if (x >= 1)
        x = x - 1;


    // now x is [0, 1)
    // get fraction part
    int *frac = getFrac01(x);
    int *exp = decToBin(cnt);

    tinyfp tf = 0b00000000;
    if (sign)
        tf = nthBitToOne(tf, 7);

    for (int i = 0; i < 4; ++i) {
        if (exp[i])
            tf = nthBitToOne(tf, i + 3);
    }


    for (int i = 0; i < 3; ++i) {
        if (frac[i])
            tf = nthBitToOne(tf, i);
    }

    return tf;

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
