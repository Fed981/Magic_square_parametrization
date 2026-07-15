#include <iostream>
#include <cmath>
#include <ctime>
#include <algorithm>
#include <thread>
#include <mutex>
#include <chrono>

#include "velo.hpp"
//#include "poly.hpp"
#include "polyvelo.hpp"
#include "sextuplet_assessor.hpp"

using namespace std;


long long maxco;// = 100; // for a2,b2 !!! above 1000 determinant is over bounds so needs velo !!!
long long maxco1;// = 200; // for c1, d1
const long szex = 2;//not 4;
vector <long long> w(szex); // a2,b2 etc

long long a0global, b0global, c0global, d0global, e0global, f0global;
long long a1global, b1global, c1global, d1global, e1global, f1global;
long long a2global, b2global, c2global, d2global, e2global, f2global;

long long coeffalpha;
long long coeffbeta;
long long determinant;

long long coeffgamma;
long long coeffdelta;

long long troisccmoinsddfoisd;
long long ccmoinstroisddfoisc;

velo a0sqmoinsb0sq;
velo c0sqmoinsd0sq;

velo troisaambbzero;
velo troisabzero;

velo a0v, b0v, c0v, d0v;

mutex mtx;
bool goingon;

vector <thread::id> overids; // finished threads

struct couple{
	long long mari;
	long long femme;
};

vector <couple> a2b2viewed;

bool higherlevelcontinue;





long contains(const vector <couple> haystack, const long long needlemari, const long long needlefemme) {
	for (long i = 0; i < haystack.size(); i++) {
		if (haystack[i].mari == needlemari && haystack[i].femme == needlefemme) {
			return true;
		}
	}
	return false;
}

inline long long next_test_number(long long previous) {
	return previous<=0 ? 1-previous:-previous;
}


bool get_next_set_lite_inner() {
	
	long i = 1;
	if (w[i] == maxco) {
        i--;
        while (i >= 0 && w[i] == maxco)
            i--;
    }
    if (i==-1)
        return false;
    w[i] = next_test_number(w[i]);
    i++;
    while (i < szex) {
        w[i] = 0;
        i++;
    }
	
    return true;
}

bool get_next_set_lite() {
	bool out = false;
	do {
		out = get_next_set_lite_inner();
		coeffalpha = (3 * w[0] * w[0] - w[1] * w[1]) * w[1];
		coeffbeta = w[0] * (w[0] * w[0] - 3 * w[1] * w[1]);
		determinant = coeffalpha * coeffdelta - coeffbeta * coeffgamma;
		//cout << w[0] << " _ " << w[1] << endl;
		
		//cout << "a2 viewed = ";for (long i = 0; i < a2viewed.size(); i++) {cout << a2viewed[i] << ", ";}cout << endl;
		//cout << "b2 viewed = ";for (long i = 0; i < b2viewed.size(); i++) {cout << b2viewed[i] << ", ";}cout << endl;
		//cout << "found at " << wherecontains(a2viewed, w[0]) << " and " << wherecontains(b2viewed, w[1]) << endl;
		//cout << "det = " << determinant << endl;
		
	} while (out && (determinant == 0 || contains(a2b2viewed, w[0], w[1]) )); // out and what we don't want
	return out;
}



