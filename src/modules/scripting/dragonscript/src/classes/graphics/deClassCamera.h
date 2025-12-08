/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _DECLASSCAMERA_H_
#define _DECLASSCAMERA_H_

#include <libdscript/libdscript.h>

class deEngine;
class deCamera;
class deClassVector;
class deClassDVector;
class deClassEffect;
class deScriptingDragonScript;



/**
 * Camera script class
 */
class deClassCamera : public dsClass{
private:
	deScriptingDragonScript &pDS;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create script class. */
	deClassCamera(deScriptingDragonScript &ds);
	
	/** \brief Clean up script class. */
	~deClassCamera();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Get camera from real object. */
	deCamera *GetCamera(dsRealObject *myself) const;
	
	/** Push camera to real object. */
	void PushCamera(dsRunTime *rt, deCamera *camera);
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsCamera;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBoolean;
		dsClass *clsObject;
		
		dsClass *clsDVector;
		dsClass *clsVector;
		dsClass *clsQuaternion;
		dsClass *clsEffect;
		dsClass *clsPoint;
		dsClass *clsLayerMask;
		dsClass *clsWorld;
		dsClass *clsDMatrix;
		dsClass *clsCurveBezier;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfGetOrientation);
	DEF_NATFUNC(nfSetOrientation);
	DEF_NATFUNC(nfGetFov);
	DEF_NATFUNC(nfSetFov);
	DEF_NATFUNC(nfGetFovRatio);
	DEF_NATFUNC(nfSetFovRatio);
	DEF_NATFUNC(nfGetImageDistance);
	DEF_NATFUNC(nfSetImageDistance);
	DEF_NATFUNC(nfGetViewDistance);
	DEF_NATFUNC(nfSetViewDistance);
	
	DEF_NATFUNC(nfGetEnableHDRR);
	DEF_NATFUNC(nfSetEnableHDRR);
	DEF_NATFUNC(nfGetExposure);
	DEF_NATFUNC(nfSetExposure);
	DEF_NATFUNC(nfGetLowestIntensity);
	DEF_NATFUNC(nfSetLowestIntensity);
	DEF_NATFUNC(nfGetHighestIntensity);
	DEF_NATFUNC(nfSetHighestIntensity);
	DEF_NATFUNC(nfGetAdaptionTime);
	DEF_NATFUNC(nfSetAdaptionTime);
	DEF_NATFUNC(nfResetAdaptedIntensity);
	
	DEF_NATFUNC(nfGetEnableGI);
	DEF_NATFUNC(nfSetEnableGI);
	
	DEF_NATFUNC(nfGetWhiteIntensity);
	DEF_NATFUNC(nfSetWhiteIntensity);
	DEF_NATFUNC(nfGetBloomIntensity);
	DEF_NATFUNC(nfSetBloomIntensity);
	DEF_NATFUNC(nfGetBloomStrength);
	DEF_NATFUNC(nfSetBloomStrength);
	DEF_NATFUNC(nfGetBloomSize);
	DEF_NATFUNC(nfSetBloomSize);
	DEF_NATFUNC(nfGetBloomBlend);
	DEF_NATFUNC(nfSetBloomBlend);
	DEF_NATFUNC(nfGetToneMapCurve);
	DEF_NATFUNC(nfSetToneMapCurve);
	
	DEF_NATFUNC(nfProject);
	DEF_NATFUNC(nfBackProject);
	DEF_NATFUNC(nfBackProject2);
	
	DEF_NATFUNC(nfGetLayerMask);
	DEF_NATFUNC(nfSetLayerMask);
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC(nfGetMatrix);
	DEF_NATFUNC(nfGetInverseMatrix);
	
	DEF_NATFUNC(nfGetEffectCount);
	DEF_NATFUNC(nfGetEffectAt);
	DEF_NATFUNC(nfAddEffect);
	DEF_NATFUNC(nfRemoveEffect);
	DEF_NATFUNC(nfRemoveAllEffects);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

#endif
