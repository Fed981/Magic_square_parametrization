// stands for "polynomial"

#ifndef __POLYVELO_H__
#define __POLYVELO_H__

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

#include "velo.hpp"

using namespace std;

// degree is the max index of a coefficient

class polyvelo {


private:

    vector <velo> vCoeffs;
    
public:
	
	void reset() {
		this->vCoeffs.clear();
	}
	
	void make(vector <velo> &v) {
		this->vCoeffs = v;
	}
	
	void setDeg(long lDeg) {
		if (lDeg > this->getDeg()) {
			this->vCoeffs.resize(lDeg + 1);
		}
	}
	
	long getDeg() const {
		return this->vCoeffs.size() - 1;
	}

    void setCoeff(long lDeg, velo llNewValue) {
        if (lDeg >= 0) {
        	if (lDeg >= this->vCoeffs.size()) {
        		this->vCoeffs.resize(lDeg + 1);
			}
            vCoeffs[lDeg] = llNewValue;
        }
    }
    
    velo getCoeff(long lDeg) const {
    	if (lDeg >= 0 && lDeg < this->vCoeffs.size()) {
            return this->vCoeffs[lDeg];
        } else {
        	return 0;
		}
	}
	
	void simplify() {
		velo dvdr = gcd_velo(this->vCoeffs);
		for (long i = 0; i < this->vCoeffs.size(); i++) {
			this->vCoeffs[i] /= dvdr;
		}
	}
    
    // operations

    polyvelo add_poly(const polyvelo p, const polyvelo q) {
        const long np = p.getDeg();
        const long nq = q.getDeg();
        const long n = np>nq?np:nq;
        polyvelo out;
        out.setDeg(n);
        for (long i = 0; i <= n; i++) {
        	out.setCoeff(i, p.getCoeff(i) + q.getCoeff(i));
		}
		return out;
    }
    
    polyvelo sub_poly(const polyvelo p, const polyvelo q) {
        const long np = p.getDeg();
        const long nq = q.getDeg();
        const long n = np>nq?np:nq;
        polyvelo out;
        out.setDeg(n);
        for (long i = 0; i <= n; i++) {
        	out.setCoeff(i, p.getCoeff(i) - q.getCoeff(i));
		}
		return out;
    }

	polyvelo mul_poly(const polyvelo p, const polyvelo q) {
        const long np = p.getDeg();
        const long nq = q.getDeg();
        polyvelo out;
        out.setDeg(np + nq);
        for (long i = 0; i <= np; i++) {
        	for (long j = 0; j <= nq; j++) {
        		out.setCoeff(i + j, out.getCoeff(i + j) + p.getCoeff(i) * q.getCoeff(j));
			}
		}
		return out;
    }
    
    polyvelo squared() {
    	return mul_poly(*this, *this);
	}

	bool eq_poly(const polyvelo p, const polyvelo q) {
		const long np = p.getDeg();
        const long nq = q.getDeg();
        if (np != nq) {
        	return false;
		}
		for (long i = 0; i <= np; i++) {
			if (p.getCoeff(i) != q.getCoeff(i)) {
				return false;
			}
		}
		return true;
	}
	
	void display() const {
		for (long i = this->getDeg(); i >= 0; i--) {
			cout << this->getCoeff(i).stringify(true);
			if (i > 0) {
				cout << ", ";
			}
		}
		cout << endl;
	}
	
	void rescaleconstant(long long k0) {
		const velo k_as_velo = k0;
		if (abs_velo(this->vCoeffs[0]) <= abs_velo(k_as_velo)) {
			const velo scale = k_as_velo / this->vCoeffs[0];
			//cout << "scale = " << scale << ", k = " << k_as_velo << ", cst coeff = " << this->vCoeffs[0] <<endl;
			for (long i = 0; i < this->vCoeffs.size(); i++) {
				this->vCoeffs[i] *= scale;
			}
		}
	}
	
	void mirror() {
		vector <velo> vDuplicate = this->vCoeffs;
		for (long i = 0; i < this->vCoeffs.size(); i++) {
			this->vCoeffs[i] = vDuplicate[this->vCoeffs.size() - i - 1];
		}
	}

};

// functions outside the class

polyvelo derivative_single(const polyvelo p) {
	polyvelo q;
	q.setCoeff(0, 0);
	for (long i = 1; i <= p.getDeg(); i++) {
		q.setCoeff(i-1, velo(i) * p.getCoeff(i));
	}
	return q;
}

