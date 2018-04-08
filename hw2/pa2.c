//---------------------------------------------------------
// 
// Project #2: TinyFP Arithmetic
//
// April 5, 2018
//
// Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
// Systems Software Laboratory
// Dept. of Computer Science and Engineering
// Seoul National University
//
//---------------------------------------------------------

#include "pa2.h"
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

// If this file contains any "float" or "double" word,
// it will be rejected by the automatic grader.
// (Do not use those words even in comments.)


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

int eq(tinyfp tf1, tinyfp tf2) {

    tf1 = changeZero(tf1);
    tf2 = changeZero(tf2);
    tinyfp s1 = checkSpecial(tf1);
    tinyfp s2 = checkSpecial(tf2);

    // not special cases
    if (s1== 0 && s2 == 0)
        return tf1 == tf2;
    if (s1 != 2 && s2 != 2)
        return s1 == s2;
    //either one is nan
    return 0;
}

int gt(tinyfp tf1, tinyfp tf2) {

    tf1 = changeZero(tf1);
    tf2 = changeZero(tf2);
    tinyfp s1 = checkSpecial(tf1);
    tinyfp s2 = checkSpecial(tf2);

    // check equal
    if (eq(tf1, tf2))
        return 0;
    // check nan
    if (s1 == 2 || s1 == 2)
        return 0;
    // check inf
    if (s1 == 1 || s2 == -1)
        return 1;
    // check -inf
    if (s1 == -1 || s2 == 1)
        return 0;

    // negate tf2 and check sign of the added result
    if ((tf2 >> 7) == 0)
        tf2 = tf2 | 0b10000000;
    else
        tf2 = tf2 & 0b01111111;
    if (add(tf1, tf2) >> 7)
        return 0;
    return 1;
}


tinyfp add(tinyfp tf1, tinyfp tf2){
	return 9;
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
        if (((f2 >> i) & 0b00000001) == 1)
            res = res + f1;
        if (i != 0)
            res = res << 1;
    }

    return res;
}

int checkNthBitOne(tinyfp tf, int n) {
    if (n < 0)
        return 0;
    tinyfp mask = 1;
    return ((tf >> n) & mask) == 1;
}

tinyfp onesMask(int n) {

    tinyfp res = 0;
    for (int i = 0; i <= n; ++i) {
        tinyfp mask = 1 << n;
        res = res | mask;
    }
    return res;
}

// check (after 3 numbers) after the first 1
tinyfp roundToEvenNorm(tinyfp tf) {

    for (int i = 7; i >= 0; --i) {
        if (checkNthBitOne(tf, i)) {
            if (checkNthBitOne(tf, i - 4)) {
                tinyfp checkHalfOne = checkNthBitOne(tf, i - 3);
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

tinyfp roundToEvenDenorm(tinyfp tf) {

    for (int i = 7; i >= 0; --i) {
        if (checkNthBitOne(tf, i)) {
            if (checkNthBitOne(tf, i - 3)) {
                tinyfp checkHalfOne = tf & onesMask(i - 2);
                tinyfp checkHalfZero = tf << 11 - i;
                tinyfp upValue = 1 << i - 2;
                if ((checkHalfZero > 0) || (checkHalfOne == 1 && checkHalfZero == 0))
                    tf = tf + upValue;
            }
            break;
        }
    }
    return tf;
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

// for normalized values only
tinyfp intToExp(int exp) {

    exp += 7; // add bias
    if (exp <= 0)
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
    tinyfp frac = mulFrac(tf1, tf2);
    if (frac == 0)
        return 0;

    int exp = -7 -7 -6; //7's due to basis, -6 due to fraction part
    tinyfp exp1 = (tf1 << 1) >> 4;
    tinyfp exp2 = (tf2 << 1) >> 4;
    exp = exp + exp1 + exp2;

    tinyfp res = 0;
    if ( (tf1 >> 7) != (tf2 >> 7) )
        res = 0b10000000;

    // find the first one of fraction part
    for (int i = 7; i >= 0; --i) {
        if (frac >> i == 1) {
            //printf("Exp: %d\n", exp);
            exp += i;
            //printf("Exp: %d\n", exp);
            if (exp < -9)
                return 0;
            if (exp >= -6) {
                frac = roundToEvenNorm(frac);
                frac = normFrac(frac);
            }
            else {
                // fix exponent to -6 and differ the fraction values
                int expDiff = -10 - exp + i;
                //printf("expDiff: %d\n", expDiff);
                frac = roundToEvenDenorm(frac);
                //PRINT_TINYFP(frac);
                frac = frac >> expDiff;
                if (frac == 0)
                    return 0;
                return res | frac;
            }
            break;
        }
    }
    // norm form
    if (frac == 0)
        return 0;
    res = res | frac;
    res = res | intToExp(exp);

	return res;
}