void myfuncex() {
	
	if (!goingon) {
		return;
	}
	
	mtx.lock();
	
	const bool local_assess = get_next_set_lite();
	const long long abaabb = w[0] * w[1] * (w[0] * w[0] - w[1] * w[1]);
	
	cout << "#" << this_thread::get_id() << ": " << w[0] << " _ " << w[1] << endl;
	
	const long long a2local = w[0];
	const long long b2local = w[1];
	const long long coeffalphalocal = coeffalpha;
	const long long coeffbetalocal = coeffbeta;
	const velo det = velo(determinant);//const long long determinantlocal = determinant;
	mtx.unlock();
	
	if (!local_assess) {
		goingon = false;
		higherlevelcontinue = false;
		return;
	}
	
	
	const velo det_sqr = det*det;
	const velo det_cub = det * det_sqr;
	
	const velo a2v = a2local;
	const velo b2v = b2local;
	
	const velo helper_alfa = velo(3)*a2v*b0v + a0v*b2v;
	const velo helper_bravo = velo(3)*a0v*b2v + a2v*b0v;
	const velo a2sqmoinsb2sq = a2v*a2v - b2v*b2v;
	
	const velo a2 = det * a2v;
	const velo b2 = det * b2v;
	const velo a0 = det * a0v;
	const velo b0 = det * b0v;
	const velo c0 = det * c0v;
	const velo d0 = det * d0v;
	
	const velo troisaambbdeux = 3*(a2local*a2local-b2local*b2local);
	const velo troisabdeux = 3*a2local*b2local;
	
	const velo dstroiscdzero = det_sqr*velo(3*c0global*d0global);
	
	const long long addedineps = 3*a0global*b2local*b0global*b0global + a2local*b0global*b0global*b0global - a0global*a0global*a0global*b2local - 3*a0global*a0global*a2local*b0global;
	const long long addedinphi = 3*a2local*b0global*b2local*b2local + a0global*b2local*b2local*b2local - a2local*a2local*a2local*b0global - 3*a2local*a2local*a0global*b2local;
	
	
	long long c2local = 0;
	while (c2local != maxco && goingon) {
		const velo c2v = c2local;
		const velo c2 = det * c2v;
		long long d2local = 0;
		while (d2local != maxco) {
			if (abaabb == c2local * d2local * (c2local * c2local - d2local * d2local)) {
				
				//cout << c2local << " $ " << d2local << endl;
			
				
				const velo d2v = d2local;
				const velo d2 = det * d2v;
				
				const velo c2sqmoinsd2sq = c2v*c2v - d2v*d2v;
			
				const long long common_c2_d2_with_c1 = (3 * c2local * c2local - d2local * d2local) * d2local;
				const long long common_c2_d2_with_d1 = c2local * (c2local * c2local - 3 * d2local * d2local);
				
				const velo g_a = coeffdelta * common_c2_d2_with_c1 - coeffbetalocal * troisccmoinsddfoisd;
				const velo h_a = coeffdelta * common_c2_d2_with_d1 - coeffbetalocal * ccmoinstroisddfoisc;
				const velo g_b = coeffalphalocal * troisccmoinsddfoisd - coeffgamma * common_c2_d2_with_c1;
				const velo h_b = coeffalphalocal * ccmoinstroisddfoisc - coeffgamma * common_c2_d2_with_d1;
				
				const velo dstroiscddeux = det_sqr*velo(3*c2local*d2local);
				
				const velo gagamgbgb = g_a*g_a - g_b*g_b;
				const velo gagb = g_a*g_b;
				const velo hahamhbhb = h_a*h_a - h_b*h_b;
				const velo hahb = h_a*h_b;
				const velo gahamgbhb = g_a*h_a - g_b*h_b;
				const velo gahbpgbha = g_a*h_b + g_b*h_a;
				
				const velo ldeps = troisabzero*gagamgbgb + troisaambbzero*gagb - dstroiscdzero;
				const velo mueps = velo(6*a0global*b0global)*gahamgbhb + troisaambbzero*gahbpgbha - det_sqr*velo(3*(c0global*c0global - d0global*d0global));
				const velo nueps = troisabzero*hahamhbhb + troisaambbzero*hahb + dstroiscdzero;
				const velo eps = det_sqr*velo(c0global*c0global*c0global*d2local - c2local*d0global*d0global*d0global + 3*c0global*c0global*c2local*d0global - 3*d0global*d0global*c0global*d2local
											+ addedineps);
				
				const velo ldphi = troisabdeux*gagamgbgb + troisaambbdeux*gagb - dstroiscddeux;
				const velo muphi = velo(6*a2local*b2local)*gahamgbhb + troisaambbdeux*gahbpgbha - det_sqr*velo(3*(c2local*c2local - d2local*d2local));
				const velo nuphi = troisabdeux*hahamhbhb + troisaambbdeux*hahb + dstroiscddeux;
				const velo phi = det_sqr*velo(c2local*c2local*c2local*d0global - c0global*d2local*d2local*d2local + 3*c2local*c2local*c0global*d2local - 3*d2local*d2local*c2local*d0global
											+ addedinphi);
				
				
				const velo factorofcc = nuphi * mueps - nueps * muphi;
				const bool ccfactorsuitable = !factorofcc.isNull();
				
				const velo helperadd = nuphi * eps - nueps * phi;
				const velo helpermul = nueps * ldphi - nuphi * ldeps;

				
				
				long long cc = 0;
			
				while (cc < maxco1) {
					
					//cout << cc << endl;
					
					
					const bool noccloop = (ccfactorsuitable && cc != 0);
					
					long long dd = 0;
					
					const velo c1v = cc;
					
					
					if (noccloop) {
						
						dd = ( (helperadd + velo(cc*cc) * helpermul) / (velo(cc) * factorofcc) ).toLongLong();
					}

					
					do {
						// check combination

						//cout << dd << endl;
						
						const velo d1v = dd;
						
						const velo rhsp = c1v * velo(common_c2_d2_with_c1) + d1v * velo(common_c2_d2_with_d1); //c1ll * common_c2_d2_with_c1 + d1ll * common_c2_d2_with_d1;
						const velo rhsq = c1v * velo(troisccmoinsddfoisd) + d1v * velo(ccmoinstroisddfoisc); //c1ll * troisccmoinsddfoisd + d1ll * ccmoinstroisddfoisc;
						
						const velo a1 = velo(coeffdelta) * rhsp - velo(coeffbetalocal) * rhsq;
						const velo b1 = velo(coeffalphalocal) * rhsq - velo(coeffgamma) * rhsp;
						
						// linear terms in a1,b1,c1,d1 already checked with the above
				
						const velo b1sq = b1*b1;
						const velo a1sqmoinsb1sq = a1*a1 - b1sq;
						const velo a1b1 = a1*b1;
						const velo commonforpowerstwoandsixwithcd = c1v*c1v-d0v*d2v-d1v*d1v;
				
						
						/*
						if (
							det_sqr*velo( c0global*(c0global*c0global*d2ll+3*(c0global*(c1ll*d1ll+c2ll*d0global)+d0global*(commonforpowerstwoandsixwithcd)))-d0global*d0global*(3*c1ll*d1ll+c2ll*d0global) -a0global*a0global*(helper_alfa) + b0global*b0global*(helper_bravo) )
							!=
							a1b1*velo(3*(a0sqmoinsb0sq)) + velo(3*a0global*b0global)*a1sqmoinsb1sq
							||
							det_sqr*velo( c2ll*    (c2ll*c2ll*d0global+    3*(c2ll*(c1ll*d1ll+c0global*d2ll)+    d2ll*(commonforpowerstwoandsixwithcd)))    -d2ll*d2ll*(3*c1ll*d1ll+c0global*d2ll)         -a2ll*a2ll*(helper_bravo) +         b2ll*b2ll*(helper_alfa) )
							!=
							a1b1*velo(3*(a2sqmoinsb2sq)) + velo(3*a2ll*b2ll)*a1sqmoinsb1sq
							) {
							return false;
						}
						*/
						if (
							det_sqr* ( c0v*(c0v*c0v*d2v+velo(3)*(c0v*(c1v*d1v+c2v*d0v)+d0v*(commonforpowerstwoandsixwithcd)))-d0v*d0v*(velo(3)*c1v*d1v+c2v*d0v) -a0v*a0v*(helper_alfa) + b0v*b0v*(helper_bravo) )
							==
							a1b1*velo(3)*(a0sqmoinsb0sq) + velo(3)*a0v*b0v*a1sqmoinsb1sq
							&&
							det_sqr* ( c2v*    (c2v*c2v*d0v+    velo(3)*(c2v*(c1v*d1v+c0v*d2v)+    d2v*(commonforpowerstwoandsixwithcd)))    -d2v*d2v*(velo(3)*c1v*d1v+c0v*d2v)         -a2v*a2v*(helper_bravo) +         b2v*b2v*(helper_alfa) )
							==
							a1b1*velo(3)*(a2sqmoinsb2sq) + velo(3)*a2v*b2v*a1sqmoinsb1sq
							) {
							// x^2 and x^6 conditions met
							/*
							if (
								a1*(a1*(a1*velo(b0global) + velo(3*a0global)*b1) + det_sqr*velo(3*(2*a0global*a2ll*b0global + b2ll*(a0sqmoinsb0sq))) - velo(3*b0global)*b1sq) + b1*(det_sqr*velo(3*(a2ll*(a0sqmoinsb0sq) - 2*a0global*b0global*b2ll)) - velo(a0global)*b1sq)
								!=
								det_cub*velo( c1ll*(c1ll*(c1ll*d0global + 3*c0global*d1ll) + 3*(d0global*(2*c0global*c2ll - d1ll*d1ll) + d2ll*(c0sqmoinsd0sq))) + d1ll*(3*(c2ll*(c0sqmoinsd0sq) - 2*c0global*d0global*d2ll) - c0global*d1ll*d1ll) )
								||
								a1*(a1*(a1*velo(b2ll) + velo(3*a2ll)*b1) + det_sqr*velo(3*(2*a0global*a2ll*b2ll + b0global*(a2sqmoinsb2sq))) - velo(3*b2ll)*b1sq) + b1*(det_sqr*velo(3*(a0global*(a2sqmoinsb2sq) - 2*a2ll*b0global*b2ll)) - velo(a2ll)*b1sq)
								!=
								det_cub*velo( c1ll*(c1ll*(c1ll*d2ll + 3*c2ll*d1ll ) + 3*(d2ll*(2*c0global*c2ll- d1ll*d1ll) + d0global*(c2sqmoinsd2sq))) + d1ll*(3*(c0global*(c2sqmoinsd2sq) - 2*c2ll*d0global*d2ll) - c2ll*d1ll*d1ll) )
								) {
								return false;
							}
							*/
							if (
								a1*(a1*(a1*b0v + velo(3)*a0v*b1) + det_sqr*velo(3)*(velo(2)*a0v*a2v*b0v + b2v*(a0sqmoinsb0sq)) - velo(3)*b0v*b1sq) + b1*(det_sqr*velo(3)*(a2v*(a0sqmoinsb0sq) - velo(2)*a0v*b0v*b2v) - a0v*b1sq)
								==
								det_cub* ( c1v*(c1v*(c1v*d0v + velo(3)*c0v*d1v) + velo(3)*(d0v*(velo(2)*c0v*c2v - d1v*d1v) + d2v*(c0sqmoinsd0sq))) + d1v*(velo(3)*(c2v*(c0sqmoinsd0sq) - velo(2)*c0v*d0v*d2v) - c0v*d1v*d1v) )
								&&
								a1*(a1*(a1*b2v + velo(3)*a2v*b1) + det_sqr*velo(3)*(velo(2)*a0v*a2v*b2v + b0v*(a2sqmoinsb2sq)) - velo(3)*b2v*b1sq) + b1*(det_sqr*velo(3)*(a0v*(a2sqmoinsb2sq) - velo(2)*a2v*b0v*b2v) - a2v*b1sq)
								==
								det_cub* ( c1v*(c1v*(c1v*d2v + velo(3)*c2v*d1v ) + velo(3)*(d2v*(velo(2)*c0v*c2v- d1v*d1v) + d0v*(c2sqmoinsd2sq))) + d1v*(velo(3)*(c0v*(c2sqmoinsd2sq) - velo(2)*c2v*d0v*d2v) - c2v*d1v*d1v) )
								) {
								// x^3 and x^5 conditions met
								const velo c1 = det * c1v; //maybe to put before, idk
								const velo d1 = det * d1v;
								
								if (
									a1*(a1*(a1*b1 + velo(3)*a2*b0 + velo(3)*a0*b2) + b1*(velo(6)*a0*a2 - velo(6)*b0*b2 - b1*b1)) - b1*b1*(velo(3)*a2*b0 + velo(3)*a0*b2) + velo(3)*(a0*b2 + a2*b0)*(a0*a2 - b0*b2)
									==
									c1*(c1*(c1*d1 + velo(3)*c2*d0 + velo(3)*c0*d2) + d1*(velo(6)*c0*c2 - velo(6)*d0*d2 - d1*d1)) - d1*d1*(velo(3)*c2*d0 + velo(3)*c0*d2) + velo(3)*(c0*d2 + c2*d0)*(c0*c2 - d0*d2)
									) {
									// x^4 condition met
									mtx.lock();
									if (goingon) {
										polyvelo alocal, blocal, clocal, dlocal;
										alocal.setCoeff(2, a2);
										blocal.setCoeff(2, b2);
										clocal.setCoeff(2, c2);
										dlocal.setCoeff(2, d2);
										alocal.setCoeff(0, a0);
										blocal.setCoeff(0, b0);
										clocal.setCoeff(0, c0);
										dlocal.setCoeff(0, d0);
										alocal.setCoeff(1, a1);
										blocal.setCoeff(1, b1);
										clocal.setCoeff(1, c1);
										dlocal.setCoeff(1, d1);
										alocal.simplify();
										blocal.simplify();
										clocal.simplify();
										dlocal.simplify();
										alocal.rescaleconstant(a0global);
										blocal.rescaleconstant(b0global);
										clocal.rescaleconstant(c0global);
										dlocal.rescaleconstant(d0global);
										
										a1global = alocal.getCoeff(1).toLongLong();
										b1global = blocal.getCoeff(1).toLongLong();
										c1global = clocal.getCoeff(1).toLongLong();
										d1global = dlocal.getCoeff(1).toLongLong();
										a2global = alocal.getCoeff(2).toLongLong();
										b2global = blocal.getCoeff(2).toLongLong();
										c2global = clocal.getCoeff(2).toLongLong();
										d2global = dlocal.getCoeff(2).toLongLong();
										
										//cout << "#" << this_thread::get_id() << ": " << endl;
										cout << "!!!" << endl;
										alocal.display();
										blocal.display();
										clocal.display();
										dlocal.display();
										//cout << "!!!" << endl;
										goingon = false;
										
										couple cpl;
										cpl.mari = a2local;
										cpl.femme = b2local;
										a2b2viewed.push_back(cpl);
										
									}
									mtx.unlock();
									return;
								}
								
								
							}
							
							
						}
				
						
						// ! check combination
						
						dd = next_test_number(dd);
					} while (dd < maxco1 && !noccloop);
					
					
					cc = next_test_number(cc);
				}
				
	
	
	
	
			}
			d2local = next_test_number(d2local);
		}
		c2local = next_test_number(c2local);
	}
	
	
	mtx.lock();
	overids.push_back(this_thread::get_id());
	//cout << this_thread::get_id() << endl;
	if (goingon) {
		couple cpl;
		cpl.mari = a2local;
		cpl.femme = b2local;
		a2b2viewed.push_back(cpl);
	}
	mtx.unlock();
	
}