polyvelo derivative_multiple(const polyvelo p, const long order) {
	polyvelo q = p;
	for (long suborder = 0; suborder < order; suborder++) {
		q = derivative_single(q);
	}
	return q;
}

velo evaluate_rational(const polyvelo p, const velo a, const velo b) {
	velo d = 1;
	for (long i = 0; i < p.getDeg(); i++) {
		d *= b;
	}
	//cout << "in ev_ra: " << d.getMagnitudeOrder() << endl;
	velo sum = 0;
	velo c = 1;
	for (long i = 0; i <= p.getDeg(); i++) {
		sum += c*d*p.getCoeff(i);
		c*= a;
		d /= b;
	}
	return sum;
}

long double evaluate_real(const polyvelo p, const long double u) {
	long double sum = 0;
	long double c = 1;
	for (long i = 0; i <= p.getDeg(); i++) {
		sum += c*(p.getCoeff(i)).toReal();
		c*= u;
	}
	return sum;
}



long double find_single_root_real(const polyvelo p, const long double startingpoint) {
	// Newton's method
	polyvelo pd = derivative_single(p);
	long double root = startingpoint;
	long double previous = root;
	long it = 0;
	if (evaluate_real(pd, root) == 0.0L) {
		root = startingpoint + 1.0L;
		previous = root;
	}
	do {
		previous = root;
		//cout << root << endl;
		root = root - evaluate_real(p, root) / evaluate_real(pd, root);
		it++;
	} while (previous != root && it < 1000);
	return root;
}

vector <long double> find_many_root_real(const polyvelo p) {
	const long double tol = 1e-4;
	vector <long double> out;
	const long double r_init = find_single_root_real(p, 0.0L);
	if (abs(evaluate_real(p, r_init)) > tol) { // not a root
		return out;
	}
	out.push_back(r_init);
	polyvelo pd = derivative_single(p);
	const long double rd_init = find_single_root_real(pd, r_init);
	const long double relevant_distance = abs(rd_init - r_init);
	const long extreme_extent = 100;
	for (long extent = -extreme_extent; extent <= extreme_extent; extent++) {
		const long double current_starting_point = r_init + ((long double)extent) * relevant_distance;
		const long double r_trial = find_single_root_real(p, current_starting_point);
		// check if not already
		const long n = out.size();
		bool already = false;
		for (long k = 0; k < n; k++) {
			if (abs(out[k] - r_trial) < tol) {
				already = true;
				break;
			}
		}
		if (!already) {
			out.push_back(r_trial);
		}
	}
	return out;
}

struct fractionvelo{
	velo numtor;
	velo dentor;
};

fractionvelo find_single_root_fraction(const polyvelo p, const long double foundroot, const long extraprecision) {
	fractionvelo out;
	long double absfoundroot = abs(foundroot);
	long initialpostcommadigits = 4;
	const long poweroften = (long)log10(absfoundroot);
	if (poweroften > 0) {
		initialpostcommadigits -= poweroften;
	}
	long long denval = 1;
	for (long tmpval = 0; tmpval < initialpostcommadigits; tmpval++) {
		absfoundroot *= 10;
		denval *= 10;
	}
	out.numtor = (long long)absfoundroot;
	out.dentor = denval;
	const bool neg = (foundroot < 0.0L);
	//cout << out.numtor << endl;
	//cout << out.dentor << endl;
	for (long digit = 0; digit < extraprecision; digit++) {
		out.dentor *= velo(10);
		out.numtor *= velo(10);
		velo juxtaposition = out.numtor - velo(1);
		if (evaluate_rational(p, neg?-out.numtor:out.numtor, out.dentor).isNegative() ^ evaluate_rational(p, neg?-juxtaposition:juxtaposition, out.dentor).isNegative()) {
			out.numtor = juxtaposition;
		}
		juxtaposition = out.numtor + velo(1);
		int it = 0;
		const int itx = 100;
		while ( it < itx && !(evaluate_rational(p, neg?-out.numtor:out.numtor, out.dentor).isNegative() ^ evaluate_rational(p, neg?-juxtaposition:juxtaposition, out.dentor).isNegative()) ) {
			out.numtor = juxtaposition;
			juxtaposition = out.numtor + velo(1);
			it++;
		}
		if (it == itx) { // forget it, we're too far
			break;
		}
	}
	if (neg) {
		out.numtor.negate();
	}
	//cout << out.numtor << endl;
	//cout << out.dentor << endl;
	return out;
}

#endif //!__POLYVELO_H__


