#ifndef EngineExceptionH
#define EngineExceptionH

#include "../../Base/BaseException.h"
//---------------------------------------------------------------------------
struct EngineException : public BaseException {
	EngineException(const std::string& vMsg): BaseException(vMsg) { };
};

struct TextureException : public EngineException {
	TextureException(const std::string& vMsg): EngineException(vMsg) { };
};


#endif
 