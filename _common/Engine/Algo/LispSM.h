//Copyright and Disclaimer:
//This code is copyright by Daniel Scherzer, Vienna University of Technology, 2005.
#ifndef LiSPSM_DMS_H
#define LiSPSM_DMS_H

#include <Engine/EngineBase/ShadowMtxCalc.h>

template<class REAL>
struct LispSM : public ShadowMtxCalc<REAL> {

	struct ILispSMSSD : public IShadowSceneData {

		virtual const M4& getEyeView() const = 0;

		virtual bool useLiSPSM() const = 0;
		virtual bool useFormula() const = 0;
		virtual void setN(const REAL& n_) = 0;
		virtual const REAL getN() const = 0;

		//for old LispSM formula from paper
		virtual const REAL getNearDist() const = 0;
		virtual const REAL getFarDist() const = 0;
	};

protected:
	const ILispSMSSD& mLispSMSSD;
	typedef Math::Geometry::Plane<REAL> Plane;
	virtual REAL getN(const M4 lightSpace, const AABox& B_ls) const;
	virtual V3 getNearCameraPointE(const ILispSMSSD&) const;
	virtual V3 getZ0_ls(const M4& lightSpace, const V3& e, const REAL& b_lsZmax, const V3& eyeDir) const;
	virtual REAL calcNoptGeneral(const M4 lightSpace, const AABox& B_ls, const ILispSMSSD&) const;
	virtual REAL calcNoptOld(const ILispSMSSD&, const REAL gamma_ = 999) const;
	virtual M4 getLispSmMtx(const M4& lightSpace, const VecVec3&, ILispSMSSD&) const;
	virtual V3 getProjViewDir_ls(const M4& lightSpace, const ILispSMSSD&) const;
	virtual void updateLightMtx(M4& lightView, M4& lightProj, const VecVec3& B, ILispSMSSD&) const;

public:
	LispSM(const ILispSMSSD& lispSMSSD_): mLispSMSSD(lispSMSSD_) { }
	virtual void updateLightMtx(M4& lightView, M4& lightProj, ILispSMSSD&) const;
};

#endif