void myfuncforthird() {
	
	mtx.lock();
	
	const long long a2local = w[0];
	w[0] = next_test_number(w[0]);
	//cout << "#" << this_thread::get_id() << ": " << a2local << " _ " << endl;

	mtx.unlock();
	
	if (!goingon || a2local == maxco) {
		goingon = false;
		return;
	}
	
	long long b2local = 0;
	
	const long long c2local = c2global;
	const long long d2local = d2global;
	
	const long long cc = c1global;
	const long long dd = d1global;
	
	while (b2local != maxco && goingon) {
		
		const long long coeffalphalocal = (3 * a2local * a2local - b2local * b2local) * b2local;
		const long long coeffbetalocal = a2local * (a2local * a2local - 3 * b2local * b2local);
		const long long determinantlocal = coeffalphalocal * coeffdelta - coeffbetalocal * coeffgamma;
		
		if (determinantlocal != 0) {
			const long long abaabb = a2local * b2local * (a2local * a2local - b2local * b2local);
			
			
			if (abaabb == c2local * d2local * (c2local * c2local - d2local * d2local)) {
				
				const velo det = velo(determinantlocal);
			
				const velo det_sqr = det*det;
				const velo det_cub = det * det_sqr;
				
				const velo a2v = a2local;
				const velo b2v = b2local;
				
				const velo helper_alfa = velo(3)*a2v*b0v + a0v*b2v;
				const velo helper_bravo = velo(3)*a0v*b2v + a2v*b0v;
				const velo a2sqmoinsb2sq = a2v*a2v - b2v*b2v;
				
				const velo a2 = det * a2v;
				const velo b2 = det * b2v;
				const velo a0 = det * a0v;
				const velo b0 = det * b0v;
				const velo c0 = det * c0v;
				const velo d0 = det * d0v;
				
				const velo troisaambbdeux = 3*(a2local*a2local-b2local*b2local);
				const velo troisabdeux = 3*a2local*b2local;
				
				const velo dstroiscdzero = det_sqr*velo(3*c0global*d0global);
				
				const long long addedineps = 3*a0global*b2local*b0global*b0global + a2local*b0global*b0global*b0global - a0global*a0global*a0global*b2local - 3*a0global*a0global*a2local*b0global;
				const long long addedinphi = 3*a2local*b0global*b2local*b2local + a0global*b2local*b2local*b2local - a2local*a2local*a2local*b0global - 3*a2local*a2local*a0global*b2local;
				
				const velo c2v = c2local;
				const velo c2 = det * c2v;
			
				
				const velo d2v = d2local;
				const velo d2 = det * d2v;
				
				const velo c2sqmoinsd2sq = c2v*c2v - d2v*d2v;
			
				const long long common_c2_d2_with_c1 = (3 * c2local * c2local - d2local * d2local) * d2local;
				const long long common_c2_d2_with_d1 = c2local * (c2local * c2local - 3 * d2local * d2local);
				
				const velo g_a = coeffdelta * common_c2_d2_with_c1 - coeffbetalocal * troisccmoinsddfoisd;
				const velo h_a = coeffdelta * common_c2_d2_with_d1 - coeffbetalocal * ccmoinstroisddfoisc;
				const velo g_b = coeffalphalocal * troisccmoinsddfoisd - coeffgamma * common_c2_d2_with_c1;
				const velo h_b = coeffalphalocal * ccmoinstroisddfoisc - coeffgamma * common_c2_d2_with_d1;
				
				const velo dstroiscddeux = det_sqr*velo(3*c2local*d2local);
				
				const velo gagamgbgb = g_a*g_a - g_b*g_b;
				const velo gagb = g_a*g_b;
				const velo hahamhbhb = h_a*h_a - h_b*h_b;
				const velo hahb = h_a*h_b;
				const velo gahamgbhb = g_a*h_a - g_b*h_b;
				const velo gahbpgbha = g_a*h_b + g_b*h_a;
				
				const velo ldeps = troisabzero*gagamgbgb + troisaambbzero*gagb - dstroiscdzero;
				const velo mueps = velo(6*a0global*b0global)*gahamgbhb + troisaambbzero*gahbpgbha - det_sqr*velo(3*(c0global*c0global - d0global*d0global));
				const velo nueps = troisabzero*hahamhbhb + troisaambbzero*hahb + dstroiscdzero;
				const velo eps = det_sqr*velo(c0global*c0global*c0global*d2local - c2local*d0global*d0global*d0global + 3*c0global*c0global*c2local*d0global - 3*d0global*d0global*c0global*d2local
											+ addedineps);
				
				const velo ldphi = troisabdeux*gagamgbgb + troisaambbdeux*gagb - dstroiscddeux;
				const velo muphi = velo(6*a2local*b2local)*gahamgbhb + troisaambbdeux*gahbpgbha - det_sqr*velo(3*(c2local*c2local - d2local*d2local));
				const velo nuphi = troisabdeux*hahamhbhb + troisaambbdeux*hahb + dstroiscddeux;
				const velo phi = det_sqr*velo(c2local*c2local*c2local*d0global - c0global*d2local*d2local*d2local + 3*c2local*c2local*c0global*d2local - 3*d2local*d2local*c2local*d0global
											+ addedinphi);
				
				
				const velo factorofcc = nuphi * mueps - nueps * muphi;
				const bool ccfactorsuitable = !factorofcc.isNull();
				
				const velo helperadd = nuphi * eps - nueps * phi;
				const velo helpermul = nueps * ldphi - nuphi * ldeps;


				const bool noccloop = (ccfactorsuitable && cc != 0);				
				
				const velo c1v = cc;
				

				
				// check combination

				
				const velo d1v = dd;
				
				const velo rhsp = c1v * velo(common_c2_d2_with_c1) + d1v * velo(common_c2_d2_with_d1); 
				const velo rhsq = c1v * velo(troisccmoinsddfoisd) + d1v * velo(ccmoinstroisddfoisc);
				
				const velo a1 = velo(coeffdelta) * rhsp - velo(coeffbetalocal) * rhsq;
				const velo b1 = velo(coeffalphalocal) * rhsq - velo(coeffgamma) * rhsp;
				
				// linear terms in a1,b1,c1,d1 already checked with the above
		
				const velo b1sq = b1*b1;
				const velo a1sqmoinsb1sq = a1*a1 - b1sq;
				const velo a1b1 = a1*b1;
				const velo commonforpowerstwoandsixwithcd = c1v*c1v-d0v*d2v-d1v*d1v;
		
				
				/*
				if (
					det_sqr*velo( c0global*(c0global*c0global*d2ll+3*(c0global*(c1ll*d1ll+c2ll*d0global)+d0global*(commonforpowerstwoandsixwithcd)))-d0global*d0global*(3*c1ll*d1ll+c2ll*d0global) -a0global*a0global*(helper_alfa) + b0global*b0global*(helper_bravo) )
					!=
					a1b1*velo(3*(a0sqmoinsb0sq)) + velo(3*a0global*b0global)*a1sqmoinsb1sq
					||
					det_sqr*velo( c2ll*    (c2ll*c2ll*d0global+    3*(c2ll*(c1ll*d1ll+c0global*d2ll)+    d2ll*(commonforpowerstwoandsixwithcd)))    -d2ll*d2ll*(3*c1ll*d1ll+c0global*d2ll)         -a2ll*a2ll*(helper_bravo) +         b2ll*b2ll*(helper_alfa) )
					!=
					a1b1*velo(3*(a2sqmoinsb2sq)) + velo(3*a2ll*b2ll)*a1sqmoinsb1sq
					) {
					return false;
				}
				*/
				if (
					det_sqr* ( c0v*(c0v*c0v*d2v+velo(3)*(c0v*(c1v*d1v+c2v*d0v)+d0v*(commonforpowerstwoandsixwithcd)))-d0v*d0v*(velo(3)*c1v*d1v+c2v*d0v) -a0v*a0v*(helper_alfa) + b0v*b0v*(helper_bravo) )
					==
					a1b1*velo(3)*(a0sqmoinsb0sq) + velo(3)*a0v*b0v*a1sqmoinsb1sq
					&&
					det_sqr* ( c2v*    (c2v*c2v*d0v+    velo(3)*(c2v*(c1v*d1v+c0v*d2v)+    d2v*(commonforpowerstwoandsixwithcd)))    -d2v*d2v*(velo(3)*c1v*d1v+c0v*d2v)         -a2v*a2v*(helper_bravo) +         b2v*b2v*(helper_alfa) )
					==
					a1b1*velo(3)*(a2sqmoinsb2sq) + velo(3)*a2v*b2v*a1sqmoinsb1sq
					) {
					// x^2 and x^6 conditions met
					/*
					if (
						a1*(a1*(a1*velo(b0global) + velo(3*a0global)*b1) + det_sqr*velo(3*(2*a0global*a2ll*b0global + b2ll*(a0sqmoinsb0sq))) - velo(3*b0global)*b1sq) + b1*(det_sqr*velo(3*(a2ll*(a0sqmoinsb0sq) - 2*a0global*b0global*b2ll)) - velo(a0global)*b1sq)
						!=
						det_cub*velo( c1ll*(c1ll*(c1ll*d0global + 3*c0global*d1ll) + 3*(d0global*(2*c0global*c2ll - d1ll*d1ll) + d2ll*(c0sqmoinsd0sq))) + d1ll*(3*(c2ll*(c0sqmoinsd0sq) - 2*c0global*d0global*d2ll) - c0global*d1ll*d1ll) )
						||
						a1*(a1*(a1*velo(b2ll) + velo(3*a2ll)*b1) + det_sqr*velo(3*(2*a0global*a2ll*b2ll + b0global*(a2sqmoinsb2sq))) - velo(3*b2ll)*b1sq) + b1*(det_sqr*velo(3*(a0global*(a2sqmoinsb2sq) - 2*a2ll*b0global*b2ll)) - velo(a2ll)*b1sq)
						!=
						det_cub*velo( c1ll*(c1ll*(c1ll*d2ll + 3*c2ll*d1ll ) + 3*(d2ll*(2*c0global*c2ll- d1ll*d1ll) + d0global*(c2sqmoinsd2sq))) + d1ll*(3*(c0global*(c2sqmoinsd2sq) - 2*c2ll*d0global*d2ll) - c2ll*d1ll*d1ll) )
						) {
						return false;
					}
					*/
					if (
						a1*(a1*(a1*b0v + velo(3)*a0v*b1) + det_sqr*velo(3)*(velo(2)*a0v*a2v*b0v + b2v*(a0sqmoinsb0sq)) - velo(3)*b0v*b1sq) + b1*(det_sqr*velo(3)*(a2v*(a0sqmoinsb0sq) - velo(2)*a0v*b0v*b2v) - a0v*b1sq)
						==
						det_cub* ( c1v*(c1v*(c1v*d0v + velo(3)*c0v*d1v) + velo(3)*(d0v*(velo(2)*c0v*c2v - d1v*d1v) + d2v*(c0sqmoinsd0sq))) + d1v*(velo(3)*(c2v*(c0sqmoinsd0sq) - velo(2)*c0v*d0v*d2v) - c0v*d1v*d1v) )
						&&
						a1*(a1*(a1*b2v + velo(3)*a2v*b1) + det_sqr*velo(3)*(velo(2)*a0v*a2v*b2v + b0v*(a2sqmoinsb2sq)) - velo(3)*b2v*b1sq) + b1*(det_sqr*velo(3)*(a0v*(a2sqmoinsb2sq) - velo(2)*a2v*b0v*b2v) - a2v*b1sq)
						==
						det_cub* ( c1v*(c1v*(c1v*d2v + velo(3)*c2v*d1v ) + velo(3)*(d2v*(velo(2)*c0v*c2v- d1v*d1v) + d0v*(c2sqmoinsd2sq))) + d1v*(velo(3)*(c0v*(c2sqmoinsd2sq) - velo(2)*c2v*d0v*d2v) - c2v*d1v*d1v) )
						) {
						// x^3 and x^5 conditions met
						const velo c1 = det * c1v; //maybe to put before, idk
						const velo d1 = det * d1v;
						
						if (
							a1*(a1*(a1*b1 + velo(3)*a2*b0 + velo(3)*a0*b2) + b1*(velo(6)*a0*a2 - velo(6)*b0*b2 - b1*b1)) - b1*b1*(velo(3)*a2*b0 + velo(3)*a0*b2) + velo(3)*(a0*b2 + a2*b0)*(a0*a2 - b0*b2)
							==
							c1*(c1*(c1*d1 + velo(3)*c2*d0 + velo(3)*c0*d2) + d1*(velo(6)*c0*c2 - velo(6)*d0*d2 - d1*d1)) - d1*d1*(velo(3)*c2*d0 + velo(3)*c0*d2) + velo(3)*(c0*d2 + c2*d0)*(c0*c2 - d0*d2)
							) {
							// x^4 condition met
							mtx.lock();
							if (goingon) {
								polyvelo alocal, blocal, clocal, dlocal;
								alocal.setCoeff(2, a2);
								blocal.setCoeff(2, b2);
								clocal.setCoeff(2, c2);
								dlocal.setCoeff(2, d2);
								alocal.setCoeff(0, a0);
								blocal.setCoeff(0, b0);
								clocal.setCoeff(0, c0);
								dlocal.setCoeff(0, d0);
								alocal.setCoeff(1, a1);
								blocal.setCoeff(1, b1);
								clocal.setCoeff(1, c1);
								dlocal.setCoeff(1, d1);
								alocal.simplify();
								blocal.simplify();
								clocal.simplify();
								dlocal.simplify();
								alocal.rescaleconstant(a0global);
								blocal.rescaleconstant(b0global);
								clocal.rescaleconstant(c0global);
								dlocal.rescaleconstant(d0global);
								
								e1global = alocal.getCoeff(1).toLongLong();
								f1global = blocal.getCoeff(1).toLongLong();
								e2global = alocal.getCoeff(2).toLongLong();
								f2global = blocal.getCoeff(2).toLongLong();
								
								//cout << "#" << this_thread::get_id() << ": " << endl;
								//cout << "!!!" << endl;
								alocal.display();
								blocal.display();
								//clocal.display();
								//dlocal.display();
								cout << "!!!" << endl;
								goingon = false;
								
							}
							mtx.unlock();
							return;
						}
						
						
					}
					
					
				}
		
				
				// ! check combination
		
		
			}
			
			
		}
		
		
		b2local = next_test_number(b2local);
	}
	
	
	
	
	mtx.lock();
	overids.push_back(this_thread::get_id());
	mtx.unlock();
	
}



