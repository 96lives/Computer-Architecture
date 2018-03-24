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


double power(double n , int m) {

    if (m == 0)
        return 1;

    if (m < 0) {
        m = -m;
        n = 1 / n;
    }
    double result = n;
    for (int i = 0; i < m - 1; ++i)
        result *= n;

    return result;
}

typedef unsigned char tinyfp;


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
    if (tf == (tf | 0b00010001))
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
    printf("newExp: %d\n", newExp);

    double newFrac = 1;
    for (int i = 0; i < 3; ++i) {
        printf("i: %d, frac: %d\n", i, frac[i]);
        if (frac[i])
            newFrac += power(2, i - 3);
    }
    printf("newFrac: %f\n", newFrac);

    int result = (int) (newFrac * power(2, newExp));

    if(sign)
        result = -result;

    return result;
}

int main(void) {

    printf("%lf\n", power(2, -3));
    tinyfp tf = 0b01010101;
    printf("result: %d\n", tinyfp2int(tf));
};