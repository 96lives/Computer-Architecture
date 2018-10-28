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
tinyfp NAN = 0b01111111;
tinyfp PINF = 0b01111000;
tinyfp NINF = 0b11111000;

// it will be rejected by the automatic grader.
// (Do not use those words even in comments.)


/*
 *  returns 1 if tf is +inf
 *  returns -1 is td is -inf
 *  returns 2 is tf is nan
 *  returns 0 if tf is else
*/

int checkSpecial(tinyfp tf) {
    tinyfp mask = 0b01111000;
    if ((tf | mask) != tf)
        return 0;
    tinyfp frac = tf << 5;

    if (frac == 0) {
        if (tf == PINF)
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

int checkNthBit(unsigned int uInt, unsigned int n){
    unsigned int mask = 1;
    mask = mask << n;
    if ((uInt & mask) != 0)
        return 1;
    return 0;
}

int checkBelowNthBit(unsigned int uInt, int n) {
    if (n < 0)
        return 0;
    unsigned int mask = 1;
    for (int i = 0; i < n; ++i){
        mask = mask << 1;
        mask++;
    }
    if ((uInt & mask) != 0)
        return 1;
    return 0;
}

unsigned int tfToUInt(tinyfp tf) {

    tf = changeZero(tf);
    if (tf == 0)
        return 0;
    unsigned int res = 0;

    // get fraction
    res = (tf & 0b00000111);
    // check normalized case
    if ((tf & 0b01111000) != 0)
        res = (res | 0x00000008);
    // get exponent, -6 is map to 0 and, 7 is map to 13
    unsigned int exp = ((tf & 0b01111000) >> 3);
    if (exp != 0)
        --exp;
    res = res << exp;

    // check sign and change it to twos complement
    if ((tf & 0b10000000) != 0) {
        res--;
        res = ~res;
    }
    return res;
}

unsigned int roundToEvenAdd(unsigned int uInt, unsigned int exp) {

    if (exp <= 3)
        return uInt;
    unsigned int mask = 1;
    mask = (mask << (exp - 4));
    if ((uInt & mask) == 0)
        return uInt;

    // 0.5 or higher removal part
    mask = 1;
    mask = mask << (exp - 3);
    unsigned int addValue = 1;
    addValue = addValue << (exp - 3);
    int fracThird = 0;
    if ((uInt & mask) != 0)    //frac third is 1
        fracThird = 1;

    if (!fracThird && !checkBelowNthBit(uInt, exp - 5))
        return uInt;
    return uInt + addValue;
}


// Decode uInt to tf
tinyfp uIntToTF(unsigned int uInt) {

    if (uInt == 0)
        return 0;

    tinyfp res = 0;
    //check sign
    if (checkNthBit(uInt, 31)) {
        res = res | 0b10000000;
        uInt = ~uInt;
        uInt++;
    }

    // get exponent and check the first 1
    unsigned int exp = 17;
    while (exp > 0) {
        if (checkNthBit(uInt, exp))
            break;
        exp--;
    }
    //if (exp != 0)
    //    exp--;

    //check out of range
    if (exp >= 17)
        return (res | 0b01111000);
    // denorm
    if (exp <= 2) {
        res += uInt;
        return res;
    }

   // round to even
    uInt = roundToEvenAdd(uInt, exp);
    unsigned int mask = 1;
    mask = (mask << exp + 1);
    if ((uInt & mask) != 0) {
        if (exp >= 17)
            return (res | 0b01111000);
        ++exp;
    }
     // normalized cases
    unsigned int frac = 0x00000007;
    frac = frac << (exp - 3);
    frac = (uInt & frac);
    frac = frac >> (exp - 3);
    res += frac;

    // get exp
    exp -= 2;
    exp = exp << 3;
    res = res | exp;
    return res;
}



tinyfp add(tinyfp tf1, tinyfp tf2){

    tf1 = changeZero(tf1);
    tf2 = changeZero(tf2);
    int s1 = checkSpecial(tf1);
    int s2 = checkSpecial(tf2);

    // special cases
    if (s1 != 0 || s2 != 0) {
        if (s1 == 2 || s2 == 2)
            return NAN;
        else if (s1*s2 == -1)
            return NAN;
        else if (s1 == 1 || s2 == 1)
            return PINF;
        else
            return NINF;
    }

    // normal case
    unsigned int etf1 = tfToUInt(tf1);
    unsigned int etf2 = tfToUInt(tf2);
    unsigned int res = etf1 + etf2;
    return uIntToTF(res);


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
    if (s1 == 2 || s2 == 2)
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

tinyfp mulFrac(tinyfp tf1, tinyfp tf2) {

    tinyfp f1 = tf1 & 0b00000111;
    tinyfp f2 = tf2 & 0b00000111;

    tinyfp mask = 0b01111000;
    if (((tf1 & mask) >> 3) != 0)
        f1 = f1 | 0b00001000;
    if (((tf2 & mask) >> 3) != 0)
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

tinyfp roundToEvenDenorm(tinyfp tf, int expDiff) {

    if (expDiff == 0)
        return tf;
    else if (expDiff > 8)
        return 0;

    tinyfp addValue = 1;
    addValue = addValue << expDiff;
    //TODO: check if expDiff >= 8 is available
    if (!checkNthBitOne(tf, expDiff - 1))
        return tf >> expDiff;
    for (int i = expDiff - 2; i >= 0; --i) {
        if (checkNthBitOne(tf, i)) {
            if (expDiff == 8)
                return 0b11111111;
            return ((tf + addValue) >> expDiff);
        }
    }
    if (checkNthBitOne(tf, expDiff))
        return (tf + addValue) >> expDiff;
    return tf >> expDiff;
}


// changes fraction part to tinyfoat fraction part
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

    tinyfp expMask = 0b01111000;
    int exp = -7 -7 -6; //7's due to basis, -6 due to fraction part
    tinyfp exp1 = (tf1 & expMask) >> 3;
    tinyfp exp2 = (tf2 & expMask) >> 3;
    exp += exp1 + exp2;
    if (exp1 == 0)
        exp++;
    if (exp2 == 0)
        exp++;
    tinyfp res = 0;

    if ( (tf1 >> 7) != (tf2 >> 7) )
        res = 0b10000000;

    // TODO --------------------------------------------------------------
    // find leading one of fraction part
    for (int i = 7; i >= 0; --i) {
        if (frac >> i == 1) {
            exp += i;
            if (exp < -10)
                return 0;
            if (exp >= -6) {
                frac = roundToEvenNorm(frac);
                if ((frac >> (i + 1)) == 1)
                    exp++;
                frac = normFrac(frac);
           }
            else {
                // fix exponent to -6 and differ the fraction values
                exp -= i;
                int expDiff = -9 - exp;
                frac = roundToEvenDenorm(frac, expDiff);
                //special case when expDiff = 10 and up occurs
                if (frac == 0b11111111)
                    frac = 1;
                if (frac == 0)
                    return 0;
                return res | frac;
            }
            break;
        }
    }
    // norm form
    res = res | frac;
    res = res | intToExp(exp);
    if((res & expMask) == 0b01111000)
        return res & 0b11111000;

	return res;
}