velo poly_evaluate(const polyvelo g, const long long p, const long long q) {
	velo out;
	velo p_as_velo(p);
	velo q_as_velo(q);
	velo p_current_power((long long)1);
	velo q_current_power((long long)1);
	const long n = g.getDeg();
	for (long j = 0; j < n; j++) {
		q_current_power *= q_as_velo;
	}
	for (long i = 0; i <= n; i++) {
		out += g.getCoeff(i) * p_current_power * q_current_power;
		p_current_power *= p_as_velo;
		q_current_power /= q_as_velo;
	}
	return out;
}

long long first_divisor(const long long n) {
	long long d = 2;
	const long long m = (long long)(1.1 * sqrt((long double)n));
	while (d <= m) {
		if (n % d == 0) {
			return d;
		}
		d++;
	}
	return n;
}

vector <long long> get_individual_prime_divisors(const long long n) {
	vector <long long> out;
	long long dividend = n;
	do {
		const long long divisor = first_divisor(dividend);
		out.push_back(divisor);
		dividend /= divisor;
	} while (dividend > 1);
	return out;
}

vector <long long> generatedivisorsrecursively(vector <long> exponents, vector <long long> primes, vector <long long> vprevious) {
	if (exponents.empty()) {
		return vprevious;
	}
	const long lastexponent = exponents.back();
	vector <long> exponents_cropped = exponents;
	exponents_cropped.pop_back();
	vector <long long> vcurrent = vprevious;
	vcurrent.reserve((1 + lastexponent) * vprevious.size());
	// build new divisors
	const long long mul = primes[exponents.size() - 1];
	for (long actualexponent = 1; actualexponent <= lastexponent; actualexponent++) {
		for (long iv = 0; iv < vprevious.size(); iv++) {
			long long base = vprevious[iv];
			for (long e = 1; e <= actualexponent; e++) {
				base *= mul;
			}
			vcurrent.push_back(base);
		}
	}
	return generatedivisorsrecursively(exponents_cropped, primes, vcurrent);
}


