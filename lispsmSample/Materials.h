//Copyright and Disclaimer:
//This code is copyright Vienna University of Technology, 2005.

#ifndef MaterialsH
#define MaterialsH

#include <Engine/Renderer/State.h>
#include "FrameState.h"

class SkyState : public State {
protected:
	TextureObjectGL& texSky;
	FrameState& fs;
public:
	SkyState(FrameState& vfs): fs(vfs), texSky(vfs.getTexture("sky")) {
		const char *circles[] = {
		"...xxx.xx.......",
		"..xx...xx..x....",
		".xx...xxx.x.x...",
		".x.xx...xxx.....",
		"xxx......x.xxx..",
		"xxx......x..xx..",
		"......x....x....",
		"xxx......xxx....",
		".xxx....xxx.....",
		".xx.xx...xx...x.",
		"..xxx....xx.....",
		"....xxxx......x.",
		"...........x....",
		"....xx.....xx...",
		"....x...x.......",
		"...x......xx....",
		};
		GLubyte oTexture[16][16];
		GLubyte *loc;

		// Setup RGB image for the texture.
		loc = (GLubyte*) oTexture;
		for(unsigned t = 0; t < 16; t++) {
			for(unsigned s = 0; s < 16; s++, loc+= 1) {
				if (circles[t][s] == 'x') {
					*loc = 0x3f;
				} else {
					*loc = 0x00;
				}
			}
		}
		texSky.load(oTexture,16,16);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	virtual void begin() {
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		glDisable(GL_LIGHTING);
		glDisable(GL_FOG);
		texSky.bind();
		GL::color(C3::WHITE());
		fs.shdSky->begin();
	}

	virtual void end() {
		fs.shdSky->end();
		glEnable(GL_FOG);
		glEnable(GL_LIGHTING);
		glDisable(GL_BLEND);
	}
};

class HeightFieldState : public State {
protected:
	FrameState& fs;
	TextureObjectGL& texNoise;
public:
	HeightFieldState(FrameState& vfs): fs(vfs), texNoise(vfs.getTexture("noise")) { }
	virtual void begin() {
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,Math::Vector4f::ONE().addr());
		GL::color(C3::WHITE());
		glActiveTextureARB(fs.texNoiseUnit);
		texNoise.bind();
		glActiveTextureARB(GL_TEXTURE0);
		glTexCoord1f(0.5);
	}
};

class TreeState : public State {
protected:
	FrameState& fs;
	TextureObjectGL& texNoise;
public:
	TreeState(FrameState& vfs): fs(vfs), texNoise(vfs.getTexture("noise")) { }
	virtual void begin() {
		glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,Math::Vector4f::ONE().addr());
		GL::color(C3::WHITE());
		glActiveTextureARB(fs.texNoiseUnit);
		texNoise.bind();
		glActiveTextureARB(GL_TEXTURE0);
		glTexCoord1f(0.7);
	}
};


#endif