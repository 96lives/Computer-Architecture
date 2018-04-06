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

#ifndef _PA_2_H_
#define _PA_2_H_

typedef unsigned char tinyfp;

// tinyfp addition
tinyfp add(tinyfp tf1, tinyfp tf2);

// tinyfp multiplication
tinyfp mul(tinyfp tf1, tinyfp tf2);

// tinyfp greater than
int gt(tinyfp tf1, tinyfp tf2);

// tinyfp equal
int eq(tinyfp tf1, tinyfp tf2);

#endif // _PA_2_H_
