// by François Federspiel

// stands for "very long"

// NB: long for indices and digit counts, long long for numbers

// all bitwise operations are NOT supported

// v2: optimizations

#ifndef __VELO_H__
#define __VELO_H__

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cmath>
#include <ctime>

using namespace std;

const long lDigitsPerSlice = 18;

const long long llSliceValueLimit = 1000000000000000000;
const long long llSqrtValueLimit = 1000000000;

//const unsigned __int128 five_to_the_eighteen = 3814697265625;
//#define LL_DIVIDEBYBILLION(__lltodivide__) ( (long long)(((unsigned __int128)4722366482870 * (unsigned __int128)__lltodivide__) >> 72) ); // NO, NO, NOOO !!!


class velo {


private:

    vector <long long> vData;
	
	bool bNegative;


    inline void setSliceValue(const long lSlice, const long long llNewValue) {
        //if (lSlice >= 0 && lSlice < this->getSliceCount() && llNewValue >= 0) {
            //vData[lSlice] = (lSlice==0&&vData[lSlice]<0)?-llNewValue:llNewValue;
			vData[lSlice] = llNewValue;
        //}
    }
    
    inline void assignFromNumber(const long long llEntry) {
		this->bNegative = (llEntry < 0);
        this->vData.clear();
        const long long absentry = abs(llEntry);
        if (absentry < llSliceValueLimit) {
            this->vData.push_back(abs(llEntry));
        } else {
			this->vData.push_back(absentry % llSliceValueLimit);
			this->vData.push_back(absentry / llSliceValueLimit);
		}
		if (absentry > 5*llSliceValueLimit) {
			throw runtime_error("integer assignment overflow");
		}
    }
	
	inline void buildFromNumber(const long long llEntry) {
		this->bNegative = (llEntry < 0);
        const long long absentry = abs(llEntry);
        if (absentry < llSliceValueLimit) {
            this->vData.push_back(abs(llEntry));
        } else {
			this->vData.push_back(absentry % llSliceValueLimit);
			this->vData.push_back(absentry / llSliceValueLimit);
		}
		if (absentry > 5*llSliceValueLimit) {
			throw runtime_error("integer assignment overflow");
		}
    }

    void makeFromString(const string sEntry, const bool bNegation) {
        this->vData.clear();
        string sEntryCopy = sEntry;
        while (sEntryCopy.substr(0, 1) == "0") {
        	sEntryCopy.erase(0, 1);
		}
        long lTotalLength = sEntryCopy.length();
        if (lTotalLength == 0) {
            this->vData.push_back(0);
        } else {
            // check the entry
            for (long lChar = lTotalLength - 1; lChar > 0; lChar--) {
                if (!isdigit(sEntryCopy[lChar])) {
                    this->vData.push_back(0);
                    return;
                }
            }
            // make slices
            long lSliceCount = (lTotalLength - 1) / lDigitsPerSlice;
            string sCurrentStringSlice = "";
            if (lSliceCount > 0) {
                for (long lSliceIndex = 1; lSliceIndex <= lSliceCount; lSliceIndex++) {
                    sCurrentStringSlice = sEntryCopy.substr(lTotalLength - lSliceIndex * lDigitsPerSlice, lDigitsPerSlice);
                    this->vData.push_back(strtoull(sCurrentStringSlice.c_str(), NULL, 10));
                }
            }
            sCurrentStringSlice = sEntryCopy.substr(0, lTotalLength - lSliceCount * lDigitsPerSlice);
            this->vData.push_back(strtoull(sCurrentStringSlice.c_str(), NULL, 10));
        }
        this->bNegative = bNegation;
		if (this->isNull()) {
			this->bNegative = false;
		}
    }

    inline void assignFromClone(const velo * pvlInput) {
		/*
        this->vData.clear();
        const long lSliceCount = pvlInput->getSliceCount();
		this->vData.reserve(lSliceCount);
        for (long lSliceIndex = 0; lSliceIndex < lSliceCount; lSliceIndex++) {
            this->vData.push_back(pvlInput->getSliceValue(lSliceIndex));
        }
		*/
		this->vData = pvlInput->vData;
		this->bNegative = pvlInput->bNegative;
    }
    
    // operations

	/*
    inline void increment(void) {
        velo vlDummy(1);
        this->addition(&vlDummy);
    }

    inline void decrement(void) {
        velo vlDummy(1);
        this->subtraction(&vlDummy);
    }
	*/
	
    
    // operations with copy
    
	/*
    velo createAdd(const velo * pvlAdded) {
    	velo vlOutput;
		vlOutput.assignFromClone(this);
		vlOutput.addition(pvlAdded);
		return vlOutput;
	}
	
	velo createSubtract(const velo * pvlSubtracted) {
    	velo vlOutput;
		vlOutput.assignFromClone(this);
		vlOutput.subtraction(pvlSubtracted);
		return vlOutput;
	}
	
	velo createMultiply(const velo * pvlMultiplied) {
    	velo vlOutput;
		vlOutput.assignFromClone(this);
		vlOutput.multiplication(pvlMultiplied);
		return vlOutput;
	}
	
	velo createDivide(const velo * pvlDivider) {
    	velo vlOutput;
		vlOutput.assignFromClone(this);
		vlOutput.division(pvlDivider);
		return vlOutput;
	}
	
	velo createRemain(const velo * pvlDivider) {
    	velo vlOutput;
		vlOutput.assignFromClone(this);
		vlOutput.modulo(pvlDivider);
		return vlOutput;
	}
	*/
	
