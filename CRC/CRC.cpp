#include <iostream>
#include <fstream>
#include <intrin.h>
#pragma intrinsic(__rdtsc)

using namespace std;

const int SIZE = 50000;


inline unsigned long Xorshift() {
	static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned long t = (x ^ (x << 11));
	x = y;	y = z;	z = w;
	return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

int main() {





	system("PAUSE");
	return 0;
}