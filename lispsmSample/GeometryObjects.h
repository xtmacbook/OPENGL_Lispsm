//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.
#ifndef GeometryObjectsH
#define GeometryObjectsH

#include <Types/DynamicArray.h>
#include <Mathematic/Vector3.h>
#include <GL/GlHeader.h>
#include <Engine/GLInterface/BaseGL.h>
#include "StateGeometry.h"
#include "MyHeightField.h"

class BaseTree : public MaterialGeometry {
private:
	GLuint body, head;
	AABox bb;
	const float size;
	const float trans;
	const float trunkThick;

	const float complexity;

	static const unsigned stacks(const float complexity) {
		return Math::lerp(3.0f,8.0f,complexity);
	}

	static const unsigned segments(const float complexity) {
		return Math::lerp(3.0f,32.0f,complexity);
	}

public:

	//for animation
	mutable unsigned keyFrame;
	
	BaseTree(const unsigned matID): MaterialGeometry(matID), bb(V3::ZERO(),V3::ZERO()),
		size(1.0f), trans(size/1.5), trunkThick(size/7.0), complexity(0.3), keyFrame(0) {
		const float outR = size/5.0;
		const float inR = size/20.0;
		const float a = outR+inR;
		const V3 min = V3(-a,0.0,-a);
		const V3 max = V3(a,size,a);
		bb = AABox(min,max);
		bb.translate(V3(0.0,-trunkThick,0.0));

		body = glGenLists(1);
		glNewList(body,GL_COMPILE);
			glutSolidCone(trunkThick,size,segments(complexity),2);
		glEndList();

		head = glGenLists(1);
		glNewList(head,GL_COMPILE);
			glutSolidTorus(inR,outR,stacks(complexity),segments(complexity));	
//			glutSolidCube(size/2.5);
		glEndList();
	}
	virtual ~BaseTree() {
		glDeleteLists(body,1);
		glDeleteLists(head,1);
	}

	virtual void drawGeometry(FrameState&) const {
		//const float winkelX = sin(keyFrame/10.0f)*25.0f;
		//const float winkelY = cos(keyFrame/8.0f)*25.0f;
		glPushMatrix();
			GL::translate(V3(0.0,-trunkThick,0.0));
			glRotatef(-90.0,1.0,0.0,0.0);
			glCallList(body);
			GL::translate(V3(0.0,0.0,trans));
			//glRotatef(winkelX,1.0,0.0,0.0);
			//glRotatef(winkelY,0.0,1.0,0.0);
			glCallList(head);
		glPopMatrix();
	}
	virtual const AABox& getBoundingBox() const { return bb; }
	virtual const unsigned triangleCount() const { return segments(complexity)*2*2+segments(complexity)*stacks(complexity)*2; }
};

class Tree : public Geometry {
protected:
	AABox box;
	BaseTree& bt;
	const V3 pos;
	float size;

public:
	Tree(const V3& position, const float& vSize, BaseTree& baseTree): 
		box(position,position), bt(baseTree), pos(position), size(vSize) {
		Math::clamp(size,1.0f,9.0f);
		AABox aBox = bt.getBoundingBox();
		//todo: in some circumstances trees disappear to early when looking down trunkThick is not the problem
		aBox.scale(size);
		aBox.translate(position);
		box = aBox;
	}

	virtual void drawGeometry(FrameState& fs) const {
		bt.keyFrame = fs.getFrame()+100+pos[0]*pos[2]+pos[1];
		glPushMatrix();
			GL::translate(pos);
			GL::scaleUniform(size);
			bt.drawGeometry(fs);
		glPopMatrix();
	}

	virtual void draw(FrameState& fs) {
		bt.keyFrame = fs.getFrame()+100+pos[0]*pos[2]+pos[1];
		glPushMatrix();
			GL::translate(pos);
			GL::scaleUniform(size);
			bt.draw(fs);
		glPopMatrix();
//		drawLineAABox(getBoundingBox());
	}
	virtual const AABox& getBoundingBox() const { return box; }
	virtual const unsigned triangleCount() const { return bt.triangleCount(); }
};

class HeightFieldRgn : public MaterialGeometry {
protected:
	typedef MyHeightField HeightField;
	const HeightField& heightField;
	const AABox bb;
	const HeightField::ArrID vertexID;
	static AABox calcAABox(const HeightField::ArrID& ids, const HeightField& hf) {
		const HeightField::ArrVector3& vertex = hf.getVertex();
		if(ids.size() < 3)
			throw BaseException("Height field too small");
		AABox box(vertex[ids[0]],vertex[ids[1]]);
		for(unsigned i = 2; i < ids.size(); i++) {
			box.expandToContain(vertex[ids[i]]);
		}
		return box;
	}

public:
	HeightFieldRgn(const unsigned matID, const HeightField::ArrID& ids, const HeightField& hf): 
		MaterialGeometry(matID), vertexID(ids), heightField(hf), bb(calcAABox(ids,hf)) { }
	virtual void drawGeometry(FrameState&) const {
		glVertexPointer(3,GL_FLOAT,0,&heightField.getVertex());
		glTexCoordPointer(1,GL_FLOAT,0,&heightField.getTexCoord());
		glNormalPointer(GL_FLOAT,0,&heightField.getNormal());
		glDrawElements(GL_TRIANGLE_STRIP,vertexID.size(),GL_UNSIGNED_INT,&vertexID);
	}
	virtual const AABox& getBoundingBox() const { return bb; }
	virtual const unsigned triangleCount() const { return vertexID.size()-2; }
};


class Sky : public MaterialGeometry {
protected:
	AABox bb;

public:
	Sky(const unsigned matID, const AABox& box): MaterialGeometry(matID), bb(box) { }
	virtual void drawGeometry(FrameState&) const {
		const float count = 10.0;
		const V3& min = bb.getMin();
		const V3& max = bb.getMax();
		glBegin(GL_QUADS);
			glTexCoord2f(0.0,0.0);
			GL::sendVertex(min);
			glTexCoord2f(count,0.0);
			glVertex3f(max.x(),min.y(),min.z());
			glTexCoord2f(count,count);
			GL::sendVertex(max);
			glTexCoord2f(0.0,count);
			glVertex3f(min.x(),max.y(),max.z());
		glEnd();
	}
	virtual const AABox& getBoundingBox() const { return bb; }
	virtual const unsigned triangleCount() const { return 2; }
};

#endif
