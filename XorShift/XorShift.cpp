#include <iostream>
#include <intrin.h>
#pragma intrinsic(__rdtsc)

using namespace std;

const int SIZE = 90000;

inline unsigned long Xorshift() {
	static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned long t = (x ^ (x << 11));
	x = y;
	y = z;
	z = w;
	return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

inline unsigned long Lihtmer() {
	const int a = 16807, c = 2147483647;
	static unsigned long z = 1;
	return (z = (z * a) % c);
}


int main() {
	unsigned long mas[2][SIZE];

	unsigned __int64 tick;
	tick = __rdtsc();
	for (int i = 0; i < SIZE / 2; i += 2) {
		mas[0][i] = Xorshift();
		mas[0][i + 1] = Xorshift();
	}
	float t_xor = __rdtsc() - tick;
	cout << "Xorshift time ->  " << t_xor << endl;
	tick = __rdtsc();
	for (int i = 0; i < SIZE / 2; i += 2) {
		mas[1][i] = Lihtmer();
		mas[1][i + 1] = Lihtmer();
	}
	float t_lix = __rdtsc() - tick;
	cout << "Lihtmer  time ->  " << t_lix << endl;
	cout << endl << "Lihtmer is " << t_lix / t_xor << " times slower than Xorshift" << endl;
	system("PAUSE");
	return 0;
}