	inline void shifting(const long lAbsoluteAmount, const bool bRemove) { // otherwise insert
	    if (lAbsoluteAmount != 0) {
	        if (bRemove) {
				this->vData.erase(this->vData.begin(), this->vData.begin() + lAbsoluteAmount);
			} else {
				for (long lSliceIndex = 0; lSliceIndex < lAbsoluteAmount; lSliceIndex++) {
					this->vData.insert(this->vData.begin(), 0);
				}
			}
	    }
	}


    // comparisons

	/*
    bool equalto(const velo * pvlOther) const {
        if (this->isNegative() != pvlOther->isNegative() || this->getSliceCount() != pvlOther->getSliceCount()) {
            return false;
        } else {
            for (long lSliceIndex = 0; lSliceIndex < this->getSliceCount(); lSliceIndex++) {
                if (this->getSliceValue(lSliceIndex) != pvlOther->getSliceValue(lSliceIndex)) {
                    return false;
                }
            }
            return true;
        }
    }

    bool notequal(const velo * pvlOther) const {
        return !this->equalto(pvlOther);
    }
	*/
	
	inline bool greaterthanabsolute(const velo * pvlOther) const {
        if (this->getSliceCount() != pvlOther->getSliceCount()) {
        	return (this->getSliceCount() > pvlOther->getSliceCount());
        }
        for (long lSliceIndex = this->getSliceCount() - 1; lSliceIndex >= 0; lSliceIndex--) {
			if ((this->getSliceValue(lSliceIndex) != pvlOther->getSliceValue(lSliceIndex))) {
				return (this->getSliceValue(lSliceIndex) > pvlOther->getSliceValue(lSliceIndex));
			}
        }
        return false;
    }
	
	inline bool lowerthanabsolute(const velo * pvlOther) const {
        if (this->getSliceCount() != pvlOther->getSliceCount()) {
        	return (this->getSliceCount() < pvlOther->getSliceCount());
        }
        for (long lSliceIndex = this->getSliceCount() - 1; lSliceIndex >= 0; lSliceIndex--) {
			if ((this->getSliceValue(lSliceIndex) != pvlOther->getSliceValue(lSliceIndex))) {
				return (this->getSliceValue(lSliceIndex) < pvlOther->getSliceValue(lSliceIndex));
			}
        }
        return false;
    }

	/*
    bool greaterthan(const velo * pvlOther, const bool bMaybeEqual = false) const {
		const bool neg = this->isNegative();
        if (neg != pvlOther->isNegative()) {
            return !neg;
        }
        if (this->getSliceCount() != pvlOther->getSliceCount()) {
			return ((this->getSliceCount() > pvlOther->getSliceCount()) ^ neg);
        }
        for (long lSliceIndex = this->getSliceCount() - 1; lSliceIndex >= 0; lSliceIndex--) {
			if ((this->getSliceValue(lSliceIndex) != pvlOther->getSliceValue(lSliceIndex))) {
				return (this->getSliceValue(lSliceIndex) > pvlOther->getSliceValue(lSliceIndex)) ^ neg;
			}
        }
        return bMaybeEqual && (*this == *pvlOther);
    }

    bool greaterequal(const velo * pvlOther) const {
        return this->greaterthan(pvlOther, true);
    }

    bool smallerthan(const velo * pvlOther, const bool bMaybeEqual = false) const {
        return !this->greaterthan(pvlOther, !bMaybeEqual);
    }

    bool smallerequal(const velo * pvlOther) const {
        return this->smallerthan(pvlOther, true);
    }
	*/
   


   
public:


    velo() {
        //this->vData.clear();
        this->vData.push_back(0);
		this->bNegative = false;
    }

    ~velo() {
		/*
		for (long lSliceIndex = 0; lSliceIndex < this->getSliceCount(); lSliceIndex++) {
			if (this->getSliceValue(lSliceIndex) < 0) {
				throw runtime_error("negative chunk value");
			}
		}
		*/
        //this->vData.clear();
    }


    // inputs

	velo & operator=(const velo& vlOther) {
	    this->assignFromClone(&vlOther);
	    return *this;
	}

	velo(const velo& vlOther) {
	    this->assignFromClone(&vlOther);
	}
	
	velo(const string& sOther, const bool bNegation) {
	    this->makeFromString(sOther, bNegation);
	}
	
	velo & operator=(const long long& llOther) {
	    this->assignFromNumber(llOther);
	    return *this;
	}
	
	velo(const long long& llOther) {
	    this->buildFromNumber(llOther);
	}

    // outputs
    
    inline bool isNegative(void) const {
        return this->bNegative;
    }
    
    inline void negate(void) {
        this->bNegative = !this->bNegative;
    }
    
    inline long double getSignificant(void) const {
		// see branchless below
		/*
		// get highest slice
        long double ldOutput = (long double) this->getSliceValue(this->getSliceCount() - 1);
        // and the second highest for more precision
        if (this->getSliceCount() > 1) {
            ldOutput *= (long double) llSliceValueLimit;
            ldOutput += (long double) this->getSliceValue(this->getSliceCount() - 2);
        }
		*/
        // and one more ???
		/*
        if (this->getSliceCount() > 2) {
            ldOutput *= (long double) llSliceValueLimit;
            ldOutput += (long double) this->getSliceValue(this->getSliceCount() - 3);
        }
		*/
		// negative case unnecessary
		/*
        if (this->isNegative()) {
            ldOutput = -ldOutput;
        }
		*/
        //return ldOutput;
		return (this->getSliceCount() > 1) ? ((long double)llSliceValueLimit) * ((long double)this->getSliceValue(this->getSliceCount() - 1)) + ((long double)this->getSliceValue(this->getSliceCount() - 2)) : ((long double)this->getSliceValue(0));
	}