vector <long long> generate_all_divisors(const long long n) {
	vector <long long> vindprimes = get_individual_prime_divisors(abs(n));
	long long currentprime = vindprimes[0];
	long long currentpower = 1;
	long k = 0;
	vector <long long> primes;
	vector <long> exponents;
	while (k < vindprimes.size()) {
		if (k == vindprimes.size() - 1 || vindprimes[k+1] != currentprime) {
			exponents.push_back(currentpower);
			primes.push_back(currentprime);
			currentpower = 1;
			currentprime = vindprimes[(k+1) % vindprimes.size()];
		} else {
			currentpower++;
		}
		k++;
	}
	vector <long long> vdummy;
	vdummy.push_back(1);
	vdummy = generatedivisorsrecursively(exponents, primes, vdummy);
	return vdummy;
}


bool poly_has_rational_roots(const polyvelo g) {
	if (g.getDeg() == 0 || g.getCoeff(0) == 0 || g.getCoeff(g.getDeg()) == 0) {
		return false;
	}
	vector <long long> nums = generate_all_divisors((g.getCoeff(0)).toLongLong());
	vector <long long> dens = generate_all_divisors((g.getCoeff(g.getDeg())).toLongLong());
	const long count_num = nums.size();
	const long count_den = dens.size();
	for (long index_num = 0; index_num < count_num; index_num++) {
		const velo p(nums[index_num]);
		for (long index_den = 0; index_den < count_den; index_den++) {
			const velo q(dens[index_den]);
			//cout << "candidate: " << p << " / " << q << endl;
			for (int sign = 0; sign < 2; sign++) {
				// evaluate
				velo x((long long)1);
				velo y((long long)1);
				for (long i = 0; i < g.getDeg(); i++) {
					y *= q;
				}
				velo res((long long)0);
				for (long j = 0; j <= g.getDeg(); j++) {
					res += g.getCoeff(j) * x * y;
					x *= p;
					if (sign != 0) {
						x.negate();
					}
					y /= q;
				}
				if (res == velo((long long)0)) {
					return true;
				}
			}
		}
	}
	return false;
}


