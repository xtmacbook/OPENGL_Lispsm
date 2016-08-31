//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef MatrixH
#define MatrixH
#include "Tuppel.h"
#include "Vector4.h"
//---------------------------------------------------------------------------
namespace Math {

template<class REAL = float, const unsigned COL = 4, const unsigned ROW = 4>
class Matrix : public Tuppel<REAL,COL*ROW> {
protected:
	typedef Tuppel<REAL,COL*ROW> FATHER;
	typedef Matrix<REAL,COL,ROW> M;

	static inline const M setIdentity() throw() { 
		M m;
		for(unsigned iCol = 1; iCol <= COL; iCol++) {
			for(unsigned iRow = 1; iRow <= ROW; iRow++) {
				if(iRow != iCol) {
					m(iCol,iRow) = REAL(0.0);
				}
				else {
					m(iCol,iRow) = REAL(1.0);
				}
			}
		}
		return m; 
	}

	template<unsigned K>
	void mulUnSave(const Matrix<REAL,COL,K>& a, const Matrix<REAL,K,ROW>& b) {
		for(unsigned iCol = 1; iCol <= COL; iCol++) {
			for(unsigned iRow = 1; iRow <= ROW; iRow++) {
				REAL& o = ( operator()(iCol,iRow) = REAL(0.0) );
				for(unsigned k = 1; k <= K; k++) {
					o += a(iCol,k)*b(k,iRow);
				}
			}
		}
	}

public:
	//array[COL*ROW];   // 00    ROW .  .  (COL-1)*ROW
						// 01     .  .  .       .
						// .      .  .  .       .
						// ROW-1  .  .  .   COL*ROW-1

	Matrix() { /* empty -> performance */ }
	Matrix(const M& m): FATHER(m) { }
	Matrix(const REAL p[COL*ROW]): FATHER(p) { }

	Tuppel<REAL,COL> operator*(const Tuppel<REAL,COL>& v) const {
		//if v == output -> overwriting problems -> so store in temp
		Tuppel<REAL,COL> t;
		for(unsigned iCol = 1; iCol <= COL; iCol++) {
			REAL& o = ( t[iCol-1] = REAL(0.0) );
			for(unsigned iRow = 1; iRow <= ROW; iRow++) {
					o += operator()(iCol,iRow)*v[iRow-1];
			}
		}
		return t;
	}

	M& operator*=(const M& lValue) {
		if(lValue.addr() == addr()) {
			M tmp(lValue);
			mulUnSave(tmp,tmp);
		}
		else {
			mulUnSave(M(*this),lValue);
		}
		return *this;
	}

	template<unsigned K>
	M& mul(const Matrix<REAL,COL,K>& a, const Matrix<REAL,K,ROW>& b) {
		if(a.array == array || b.array == array) {
			M tmp(*this);
			tmp.mulUnSave(a,b);
			*this = tmp;
		}
		else {
			mulUnSave(a,b);
		}
		return *this;
	}

	template<unsigned R>
	Matrix<REAL,COL,R> operator*(const Matrix<REAL,ROW,R>& b) const {
		Matrix<REAL,COL,R> o;
		o.mulUnSave(*this,b);
		return o;
	}

	static const unsigned colCount() { return COL; }
	static const unsigned rowCount() { return ROW; }

	const REAL& operator()(const unsigned col, const unsigned row) const { return array[col-1+ROW*(row-1)]; }
	REAL& operator()(const unsigned col, const unsigned row) { return array[col-1+ROW*(row-1)]; }

//	Matrix<REAL,ROW,COL> transpose(const Matrix<REAL,COL,ROW>& m) { }

	static inline const M IDENTITY() throw() { 
		const static M m = setIdentity();
		return m;
	}
};

//namespace
};
#endif