    string stringify(bool bEverything) const {
        string sOutput = "";
        if (this->getSliceCount() > 1) {
        	long long llSlice = this->getSliceValue(this->getSliceCount() - 1);
    		stringstream ss;
    		ss << llSlice;
        	sOutput = sOutput + ss.str();
			
        }
        long lExtraSlice = 0;
        if (this->getSliceCount() > 4) {
        	lExtraSlice = 1;
        	long long llSlice = this->getSliceValue(this->getSliceCount() - 2);
        	stringstream ss;
    		ss << llSlice;
    		string s = ss.str();
    		while (s.length() < 18) {
    			s = "0" + s;
			}
        	sOutput = sOutput + s;
    	}
        if (this->getSliceCount() > 2) {
            if (bEverything) {
                for (long lSliceIndex = this->getSliceCount() - 2 - lExtraSlice; lSliceIndex >= 1; lSliceIndex--) {
                	long long llSlice = this->getSliceValue(lSliceIndex);
		        	stringstream ss;
		    		ss << llSlice;
		    		string s = ss.str();
		    		while (s.length() < 18) {
		    			s = "0" + s;
					}
		        	sOutput = sOutput + s;
                }
            } else {
                sOutput = sOutput + "...";
            }
        }
        long long llSlice = this->getSliceValue(0);
		stringstream ss;
		ss << llSlice;
		string s = ss.str();
		while (s.length() < 18 && this->getSliceCount() > 1) {
			s = "0" + s;
		}
    	sOutput = sOutput + s;
        if (this->isNegative()) {
            sOutput = "-" + sOutput;
        }
        return sOutput;
    }

    string getMagnitudeOrder(void) {
    	stringstream ss;
		if (this->isNegative()) {
            ss << "-";
        }
    	ss << setprecision(17) << this->getSignificant();
        string sSignificant = ss.str();
        string sOutput = sSignificant;
        sOutput = sSignificant.substr(0, sSignificant.find("e"));
		if (sSignificant.find("e") != string::npos) {
			long lPower = strtol(sSignificant.substr(sSignificant.find("e")+2, string::npos).c_str(), NULL, 10);
			if (this->getSliceCount() > 1) {
				lPower += (this->getSliceCount() - 2) * 18;
			}
			stringstream sss;
			sss << lPower;
			sOutput += "e+";
			sOutput += sss.str();
		}
        return sOutput;
    }
    
    inline long long getSliceValue(const long lSlice) const {
        //if (lSlice >= 0 && lSlice < this->getSliceCount()) {
            return this->vData[lSlice];
        //} else {
        //    return 0;
        //}
    }
	
	inline long getSliceCount(void) const {
        return this->vData.size();
    }

	void display() const {
		cout << this->stringify(true) << endl;
	}
    

    // overloads
    
    velo operator+(const velo& vlOther) const {
		//cout << "DEBUG: adding " << *this << " and " << vlOther << endl;
	    const bool bSameSign = ((this->isNegative() == vlOther.isNegative()));
	    const bool bInitiallyNegative = this->isNegative();
	    /*
        const long long pm = bSameSign ? 1 : -1;
        velo vlSelf = *this;
        velo vlTarget = vlOther;
        bool bChangeSign = false;
		// determine sign of the result and maybe exchange
        if (!bSameSign && !this->greaterthanabsolute(&vlOther)) {
			bChangeSign = true;
			// exchange them
			vlTarget = *this;
			vlSelf = vlOther;
		}
		*/
		const bool bChangeSign = !bSameSign && !this->greaterthanabsolute(&vlOther);
		velo vlSelf = bChangeSign?vlOther:*this;
		const velo vlTarget = bChangeSign?*this:vlOther;
		const long long toaddend = (bSameSign?-llSliceValueLimit:llSliceValueLimit);
		
        const long lLastIndex = vlSelf.getSliceCount();
        const long lLastIndexOther = vlTarget.getSliceCount();
        long long llCarryover = 0;
        for (long lSliceIndex = 0; lSliceIndex < lLastIndex && lSliceIndex < lLastIndexOther; lSliceIndex++) {
            //const long long llSliceResult = vlSelf.getSliceValue(lSliceIndex) + pm * (vlTarget.getSliceValue(lSliceIndex) + llCarryover);
			const long long llSliceResult = vlSelf.getSliceValue(lSliceIndex) + (bSameSign?(vlTarget.getSliceValue(lSliceIndex) + llCarryover):-(vlTarget.getSliceValue(lSliceIndex) + llCarryover));
			llCarryover = bSameSign ? ((long long) (llSliceResult >= llSliceValueLimit)) : ((long long) (llSliceResult < 0));
            //vlSelf.setSliceValue(lSliceIndex, llCarryover == 0 ? llSliceResult : llSliceResult - pm * llSliceValueLimit);
			vlSelf.setSliceValue(lSliceIndex, llCarryover == 0 ? llSliceResult : llSliceResult + toaddend);
        }
        // add remaining slices of other and/or propagate carryover
        if (lLastIndexOther > lLastIndex) {
            for (long lSliceIndex = lLastIndex; lSliceIndex < lLastIndexOther; lSliceIndex++) {
                //const long long llSliceResult = vlTarget.getSliceValue(lSliceIndex) + pm * llCarryover;
				const long long llSliceResult = vlTarget.getSliceValue(lSliceIndex) + (bSameSign?llCarryover:-llCarryover);
                llCarryover = bSameSign ? ((long long) (llSliceResult >= llSliceValueLimit)) : ((long long) (llSliceResult < 0));
                //vlSelf.vData.push_back(llCarryover == 0 ? llSliceResult : llSliceResult - pm * llSliceValueLimit);
				(vlSelf.vData).push_back(llCarryover == 0 ? llSliceResult : llSliceResult + toaddend);
            }
        } else if (lLastIndexOther < lLastIndex) {
        	for (long lSliceIndex = lLastIndexOther; lSliceIndex < lLastIndex; lSliceIndex++) {
	            //const long long llSliceResult = vlSelf.getSliceValue(lSliceIndex) + pm * llCarryover;
				const long long llSliceResult = vlSelf.getSliceValue(lSliceIndex) + (bSameSign?llCarryover:-llCarryover);
	            llCarryover = bSameSign ? ((long long) (llSliceResult >= llSliceValueLimit)) : ((long long) (llSliceResult < 0));
	            //vlSelf.setSliceValue(lSliceIndex, llCarryover == 0 ? llSliceResult : llSliceResult - pm * llSliceValueLimit);
				vlSelf.setSliceValue(lSliceIndex, llCarryover == 0 ? llSliceResult : llSliceResult + toaddend);
	    	}
        }
        // append ultimate carryover
        if (llCarryover != 0) {
            (vlSelf.vData).push_back(llCarryover);
        } else {
			// check leading zeros
			while (vlSelf.vData.back() == 0 && vlSelf.getSliceCount() > 1) {
				vlSelf.vData.pop_back();
			}
		}
		if (bChangeSign) {
			vlSelf.bNegative = !bInitiallyNegative;
		}
		if (vlSelf.isNull()) {
			vlSelf.bNegative = false;
		}
		//cout << "DEBUG: sum is " << vlSelf << endl;
		return vlSelf;
	}

