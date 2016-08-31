//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.

#ifndef DataTypesH
#define DataTypesH

#include <vector>
#include <Types/DynamicArray.h>
#include <Types/StaticArray.h>
#include <Mathematic/Vector3.h>
#include <Mathematic/Matrix4.h>
#include <Mathematic/Geometry/Line.h>
#include <Mathematic/Geometry/Plane.h>
#include <Mathematic/Geometry/AABox.h>
#include <Mathematic/Geometry/Frustum.h>
#include <Mathematic/Geometry/Polygon3D.h>
#include <Mathematic/Geometry/Object.h>
#include <Mathematic/Geometry/Polyhedron.h>
#include <Mathematic/Geometry/Converter.h>
#include <Engine/EngineBase/Color3.h>
#include <Engine/EngineBase/Color4.h>

typedef double Real;
typedef Math::Vector3<Real> V3;
typedef Math::Vector4<Real> V4;
typedef Color4<float> C4;
typedef Color3<float> C3;

//4x4 matrix
typedef Math::Matrix4<Real> M4;

typedef Math::Geometry::Line<Real> Line;

typedef StaticArray<V3,8> Vector3x8;

//a dynamic array of planes
typedef Math::Geometry::Plane<Real> Plane;

//vector of 3d points
typedef Math::Geometry::VecVector3<Real> VecPoint;

typedef Math::Geometry::Object<Real> Object;

typedef Math::Geometry::Polyhedron<Real> Polyhedron;

//Axis-Aligned Bounding Box defined through the two extreme points
typedef Math::Geometry::AABox<Real> AABox;

typedef Math::Geometry::Frustum<float> Frustum;
typedef Math::Geometry::Frustum<Real> FrustumR;


#endif
