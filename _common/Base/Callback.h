//Copyright and Disclaimer:
//This code is copyright Daniel Scherzer, 2005.
//---------------------------------------------------------------------------
#ifndef CallbackH
#define CallbackH
//---------------------------------------------------------------------------
template<class PARAM>
struct ICallback {
	virtual void execute(PARAM) const = 0;
};


template<class INSTANCE, class PARAM>
class TCallback : public ICallback<PARAM> {
public:
	typedef TCallback<INSTANCE,PARAM> SELF;
	typedef INSTANCE* PClass;
	typedef void (INSTANCE::*PFunc) (PARAM);

private:
	PClass pClass;
	PFunc  pFunc;

public:
	TCallback(): pFunc(0), pClass(0) { }

	TCallback(PClass pInstance, PFunc pFunction):
		pClass(pInstance), pFunc(pFunction) { }
	
	TCallback(const TCallback& p): pClass(p.pClass), pFunc(p.pFunc) { }
	
	SELF& operator=(const SELF& p) {
		pClass = p.pClass;
		pFunc = p.pFunc;
	}
	
	virtual void execute(PARAM p) const {
		if(0 == pFunc) {
			throw "Callback function has not been defined.";
		}
		(pClass->*pFunc)(p);
	}

};

#endif // CallbackH




