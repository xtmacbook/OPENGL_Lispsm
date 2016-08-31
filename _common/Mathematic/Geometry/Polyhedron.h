//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.
#ifndef PolyhedronH
#define PolyhedronH
//---------------------------------------------------------------------------
#include <list>
#include <vector>
#include <Base/MakeSmartPointerCompatible.h>
#include "../Vector3.h"
#include "../Matrix4.h"
#include "Line.h"
#include "Plane.h"
#include "Polygon2D.h"
//---------------------------------------------------------------------------
namespace Math {
namespace Geometry {

template<class REAL = float>
class Polyhedron {
protected:
	typedef Polyhedron<REAL> Polyh;
	typedef Vector3<REAL> V3;
	typedef Matrix4<REAL> M4;
	typedef Line<REAL> Line;
	typedef Plane<REAL> Plane;
	typedef MakeSmartPointerCompatible<V3> Point;
	typedef SmartPointer<Point> SmpPoint;
public:
	typedef std::list<SmpPoint> LstSmpPoint;
protected:
	LstSmpPoint lstSmpPoint;

	SmpPoint addPoint(const V3& pt) {
		//check if it already exists
		for(LstSmpPoint::iterator i = lstSmpPoint.begin(); i != lstSmpPoint.end(); i++) {
			if( (*i)->object.alike(pt,(REAL)0.001) ) {
				return (*i);
			}
		}
		//add to list
		SmpPoint point = new Point(pt);
		lstSmpPoint.push_back(point);
		//return smp
		return point;
	}


	void add(const Polyhedron& ph) {
		for(VecPolygon::const_iterator poly = ph.polygon.begin(); poly != ph.polygon.end(); poly++) {
			Polygon& newP = newPolygon();
			for(LstSmpPoint::const_iterator i = poly->points.begin(); i != poly->points.end(); i++) {
				newP.addPoint((*i)->object);
			}
		}
	}

	void assign(const Polyhedron& ph) {
		clear();
		add(ph);
	}

public:
	class Polygon {
		Polyh* parent;
	public:

		struct Edge : public std::pair<SmpPoint,SmpPoint> {
			typedef std::pair<SmpPoint,SmpPoint> E;
			Edge(const SmpPoint& a, const SmpPoint& b): E(a,b) { } 
			
			bool same(const Edge& e) const {
				return (*this == e) || (e.first == second && e.second == first) ; 
			}
		};

		typedef std::list<Edge> LstEdge;
		LstSmpPoint points;

		
		Polygon(Polyh& vParent): parent(&vParent) { }

		void addPoint(const V3& pt) {
			points.push_back(parent->addPoint(pt));
		}

		void add2EdgeList(LstEdge& lstEdge) const {
			//first edge from last to first;
			lstEdge.push_back(Edge(points.back(),points.front()));
			//iterate over the rest
			LstSmpPoint::const_iterator i = points.begin();
			i++;
			for(; i != points.end(); ++i) {
				LstSmpPoint::const_iterator pre = i;
				pre--;
				lstEdge.push_back(Edge(*pre,*i));
			}
		}

		void check() const {
			if(points.size() < 3) {
				throw GeometryException("Polyhedron::Polygon with less than 3 points");
			}
		}

		//at most on intersection; no intersection with parallel lines
		bool intersection(const Line& l, REAL& t) const {
//			check();
			LstSmpPoint::const_iterator pts = points.begin();
			const V3& a = (*pts)->object;
			pts++;
			const V3& b = (*pts)->object;
			pts++;
			const V3& c = (*pts)->object;
			Plane p(a,b,c);
			if(p.intersection(l,t)) {
				V3 n = p.getN();
				n.abs();
				const unsigned ignoreAxe = n.getMaxID();
				Polygon2D<REAL> poly;
				for(pts = points.begin(); pts != points.end(); pts++) {
					//const Tuppel<REAL,2> point = 
					//	(*pts)->object.getReduced(ignoreAxe);
					//poly.push_back(point);

				}

			}
			return false;
		}
	};

	//a dynamic array of point list each point list is a ccw polygon
	typedef std::vector<Polygon> VecPolygon;

	VecPolygon polygon;

	Polyhedron() { }
	Polyhedron(const Polyhedron& ph) {
		assign(ph);
	}
	
	Polyhedron& operator=(const Polyhedron& ph) {
		assign(ph);
		return *this;
	}


	Polyhedron& operator*=(const M4& frame) const {
		for(LstSmpPoint::iterator i = lstSmpPoint.begin(); i != lstSmpPoint.end(); i++) {
			(*i)->object = frame.mulHomogenPoint((*i)->object);
		}
		return *this;
	}

	const Polyhedron operator*(const M4& frame) const {
		Polyhedron ph;
		return ph;
	}

	void clear() {
		polygon.clear();
		lstSmpPoint.clear();
	}

	Polygon& newPolygon() {
		polygon.push_back(Polygon(*this));
		return polygon.back();
	}

	const LstSmpPoint& getPointList() const { return lstSmpPoint; }

	template<class T> void addPointsToStruct(T& out) const {
		const LstSmpPoint& p = getPointList();
		out.reserve(out.size()+p.size());
		for(LstSmpPoint::const_iterator i = p.begin(); i != p.end(); i++) {
			out.push_back((*i)->object);
		}
	}
	
	bool isClosed() const {
//		typedef std::pair<Edge,int> EdgeCount;
//		typedef std::list<EdgeCount> LstEdgeCount;
		//todo: isClosed implementation
		//count 1 ccw, -1 cw, 1000 for multi occurs
		return true;
	}

	template<class LIST> bool intersection(const Line& l, LIST& out) const {
		for(VecPolygon::const_iterator poly = polygon.begin(); poly != polygon.end(); poly++) {
			REAL t;
			if(poly->intersection(l,t)) {
				out.push_back(t);
			}
		}
		return true;
	}
};

//namespace
}
//namespace
}
#endif
