//Copyright and Disclaimer:
//This code is copyright Vienna University of Technology, 2004.

#ifndef GeometryH
#define GeometryH

#include <Base/Base.h>
#include <Base/SmartPointer.h>
#include <Mathematic/Geometry/AABox.h>

class FrameState;

class Geometry : public Base {
public:
	typedef Math::Geometry::AABox<float> AABox;
	typedef Math::Vector3<float> V3;

	Geometry() {}
	virtual ~Geometry() {}

	virtual void draw(FrameState&) = 0;
	virtual void drawGeometry(FrameState&) const = 0;

	virtual const AABox& getBoundingBox() const = 0;
	virtual const unsigned triangleCount() const = 0;
};
typedef SmartPointer<Geometry> SmpGeometry;

#endif
