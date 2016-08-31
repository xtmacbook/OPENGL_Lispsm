//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.

#ifndef FBOH
#define FBOH

#include <GL/GLheader.h>

class FrameBufferObject {
	GLuint fb;
	GLint old_fb;
	
public:
	FrameBufferObject(): fb(0), old_fb(0) { 
		// create objects
		glGenFramebuffersEXT(1, &fb);
	}

	virtual ~FrameBufferObject() {
		glDeleteFramebuffersEXT(1, &fb);
	}

	virtual void bind() {
		glGetIntegerv(GL_FRAMEBUFFER_BINDING_EXT, &old_fb);
		if(old_fb != fb) {
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, fb);
		}
	}

	virtual void unbind() {
		// unbind buffers and revert to window
		if(old_fb != fb) {
			glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, old_fb);
		}
	}

};


#endif