	velo operator-(const velo& vlOther) const {
	    return *this + (-vlOther);
	}
	
	velo operator-() const {
		velo vlOld = *this;
	    vlOld.negate();
	    return vlOld;
	}
	
	velo& operator++() {
	    //this->increment();
	    *this = *this + velo(1);
	    return *this;
	}
	
	velo operator++(int) {
	    velo vlOld = *this;
	    this->operator++();
	    return vlOld;
	}
	
	velo& operator--() {
	    //this->decrement();
	    *this = *this + velo(-1);
	    return *this;
	}
	
	velo operator--(int) {
	    velo vlOld = *this;
	    this->operator--();
	    return vlOld;
	}
	
	velo& operator+=(const velo& vlOther) {
	    //this->addition(&vlOther);
	    *this = *this + vlOther;
	    return *this;
	}
	
	velo& operator-=(const velo& vlOther) {
		//this->subtraction(&vlOther);
		*this = *this - vlOther;
		return *this;
	}
	
	
	velo operator*(const velo& vlOther) const {
		if (this->isNull() || vlOther.isNull()) {
			return velo(0);
		}
		//cout << "DEBUG: multiplying " << *this << " and " << vlOther << endl;
		const long initialSliceCount = this->getSliceCount();
		const long otherSliceCount = vlOther.getSliceCount();
		velo vlOutput;
		//vlOutput.vData.reserve(initialSliceCount+otherSliceCount-1); // useless because of first slice case?
        for (long lSliceIndex = 0; lSliceIndex < otherSliceCount; lSliceIndex++) {
        	velo vlTemporary = *this;
			const long long llMultiplier = vlOther.getSliceValue(lSliceIndex);
			//cout << "DEBUG: starting other slice " << lSliceIndex << " with value " << llMultiplier << endl;
			// times
			if (llMultiplier != 0) {
				//const long long llEffectiveMultiplier = llMultiplier;
				/*
				if (llMultiplier < 0) {
					vlTemporary.negate();
					llEffectiveMultiplier = -llMultiplier;
				}
				*/
				//unsigned __int128 llPartialCarryOver = 0;
				long long llPartialCarryOver = 0;
				
				//const long long alpha = llMultiplier % llSqrtValueLimit;
				const long long beta = llMultiplier / llSqrtValueLimit;
				//const long long beta = LL_DIVIDEBYBILLION(llMultiplier);
				const long long alpha = llMultiplier - beta * llSqrtValueLimit;
				
				//cout << "DEBUG: alpha = " << alpha << ", beta = " << beta << endl;
				
				for (long lSliceIndex_inner = 0; lSliceIndex_inner < initialSliceCount; lSliceIndex_inner++) {
					
					//cout << "DEBUG: starting inner slice " << lSliceIndex_inner << " with value " << vlTemporary.getSliceValue(lSliceIndex_inner) << endl;
					
					//const long long a = vlTemporary.getSliceValue(lSliceIndex_inner) % llSqrtValueLimit;
					const long long b = vlTemporary.getSliceValue(lSliceIndex_inner) / llSqrtValueLimit;
					//const long long b = LL_DIVIDEBYBILLION(vlTemporary.getSliceValue(lSliceIndex_inner));
					const long long a = vlTemporary.getSliceValue(lSliceIndex_inner)  - b * llSqrtValueLimit;
					
					//cout << "DEBUG: a = " << a << ", b = " << b << endl;
					
					long long llPartialResult = alpha * b + beta * a;
					
					//const long long reste = llPartialResult % llSqrtValueLimit;
					const long long quotient = llPartialResult / llSqrtValueLimit;
					//const long long quotient = LL_DIVIDEBYBILLION(llPartialResult);
					const long long reste = llPartialResult - quotient * llSqrtValueLimit;
					
					//cout << "DEBUG: partialresult (intermediate) = " << llPartialResult << endl;
					
					//cout << "DEBUG: quotient = " << quotient << ", reste = " << reste << endl;
					
					llPartialResult = alpha * a + llSqrtValueLimit * reste + llPartialCarryOver;
					
					/*
					llPartialCarryOver = llPartialResult / llSliceValueLimit;
					if (llPartialCarryOver != 0) {
						llPartialResult -= llPartialCarryOver * llSliceValueLimit;
					}
					*/
					
					llPartialCarryOver = 0;
					while (llPartialResult > llSliceValueLimit) {
						llPartialResult -= llSliceValueLimit;
						llPartialCarryOver++;
					}
					
					
					
					
					vlTemporary.setSliceValue(lSliceIndex_inner, llPartialResult);
					llPartialCarryOver += beta * b + quotient;
					
					//cout << "DEBUG: partialresult = " << llPartialResult << ", carryover = " << llPartialCarryOver << endl;
					
					/*
					const unsigned __int128 produit = ((unsigned __int128)llMultiplier) * ((unsigned __int128)vlTemporary.getSliceValue(lSliceIndex_inner)) + llPartialCarryOver;
					//llPartialCarryOver = produit / ((unsigned __int128)llSliceValueLimit);
					llPartialCarryOver = (produit >> lDigitsPerSlice) / five_to_the_eighteen;
					vlTemporary.setSliceValue(lSliceIndex_inner, (long long)(produit - llPartialCarryOver * ((unsigned __int128)llSliceValueLimit)));
					*/
					
				}
				if (llPartialCarryOver != 0) {
					vlTemporary.vData.push_back(llPartialCarryOver);
				}
				
				//vlTemporary.shifting(lSliceIndex, false);
				//vlOutput += vlTemporary;
				
				if (lSliceIndex == 0) {
					vlOutput = vlTemporary;
				} else {
					vlTemporary.shifting(lSliceIndex, false); // add trailing zeros
					vlOutput += vlTemporary;
				}
				//cout << "DEBUG: temporary is " << vlTemporary << endl;
				//cout << "DEBUG: output is " << vlOutput << endl;
				
			}
			// !times
        	
    	}
    	if (vlOther.isNegative()) {
    		vlOutput.negate();
		}
		//cout << "DEBUG: product is " << vlOutput << endl;
		return vlOutput;
	}
	