long double target_percentage = 28.1L;
long double working_percentage;
polyvelo aap,bbp,ccp,ddp,eep,ffp;
polyvelo pp;
vector <long double> vr;

void check_near_miss_inner(int ior) {
	const long double this_root = vr[ior];
	//cout << "root: " << this_root << endl;
	//cout << endl;
	
	if (this_root < 0.134L || this_root > 0.135L) {
		return;
	}
	
	const fractionvelo fv = find_single_root_fraction(pp, this_root, 1000);
	
	
	vector <velo> vv = continuedfraction(fv.numtor, 1000, 0, 1000);
	
	//cout << vv[0] << endl;
	//cout << vv[1] << endl;
	velo p = vv[0];
	velo q = vv[1];
	// when greater than 1
	const bool neg = p.isNegative();
	p = abs_velo(p);
	p += velo((long long)abs(this_root)) * q;
	p = neg?-p:p;
	aap.display();
	bbp.display();
	ccp.display();
	ddp.display();
	eep.display();
	ffp.display();
	velo aa = evaluate_rational(aap, p, q);
	velo bb = evaluate_rational(bbp, p, q);
	velo cc = evaluate_rational(ccp, p, q);
	velo dd = evaluate_rational(ddp, p, q);
	velo ee = evaluate_rational(eep, p, q);
	velo ff = evaluate_rational(ffp, p, q);
	velo realb = cc * cc - velo(2) * cc * dd - dd * dd;
	velo reali = cc * cc + dd * dd;
	velo reald = cc * cc + velo(2) * cc * dd - dd * dd;
	velo realg = aa * aa - velo(2) * aa * bb - bb * bb;;
	velo reale = aa * aa + bb * bb;
	velo realc = aa * aa + velo(2) * aa * bb - bb * bb;
	velo realf = ee * ee - velo(2) * ee * ff - ff * ff;
	velo reala = ee * ee + ff * ff;
	velo realh = ee * ee + velo(2) * ee * ff - ff * ff;
	vector<velo> vvvvvv;
	vvvvvv.push_back(reala);
	vvvvvv.push_back(realb);
	vvvvvv.push_back(realc);
	vvvvvv.push_back(reald);
	vvvvvv.push_back(reale);
	vvvvvv.push_back(realf);
	vvvvvv.push_back(realg);
	vvvvvv.push_back(realh);
	vvvvvv.push_back(reali);
	const velo their_gcd = gcd_velo(vvvvvv);
	reala /= their_gcd;
	realb /= their_gcd;
	realc /= their_gcd;
	reald /= their_gcd;
	reale /= their_gcd;
	realf /= their_gcd;
	realg /= their_gcd;
	realh /= their_gcd;
	reali /= their_gcd;
	velo a = reala * reala;
	velo b = realb * realb;
	velo c = realc * realc;
	velo d = reald * reald;
	velo e = reale * reale;
	velo f = realf * realf;
	velo g = realg * realg;
	velo h = realh * realh;
	velo i = reali * reali;
	velo sum_diag_tlbr = a + e + i;
	velo sum_diag_bltr = g + e + c;
	velo diff = sum_diag_tlbr - sum_diag_bltr;
	//cout << endl;
	const long double mypercentage = 100.0L * (1.0L - log_base_ten(diff) / log_base_ten(sum_diag_bltr));
	const long mydigits = 1 + (long)log_base_ten(sum_diag_bltr);
	//cout << mypercentage << " % and " << mydigits << " digits" << endl;
	//cout << endl;
	
	mtx.lock();
	const bool displaythis = true;
	if (displaythis) {
		cout << their_gcd << endl;
		cout << endl;
		cout << endl;
		cout << reala.stringify(true) << endl;
		cout << realb.stringify(true) << endl;
		cout << realc.stringify(true) << endl;
		cout << reald.stringify(true) << endl;
		cout << reale.stringify(true) << endl;
		cout << realf.stringify(true) << endl;
		cout << realg.stringify(true) << endl;
		cout << realh.stringify(true) << endl;
		cout << reali.stringify(true) << endl;
		cout << endl;
		cout << a.stringify(true) << endl;
		cout << b.stringify(true) << endl;
		cout << c.stringify(true) << endl;
		cout << d.stringify(true) << endl;
		cout << e.stringify(true) << endl;
		cout << f.stringify(true) << endl;
		cout << g.stringify(true) << endl;
		cout << h.stringify(true) << endl;
		cout << i.stringify(true) << endl;
		cout << endl;
		velo sum_col_lef = a + d + g;
		velo sum_col_mid = b + e + h;
		velo sum_col_rig = c + f + i;
		velo sum_lin_top = a + b + c;
		velo sum_lin_mid = d + e + f;
		velo sum_lin_bot = g + h + i;
		cout << sum_col_lef.stringify(true) << endl;
		cout << sum_col_mid.stringify(true) << endl;
		cout << sum_col_rig.stringify(true) << endl;
		cout << sum_lin_top.stringify(true) << endl;
		cout << sum_lin_mid.stringify(true) << endl;
		cout << sum_lin_bot.stringify(true) << endl;
		cout << sum_diag_tlbr.stringify(true) << endl;
		cout << sum_diag_bltr.stringify(true) << endl;
		cout << endl;
		cout << diff << endl;
		cout << diff.getMagnitudeOrder() << endl;
		cout << sum_diag_bltr.getMagnitudeOrder() << endl;
		cout << endl;
	}
	cout << "root " << this_root << " yields " << mypercentage << " % with " << mydigits << " digits" << endl;
	//cout << mydigits << " digits" << endl;
	//cout << endl;
	if (mypercentage > working_percentage && mydigits > 2) {
		working_percentage = mypercentage;
	}
	mtx.unlock();
}

