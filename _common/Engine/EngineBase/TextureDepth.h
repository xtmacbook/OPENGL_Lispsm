//---------------------------------------------------------------------------
#ifndef TextureDepthH
#define TextureDepthH

#include "Texture2D.h"
//---------------------------------------------------------------------------
class TextureDepth : public Texture2D {
public:
	TextureDepth();
};

typedef SmartPointer<TextureDepth> SmpTextureDepth;

#endif
