//---------------------------------------------------------------------------
#pragma hdrstop
#include "TextureGL.h"
//#pragma package(smart_init)
#pragma warning(disable:4290) //ignoring exception specification
//---------------------------------------------------------------------------
TextureGL::TextureGL(): Texture(), alpha(false) {
	colorFormat = GL_RGB;
	targetType = GL_TEXTURE_2D;
	wrapS = GL_REPEAT;
	wrapT = GL_REPEAT;
	minFilter = GL_LINEAR_MIPMAP_LINEAR;
	maxFilter = GL_LINEAR;
}


void TextureGL::bind() {
	glBindTexture(targetType,texObject);
}

void TextureGL::deleteTexRepresentation() {
	glDeleteTextures(1,&texObject);
}

void TextureGL::generateTexRepresentation() {
	glGenTextures(1,&texObject);
}

void TextureGL::reloadFromImage() throw(TextureException) {
//	if(image == 0) {
//		throw TextureException("TextureGL::reloadFromImage called, but no image present");
//	}
	bind();
	glTexParameteri(targetType,GL_TEXTURE_WRAP_S,wrapS);
	glTexParameteri(targetType,GL_TEXTURE_WRAP_T,wrapT);
	glTexParameteri(targetType,GL_TEXTURE_MIN_FILTER,minFilter);
	glTexParameteri(targetType,GL_TEXTURE_MAG_FILTER,maxFilter);
	GLenum internFormat;
	if(image.getImageType() == Image::TGA) {
		if(image.getBpp() == 32) {
			colorFormat = GL_BGRA_EXT;
			if(compressed) {
				internFormat = GL_COMPRESSED_RGBA_ARB;
			}
			else {
				internFormat = GL_RGBA;
			}
			alpha = true;
		}
		else {
			colorFormat = GL_BGR_EXT;
			if(compressed) {
				internFormat = GL_COMPRESSED_RGB_ARB;
			}
			else {
				internFormat = GL_RGB;
			}
		}
	}
	
/*	if(GLEW_SGIS_generate_mipmap) { //not nice with radeon
		glTexParameteri(targetType,GL_GENERATE_MIPMAP_SGIS,GL_TRUE);
		glTexImage2D(targetType,0,internFormat,image.getWidth(),image.getHeight(),
			0,colorFormat,GL_UNSIGNED_BYTE,image.getData());
	}
	else*/ {
		if(0 != gluBuild2DMipmaps(targetType,internFormat,image.getWidth(),
			image.getHeight(),colorFormat,GL_UNSIGNED_BYTE,image.getData())) {
			throw TextureException("gluBuild2DMipmaps error");
		}
	}
}

/*void Texture::copyFromCurBuffer(const int vX, const int vY,
		const int vWidth, const int vHeight) {
	bind();
	glTexParameteri(targetType,GL_TEXTURE_WRAP_S,wrapS);
	glTexParameteri(targetType,GL_TEXTURE_WRAP_T,wrapT);
	glTexParameteri(targetType,GL_TEXTURE_MIN_FILTER,minFilter);
	glTexParameteri(targetType,GL_TEXTURE_MAG_FILTER,maxFilter);
	glCopyTexImage2D(targetType,0,internFormat,vX,vY,vWidth,vHeight,0);
/*	glTexImage2D(targetType,0,internFormat,vWidth,vHeight,0,
		colorFormat,GL_UNSIGNED_INT,0);
	glCopyTexSubImage2D(targetType,0,0,0,vX,vY,vWidth,vHeight);*/
//}

