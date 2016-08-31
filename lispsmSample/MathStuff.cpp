//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.

#include "MathStuff.h"


V3 setDir(const Real& xAngle, const Real& yAngle) {
	const double cosM = cos(yAngle);
	const double sinM = sin(yAngle);
	const double cosN = cos(xAngle);
	const double sinN = sin(xAngle);
	V3 dir;
	dir[0] = -cosM*sinN;
	dir[1] = sinM;
	dir[2] = -cosM*cosN;
	dir.normalize();
	return dir;
}