	velo& operator*=(const velo& vlOther) {
		*this = *this * vlOther;
		return *this;
	}
	
	
	velo operator/(const velo& vlOther) const {
		//cout << "DEBUG: starting division dividend = " << this->stringify(true) << ", divisor = " << vlOther.stringify(true) << endl;
	    if (vlOther.isNull()) {
    		//cout << "DEBUG: self = " << this->stringify(true) << ", divider = " << pvlDivider->stringify(true) << endl;
    		throw runtime_error("division by zero");
		}
		// if it's one we don't need anything
		velo vlRatio;
		if (vlOther.getSliceCount()==1 && vlOther.vData[0]==1) {
			vlRatio = *this;
			if (vlOther.isNegative()) {
				vlRatio.negate();
			}
			return vlRatio;
		}
		if (this->lowerthanabsolute(&vlOther)) {
        	//this->assignFromNumber(0);
        	return velo(0);
		}
		const bool bToBeNegative = this->isNegative() ^ vlOther.isNegative();
    	const bool bWasNegative = this->isNegative();
		/*
		if (this->isNegative()) {
			this->negate();
		}
		*/
		velo vlDividerCopy = vlOther;
		if (vlDividerCopy.isNegative()) {
			vlDividerCopy.negate();
		}
		// this is n, this->getSliceValue() is alpha, this->getSliceCount() is k (reverse!)
		// pvlDivider is m, pvlDivider->getSliceCount() is l (reverse!)
		velo vlRemainder; // r
		//velo vlRatio; // q
		velo vlDividend; // d
		velo vlIntermediate;
		//velo vlChunk; // b
		velo vlBeta;
		//vlChunk.assignFromNumber(llSliceValueLimit);
		// initialize (unnecessary)
		//vlRatio.assignFromNumber(0);
		//vlRemainder.assignFromNumber(0);
		// we iterate from {\displaystyle 0\leq i\leq k-l}, before stopping.
		for (long lSliceIndex = this->getSliceCount() - 1; lSliceIndex >= 0; lSliceIndex--) {
		    // {\displaystyle d_{i}=br_{i-1}+\alpha _{i+l-1}}
		    vlDividend = vlRemainder;
		    vlDividend.shifting(1, false);//multiplication(&vlChunk);
		    vlIntermediate = this->getSliceValue(lSliceIndex);
		    vlDividend += vlIntermediate;
		    // {\displaystyle r_{i}=d_{i}-m\beta _{i}=br_{i-1}+\alpha _{i+l-1}-m\beta _{i}}
		    if (vlDividend.getSliceCount() < vlDividerCopy.getSliceCount()) {
		    	vlBeta.assignFromNumber(0);
			} else {
				velo * pvlDor = &vlDividerCopy;
				velo * pvlDend = &vlDividend;
				// getsubquotientvelo
				//cout << "DEBUG: starting getSubQuotientVelo w/ dividend = " << pvlDend->stringify(true) << ", divisor = " << pvlDor->stringify(true) << endl;
				/*
				if (pvlDend->getSliceCount() < pvlDor->getSliceCount()) {
					throw runtime_error("sub quotient velo works only with superior slice count");
				}
				*/
				velo vlSubQuotient;
				//velo vlChunk;
				//vlChunk.assignFromNumber(llSliceValueLimit);
				velo vlDendCopy;
				//velo vlZero;
				velo vlNeeded;
				for (long lSliceIndex = pvlDend->getSliceCount() - 1; lSliceIndex >= pvlDor->getSliceCount() - 1; lSliceIndex--) {
					vlDendCopy.assignFromClone(pvlDend);
					vlNeeded = vlSubQuotient;
					vlNeeded *= *pvlDor;
					vlDendCopy -= vlNeeded;
					vlDendCopy.shifting(lSliceIndex - (pvlDor->getSliceCount() - 1), true);
					if (lSliceIndex < pvlDend->getSliceCount() - 1 && !vlSubQuotient.isNull()) {
						velo vlCapital;
						for (long lSliceIndexCore = 0; lSliceIndexCore < pvlDend->getSliceCount() - 1 - lSliceIndex ; lSliceIndexCore++) {
							// remove first digits because already done
							vlCapital = vlDendCopy.getSliceValue(vlDendCopy.getSliceCount() - 1);
							vlCapital.shifting(vlDendCopy.getSliceCount() - 1, false);
							vlDendCopy -= vlCapital;
						}
					}
					velo * pvlDendCopy = &vlDendCopy;
					// getsubquotientnumber
					//cout << "DEBUG: starting getSubQuotientNumber w/ dividend = " << pvlDendCopy->stringify(true) << ", divisor = " << pvlDor->stringify(true) << endl;
					velo vlNumber;
					long double ldSubQuotient = pvlDendCopy->getSignificant() / pvlDor->getSignificant();
					if (pvlDendCopy->getSliceCount() > pvlDor->getSliceCount() && ldSubQuotient < 1.0L) {
						ldSubQuotient *= 1.0e18L;
					}
					const long lPower = (long) log10(ldSubQuotient);
					if (lPower >= 0) {
						long long llFactor = 1;
						if (lPower > 0) {
							for (long lPowerInner = 0; lPowerInner < lPower; lPowerInner++) {
								llFactor *= 10;
							}
						}
						long double ldSubDor = pvlDor->getSignificant();
						const long double ldFactor = (long double) llFactor;
						ldSubDor *= ldFactor;
						ldSubQuotient = pvlDendCopy->getSignificant() / ldSubDor;
						if (pvlDendCopy->getSliceCount() > pvlDor->getSliceCount() && ldSubQuotient < 1.0L) {
							ldSubQuotient *= 1.0e18L;
						}
						ldSubQuotient *= ldFactor;
						long long llSubQuotient = (long long) ldSubQuotient;
						vlNumber.assignFromNumber(llSubQuotient);
						// check that it is the right one
						velo vlBeta;
						vlBeta.assignFromClone(pvlDor);
						vlBeta *= vlNumber;
						velo vlCopy;
						vlCopy.assignFromClone(pvlDendCopy);
						vlCopy -= vlBeta;
						int tries = 0;
						while (vlCopy.isNegative()) {
							tries++;
							--vlNumber;
							vlBeta.assignFromClone(pvlDor);
							vlBeta *= vlNumber;
							vlCopy.assignFromClone(pvlDendCopy);
							vlCopy -= vlBeta;
							if (tries > 1000) {
								cout << "inside getsubquotientnumber; dividend copy = " << pvlDendCopy->stringify(true) << ", divisor = " << pvlDor->stringify(true) << endl;
								cout << "dividend = " << this->stringify(true) << ", divisor = " << vlOther.stringify(true) << endl;
								throw runtime_error("too much decrease in subdivision");
							}
						}
						tries = 0;
						while (!vlCopy.lowerthanabsolute(pvlDor)) {
							tries++;
							++vlNumber;
							vlBeta.assignFromClone(pvlDor);
							vlBeta *= vlNumber;
							vlCopy.assignFromClone(pvlDendCopy);
							vlCopy -= vlBeta;
							if (tries > 1000) {
								cout << "inside getsubquotientnumber; dividend copy = " << pvlDendCopy->stringify(true) << ", divisor = " << pvlDor->stringify(true) << endl;
								cout << "dividend = " << this->stringify(true) << ", divisor = " << vlOther.stringify(true) << endl;
								throw runtime_error("too much decrease in subdivision");
							}
						}
						//return vlNumber;
					}
					// !getsubquotientnumber
					//cout << "DEBUG: number = " << vlNumber.stringify(true) << endl;
					//vlNumber = getSubQuotientNumber(&vlDendCopy, pvlDor);
					vlSubQuotient.shifting(1, false);//multiplication(&vlChunk);
					vlSubQuotient += vlNumber;
				}
				//return vlSubQuotient;
				// !getsubquotientvelo
				//vlBeta = getSubQuotientVelo(&vlDividend, &vlDividerCopy);
				vlBeta = vlSubQuotient;
				//cout << "DEBUG: beta = " << vlBeta.stringify(true) << endl;
			}
		    // There only exists one such \beta _{{i}} such that {\displaystyle 0\leq r_{i}<m}.
		    vlIntermediate = vlBeta * vlDividerCopy;
		    vlRemainder = vlDividend;
			if (vlIntermediate.isNegative()) {
				vlRemainder += vlIntermediate;
			} else {
				vlRemainder -= vlIntermediate;
			}
		    //vlRemainder.addition(&vlIntermediate, !vlIntermediate.isNegative());
		    int tries = 0;
		    while (vlRemainder.isNegative()) {
		    	tries++;
		        --vlBeta;
		        vlIntermediate = vlBeta * vlDividerCopy;
		        vlRemainder.assignFromClone(&vlDividend);
				if (vlIntermediate.isNegative()) {
					vlRemainder += vlIntermediate;
				} else {
					vlRemainder -= vlIntermediate;
				}
		        //vlRemainder.addition(&vlIntermediate, !vlIntermediate.isNegative());
		        if (tries > 1000) {
					cout << "inside getsubquotientvelo; dividend = " << this->stringify(true) << ", divisor = " << vlOther.stringify(true) << endl;
		            throw runtime_error("too much decrease in division");
		        }
		    }
		    tries = 0;
		    while (!vlRemainder.lowerthanabsolute(&vlOther)) {
		    	tries++;
		        ++vlBeta;
		        vlIntermediate = vlBeta * vlDividerCopy;
		        vlRemainder.assignFromClone(&vlDividend);
				if (vlIntermediate.isNegative()) {
					vlRemainder += vlIntermediate;
				} else {
					vlRemainder -= vlIntermediate;
				}
		        //vlRemainder.addition(&vlIntermediate, !vlIntermediate.isNegative());
		        if (tries > 1000) {
		        	cout << "inside getsubquotientvelo; dividend = " << this->stringify(true) << ", divisor = " << vlOther.stringify(true) << endl;
		            throw runtime_error("too much increase in division");
		        }
		    }
		    // {\displaystyle q_{i}=bq_{i-1}+\beta _{i}}
		    vlRatio.shifting(1, false);//multiplication(&vlChunk);
		    vlIntermediate.assignFromClone(&vlBeta);
		    vlRatio += vlIntermediate;
		}
		if (bToBeNegative) {
			vlRatio.negate();
		}
		return vlRatio;
	}
	
