//---------------------------------------------------------------------------
#ifndef TextureH
#define TextureH

#include <string>
#include "../Image/Image.h"
#include "EngineException.h"
//---------------------------------------------------------------------------
class Texture : public Base {
	enum TexState { GRAPHIC, MEM, DISC };
	TexState texState;
	void setToGraphicState();
protected:
	Image image;
	uint notUsed;
	bool compressed;

	void graphicToMem();
	void memToDisc();

	virtual void deleteTexRepresentation() = 0;
	virtual void generateTexRepresentation() = 0;
	virtual void reloadFromImage() /*throw(TextureException)*/ = 0;
	virtual void bind() = 0;
	friend class TextureManager;
	void setFileName(const std::string&);
public:
	Texture(): notUsed(0), texState(DISC), compressed(true) { }
	virtual ~Texture();


	void loadFromFile(const std::string&)
        /*throw(FileException,ExistException,TextureException)*/;
	//copy of image is made
	void loadFromImage(const Image&) /*throw(TextureException)*/;
	void makeActive();
	const std::string& getFileName() const;
	virtual const bool hasAlpha() const = 0;
};

typedef SmartPointer<Texture> SmpTexture;

#endif
