//---------------------------------------------------------------------------
#pragma hdrstop
#include <Mathematic/Mathematic.h>
#include "TextureObjectGL.h"
//#pragma package(smart_init)
//---------------------------------------------------------------------------
TextureObjectGL::TextureObjectGL(const GLenum vTargetType, const GLint vFormatExtern, 
								 const GLint vFormatIntern, const bool mipmap, const GLenum dataType_):
//								dataType(GL_FLOAT) {
								mDataType(dataType_),
								mipmapping(mipmap) {
	glGenTextures(1,&texObject);
	targetType = vTargetType;
	externFormat = vFormatExtern;
	internFormat = vFormatIntern;
	width = 0;
	height = 0;
	depth = 0;
}

TextureObjectGL::~TextureObjectGL() {
	glDeleteTextures(1,&texObject);
}


void TextureObjectGL::bind() const {
	glBindTexture(targetType,texObject);
}

void TextureObjectGL::load(void* data, const unsigned vWidth, const unsigned vHeight, const unsigned vDepth) {
	bind();
	if(mipmapping) {
		glTexParameteri(type(),GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
	}
	if(0 == vDepth) {
		if(0 == vHeight) {
			glTexImage1D(type(),0,formatIntern(),vWidth,0,formatExtern(),mDataType,data);
		}
		else {
			glTexImage2D(type(),0,formatIntern(),vWidth,vHeight,0,formatExtern(),mDataType,data);
		}
	}
	else {
		glTexImage3D(type(),0,formatIntern(),vWidth,vHeight,vDepth,0,formatExtern(),mDataType,data);
	}
	width = vWidth;
	height = vHeight;
	depth = vDepth;
	CHECK_GL_ERROR();
}

void TextureObjectGL::resize(const unsigned vWidth, const unsigned vHeight, const unsigned vDepth) {
	if( (width != vWidth) || (height != vHeight) || (depth != vDepth) ) {
		load(0,vWidth,vHeight,vDepth);
	}
}

void TextureObjectGL::copyCurrentBufferToTexture() {
	bind();
	glCopyTexSubImage2D(type(),0,0,0,0,0,width,height);
	if(mipmapping) 
		throw GLException("TextureObjectGL no mipmapping for copy sub image");
	CHECK_GL_ERROR();
}

void TextureObjectGL::copyCurrentBufferToTexture(const unsigned vWidth, const unsigned vHeight) {
	bind();
	if(vWidth <= width && vHeight <= height) {
		glCopyTexSubImage2D(type(),0,0,0,0,0,vWidth,vHeight);
	}
	else {
		glCopyTexSubImage2D(type(),0,0,0,0,0,width,height);
	}
	CHECK_GL_ERROR();
}

void TextureObjectGL::filterLinear() {
	if(mipmapping) {
		glTexParameteri(type(),GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	}
	else {
		glTexParameteri(type(),GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	}
	glTexParameteri(type(),GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

void TextureObjectGL::filterNearest() {
	if(mipmapping) {
		glTexParameteri(type(),GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
	}
	else {
		glTexParameteri(type(),GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	}
	glTexParameteri(type(),GL_TEXTURE_MAG_FILTER,GL_NEAREST);
}

void TextureObjectGL::clampToEdge() {
	glTexParameteri(type(),GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	if(getHeight() != 0) {
		glTexParameteri(type(),GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
		if(getDepth() != 0) {
			glTexParameteri(type(),GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
		}
	}
}

void TextureObjectGL::mirrorRepeat() {
	glTexParameteri(type(),GL_TEXTURE_WRAP_S,GL_MIRRORED_REPEAT_IBM);
	if(getHeight() != 0) {
		glTexParameteri(type(),GL_TEXTURE_WRAP_T,GL_MIRRORED_REPEAT_IBM);
		if(getDepth() != 0) {
			glTexParameteri(type(),GL_TEXTURE_WRAP_R,GL_MIRRORED_REPEAT_IBM);
		}
	}
}

/*void TextureObjectGL::reloadFromImage() throw(TextureException) {
//	if(image == 0) {
//		throw TextureException("TextureGL::reloadFromImage called, but no image present");
//	}
	bind();
	glTexParameteri(targetType,GL_TEXTURE_WRAP_S,wrapS);
	glTexParameteri(targetType,GL_TEXTURE_WRAP_T,wrapT);
	glTexParameteri(targetType,GL_TEXTURE_MIN_FILTER,minFilter);
	glTexParameteri(targetType,GL_TEXTURE_MAG_FILTER,maxFilter);
	if(image.getImageType() == Image::TGA) {
		if(image.getBpp() == 32) {
			internFormat = GL_COMPRESSED_RGBA_ARB;
			colorFormat = GL_BGRA_EXT;
			alpha = true;
		}
		else {
			colorFormat = GL_BGR_EXT;
			internFormat = GL_COMPRESSED_RGB_ARB;
		}
	}
	
	if(GLEW_SGIS_generate_mipmap) {
		glTexParameteri(targetType,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
		glTexImage2D(targetType,0,internFormat,image.getWidth(),image.getHeight(),
			0,colorFormat,GL_UNSIGNED_BYTE,image.getData());
	}
	else {
		if(0 != gluBuild2DMipmaps(targetType,internFormat,image.getWidth(),
			image.getHeight(),colorFormat,GL_UNSIGNED_BYTE,image.getData())) {
			throw TextureException("gluBuild2DMipmaps error");
		}
	}
}*/

