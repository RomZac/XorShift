#include <iostream>

using namespace std;

unsigned long Xorshift() {
	static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned long t = (x ^ (x << 11));
	x = y;
	y = z;
	z = w;
	return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

unsigned long Rachmer() {
	const int a = 16807, c = 2147483647;
	static unsigned long z = 1;
	return (z = (z * a) % c);
}


int main() {
	for (int i = 0; i < 10; i++) {
		cout << "tun tun tun -> " << Xorshift() << "\ttun tun tun -> " << Rachmer() << endl;
	}
	system("PAUSE");
	return 0;
}