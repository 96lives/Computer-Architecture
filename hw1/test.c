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

typedef unsigned char tinyfloat;

tinyfloat nthBitToOne(tinyfloat tf, int n) {

    switch (n) {
        case 0 :
            tf = tf | 0b00000001;
            printf("0\n");
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


tinyfloat int2tinyfp(int x){

    //check 0
    if (x == 0)
        return 0b00000000;

    // check +inf
    if (x > 480)
        return 0b01111000;
    else if (x < -480)
        return 0b11111000;

    // x is in denormalizable range
    tinyfloat tf = 0b00000000;

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

int main(void) {
    int n = 241;
    PRINT_TINYFP(int2tinyfp(n));
};