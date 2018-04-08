//
// Created by ds-ubuntu on 18. 4. 6.
//

#define BYTE_FMT "%c%c%c%c%c%c%c%c"
#define BITS(byte)  (byte & 0x80 ? '1' : '0'), \
  					(byte & 0x40 ? '1' : '0'), \
  					(byte & 0x20 ? '1' : '0'), \
  					(byte & 0x10 ? '1' : '0'), \
  					(byte & 0x08 ? '1' : '0'), \
  					(byte & 0x04 ? '1' : '0'), \
  					(byte & 0x02 ? '1' : '0'), \
  					(byte & 0x01 ? '1' : '0')

#define isZero(t)			(((t) & 0x7f) == 0)
#define isNaN(t)			(((((t) >> 3) & 0x0f) == 0x0f) && ((t) & 0x07))
#define PRINT_TINYFP(s,t1,t2,a,r)	\
							printf(BYTE_FMT" "s" "BYTE_FMT" = "BYTE_FMT" ("BYTE_FMT") %s\n",\
								BITS(t1), BITS(t2), BITS(a), BITS(r), \
								((isZero(a) && isZero(r)) || (isNaN(a) && isNaN(r)) || (a) == (r))? \
								"CORRECT" : "WRONG")


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

    int n = 3;
    while(n >= 0) {
        if ( ((f2 >> n) && 0b00000001) == 1)
             res = res + tf1;
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

// changes fraction part to tinyfloat fraction part
tinyfp normFrac(tinyfp tf) {
    tinyfp res = 0;
    for (int i = 7; i >= 0; --i) {
        if (tf >> i == 1) {
            if ( (tf << (8 - i) ) >> 7 == 1)
                res = res | 0b00000100;
            if ( (tf << (9 - i) ) >> 7 == 1)
                res = res | 0b00000010;
            if ( (tf << (10 - i) ) >> 7 == 1)
                res = res | 0b00000001;
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
            temp = temp | 0b01000000;
        exp %= 2;
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
    tinyfp frac = roundToEven(mulFrac(tf1, tf2));
    if (frac == 0)
        return 0;

    int exp = -7 -7 -6; //7's due to basis, -6 due to fraction part
    tinyfp exp1 = (tf1 << 1) >> 4;
    tinyfp exp2 = (tf2 << 1) >> 4;

    for (int i = 3; i >= 0; --i) {
        int twoPower = 1;
        for (int j = 0; j < i; ++i)
            twoPower *= 2;
        if (exp1 >> i == 1)
            exp += twoPower;
        if (exp2 >> i == 1)
            exp += twoPower;
    }

    // find the first one of fraction part
    for (int i = 7; i >= 0; --i) {
        if (frac >> i == 1) {
            exp += i;
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
    if ( (tf1 >> 7) == (tf2 >> 7) )
        res = 0b10000000;
    res = res | frac;
    res = res | intToExp(exp);

	return res;
}



int main (){



}