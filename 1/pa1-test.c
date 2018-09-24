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
#include <stdlib.h>
#include "pa1.h"

#define RANDOM_TESTS	4


typedef u64 (*fop) (u64 u, u64 v);

u64 Real_Uadd64 (u64 u, u64 v)
{
	return u + v;
}

u64 Real_Usub64 (u64 u, u64 v)
{
	return u - v;
}

u64 Real_Umul64 (u64 u, u64 v)
{
	return u * v;
}

u64 HL_Uadd64 (u64 u, u64 v)
{
	HL64 a, b, x;
	u64 result;

	U64_TO_HL64 (u, a);
	U64_TO_HL64 (v, b);
	x = Uadd64 (a, b);
	HL64_TO_U64 (x, result);
	return result;
}

u64 HL_Usub64 (u64 u, u64 v)
{
	HL64 a, b, x;
	u64 result;

	U64_TO_HL64 (u, a);
	U64_TO_HL64 (v, b);
	x = Usub64 (a, b);
	HL64_TO_U64 (x, result);
	return result;
}

u64 HL_Umul64 (u64 u, u64 v)
{
	HL64 a, b, x;
	u64 result;

	U64_TO_HL64 (u, a);
	U64_TO_HL64 (v, b);
	x = Umul64 (a, b);
	HL64_TO_U64 (x, result);
	return result;
}


void Test (char *name, char *op, fop f1, fop f2)
{
	int i;
	u64 u, v, res1, res2;

	printf ("%s:\n", name);
	for (i = 0; i < RANDOM_TESTS; i++)
	{
		u = random() << 48 | random() << 24 | random();
		v = random() << 48 | random() << 24 | random();
		res1 = (u64) f1 (u, v);
		res2 = (u64) f2 (u, v);
		printf ("u = 0x%016llx, v = 0x%016llx, u %s v = 0x%016llx, result = 0x%016llx %s\n",
				u, v, op, res1, res2, (res1 == res2)? "CORRECT" : "WRONG");
	}
}


int main (void)
{

	Test ("Unsigned addition", "+", Real_Uadd64, HL_Uadd64);
	Test ("Unsigned subtraction", "-", Real_Usub64, HL_Usub64);
	Test ("Unsigned multiplication", "*", Real_Umul64, HL_Umul64);

}

