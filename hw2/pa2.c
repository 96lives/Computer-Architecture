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
    if ((tf >> 7) == 0)
        tf2 = tf2 | 0b10000000;
    else
        tf2 = tf2 & 0b01111111;

    return (add(tf1, tf2) >> 7);
}


tinyfp add(tinyfp tf1, tinyfp tf2){
	return 9;
}

tinyfp mul(tinyfp tf1, tinyfp tf2){
	return 9;
}


