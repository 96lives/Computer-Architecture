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

typedef unsigned int 		u32;
typedef int 				s32;
typedef unsigned long long	u64;
typedef long long			s64;


// A 64-bit unsigned integer 'x' is represented by two 32-bit unsigned
// integers, where 'x.hi' and 'x.lo' hold upper and lower 32 bits of 'x',
// respectively.

struct _HL64 {
	u32 	hi;		// high 32 bits
	u32		lo;		// low 32 bits
};
typedef struct _HL64		HL64;


// Convert 'u' (u64 type) to 'x' (HL64 type), and vice versa.

#define U64_TO_HL64(u,x)	(x).hi = (u32) ((u64) (u) >> 32), \
							(x).lo = (u32) ((u64) (u) & 0xffffffff)

#define HL64_TO_U64(x,u)	(u) = (((u64) (x).hi << 32) | (u64) (x).lo)


// Function prototypes

extern HL64 Uadd64 (HL64 a, HL64 b);
extern HL64 Usub64 (HL64 a, HL64 b);
extern HL64 Umul64 (HL64 a, HL64 b);

