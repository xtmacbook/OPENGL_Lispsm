//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, Vienna University of Technology, 2005.
#include "Converter.h"
namespace Math {
namespace Geometry {


template <class REAL> 
void convert(Polyhedron<REAL>& ph, const Object<REAL>& obj) {
	typedef Polyhedron<REAL> Ph;
	typedef Object<REAL> O;
	typedef VecVector3<REAL> VecVec3;
	//convert to Polyhedron
	for(O::VecPolygon::const_iterator i = obj.polygon.begin(); i != obj.polygon.end(); i++) {
		const VecVec3& poly = i->points;
		Ph::Polygon& polyNew = ph.newPolygon();
		for(VecVec3::const_iterator j = poly.begin(); j != poly.end(); j++) {
			polyNew.addPoint(*j);
		}
	}
}

template<class REAL> 
void convert(Object<REAL>& obj, const Polyhedron<REAL>& ph) {
	typedef Polyhedron<REAL> P;
	typedef VecVector3<REAL> VecVec3;
	//convert to Object
	obj.polygon.resize(ph.polygon.size());
	for(unsigned i = 0; i < ph.polygon.size(); i++) {
		const P::LstSmpPoint& points = ph.polygon[i].points;
		VecVec3& pointsObj = obj.polygon[i].points;
		pointsObj.resize(points.size());
		unsigned objJ = 0;
		for(P::LstSmpPoint::const_iterator j = points.begin(); j != points.end(); j++, objJ++) {
			pointsObj[objJ] = (*j)->object;
		}
	}
}

template<class REAL> 
void calcPoints(VecVector3<REAL>& B, const Object<REAL>& obj) {
	Polyhedron<REAL> ph;
	convert(ph,obj);
	B.clear();
	ph.addPointsToStruct(B);
}


template<class REAL> 
void convexHullOfObjectAndPoint(Object<REAL>& obj, const Vector3<REAL>& p) {
	typedef Vector3<REAL> V3;
	typedef Polyhedron<REAL> Polyhedron;
	typedef Object<REAL> Object;
	struct CH {
		typedef Polyhedron::Polygon::LstEdge LstEdge;
		typedef Polyhedron::Polygon::Edge Edge;

		static void leaveSingles(LstEdge& output, const Edge& e) {
			for(LstEdge::iterator i = output.begin(); i != output.end(); i++) {
				if( i->same(e) ) {
					output.erase(i);
					return;
				}
			}
			output.push_back(e);
		}

		static void leaveSingles(LstEdge& output, const LstEdge& input) {
			for(LstEdge::const_iterator i = input.begin(); i != input.end(); i++) {
				leaveSingles(output,*i);
			}
		}

		static void singleEdgeList(LstEdge& output, const Polyhedron& ph) {
			for(Polyhedron::VecPolygon::const_iterator i = ph.polygon.begin(); i != ph.polygon.end(); i++) {
				LstEdge lstEdge;
				i->add2EdgeList(lstEdge);
				//add edges with only one neighbour to lstOnceEdge
				//-> leave edges which are only used once
				leaveSingles(output,lstEdge);
			}
		}
		static void close(Polyhedron& ph, const LstEdge& lstOnceEdge, const V3& p) {
			//with each of these edges make a triangle with the new point
			for(LstEdge::const_iterator i = lstOnceEdge.begin(); i != lstOnceEdge.end(); i++) {
				Polyhedron::Polygon& polyNew = ph.newPolygon();
				polyNew.addPoint(i->second->object);
				polyNew.addPoint(i->first->object);
				polyNew.addPoint(p);
			}
		}
	};

	//delete all polygons that the point can see ->
	obj.removeVisiblePolygons(p);
	//convert to Polyhedron
	Polyhedron ph;
	convert(ph,obj);
	//create list of single edges (~only one neighbour)
	CH::LstEdge lstOnceEdge;
	CH::singleEdgeList(lstOnceEdge,ph); //done: check inside case
	//close polyhedron
	CH::close(ph,lstOnceEdge,p);
//	assert(!ph.isClosed()) //todo
	convert(obj,ph);
}


template void calcPoints(VecVector3<float>&, const Object<float>&);
template void calcPoints(VecVector3<double>&, const Object<double>&);
template void convexHullOfObjectAndPoint(Object<float>&, const Vector3<float>&);
template void convexHullOfObjectAndPoint(Object<double>&, const Vector3<double>&);
//namespace
}
//namespace
}