	velo operator%(const velo& vlOther) const {
	    return (*this) - (*this / vlOther) * vlOther;
	}
	
	velo& operator/=(const velo& vlOther) {
		*this = *this / vlOther;
		return *this;
	}
	
	velo& operator%=(const velo& vlOther) {
		*this = *this % vlOther;
		return *this;
	}
	
	
	inline bool isNull() const {
		return (this->getSliceCount()==1 && this->vData[0]==0);
	}
	
	
	bool operator==(const velo& vlOther) const {
	    if (this->isNegative() != vlOther.isNegative() || this->getSliceCount() != vlOther.getSliceCount()) {
            return false;
        } else {
            for (long lSliceIndex = 0; lSliceIndex < this->getSliceCount(); lSliceIndex++) {
                if (this->getSliceValue(lSliceIndex) != vlOther.getSliceValue(lSliceIndex)) {
                    return false;
                }
            }
            return true;
        }
	}
	
	bool operator!=(const velo& vlOther) const {
	    return !(*this == vlOther);
	}
	
	bool operator>(const velo& vlOther) const {
	    const bool neg = this->isNegative();
        if (neg != vlOther.isNegative()) {
            return !neg;
        }
        if (this->getSliceCount() != vlOther.getSliceCount()) {
			return ((this->getSliceCount() > vlOther.getSliceCount()) ^ neg);
        }
        for (long lSliceIndex = this->getSliceCount() - 1; lSliceIndex >= 0; lSliceIndex--) {
			if ((this->getSliceValue(lSliceIndex) != vlOther.getSliceValue(lSliceIndex))) {
				return (this->getSliceValue(lSliceIndex) > vlOther.getSliceValue(lSliceIndex)) ^ neg;
			}
        }
		return false;
	}
	
