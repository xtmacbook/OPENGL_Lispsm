//---------------------------------------------------------------------------
#ifndef ObjectsGL_DMS_H
#define ObjectsGL_DMS_H

#include <Types/StaticArray.h>
#include <Mathematic/Geometry/AABox.h>
#include <Mathematic/Geometry/Polyhedron.h>
#include "BaseGL.h"

//---------------------------------------------------------------------------
namespace GL {

template <class V>
void drawLine(const V& a, const V& b) {
	glBegin(GL_LINES);
		sendVertex(a);
		sendVertex(b);
	glEnd();
}

template<class T>
inline void drawQuad(const T& a, const T& b, const T& c, const T& d) {
	glBegin(GL_QUADS);
		sendVertex(a);
		sendVertex(b);
		sendVertex(c);
		sendVertex(d);
	glEnd();
}

template<class T>
inline void drawQuad(const T& v) {
	if(4 > v.size()) {
		throw BaseException("drawQuad with type smaller 4 elements called");
	}
	drawQuad(v[0],v[1],v[2],v[3]);
}

//draws the edges of a volume with 8 corner points, like a box or a frustum (GL_LINES)
template<class T>
inline void drawLineBoxVolume(const T& v) {
	if(8 > v.size()) {
		throw BaseException("drawLineBoxVolume with Array smaller 8 called");
	}
	glBegin(GL_LINE_LOOP);
		sendVertex(v[0]);
		sendVertex(v[1]);
		sendVertex(v[5]);
		sendVertex(v[6]);
		sendVertex(v[2]);
		sendVertex(v[3]);
		sendVertex(v[7]);
		sendVertex(v[4]);
	glEnd();
	glBegin(GL_LINES);
		sendVertex(v[0]);
		sendVertex(v[3]);
		sendVertex(v[1]);
		sendVertex(v[2]);
		sendVertex(v[4]);
		sendVertex(v[5]);
		sendVertex(v[6]);
		sendVertex(v[7]);
	glEnd();
}


//draws a volume with 8 corner points, like a box or a frustum (GL_TRIANGLE_STRIP)
template<class T>
inline void drawBoxVolume(const T& v) {
	if(8 > v.size()) {
		throw BaseException("drawBoxVolume with Array smaller 8 called");
	}
	glBegin(GL_TRIANGLE_STRIP);
		sendVertex(v[0]);
		sendVertex(v[1]);
		sendVertex(v[3]);
		sendVertex(v[2]);
		sendVertex(v[7]);
		sendVertex(v[6]);
		sendVertex(v[4]);
		sendVertex(v[5]);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP);
		sendVertex(v[6]);
		sendVertex(v[2]);
		sendVertex(v[5]);
		sendVertex(v[1]);
		sendVertex(v[4]);
		sendVertex(v[0]);
		sendVertex(v[7]);
		sendVertex(v[3]);
	glEnd();
}

template<class T>
inline void drawLineAABox(const Math::Geometry::AABox<T>& box) {
	typedef Math::Geometry::AABox<T> AABox;
	StaticArray<AABox::V3,8> p;
	box.computeVerticesRightHanded(p);
	drawLineBoxVolume(p);
}

template<class T>
inline void drawAABox(const Math::Geometry::AABox<T>& box) {
	typedef Math::Geometry::AABox<T> AABox;
	StaticArray<AABox::V3,8> p;
	box.computeVerticesRightHanded(p);
	drawBoxVolume(p);
}

template<class T>
inline void drawPolyhedronVolume(const Math::Geometry::Polyhedron<T>& ph) {
	typedef Math::Geometry::Polyhedron<T> P;
	typedef P::VecPolygon Poly;
	for(Poly::const_iterator i = ph.polygon.begin(); i != ph.polygon.end(); i++) {
		const P::LstSmpPoint& points = i->points;
		glBegin(GL_POLYGON);
		for(P::LstSmpPoint::const_iterator j = points.begin(); j != points.end(); j++) {
			sendVertex((*j)->object);
		}
		glEnd();
	}
}

template<class T>
inline void drawPolyhedronLine(const Math::Geometry::Polyhedron<T>& ph) {
	typedef Math::Geometry::Polyhedron<T> P;
	typedef P::VecPolygon Poly;
	for(Poly::const_iterator i = ph.polygon.begin(); i != ph.polygon.end(); i++) {
		const P::LstSmpPoint& points = i->points;
		glBegin(GL_LINE_LOOP);
		for(P::LstSmpPoint::const_iterator j = points.begin(); j != points.end(); j++) {
			sendVertex((*j)->object);
		}
		glEnd();
	}
}

//namespace
}
#endif