bool check_near_miss(vector <polyvelo> vvp) { // must be size 6

	working_percentage = 0.0L;
	
	for (int mirroring = 0; mirroring < 2; mirroring++) {
	
		for (int i = 0; i < 3; i++) { // permutations
			cout << "mirror " << mirroring << ", permutation " << i << endl;
			//cout << endl;
			aap = vvp[(2*i) % 6];
			bbp = vvp[(2*i+1) % 6];
			ccp = vvp[(2*i+2) % 6];
			ddp = vvp[(2*i+3) % 6];
			eep = vvp[(2*i+4) % 6];
			ffp = vvp[(2*i+5) % 6];
			
			//polyvelo tmpp;
			//tmpp = tmpp.mul_poly(tmpp.mul_poly(aap,bbp), tmpp.sub_poly(aap.squared(), bbp.squared()));
			//tmpp.display();
			//continue;
			
			polyvelo aabb,ccdd,eeff;
			aabb = aabb.add_poly(aap.squared(), bbp.squared()).squared();
			ccdd = ccdd.add_poly(ccp.squared(), ddp.squared()).squared();
			eeff = eeff.add_poly(eep.squared(), ffp.squared()).squared();
			
			pp = pp.sub_poly(pp.add_poly(ccdd,eeff), pp.add_poly(aabb,aabb));
			pp.display();
			pp.simplify();
			pp.display();
			//const long double r = find_single_root_real(pp, 0);
			//cout << r << endl;
			//cout << evaluate_real(pp, r) << endl;
			vr = find_many_root_real(pp);
			
			cout << "found " << vr.size() << " roots" << endl;
			if (vr.size() > 0/*8*/) {
				for (int indexofroot = 0; indexofroot < vr.size(); indexofroot++) {
					cout << vr[indexofroot] << ", ";
				}
				cout << endl;
				//return true; // not the actual purpose but meh
			}
			
			vector <thread> vtchecknearmiss;
			
			for (int indexofroot = 0; indexofroot < vr.size(); indexofroot++) {
				vtchecknearmiss.push_back(thread(check_near_miss_inner, indexofroot));
			}
			for (int indexofroot = 0; indexofroot < vr.size(); indexofroot++) {
				vtchecknearmiss[indexofroot].join();
			}
			
		} // ! permutation
		
		if (mirroring == 0) {
			for (int j = 0; j < 6; j++) {
				vvp[j].mirror();
			}
		}
		
	} // ! mirroring
	
	return working_percentage > target_percentage;
	
}