	bool operator<(const velo& vlOther) const {
	    const bool neg = this->isNegative();
        if (neg != vlOther.isNegative()) {
            return neg;
        }
        if (this->getSliceCount() != vlOther.getSliceCount()) {
			return ((this->getSliceCount() < vlOther.getSliceCount()) ^ neg);
        }
        for (long lSliceIndex = this->getSliceCount() - 1; lSliceIndex >= 0; lSliceIndex--) {
			if ((this->getSliceValue(lSliceIndex) != vlOther.getSliceValue(lSliceIndex))) {
				return (this->getSliceValue(lSliceIndex) < vlOther.getSliceValue(lSliceIndex)) ^ neg;
			}
        }
		return false;
	}
	
	bool operator>=(const velo& vlOther) const {
	    return !(*this < vlOther);
	}
	
	bool operator<=(const velo& vlOther) const {
	    return !(*this > vlOther);
	}
	
	
	void assignFromReal(long double ldInput) {
	    long double ldCopy = ldInput;
	    long lPower = log10(ldInput);
	    if (ldInput > 1.0e300L) {
	        throw runtime_error("overflow in real constructor");
	    }
	    long lChunks = lPower / 18;
	    if (lChunks > 0) {
	        for (long k = 0; k < lChunks; k++) {
	            ldCopy /= 1.0e18L;
	        }
	    }
	    this->assignFromNumber((long long) ldCopy);
	    if (lChunks > 0) {
	        for (long k = 0; k < lChunks; k++) {
	            this->vData.insert(this->vData.begin(), 0);
	        }
	    }
	}
	
	long double toReal(void) const {
		long double out = this->getSignificant();
    	stringstream ss;
    	ss << setprecision(17) << out;
        string sSignificant = ss.str();
		if (sSignificant.find("e") != string::npos) {
			long lPower = strtol(sSignificant.substr(sSignificant.find("e")+2, string::npos).c_str(), NULL, 10);
			if (this->getSliceCount() > 1) {
				lPower += (this->getSliceCount() - 2) * 18;
			}
			if (lPower > 300) {
				throw runtime_error("overflow in real conversion");
			}
			out *= pow(10, lPower);
		}
        return this->isNegative()?-out:out;
    }
	
