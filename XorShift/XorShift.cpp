#include <iostream>
#include <fstream>
#include <intrin.h>
#pragma intrinsic(__rdtsc)

using namespace std;

const int SIZE = 50000;
const int M = 5;
const int k = 15;
const int N = 3;
const float MAX_LIX = 2147483647;
const float MAX_LIX_2 = MAX_LIX / 2;
const float MAX_XOR_2 = INT64_MAX / 2;


inline unsigned long Xorshift() {
	static unsigned long x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned long t = (x ^ (x << 11));
	x = y;	y = z;	z = w;
	return (w = (w ^ (w >> 19)) ^ (t ^ (t >> 8)));
}

inline unsigned long Lihtmer() {
	//const int a = 16807, c = 2147483647, b = 0 ;
	const int a = 1664525, c = pow(2,32), b = 1013904223;
	static unsigned long z = 1;
	return (z = (z * a + b) % c);
}

inline int weightH(unsigned long number, int j) {
	int sum = 0;
	for (int i = 0; i < j; i++) {
		sum += (number % 2) ? 1 : 0;
		number = number >> 1;
	}
	return sum;
}

void correlation_all(long mas[SIZE], float * corr, long tmp) {
	/*long t;
	for (int i = 0; i < k; i++)
		for (int j = 0; j < SIZE - i; j++) {
			t = mas[j] * mas[j - i];
			corr[i] += t;
		}
	for (int i = 1;i < k; i++)
		corr[i] = corr[i] / (corr[0] * 8 * sizeof(unsigned long)*SIZE);*/
	for (int i = 0; i < k; i++) {			//	сдвиг последовательности
		double sum = 0, sum_1 = 0, sum_2 = 0;
		double t = 0, t1 = 0;
		for (int z = 0; z < SIZE - i;z++) {
			t = mas[z];
			t1 = mas[z + i];
			sum += t*t1;
			sum_1 += pow(t, 2);
			sum_2 += pow(t1, 2);
		}
		corr[i] = sum / (sqrt(sum_1*sum_2));
	}

}

void correlation(long mas[SIZE], float ** corr, long tmp) {
	for (int i = 0; i < k; i++)				//	сдвиг последовательности
		for (int j = 0; j < M; j++) {		//	сдвиг разрядов
			double sum = 0, sum_1 = 0, sum_2 = 0;
			double t = 0, t1 = 0;
			for (int z = 0; z < SIZE - i;z++) {
				t = mas[z] % static_cast<int>(pow(10, j + 1));
				t1 = mas[z + i] % static_cast<int>(pow(10, j + 1));
				sum += t*t1;
				sum_1 += pow(t, 2);
				sum_2 += pow(t1, 2);
			}
			corr[j][i] = sum / (sqrt(sum_1*sum_2));
		}
}

void reshet_xor(long mas[SIZE]) {
	ofstream file;
	file.open("Xor.csv");
	for (int i = 0; i < SIZE - N; i++) {
		file << static_cast<float>(mas[i]) / static_cast<float>(INT64_MAX) << ";" << static_cast<float>(mas[i + N]) / static_cast<float>(INT64_MAX) << endl;
	}
	file.close();
}

void reshet_xor(long mas[SIZE], int p) {
	ofstream file;
	file.open("Xor_s.csv");
	for (int i = 0; i < SIZE - N; i++) {
		file << (mas[i] % static_cast<int>(pow(10,p))) / static_cast<float>(INT64_MAX) << ";" <<(mas[i + N] % static_cast<int>(pow(10, p))) / static_cast<float>(INT64_MAX) << endl;
	}
	file.close();
}

void reshet_lix(long mas[SIZE]) {
	ofstream file;
	file.open("lix.csv");
	for (int i = 0; i < SIZE - N; i++) {
		file << static_cast<float>(mas[i]) / MAX_LIX << ";" << static_cast<float>(mas[i + N]) / MAX_LIX << endl;
	}
	file.close();
}

void reshet_lix(long mas[SIZE], int p) {
	ofstream file;
	file.open("lix_s.csv");
	for (int i = 0; i < SIZE - N; i++) {
		file << (mas[i] % static_cast<int>(pow(10, p))) / MAX_LIX << ";" << (mas[i + N] % static_cast<int>(pow(10, p))) / MAX_LIX << endl;
	}
	file.close();
}

int main() {
	unsigned long mas[2][SIZE];
	long mas2[2][SIZE];
	unsigned __int64 tick;
	tick = __rdtsc();
	for (int i = 0; i < SIZE; i += 2) {
		mas[0][i] = Xorshift();
		mas[0][i + 1] = Xorshift();
	}
	float t_xor = __rdtsc() - tick;
	cout << "\nXorshift time ->  " << t_xor << endl;
	tick = __rdtsc();
	for (int i = 0; i < SIZE; i += 2) {
		mas[1][i] = Lihtmer();
		mas[1][i + 1] = Lihtmer();
	}
	float t_lix = __rdtsc() - tick;
	cout << "Lihtmer  time ->  " << t_lix << endl;
	cout << endl << "Lihtmer is " << t_lix / t_xor << " times slower than Xorshift" << endl << endl;

	double tmp = 0, tmp1 = 0;
	for (int i = 0; i < SIZE; i++) {
		tmp += (static_cast<double>(mas[0][i]) / static_cast<double>(SIZE));
		tmp1 += (static_cast<double>(mas[1][i]) / static_cast<double>(SIZE));
	}

	for (int i = 0; i < SIZE; i++) {
		mas2[0][i] = static_cast<long>(mas[0][i]) - static_cast<long>(tmp);
		mas2[1][i] = static_cast<long>(mas[1][i]) - static_cast<long>(tmp1);
	}

	float * corr_all_xor = new float[k];
	float * corr_all_lix = new float[k];
	float ** corr_xor = new float*[M];
	float ** corr_lix = new float*[M];
	for (int i = 0; i < k; i++) {
		corr_all_xor[i] = 0;
		corr_all_lix[i] = 0;
	}
	for (int j = 0; j < M; j++) {
		corr_xor[j] = new float[k];
		corr_lix[j] = new float[k];
		for (int i = 0; i < k; i++) {
			corr_xor[j][i] = 0;
			corr_lix[j][i] = 0;
		}
	}
	correlation_all(mas2[0], corr_all_xor, tmp);
	correlation_all(mas2[1], corr_all_lix, tmp1);

	correlation(mas2[0], corr_xor, tmp);
	correlation(mas2[1], corr_lix, tmp1);
	ofstream file;
	/*file.open("corr_all.csv");
	for (int i = 0; i < k; i++) {
		//cout << "Xor " << corr_all_xor[i] << "\t Lix " << corr_all_lix[i] << endl;
		file << i << ";" << corr_all_xor[i] << ";" << corr_all_lix[i] << endl;
	}
	file.close();
	file.open("corr_xor.csv");
	for (int i = 0; i < k; i++) {
		file << i;
		for (int j = 1; j < M; j++) {
			file << ";" << corr_xor[j][i];
		}
		file << endl;
	}
	file.close();*/
	file.open("corr_lix.csv");
	for (int i = 0; i < k; i++) {
		file << i;
		for (int j = 1; j < M; j++) {
			file << ";" << corr_lix[j][i];
		}
		file << endl;
	}
	file.close();

	//reshet_xor(mas2[0]);
	//reshet_lix(mas2[1]);
	reshet_lix(mas2[1],1);

	system("PAUSE");
	return 0;
}