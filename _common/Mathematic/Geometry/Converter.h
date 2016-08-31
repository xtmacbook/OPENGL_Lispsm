//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, Vienna University of Technology, 2005.

#ifndef Converter_DMS_H
#define Converter_DMS_H

#include "Object.h"
#include "Polyhedron.h"

namespace Math {
namespace Geometry {

template <class REAL> void convert(Polyhedron<REAL>&, const Object<REAL>&);
template <class REAL> void convert(Object<REAL>&, const Polyhedron<REAL>&);

//returns a pointlist of the points of the given object
template<class REAL> void calcPoints(VecVector3<REAL>&, const Object<REAL>&);

//the convex hull of the given object and the point is calculated
template<class REAL> void convexHullOfObjectAndPoint(Object<REAL>& obj, const Vector3<REAL>& p);

//namespace
}
//namespace
}
#endif
