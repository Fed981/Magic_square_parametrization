#include <iostream>
#include <vector>
//#include <iomanip>


#include "parametrization_finder.hpp"


using namespace std;

#define NOWINMICROSECONDS ((long long)(chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now().time_since_epoch()).count()))


///////////////////////////main

int main() {
	
	
	const long long seedmax = 1000;
	const long long startingpoint = 1;
	
	cout << "Here we go!" << endl;
	
	for (long long a = startingpoint; a < seedmax; a++) {
		for (long long b = 1+a; b < seedmax; b++) {
			const long long ab = a*b*(a*a-b*b);
			for (long long c = 1+a; c < seedmax; c++) {
				for (long long d = 1+c; d < seedmax; d++) {
					if (ab == c*d*(c*c-d*d)) {
						for (long long e = 1+c; e < seedmax; e++) {
							for (long long f = 1+e; f < seedmax; f++) {
								if (ab == e*f*(e*e-f*f)) {
									vector <long long> v{a,b,c,d,e,f};
									bool good = true;
									for (long long div = 2; div <= a; div++) {
										bool alldiv = true;
										for (long k = 0; k < 6; k++) {
											if (v[k] % div != 0) {
												alldiv = false;
												break;
											}
										}
										if (alldiv) {
											good = false;
											break;
										}
									}
									if (good) {
										cout << "}}} " << a << " " << b << " " << c << " " << d << " " << e << " " << f << " {{{" << endl;
										if (find_parametrization(a,b,c,d,e,f)) {
											int placeholder;
											cin >> placeholder;
											return 72;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	
	return 0;
	
}

