//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Color3H
#define Color3H
//---------------------------------------------------------------------------
#include "Color.h"
//---------------------------------------------------------------------------
template<class REAL>
struct Color3 : public Color<REAL,3> {
	typedef Color<REAL,3> FATHER;
	typedef Color3<REAL> C3;

protected:
	static inline const C3 setRed() throw() { return C3(1.0,0.0,0.0); }
	static inline const C3 setGreen() throw() { return C3(0.0,1.0,0.0); }
	static inline const C3 setBlue() throw() { return C3(0.0,0.0,1.0); }
	static inline const C3 setCyan() throw() { return C3(0.0,1.0,1.0); }
	static inline const C3 setMagenta() throw() { return C3(1.0,0.0,1.0); }
	static inline const C3 setYellow() throw() { return C3(1.0,1.0,0.0); }
	static inline const C3 setOrange() throw() { return C3(1.0,REAL(0.65),0.0); }

public:
	// components in range [0,1]
	Color3(const REAL& vR, const REAL& vG, const REAL& vB) { 
		array[0] = vR;
		array[1] = vG;
		array[2] = vB;
	}

	Color3(const FATHER& rgb): FATHER(rgb) { }
	Color3(const REAL rgb[3]) : FATHER(rgb) { }

	REAL& r() { return array[0]; }
	REAL& g() { return array[1]; }
	REAL& b() { return array[2]; }


	static inline const C GREY(const REAL fact) throw() { return C().fillWith(REAL(1.0)-fact); }
	static inline const C BLACK() throw() { return ZERO(); }

	static inline const C3 RED() throw() { 
		const static C3 c = setRed();
		return c;
	}

	static inline const C3 GREEN() throw() { 
		const static C3 c = setGreen();
		return c;
	}

	static inline const C3 BLUE() throw() { 
		const static C3 c = setBlue();
		return c;
	}

	static inline const C3 CYAN() throw() { 
		const static C3 c = setCyan();
		return c;
	}

	static inline const C3 MAGENTA() throw() { 
		const static C3 c = setMagenta();
		return c;
	}

	static inline const C3 YELLOW() throw() { 
		const static C3 c = setYellow();
		return c;
	}

	static inline const C3 ORANGE() throw() { 
		const static C3 c = setOrange();
		return c;
	}

	static inline const C3 RED(const REAL fact) throw() { return RED()*fact; }
	static inline const C3 GREEN(const REAL fact) throw() { return GREEN()*fact; }
	static inline const C3 BLUE(const REAL fact) throw() { return BLUE()*fact; }
};

#endif