bool find_parametrization(const long long a0global_input, const long long b0global_input, const long long c0global_input, const long long d0global_input, const long long e0global_input, const long long f0global_input) {
	
	const long long m = assess_sextuplet(a0global_input, b0global_input, c0global_input, d0global_input, e0global_input, f0global_input);
	cout << "(m=" << m << ")" << endl;
	maxco = m / 4;
	maxco1 = m / 2;
	if (m < 100) {
		maxco = m / 2;
		maxco1 =  m;
	}
	if (m < 50) {
		maxco = m;
		maxco1 = 2 * m;
	}
	
	
	higherlevelcontinue = true;
	
	a2b2viewed.clear();
	
	while (higherlevelcontinue) {
		
		overids.clear();
	
		// first four polynomials
		
		w[0] = 0;
		w[1] = 0;
	
		goingon = true;
	
		a0global = a0global_input;
		b0global = b0global_input;
		c0global = c0global_input;
		d0global = d0global_input;
		
		e2global = 0;
		f2global = 0;
		e1global = 0;
		f1global = 0;
	
		a0v = a0global;
		b0v = b0global;
		c0v = c0global;
		d0v = d0global;
	
		coeffgamma = (3 * a0global * a0global - b0global * b0global) * b0global;
		coeffdelta = a0global * (a0global * a0global - 3 * b0global * b0global);
	
		troisccmoinsddfoisd = (3 * c0global * c0global - d0global * d0global) * d0global;
		ccmoinstroisddfoisc = (c0global * c0global - 3 * d0global * d0global) * c0global;
	
		a0sqmoinsb0sq = a0v*a0v - b0v*b0v;
		c0sqmoinsd0sq = c0v*c0v - d0v*d0v;
	
		troisaambbzero = 3*(a0global*a0global-b0global*b0global);
		troisabzero = 3*a0global*b0global;
		
		const long myhardwareconcurrency = thread::hardware_concurrency();
		
		vector <thread> vt;
		vector <thread::id> tids;
		
		while (goingon) {
			
			//cout << w[0] << " _ " << w[1] << " _ " << w[2] << " _ " << w[3] <<endl;
			
			if (vt.size() >= myhardwareconcurrency) {
				//cout << "pool capacity reached" << endl;
				// wait for the fastest to finish
				bool waiting = true;
				while (waiting) {
					mtx.lock();
					waiting = overids.empty();
					mtx.unlock();
					this_thread::sleep_for(chrono::milliseconds(2));
				}
				// search for which one it was and manage
				mtx.lock();
				thread::id endedid = overids[0];
				overids.erase(overids.begin());
				bool found = false;
				for (long it = 0; it < vt.size(); it++) {
					if (tids[it] == endedid) {
						//cout << "thread " << tids[it] << " is over" << endl;
						vt[it].join();
						vt.erase(vt.begin() + it);
						tids.erase(tids.begin() + it);
						found = true;
						break;
					}
				}
				if (!found) {
					cout << "finished thread not found" << endl;
					return false;
				}
				mtx.unlock();
			}
			
			vt.push_back(thread(myfuncex));
			tids.push_back((vt.back()).get_id());
			//cout << "thread " << tids.back() << " just started" << endl;
			
			this_thread::sleep_for(chrono::milliseconds(1));
	
		}
		
		// voiture-balai
		for (long it = 0; it < vt.size(); it++) {
			vt[it].join();
		}
		
		
		
		
		
		
		// last two polynomials
		
		goingon = true;
	
		w[0] = 0;
	
		a0global = e0global_input;
		b0global = f0global_input;
	
		a0v = a0global;
		b0v = b0global;
	
		coeffgamma = (3 * a0global * a0global - b0global * b0global) * b0global;
		coeffdelta = a0global * (a0global * a0global - 3 * b0global * b0global);
	
		a0sqmoinsb0sq = a0v*a0v - b0v*b0v;
	
		troisaambbzero = 3*(a0global*a0global-b0global*b0global);
		troisabzero = 3*a0global*b0global;
	
		overids.clear();
		vt.clear();
		tids.clear();
		
		
		while (goingon && higherlevelcontinue) {
			
			if (vt.size() >= myhardwareconcurrency) {
				//cout << "pool capacity reached" << endl;
				// wait for the fastest to finish
				bool waiting = true;
				while (waiting) {
					mtx.lock();
					waiting = overids.empty();
					mtx.unlock();
					this_thread::sleep_for(chrono::milliseconds(1));
				}
				// search for which one it was and manage
				mtx.lock();
				thread::id endedid = overids[0];
				overids.erase(overids.begin());
				bool found = false;
				for (long it = 0; it < vt.size(); it++) {
					if (tids[it] == endedid) {
						//cout << "thread " << tids[it] << " is over" << endl;
						vt[it].join();
						vt.erase(vt.begin() + it);
						tids.erase(tids.begin() + it);
						found = true;
						break;
					}
				}
				if (!found) {
					cout << "finished thread not found" << endl;
					return false;
				}
				mtx.unlock();
			}
			
			vt.push_back(thread(myfuncforthird));
			tids.push_back((vt.back()).get_id());
			//cout << "thread " << tids.back() << " just started" << endl;
			
			this_thread::sleep_for(chrono::milliseconds(1));
	
		}
		
		// voiture-balai
		for (long it = 0; it < vt.size(); it++) {
			vt[it].join();
		}
		
		
		
		// check result
		
		a0global = a0global_input;
		b0global = b0global_input;
		e0global = e0global_input;
		f0global = f0global_input;
		
		polyvelo dummypoly, a, b, c, d, e, f;
		a.setCoeff(0, a0global);
		b.setCoeff(0, b0global);
		c.setCoeff(0, c0global);
		d.setCoeff(0, d0global);
		e.setCoeff(0, e0global);
		f.setCoeff(0, f0global);
		a.setCoeff(1, a1global);
		b.setCoeff(1, b1global);
		c.setCoeff(1, c1global);
		d.setCoeff(1, d1global);
		e.setCoeff(1, e1global);
		f.setCoeff(1, f1global);
		a.setCoeff(2, a2global);
		b.setCoeff(2, b2global);
		c.setCoeff(2, c2global);
		d.setCoeff(2, d2global);
		e.setCoeff(2, e2global);
		f.setCoeff(2, f2global);
		
		
		polyvelo lhs = dummypoly.mul_poly(dummypoly.mul_poly(a, b), dummypoly.sub_poly(dummypoly.mul_poly(a, a), dummypoly.mul_poly(b, b)));
		polyvelo mhs = dummypoly.mul_poly(dummypoly.mul_poly(c, d), dummypoly.sub_poly(dummypoly.mul_poly(c, c), dummypoly.mul_poly(d, d)));
		polyvelo rhs = dummypoly.mul_poly(dummypoly.mul_poly(e, f), dummypoly.sub_poly(dummypoly.mul_poly(e, e), dummypoly.mul_poly(f, f)));
		//lhs.display();
		//mhs.display();
		//rhs.display();
		if (e2global != 0 || f2global != 0 || e1global != 0 || f1global != 0) { // third polynomial was actually found
			if (!dummypoly.eq_poly(lhs, mhs) || !dummypoly.eq_poly(lhs, rhs)) {
				cout << "Wrong result" << endl;
				return false;
			}
			
			// check difference
			polyvelo r = (dummypoly.add_poly(a.squared(), b.squared())).squared();
			polyvelo s = (dummypoly.add_poly(c.squared(), d.squared())).squared();
			polyvelo t = (dummypoly.add_poly(e.squared(), f.squared())).squared();
			
			polyvelo x = dummypoly.add_poly(dummypoly.sub_poly(t, s), dummypoly.sub_poly(r, s));
			polyvelo y = dummypoly.add_poly(dummypoly.sub_poly(s, t), dummypoly.sub_poly(r, t));
			polyvelo z = dummypoly.add_poly(dummypoly.sub_poly(s, r), dummypoly.sub_poly(t, r));
			
			if (poly_has_rational_roots(x) || poly_has_rational_roots(y) || poly_has_rational_roots(z)) {
				cout << "YESSS" << endl;
				return true;
			} else {
				cout << "nope" << endl;
				
				/*
				cout << "now checking approximation(s)" << endl;
				vector <polyvelo> vvp;
				vvp.push_back(a);
				vvp.push_back(b);
				vvp.push_back(c);
				vvp.push_back(d);
				vvp.push_back(e);
				vvp.push_back(f);
				if (check_near_miss(vvp)) {
					cout << "yes" << endl;
					return true;
				} else {
					cout << "neither" << endl;
				}
				*/
				
			}
		}
		
		
	} //!while
	
	return false;
	
}

