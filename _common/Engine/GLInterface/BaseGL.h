//---------------------------------------------------------------------------
#ifndef BaseGLH
#define BaseGLH
//---------------------------------------------------------------------------
#include <Types/StaticArray.h>
#include <Mathematic/Tuppel.h>
#include <Mathematic/Vector4.h>
#include <GL/glHeader.h>
//---------------------------------------------------------------------------
namespace GL {

inline void sendVertex(const float& x, const float& y, const float& z) {
	glVertex3f(x,y,z);
}

inline void sendVertex(const double& x, const double& y, const double& z) {
	glVertex3d(x,y,z);
}

inline void sendVertex(const Math::Tuppel<float,2>& v) {
	glVertex2fv(v.addr());
}

inline void sendVertex(const Math::Tuppel<double,2>& v) {
	glVertex2dv(v.addr());
}

inline void sendVertex(const Math::Tuppel<float,3>& v) {
	glVertex3fv(v.addr());
}

inline void sendVertex(const Math::Tuppel<double,3>& v) {
	glVertex3dv(v.addr());
}

inline void sendVertex(const Math::Tuppel<float,4>& v) {
	glVertex4fv(v.addr());
}

inline void sendVertex(const Math::Tuppel<double,4>& v) {
	glVertex4dv(v.addr());
}

inline void sendTexCoord(const Math::Tuppel<float,2>& t) {
	glTexCoord2fv(t.addr());
}

inline void sendTexCoord(const Math::Tuppel<float,3>& t) {
	glTexCoord3fv(t.addr());
}

inline void sendTexCoord(const Math::Tuppel<float,4>& t) {
	glTexCoord4fv(t.addr());
}

inline void color(const Math::Tuppel<float,3>& v) {
	glColor3f(v[0],v[1],v[2]);
}

inline void color(const Math::Tuppel<double,3>& v) {
	glColor3d(v[0],v[1],v[2]);
}

inline void color(const Math::Tuppel<float,4>& v) {
	glColor4f(v[0],v[1],v[2],v[3]);
}

inline void color(const Math::Tuppel<double,4>& v) {
	glColor4d(v[0],v[1],v[2],v[3]);
}

inline void scale(const Math::Tuppel<float,3>& v) {
	glScalef(v[0],v[1],v[2]);
}

inline void scale(const Math::Tuppel<double,3>& v) {
	glScaled(v[0],v[1],v[2]);
}

template <class REAL>
inline void scaleUniform(const REAL& v) {
	scale(Math::Vector3<REAL>(v,v,v));
}

inline void rotate(const float& angle, const Math::Tuppel<float,3>& v) {
	glRotatef(angle,v[0],v[1],v[2]);
}

inline void rotate(const double& angle, const Math::Tuppel<double,3>& v) {
	glRotated(angle,v[0],v[1],v[2]);
}

inline void translate(const Math::Tuppel<float,3>& v) {
	glTranslatef(v[0],v[1],v[2]);
}

inline void translate(const Math::Tuppel<double,3>& v) {
	glTranslated(v[0],v[1],v[2]);
}

inline void clearColor(const Math::Tuppel<float,4>& c) {
	glClearColor(c[0],c[1],c[2],c[3]);
}

inline void loadMatrix(const Math::Tuppel<float,16>& m) {
	glLoadMatrixf(m.addr());
}

inline void loadMatrix(const Math::Tuppel<double,16>& m) {
	glLoadMatrixd(m.addr());
}

inline void multMatrix(const Math::Tuppel<float,16>& m) {
	glMultMatrixf(m.addr());
}

inline void multMatrix(const Math::Tuppel<double,16>& m) {
	glMultMatrixd(m.addr());
}

template<class T>
const Math::Vector4<T> convertDirToPos(const Math::Tuppel<T,3>& dir) {
	return Math::Vector4<T>(-dir[0], -dir[1], -dir[2], 0.0);
}

//namespace
}
#endif
