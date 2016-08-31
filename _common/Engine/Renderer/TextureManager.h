//---------------------------------------------------------------------------
#ifndef TextureManagerH
#define TextureManagerH

#include <vector>
//#include <list>
#include "../EngineBase/Texture.h"
#include "../../Base/Debug.h"
//---------------------------------------------------------------------------
class ITexture;
class Renderer;

class TextureManager : public Base {
protected:
	typedef std::vector<SmpTexture> VecTex;
//	typedef std::list<SmpTexture> LstTex;
    VecTex vecTex;
//	LstTex lstFreeTex;
	uint totalUsageCount;
    Texture* getTexture(cuint);
	void use(Texture& tex);
	TextureManager(const TextureManager&) { throw SingeltonException(); }
	TextureManager& operator=(const TextureManager&) { throw SingeltonException(); }
public:
	TextureManager(): totalUsageCount(0), updateRoundsToMem(20), updateRoundsToDisk(400) { }
	virtual ~TextureManager() {
		DEBUG("TextureManager");
	}

	uint updateRoundsToMem;
	uint updateRoundsToDisk;

    const ITexture addTexture(const std::string&, Renderer&);
	void deleteFreeTexture(ITexture&);
	void deleteTexture(ITexture&);
	void updateTextureUsageDependentLoadState();
	friend class ITexture;
};
typedef SmartPointer<TextureManager> SmpTextureManager;

class ITexture {
	uint id;
	SmpTextureManager textureManager;
	static uint NO_TEX_ID;
protected:
	friend class TextureManager;
	cuint getID() const { return id; }
	//void setID(cuint vID) { id = vID; }
	ITexture(cuint vID, TextureManager* m): id(vID), textureManager(m) { }
	const bool operator==(const ITexture& tex) const;

public:
	ITexture(): id(NO_TEX_ID) { }
	Texture* getTexture();
	const bool empty() const { return (id == NO_TEX_ID); }
};

#endif
