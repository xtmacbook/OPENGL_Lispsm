//---------------------------------------------------------------------------
#ifndef ColorH
#define ColorH
#include <Mathematic/Tuppel.h>
//---------------------------------------------------------------------------
template<class REAL, const unsigned SIZE>
class Color : public Math::Tuppel<REAL,SIZE> {
protected:
	typedef Math::Tuppel<REAL,SIZE> FATHER;
	typedef Color<REAL,SIZE> C;
	Color() { }
public:
	// components in range [0,1]
	Color(const REAL col[SIZE]) : FATHER(col) { }
	Color(const FATHER& v): FATHER(v) { }
	Color(const C& v): FATHER(v) { }

	void clamp() {
		for(unsigned i = 0; i < size(); i++) {
			Math::clamp(array[i],REAL(0.0),REAL(1.0));
		}
	}

	void scaleByMax() {
		const REAL maximum = getMax();
		if(maximum > 1.0) {
			operator/=(maximum);
		}
		else {
			operator*=(maximum);
		}
	}

	C& operator*=(const C& rColor) {
		for(unsigned i = 0; i < size(); i++) {
			array[i] *= rColor[i];
		}
		return *this;
	}

	C& operator*=(const REAL& r) {
		for(unsigned i = 0; i < size(); i++) {
			array[i] *= r;
		}
		return *this;
	}

	C& operator/=(const C& rColor) {
		for(unsigned i = 0; i < size(); i++) {
			(rColor[i] == REAL(0.0)) ? (array[i] = REAL(1.0)) : (array[i] /= rColor[i]);
		}
		return *this;
	}

	C operator*(const C& r) const { return C(*this) *= r; } 
	C operator*(const REAL& r) const { return C(*this) *= r; } 
	C operator/(const C& r) const { return C(*this) /= r; } 

	static inline const C WHITE() throw() { return ONE(); }
};

#endif
