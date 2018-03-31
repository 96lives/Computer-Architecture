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

#define PRINT_WORD(s,i)		printf(s"("BYTE_FORMAT" ", BYTE_TO_BITS((i) >> 24));	\
							printf(BYTE_FORMAT" ", BYTE_TO_BITS((i) >> 16)); 		\
							printf(BYTE_FORMAT" ", BYTE_TO_BITS((i) >> 8)); 		\
							printf(BYTE_FORMAT") => ", BYTE_TO_BITS(i));
#define PRINT_INT(i)		PRINT_WORD("int", i)
#define PRINT_FLOAT(i)		PRINT_WORD("float", i)
#define PRINT_TINYFP(t)		printf("tinyfp("BYTE_FORMAT"), ", BYTE_TO_BITS(t));

#define TINYFP_INF_NAN(t)	((((t) >> 3) & 0x0f) == 0x0f)
#define FLOAT_INF_NAN(f)	((((f) >> 23) & 0xff) == 0xff)
#define TEST_INF_NAN(f,t)	(TINYFP_INF_NAN(t) && FLOAT_INF_NAN(f) && 				\
							((((t) >> 7) == (f) >> 31) && 							\
							(((((t) & 0x07) == 0) && (((f) & 0x007fffff) == 0)) || 	\
							((((t) & 0x07) > 0) && (((f) & 0x007fffff) > 0)))))

#define CHECK_VALUE(r,a)	printf("%s\n", ((r) == (a))? "CORRECT" : "WRONG")
#define CHECK_INF_NAN(f,t)	printf("%s\n", TEST_INF_NAN(f,t)? "CORRECT" : "WRONG")

#define N 	6

typedef unsigned char tinyfp;

union Data {
    float f;
    unsigned int i;
    tinyfp tf;
};

tinyfp float2tinyfp(float x) {

    // check out of range or +-inf
    if (x > 480 || (x == 1.0 / 0)) {
        return 0b01111000;
    }
    else if (x < -480 || (x == -1.0 / 0)) {
        return 0b11111000;
    }

    // check +-nan
    if (x != x) {
        if (x < 0)
            return 0b01111100;
        return 0b11111100;
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
        printf("bias: %d\n", exp.i);
        exp.i = exp.i << 3;
        result = result | exp.tf;

        union Data frac;
        frac.f = x;
        frac.i = (frac.i << 9) >> 29;
        PRINT_FLOAT(frac.f);
        result = result | frac.tf;

    }
    return result;
}


int main(void) {
    /*
     * float float_literal[N] = {0.001953125, 0.000732421875, -12.345, 1.6, -0.0/0.0, 314.0};
tinyfp float2tinyfpAnswer[N] = {0b00000001, 0b00000000, 0b11010100, 0b00111100, 0b11111001, 0b01111000};

     */
    float x = 1.6;
    printf("%f\n", x);
    PRINT_TINYFP(float2tinyfp(x));

};
