//---------------------------------------------------------------------------
#ifndef TextureGLH
#define TextureGLH

#include "../EngineBase/Texture.h"
#include <GL/glHeader.h>
//---------------------------------------------------------------------------
class TextureGL : public Texture {
protected:
	GLenum colorFormat;
	GLenum targetType;
	GLint wrapS,wrapT;
	GLint minFilter,maxFilter;
	GLuint texObject;
	bool alpha;
	virtual void deleteTexRepresentation();
	virtual void generateTexRepresentation();
	virtual void reloadFromImage() /*throw(TextureException)*/;
public:
	TextureGL();
	virtual void bind();
	virtual const bool hasAlpha() const { return alpha; }
//	void copyFromCurBuffer(const int vX, const int vY, const int vWidth, const int vHeight);
};

typedef SmartPointer<TextureGL> SmpTextureGL;

#endif
