//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Matrix4_DMS_H
#define Matrix4_DMS_H
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
//---------------------------------------------------------------------------
namespace Math {

template<class REAL = float>
class Matrix4 : public Matrix<REAL,4,4> {
public:
	typedef Vector3<REAL> V3;
	typedef Vector4<REAL> V4;
	typedef Matrix4<REAL> M4;

protected:
	typedef Matrix<REAL,4,4> FATHER;

	static inline const Matrix4 setT05_S05() throw() { return Matrix4(0.5,0,0,0.5, 0,0.5,0,0.5, 0,0,0.5,0.5, 0,0,0,1); }

public:
	Matrix4() { /* empty -> performance */ }
	Matrix4(const Matrix4& m): FATHER(m) { }
	Matrix4(const FATHER& m): FATHER(m) { }
	Matrix4(const FATHER::FATHER& m): FATHER(m.addr()) { }
	Matrix4(const REAL p[16]): FATHER(p) { }

	Matrix4(const REAL& m11, const REAL& m12, const REAL& m13, const REAL& m14,
			const REAL& m21, const REAL& m22, const REAL& m23, const REAL& m24,
			const REAL& m31, const REAL& m32, const REAL& m33, const REAL& m34,
			const REAL& m41, const REAL& m42, const REAL& m43, const REAL& m44) {
		array[0] = m11;  array[4] = m12;  array[ 8] = m13;  array[12] = m14;
		array[1] = m21;  array[5] = m22;  array[ 9] = m23;  array[13] = m24;
		array[2] = m31;  array[6] = m32;  array[10] = m33;  array[14] = m34;
		array[3] = m41;  array[7] = m42;  array[11] = m43;  array[15] = m44;
	}

	M4& transpose() { 
		std::swap(array[ 1],array[ 4]);
		std::swap(array[ 2],array[ 8]);
		std::swap(array[ 3],array[12]);
		std::swap(array[ 6],array[ 9]);
		std::swap(array[ 7],array[13]);
		std::swap(array[11],array[14]);
		return *this;
	}

	Matrix4 getTransposed() const {
		return M4(*this).transpose();
	}

	//output = i^(-1)
	void invert(const Matrix4& i) {
		const REAL a11 =  det3x3(i[5],i[6],i[7],i[9],i[10],i[11],i[13],i[14],i[15]);
		const REAL a21 = -det3x3(i[1],i[2],i[3],i[9],i[10],i[11],i[13],i[14],i[15]);
		const REAL a31 =  det3x3(i[1],i[2],i[3],i[5],i[6],i[7],i[13],i[14],i[15]);
		const REAL a41 = -det3x3(i[1],i[2],i[3],i[5],i[6],i[7],i[9],i[10],i[11]);

		const REAL a12 = -det3x3(i[4],i[6],i[7],i[8],i[10],i[11],i[12],i[14],i[15]);
		const REAL a22 =  det3x3(i[0],i[2],i[3],i[8],i[10],i[11],i[12],i[14],i[15]);
		const REAL a32 = -det3x3(i[0],i[2],i[3],i[4],i[6],i[7],i[12],i[14],i[15]);
		const REAL a42 =  det3x3(i[0],i[2],i[3],i[4],i[6],i[7],i[8],i[10],i[11]);

		const REAL a13 =  det3x3(i[4],i[5],i[7],i[8],i[9],i[11],i[12],i[13],i[15]);
		const REAL a23 = -det3x3(i[0],i[1],i[3],i[8],i[9],i[11],i[12],i[13],i[15]);
		const REAL a33 =  det3x3(i[0],i[1],i[3],i[4],i[5],i[7],i[12],i[13],i[15]);
		const REAL a43 = -det3x3(i[0],i[1],i[3],i[4],i[5],i[7],i[8],i[9],i[11]);

		const REAL a14 = -det3x3(i[4],i[5],i[6],i[8],i[9],i[10],i[12],i[13],i[14]);
		const REAL a24 =  det3x3(i[0],i[1],i[2],i[8],i[9],i[10],i[12],i[13],i[14]);
		const REAL a34 = -det3x3(i[0],i[1],i[2],i[4],i[5],i[6],i[12],i[13],i[14]);
		const REAL a44 =  det3x3(i[0],i[1],i[2],i[4],i[5],i[6],i[8],i[9],i[10]);

		const REAL det = (i[0]*a11) + (i[4]*a21) + (i[8]*a31) + (i[12]*a41);
		const REAL oodet = 1/det;

		array[ 0] = a11*oodet;
		array[ 1] = a21*oodet;
		array[ 2] = a31*oodet;
		array[ 3] = a41*oodet;

		array[ 4] = a12*oodet;
		array[ 5] = a22*oodet;
		array[ 6] = a32*oodet;
		array[ 7] = a42*oodet;

		array[ 8] = a13*oodet;
		array[ 9] = a23*oodet;
		array[10] = a33*oodet;
		array[11] = a43*oodet;

		array[12] = a14*oodet;
		array[13] = a24*oodet;
		array[14] = a34*oodet;
		array[15] = a44*oodet;
	}

	void invert() { invert(*this); }
	
	Matrix4 getInverse() const { 
		Matrix4 m;
		m.invert(*this);
		return m;
	}

	//calc matrix-vector product; input has assumed homogenous component w = 1
	//before the output is  written homogen division is performed (w = 1)
	V3 mulHomogenPoint(const V3& v) const {
		//if v == output -> overwriting problems -> so store in temp
		const REAL x = array[0]*v[0] + array[4]*v[1] + array[ 8]*v[2] + array[12];
		const REAL y = array[1]*v[0] + array[5]*v[1] + array[ 9]*v[2] + array[13];
		const REAL z = array[2]*v[0] + array[6]*v[1] + array[10]*v[2] + array[14];
		const REAL w = array[3]*v[0] + array[7]*v[1] + array[11]*v[2] + array[15];

		return V3(x/w, y/w, z/w);
	}

	static inline const Matrix4 T05_S05() throw() { 
		const static Matrix4 m = setT05_S05();
		return m;
	}

};

typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

//namespace
}
#endif
