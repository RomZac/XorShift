#include <iostream>
#include <fstream>
#include <intrin.h>
#pragma intrinsic(__rdtsc)

using namespace std;

const char * filename = "name.txt";


unsigned long Xorshift_code() {
	static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned long t = (x ^ (x << 11));
	x = y;	y = z;	z = w;
	return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

unsigned long Xorshift_decode() {
	static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned long t = (x ^ (x << 11));
	x = y;	y = z;	z = w;
	return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

int main() {
	ifstream file;
	file.open(filename);
	ofstream fil;
	fil.open("cod.txt");
	char buf;
	
	while (file) {
		file.get(buf);
		buf = buf ^ (Xorshift_code() % 256);
		fil << buf;
	}
	file.close();
	fil.close();
	//////////////////////////////////////////////////////////////////
	fil.open("reset.txt");
	file.open("cod.txt");

	while (file) {
		file.get(buf);
		buf = buf ^ (Xorshift_decode() % 256);
		fil << buf;
	}
	file.close();
	fil.close();
	system("PAUSE");
	return 0;
}