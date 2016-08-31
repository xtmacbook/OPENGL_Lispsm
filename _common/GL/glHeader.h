//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2004.

#ifndef GlHeaderH
#define GlHeaderH

#include <Base/BaseException.h>
#include <Base/StringTools.h>
struct GLException : public BaseException {
	GLException(const std::string& vMsg): BaseException(vMsg) { };
};

//includes an openGL extension header file
//http://glew.sourceforge.net/
#include <GL/glew.h>
//includes the glut header file
#include <GL/glut.h>

//includes  windows specific stuff
#ifdef _WIN32
	#include <GL/wglew.h>
#else
//include linux specific stuff
	#include <GL/glxew.h>
#endif	

inline void startupGLEW() {
	GLenum err = glewInit();
	if(GLEW_OK != err) {
		/* problem: glewInit failed, something is seriously wrong */
		throw GLException("Error:"+Tools::toString(glewGetErrorString(err)));
	}
}

inline float getOpenGLVersion() {
	if(GLEW_VERSION_2_0) {
		return 2.0;
	}
	else if(GLEW_VERSION_1_5) {
		return 1.5;
	}
	else if(GLEW_VERSION_1_4) {
		return 1.4;
	}
	else if(GLEW_VERSION_1_3) {
		return 1.3;
}
	else if(GLEW_VERSION_1_2) {
		return 1.2;
	}
	else if(GLEW_VERSION_1_1) {
		return 1.1;
	}
	else 
		return 1.0;
}

inline std::string getErrorString(const GLenum err) {
	return Tools::toString(gluErrorString(err));
}

inline void CheckForGLError(char *file, int line) {
	GLenum glErr;
	while((glErr = glGetError()) != GL_NO_ERROR) {
		std::string msg("GL Error:");
		msg += Tools::toString(glErr);
		msg += '(';
		msg += getErrorString(glErr);
		msg += ")  in File ";
		msg += file;
		msg += " at line: ";
		msg += Tools::toString(line);
		msg += '\n';
		throw GLException(msg);
	}
}
#define CHECK_GL_ERROR() CheckForGLError(__FILE__, __LINE__)

inline bool unsupportedFramebufferStatus() {
	return GL_FRAMEBUFFER_UNSUPPORTED_EXT == glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
}
inline void checkRaiseFramebufferException(const std::string& szCaller) {
	const std::string szMsg('['+szCaller+']');
	switch(glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT)) {
	case GL_FRAMEBUFFER_COMPLETE_EXT:
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT_EXT:
		throw GLException(szMsg+"Framebuffer incomplete, framebuffer completness not satisfied");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		throw GLException(szMsg+"Framebuffer incomplete, attached images must have same dimensions");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT_EXT:
		throw GLException(szMsg+"Framebuffer incomplete, duplicate attachment");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		throw GLException(szMsg+"Framebuffer incomplete, attached color images must have same format");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT_EXT:
		throw GLException(szMsg+"Framebuffer incomplete, no attachment");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER_EXT:
		throw GLException(szMsg+"Framebuffer incomplete, drawBuffer states drawing buffer that is not attached to fbo");
		break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER_EXT:
		throw GLException(szMsg+"Framebuffer incomplete, readBuffer states reading buffer that is not attached to fbo");
		break;
/*	case GL_FRAMEBUFFER_STATUS_ERROR_EXT: //deprecaded
		break;*/
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		throw GLException(szMsg+"Unsupported framebuffer format (implementation dependent)");
		break;
	default:
		throw GLException(szMsg+"Unknown framebuffer error");
	}
}

#endif
