//---------------------------------------------------------------------------
#pragma hdrstop
#include "Texture.h"
//#pragma package(smart_init)
#pragma warning(disable:4290) //ignoring exception specification
//---------------------------------------------------------------------------
Texture::~Texture() {
	graphicToMem();
	memToDisc();
}

void Texture::setFileName(const std::string& v) { 
//	if(image != 0) {
//		image->fileName = v; 
//	}
	image.fileName = v; 
}

void Texture::setToGraphicState() {
	if(GRAPHIC != texState) {
		generateTexRepresentation();
		switch(texState) {
			case DISC:
//				if(image != 0) { 
					image.reload();
					texState = MEM;
//				}
			case MEM:
				reloadFromImage();
				texState = GRAPHIC;
				break;
		}
	}
}

void Texture::makeActive() {
	setToGraphicState();
	bind();
}

void Texture::loadFromImage(const Image& img) throw(TextureException) {
	graphicToMem();
	image = img; //copy!!
	texState = MEM;
	setToGraphicState();
}

void Texture::loadFromFile(const std::string& fileName) {
	graphicToMem();
//	image = new Image;
//	image->fileName = fileName;
	image.fileName = fileName;
	texState = DISC;
	setToGraphicState();
}

void Texture::graphicToMem() {
	if(GRAPHIC == texState) {
		deleteTexRepresentation();
		texState = MEM;
	}
}

void Texture::memToDisc() {
//	if( (MEM == texState) && (image != 0) ) {
//		image->clear();
	if(MEM == texState) {
		image.clear();
		texState = DISC;
	}
}

const std::string& Texture::getFileName() const { 
	return image.fileName;
/*	if(image == 0) {
		throw TextureException("Texture::getFileName called but no image present");
	}
	return image->fileName; */
}
