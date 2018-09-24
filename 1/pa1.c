//--------------------------------------------------------------
//
//  4190.308 Computer Architecture (Fall 2018)
//
//  Project #1: 64-bit Arithmetic using 32-bit integers
//
//  September 18, 2018
//
//  Jin-Soo Kim (jinsoo.kim@snu.ac.kr)
//  Systems Software & Architecture Laboratory
//  Dept. of Computer Science and Engineering
//  Seoul National University
//
//--------------------------------------------------------------

#include <stdio.h>
#include "pa1.h"

// NOTE!!!!!
// You should use only 32-bit integer operations inside Uadd64(), Usub64(),
// and Umul64() functions.


// Uadd64() implements the addition of two 64-bit unsigned integers.
// Assume that A and B are the 64-bit unsigned integer represented by
// a and b, respectively. In the following example, x.hi and x.lo should
// have the upper and lower 32 bits of (A + B), respectively.

HL64 Uadd64 (HL64 a, HL64 b)
{
	HL64 	x;
    x.lo = a.lo + b.lo;
    x.hi = a.hi + b.hi;
    if (x.lo < a.lo || x.lo < b.lo)
        x.hi += 1;
	return x;
}

// Usub64() implements the subtraction between two 64-bit unsigned integers.
// Assume that A and B are the 64-bit unsigned integer represented by
// a and b, respectively. In the following example, x.hi and x.lo should
// have the upper and lower 32 bits of (A - B), respectively.

HL64 Usub64 (HL64 a, HL64 b)
{
	HL64 	x;
    x.lo = a.lo - b.lo;
    b.hi = (a.lo - b.lo > a.lo)? b.hi + 1 : b.hi;
    x.hi = a.hi - b.hi;

	return x;
}

// Usub64() implements the multiplication of two 64-bit unsigned integers.
// Assume that a' and b' are the 64-bit unsigned integer represented by
// a and b, respectively.  In the following example, x.hi and x.lo should
// have the upper and lower 32 bits of (A * B), respectively.

HL64 Umul32(u32 a, u32 b) {
    HL64 x;
    u32 mask = 0xFFFF;

    u32 hh = (a >> 16) * (b >> 16);
    u32 hl = (a >> 16) * (b & mask);
    u32 lh = (a & mask) * (b >> 16);
    u32 ll = (a & mask) * (b & mask);

    x.hi = hh + (hl >> 16) + (lh >> 16);
    u32 temp = ((hl & mask) << 16) + ((lh & mask) << 16);
    if ( (temp < ((hl & mask) << 16) ) || (temp < ((lh & mask) << 16) ) )
        x.hi += 1;
    x.lo = temp + ll;
    if ( (x.lo < temp) || (x.lo < ll) )
        x.hi += 1;

    return x;
}

HL64 Umul64 (HL64 a, HL64 b)
{
	HL64 	x;

    HL64 ll = Umul32(a.lo, b.lo);
    HL64 hl = Umul32(a.hi, b.lo);
    HL64 lh = Umul32(a.lo, b.hi);

    x.lo = ll.lo;
    x.hi = ll.hi + hl.lo + lh.lo;

	return x;
}




