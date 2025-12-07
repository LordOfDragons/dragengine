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

#ifndef _DECLASSLIGHT_H_
#define _DECLASSLIGHT_H_

#include <libdscript/libdscript.h>

class deEngine;
class deLight;
class deScriptingDragonScript;



/**
 * \brief Light class.
 */
class deClassLight : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsLightType;
	dsClass *pClsLightHintMovement;
	dsClass *pClsLightHintParameter;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create native class. */
	deClassLight(deScriptingDragonScript &ds);
	
	/** \brief Clean up native class. */
	virtual ~deClassLight();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Create class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Scripting module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Light or \em NULL if object is \em NULL. */
	deLight *GetLight(dsRealObject *myself) const;
	
	/** \brief Push light to stack or \em null light is \em NULL. */
	void PushLight(dsRunTime *rt, deLight *light);
	
	inline dsClass *GetClassLightType() const{ return pClsLightType; }
	inline dsClass *GetClassLightHintMovement() const{ return pClsLightHintMovement; }
	inline dsClass *GetClassLightHintParameter() const{ return pClsLightHintParameter; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsLight;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsBool;
		dsClass *clsObject;
		
		dsClass *clsCanvasView;
		dsClass *clsColor;
		dsClass *clsComponent;
		dsClass *clsDynamicSkin;
		dsClass *clsDVector;
		dsClass *clsLayerMask;
		dsClass *clsQuaternion;
		dsClass *clsShapeList;
		dsClass *clsSkin;
		dsClass *clsTexMatrix2;
		dsClass *clsLightType;
		dsClass *clsLightHintMovement;
		dsClass *clsLightHintParameter;
		dsClass *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetType);
	DEF_NATFUNC(nfSetType);
	DEF_NATFUNC(nfGetColor);
	DEF_NATFUNC(nfSetColor);
	DEF_NATFUNC(nfGetIntensity);
	DEF_NATFUNC(nfSetIntensity);
	DEF_NATFUNC(nfGetRange);
	DEF_NATFUNC(nfSetRange);
	DEF_NATFUNC(nfGetHalfIntensityDistance);
	DEF_NATFUNC(nfSetHalfIntensityDistance);
	DEF_NATFUNC(nfGetAmbientRatio);
	DEF_NATFUNC(nfSetAmbientRatio);
	DEF_NATFUNC(nfGetShape);
	DEF_NATFUNC(nfSetShape);
	DEF_NATFUNC(nfGetLayerMask);
	DEF_NATFUNC(nfSetLayerMask);
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfGetOrientation);
	DEF_NATFUNC(nfSetOrientation);
	DEF_NATFUNC(nfGetSpotAngle);
	DEF_NATFUNC(nfSetSpotAngle);
	DEF_NATFUNC(nfGetSpotRatio);
	DEF_NATFUNC(nfSetSpotRatio);
	DEF_NATFUNC(nfGetSpotSmoothness);
	DEF_NATFUNC(nfSetSpotSmoothness);
	DEF_NATFUNC(nfGetSpotExponent);
	DEF_NATFUNC(nfSetSpotExponent);
	
	DEF_NATFUNC(nfGetCastShadows);
	DEF_NATFUNC(nfSetCastShadows);
	DEF_NATFUNC(nfGetLayerMaskShadow);
	DEF_NATFUNC(nfSetLayerMaskShadow);
	DEF_NATFUNC(nfGetShadowIgnoreComponentCount);
	DEF_NATFUNC(nfGetShadowIgnoreComponentAt);
	DEF_NATFUNC(nfHasShadowIgnoreComponent);
	DEF_NATFUNC(nfAddShadowIgnoreComponent);
	DEF_NATFUNC(nfRemoveShadowIgnoreComponent);
	DEF_NATFUNC(nfRemoveAllShadowIgnoreComponents);
	
	DEF_NATFUNC(nfGetActivated);
	DEF_NATFUNC(nfSetActivated);
	
	DEF_NATFUNC(nfGetHintLightImportance);
	DEF_NATFUNC(nfSetHintLightImportance);
	DEF_NATFUNC(nfGetHintShadowImportance);
	DEF_NATFUNC(nfSetHintShadowImportance);
	DEF_NATFUNC(nfGetHintMovement);
	DEF_NATFUNC(nfSetHintMovement);
	DEF_NATFUNC(nfGetHintParameter);
	DEF_NATFUNC(nfSetHintParameter);
	
	DEF_NATFUNC(nfGetLightSkin);
	DEF_NATFUNC(nfSetLightSkin);
	DEF_NATFUNC(nfGetLightCanvas);
	DEF_NATFUNC(nfSetLightCanvas);
	DEF_NATFUNC(nfGetDynamicSkin);
	DEF_NATFUNC(nfSetDynamicSkin);
	DEF_NATFUNC(nfGetTransform);
	DEF_NATFUNC(nfSetTransform);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
