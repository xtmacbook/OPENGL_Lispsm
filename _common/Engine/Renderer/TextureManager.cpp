//---------------------------------------------------------------------------
#include "../../Base/FileTools.h"
#pragma hdrstop
#include "TextureManager.h"
#include "Renderer.h"
//#pragma package(smart_init)
//---------------------------------------------------------------------------
uint ITexture::NO_TEX_ID = UINT_MAX;

const ITexture TextureManager::addTexture(const std::string& texFile, Renderer& r) {
    if(texFile.empty()) {
		throw TextureException("empty TextureFile name");
	}
	else {
		for(uint id = 0; id < vecTex.size(); id++) {
			Texture& tex = *vecTex[id];
			if(texFile == tex.getFileName()) {
				//rereference texture -> notUsed has a value
				return ITexture(id,this);
			}
		}
		std::string f;
		if(!Tools::fileExists(texFile)) {
			f = Tools::changeFileExt(texFile,".tga");
			if(!Tools::fileExists(f)) {
				throw TextureException(texFile+" not found or convertable");
			}
		}
		else {
			f = texFile;
		}
		SmpTexture tex = r.createTexture();	//new texture -> notUsed has value 0
		tex->setFileName(f);
		vecTex.push_back(tex);
	    return ITexture(vecTex.size()-1,this);
    }
}

Texture* TextureManager::getTexture(cuint id) {
	return vecTex[id];
}

void TextureManager::use(Texture& tex) {
	totalUsageCount++;
	tex.notUsed = 0; // tex used -> notUsed = 0
}

void TextureManager::deleteFreeTexture(ITexture& texID) {
	if(0 != texID.getTexture()) {
		vecTex[texID.getID()] = 0;
		texID = ITexture();
	}
}

void TextureManager::deleteTexture(ITexture& texID) {
	if(0 != texID.getTexture()) {
//		lstFreeTex.push_back(vecTex[texID.getID()]);  // ACHTUNG: funktioniert so nur mit SmartPointern
		vecTex[texID.getID()] = 0; //delete the ref
		texID = ITexture();
	}
}

void TextureManager::updateTextureUsageDependentLoadState() {
	//todo: updateTextureUsageDependentLoadState
	totalUsageCount = 0;
	for(uint id = 0; id < vecTex.size(); id++) {
		Texture* tex = vecTex[id];
		if(0 != tex) {
			if(updateRoundsToMem <= tex->notUsed) {
				//unused texture
				tex->graphicToMem();
				if(updateRoundsToDisk <= tex->notUsed) {
					//even less used
					tex->memToDisc();
				}
			}
			tex->notUsed++;
		}
	}
}

const bool ITexture::operator==(const ITexture& tex) const { 
	return (id == tex.id) && (textureManager == tex.textureManager); 
}

Texture* ITexture::getTexture() {
	if(empty()) {
		return 0;
	}
	else {
		Texture* tex = textureManager->getTexture(id);
		if(0 == tex) {
			id = NO_TEX_ID;
			return 0;
		}
		else {
			textureManager->use(*tex);
			return tex;
		}
	}
} 

