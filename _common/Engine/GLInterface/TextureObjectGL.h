//---------------------------------------------------------------------------
#ifndef TextureObjectGLH
#define TextureObjectGLH

#include <Base/Singleton.h>
#include "../Smp.h"
#include <GL/glHeader.h>
//---------------------------------------------------------------------------
class TextureObjectGL : public Singleton, public Base {
protected:
	GLenum targetType;
	GLenum mDataType;
	GLint externFormat;
	GLint internFormat;
	GLuint texObject;
	bool mipmapping;
	unsigned width;
	unsigned height;
	unsigned depth;
//	virtual void reloadFromImage() /*throw(TextureException)*/;
//	void copyFromCurBuffer(const int vX, const int vY, const int vWidth, const int vHeight);
public:
	TextureObjectGL(const GLenum vTargetType = GL_TEXTURE_2D,
		const GLint vFormatExtern = GL_RGBA, const GLint vFormatIntern = GL_RGBA, 
		const bool mipmap = false, const GLenum dataType_ = GL_UNSIGNED_BYTE);
	~TextureObjectGL();
	const GLuint getTexObject() const { return texObject; }
	const unsigned getWidth() const { return width; }
	const unsigned getHeight() const { return height; }
	const unsigned getDepth() const { return depth; }
	void bind() const;
	void load(void* data, const unsigned vWidth, const unsigned vHeight = 0, const unsigned vDepth = 0);
	void resize(const unsigned vWidth, const unsigned vHeight = 0, const unsigned vDepth = 0);
	void copyCurrentBufferToTexture();
	void copyCurrentBufferToTexture(const unsigned vWidth, const unsigned vHeight);
	void filterLinear();
	void filterNearest();
	void clampToEdge();
	void mirrorRepeat();

	const GLenum type() const { return targetType; }
	const GLint formatExtern() const { return externFormat; }
	const GLint formatIntern() const { return internFormat; }
	const bool mipMapping() const { return mipmapping; }
};

typedef SmartPointer<TextureObjectGL> SmpTextureObjectGL;
#endif
