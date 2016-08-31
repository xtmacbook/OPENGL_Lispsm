//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#ifndef FBODepthH
#define FBODepthH

#include <Engine/GLInterface/TextureObjectGL.h>
#include <Engine/EngineBase/FBO.h>
#include <iostream>

class FBODepth : FrameBufferObject {
	TextureObjectGL texDepth, dummy;

public:
	FBODepth(const unsigned size): 
			texDepth(GL_TEXTURE_2D,GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT,false) { 
		std::cout << "FrameBufferObject " << size << std::endl;
		// create objects
		bind();

		// specify size of depth texture
		texDepth.resize(size,size);
		//switched off pcf for nvidia (ati has no pcf filtering)
		//we do pcf filtering in the shader
		texDepth.filterNearest();
		//no repeat but clamp
		texDepth.clampToEdge();
		//bind texture as render target of current framebuffer
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_TEXTURE_2D, texDepth.getTexObject(), 0);
		CHECK_GL_ERROR();
		//everything setup -> check for correct fbo
		if(unsupportedFramebufferStatus()) {
			std::cerr << "FrameBufferObject: first configuration unsupported trying another\n";
			//some cards don't work with just a depth texture bound -> bind dummy color tex
			dummy.resize(size,size);
			dummy.filterNearest();
			glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, dummy.getTexObject(), 0);
		}
		checkRaiseFramebufferException("FBODepth");
		unbind();
		std::cout << "FrameBufferObject: finished configuration\n";
	}

	const TextureObjectGL& getDepthTex() const { return texDepth; }

	const unsigned size() const { return texDepth.getWidth(); }

	virtual void bind() {
		FrameBufferObject::bind();
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	void unbind() {
		FrameBufferObject::unbind();
		glDrawBuffer(GL_BACK);
		glReadBuffer(GL_BACK);
		if(texDepth.mipMapping()) {
			texDepth.bind();
			glGenerateMipmapEXT(texDepth.type());
		}
	}

};


#endif