	long long toLongLong(void) const {
		if (this->getSliceCount() > 1) {
			throw runtime_error("overflow in integer conversion");
		}
		return this->isNegative()?-this->vData[0]:this->vData[0];
	}
	
	friend ostream& operator<<(ostream& os, const velo& vin) {
        os << vin.stringify(true);
        return os;
    }
    
    void HalfLife(void) {
    	// quick divide by two
    	bool retenue = false;
    	for (long lSliceIndex = this->getSliceCount() - 1; lSliceIndex >= 0; lSliceIndex--) {
    		this->vData[lSliceIndex] += retenue?llSliceValueLimit:0;
    		retenue = ((this->vData[lSliceIndex] & 1) != 0);
    		this->vData[lSliceIndex] >>= 1;
		}
		if (this->vData.back() == 0 && this->getSliceCount() > 1) {
			this->vData.pop_back();
		}
	}
	
	bool LastBitOfHope(void) const {
		// quick last bit
		return ((this->getSliceValue(0) & 1) != 0);
	}
	

};




// functions outside the class
velo abs_velo(const velo v) {
	if (v.isNegative()) {
		velo w = v;
		w.negate();
		return w;
	} else {
		return v;
	}
}

velo gcd_velo(const velo v1, const velo v2) {
	//cout << "DEBUG: gcd of " << v1.stringify(true) << " and " << v2.stringify(true) << endl;
	if (v1 == velo((long long)0)) {
		if (v2 == velo((long long)0)) {
			return velo((long long)1);
		} else {
			return v2;
		}
	}
	if (v2 == velo((long long)0)) {
		if (v1 == velo((long long)0)) {
			return velo((long long)1);
		} else {
			return v1;
		}
	}
	velo v1a = abs_velo(v1);
	velo v2a = abs_velo(v2);
	velo x = v1a>v2a?v1a:v2a;
	velo y = v1a<v2a?v1a:v2a;
	return gcd_velo(y, x % y);
}

velo gcd_velo(const vector<velo> vv) {
	if (vv.size() <= 1) {
		return velo((long long)1);
	}
	velo gcd_out = gcd_velo(vv[0], vv[1]);
	for (long k = 2; k < vv.size(); k++) {
		gcd_out = gcd_velo(gcd_out, vv[k]);
	}
	return gcd_out;
}

velo floorofsquareroot(const velo vin) {
	if (vin.isNegative()) {
		throw runtime_error("square root of negative");
	}
	velo newguess;
	newguess.assignFromReal(sqrt(vin.toReal()));
	velo oldguess(0);
	velo ancientguess(0);
	velo twoasvelo(2);
	while (oldguess != newguess && ancientguess != newguess) {
		ancientguess = oldguess;
		oldguess = newguess;
		newguess = (newguess * newguess + vin) / (twoasvelo * newguess);
		//newguess = newguess * (newguess * newguess * newguess + twoasvelo * vin) / (twoasvelo * newguess * newguess * newguess + vin); // for cubic root
	}
	velo endguess = newguess;
	if (endguess == ancientguess && endguess > oldguess) {
		endguess = oldguess;
	}
	return endguess;
}

vector <velo> continuedfraction(const velo vin, const long maxdepth, const long leadingzeros, const long long stopthreshold) {
	// check arguments
	const long actualmaxdepth = maxdepth<=0?1:maxdepth;
	const long actualleadingzeros = leadingzeros<0?0:leadingzeros;
	const long long actualstopthreshold = (stopthreshold <= 0 || stopthreshold > llSqrtValueLimit)?llSqrtValueLimit:stopthreshold;
	// number of digits
	long lPower = log10(vin.getSignificant());
	if (vin.getSliceCount() > 2) {
		lPower += (vin.getSliceCount() - 2) * 18;
	}
	velo dentor = 10;
	for (long lDigit = 0; lDigit < lPower + actualleadingzeros; lDigit++) {
		dentor *= velo(10);
	}
	// extra
	velo numtor = vin;
	for (long lPrecision = 0; lPrecision < actualmaxdepth; lPrecision++) {
		numtor *= velo(llSqrtValueLimit);
		dentor *= velo(llSqrtValueLimit);
	}
	//cout << "DEBUG: num = " << numtor.getMagnitudeOrder();
	//cout << ", den = " << dentor.getMagnitudeOrder() << endl;
	// the deed
	vector <velo> out;
	for (long step = 0; step < actualmaxdepth; step++) {
		// exchange
		velo tmp = numtor;
		numtor = dentor;
		dentor = tmp;
		// fraction
		tmp = numtor / dentor;
		// remainder
		numtor -= dentor * tmp;
		const long long element = tmp.toLongLong();
		out.push_back(tmp);
		//cout << tmp << ",";
		// check
		if (numtor.isNull() || element > actualstopthreshold) {
			break;
		}
	}
	const bool earlyexit = out.back() > actualstopthreshold;
	if (earlyexit) {
		out.pop_back();
	}
	dentor = out.back();
	numtor = velo(1);
	for (long backstep = out.size()-2; backstep >= 0; backstep--) {
		// compute
		numtor += dentor * out[backstep];
		// exchange
		velo tmp = numtor;
		numtor = dentor;
		dentor = tmp;
	}
	out.clear();
	out.push_back(numtor);
	out.push_back(dentor);
	return out;
}

long double log_base_ten(const velo v) {
	long double out = log10(v.getSignificant());
	if (v.getSliceCount() > 2) {
		out += (v.getSliceCount() - 2) * 18;
	}
	return out;
}



#endif //!__VELO_H__









