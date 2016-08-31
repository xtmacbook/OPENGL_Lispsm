//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
//---------------------------------------------------------------------------
#ifndef Matrix4AccessH
#define Matrix4AccessH
#include "Matrix4.h"
//---------------------------------------------------------------------------
namespace Math {

template<class REAL>
inline Vector4<REAL> Matrix4<REAL>::getCol(const Matrix4<REAL>& m, const unsigned int i) const {
	return Vector4(&m.array[(i-1)*4]);
}

template<class REAL>
inline void setCol(Matrix4<REAL>& m, const unsigned int i,const Vector4<REAL>& vec) {
	const int n = (i-1)*4;
	m.array[n] = vec.x();
	m.array[n+1] = vec.y();
	m.array[n+2] = vec.z();
	m.array[n+3] = vec.w();
}

template<class REAL>
inline Vector4<REAL> getRow(const Matrix4<REAL>& m, const unsigned int i) const {
	return Vector4<REAL>(m.array[i-1],m.array[i+3],m.array[i+7],m.array[i+11]);
}

template<class REAL>
inline void setRow(Matrix4<REAL>& m, const unsigned int i,const Vector4<REAL>& vec) {
	m.array[i-1] = vec.x();
	m.array[i+3] = vec.y();
	m.array[i+7] = vec.z();
	m.array[i+11] = vec.w();
}

//namespace
}
#endif
