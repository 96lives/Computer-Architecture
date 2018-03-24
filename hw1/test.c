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

int* decToBin(int n) {
    int *result = (int*) malloc(sizeof(int) * 4);
    for (int i = 0; i < 4; ++i) {
        result[i] = (n % 2);
        n = n / 2;
    }
    return result;
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
        return 0b01111100;
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

int main(void) {
    float x = 314.0;
    PRINT_TINYFP(float2tinyfp(x));
};