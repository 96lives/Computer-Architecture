//
// Created by ds-ubuntu on 18. 4. 6.
//
#include <stdio.h>

#define BYTE_FORMAT "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BITS(byte)  (byte & 0x80 ? '1' : '0'), \
  							(byte & 0x40 ? '1' : '0'), \
  							(byte & 0x20 ? '1' : '0'), \
  							(byte & 0x10 ? '1' : '0'), \
  							(byte & 0x08 ? '1' : '0'), \
  							(byte & 0x04 ? '1' : '0'), \
  							(byte & 0x02 ? '1' : '0'), \
  							(byte & 0x01 ? '1' : '0')

#define PRINT_TINYFP(t)		printf("tinyfp("BYTE_FORMAT"), ", BYTE_TO_BITS(t))

typedef unsigned char tinyfp;
/*
 *  returns 1 if tf is +inf
 *  returns -1 is td is -inf
 *  returns 2 is tf is nan
 *  returns 0 if tf is else
*/

int checkSpecial(tinyfp tf) {

    if ((tf | 0b01111000) != tf)
        return 0;
    if ((tf << 5) == 0) {
        if (tf >> 7 == 0)
            return 1;
        return -1;
    }
    return 2;
}

/*
 * normal form of 0 in this project is considered +0
 * change -0 to 0 is -0
 * else return original value
 *
 */

tinyfp changeZero(tinyfp tf) {
   if (tf == 0b10000000)
       return 0;
    return tf;
}

tinyfp mulFrac(tinyfp tf1, tinyfp tf2) {

    tinyfp f1 = tf1 & 0b00000111;
    tinyfp f2 = tf2 & 0b00000111;

    if (((tf1 << 1) >> 4) != 0)
        f1 = f1 | 0b00001000;
    if (((tf2 << 1) >> 4) != 0)
        f2 = f2 | 0b00001000;

    tinyfp res = 0;
    for (int i = 3; i >= 0; --i) {
        if ( ((f2 >> i) & 0b00000001) == 1)
             res = res + f1;
        if (i != 0)
            res = res << 1;
    }

    return res;
}


tinyfp roundToEven(tinyfp tf) {
    for (int i = 7; i >= 0; --i) {
        if (tf >> i == 1) {
            if ( ( (tf << (11 - i) ) >> 7) == 1) {
                tinyfp checkHalfOne = (tf << (10 - i) ) >> 7;
                tinyfp checkHalfZero = tf << 12 - i;
                tinyfp upValue = 1 << i - 3;
                if ((checkHalfZero > 0) || (checkHalfOne == 1 && checkHalfZero == 0))
                    tf = tf + upValue;
            }
            break;
        }
    }
    return tf;
}

int checkNthBitOne(tinyfp tf, int n) {
    if (n < 0)
        return 0;
    tinyfp mask = 1;
    return ((tf >> n) & mask) == 1;
}

// changes fraction part to tinyfloat fraction part
tinyfp normFrac(tinyfp tf) {
    tinyfp res = 0;
    for (int i = 7; i >= 0; --i) {
        if (tf >> i == 1) {
            if (checkNthBitOne(tf, i - 1))
                res = res | 0b00000100;
            //PRINT_TINYFP(res);
            if (checkNthBitOne(tf, i - 2))
                res = res | 0b00000010;
            //PRINT_TINYFP(res);
            if (checkNthBitOne(tf, i - 3))
                res = res | 0b00000001;
            //PRINT_TINYFP(res);
            break;
        }
    }
    return res;
}

tinyfp intToExp(int exp) {

    exp += 7; // add bias
    if (exp < 0)
        return 0;
    if (exp >= 15)
        return 0b01111000;
    tinyfp temp = 0;
    for (int i = 0; i < 4; i++) {
        if (exp % 2 == 1)
            temp = temp | 0b10000000;
        exp /= 2;
        temp = temp >> 1;
    }
    return temp;

}

tinyfp mul(tinyfp tf1, tinyfp tf2){

    tf1 = changeZero(tf1);
    tf2 = changeZero(tf2);
    int s1 = checkSpecial(tf1);
    int s2 = checkSpecial(tf2);

    // nan cases
    if ((s1 == 2) || (s2 == 2))
        return 0b01111111;
    if ((s1 * s1 == 1) && tf2 == 0)
        return 0b01111111;
    if  ((s2 * s2 == 1) && tf1 == 0)
        return 0b01111111;

    // check inf cases
    if ((s1 != 0) || (s2 != 0)) {
        // one of them is +-inf
        if ((tf1 >> 7) == (tf2 >> 7))
            return 0b01111000;
        return 0b11111000;
    }

    // normal cases
    // get fraction
    tinyfp mul = mulFrac(tf1, tf2);
    printf("multiply fraction\n");
    PRINT_TINYFP(mul);
    printf("\n");
    tinyfp frac = roundToEven(mul);
    printf("round to even\n");
    PRINT_TINYFP(frac);
    printf("\n");
    if (frac == 0)
        return 0;

    int exp = -7 -7 -6; //7's due to basis, -6 due to fraction part
    tinyfp exp1 = (tf1 << 1) >> 4;
    tinyfp exp2 = (tf2 << 1) >> 4;
    exp = exp + exp1 + exp2;

    // find the first one of fraction part
    for (int i = 7; i >= 0; --i) {
        if (frac >> i == 1) {
            exp += i;
            //printf("Exp: %d\n", exp);
            if (exp < -9)
                return 0;
            if (exp >= -6)
                frac = normFrac(frac);
            else {
                // fix exponent to -6 and differ the fraction values
                int expDiff = -6 - exp + i;
                frac = frac >> expDiff;
            }
            break;
        }
    }
    tinyfp res = 0;
    if ( (tf1 >> 7) != (tf2 >> 7) )
        res = 0b10000000;
    res = res | frac;
    //printf("Exp: %d\n", exp);
    res = res | intToExp(exp);

	return res;
}



int main (){

    tinyfp a = 0b00111100;
    tinyfp b = a;
    // tinyfp mulAnswer[N][N] = {0b01000001, 0b01001111, 0b01001111, 0b01011100};
    tinyfp res = mul(a, b);
    PRINT_TINYFP(res);



}