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

#ifndef _DEDSCLASSSKYLAYER_H_
#define _DEDSCLASSSKYLAYER_H_

#include <libdscript/libdscript.h>

class deScriptingDragonScript;

class deSky;



/**
 * \brief Sky layer script class.
 */
class deClassSkyLayer : public dsClass{
private:
	deScriptingDragonScript &pDS;
	dsClass *pClsSkyLayerTargetType;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create class. */
	deClassSkyLayer(deScriptingDragonScript &ds);
	
	/** \brief Clean up class. */
	virtual ~deClassSkyLayer();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** \brief Creates class members. */
	void CreateClassMembers(dsEngine *engine);
	
	/** \brief Push layer. */
	void PushLayer(dsRunTime *rt, deSky::Ref sky, int index);
	
	inline dsClass *GetClassSkyLayerTargetType() const{ return pClsSkyLayerTargetType; }
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsSkyLink;
		
		dsClass *clsVoid;
		dsClass *clsBool;
		dsClass *clsInt;
		dsClass *clsFloat;
		dsClass *clsStr;
		dsClass *clsObj;
		
		dsClass *clsSky;
		dsClass *clsSkyBody;
		dsClass *clsSkyTarget;
		dsClass *clsColor;
		dsClass *clsQuaternion;
		dsClass *clsVector;
		dsClass *clsSkin;
		dsClass *clsSkyLayerTargetType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetSky);
	DEF_NATFUNC(nfGetLayerIndex);
	
	DEF_NATFUNC(nfGetOffset);
	DEF_NATFUNC(nfSetOffset);
	DEF_NATFUNC(nfGetOrientation);
	DEF_NATFUNC(nfSetOrientation);
	DEF_NATFUNC(nfGetColor);
	DEF_NATFUNC(nfSetColor);
	DEF_NATFUNC(nfGetIntensity);
	DEF_NATFUNC(nfSetIntensity);
	DEF_NATFUNC(nfGetTransparency);
	DEF_NATFUNC(nfSetTransparency);
	DEF_NATFUNC(nfGetSkin);
	DEF_NATFUNC(nfSetSkin);
	
	DEF_NATFUNC(nfGetLightOrientation);
	DEF_NATFUNC(nfSetLightOrientation);
	DEF_NATFUNC(nfGetLightColor);
	DEF_NATFUNC(nfSetLightColor);
	DEF_NATFUNC(nfGetLightIntensity);
	DEF_NATFUNC(nfSetLightIntensity);
	DEF_NATFUNC(nfGetAmbientIntensity);
	DEF_NATFUNC(nfSetAmbientIntensity);
	
	DEF_NATFUNC(nfGetBodyCount);
	DEF_NATFUNC(nfSetBodyCount);
	DEF_NATFUNC(nfGetBodyAt);
	
	DEF_NATFUNC(nfGetMultiplyBySkyLight);
	DEF_NATFUNC(nfSetMultiplyBySkyLight);
	DEF_NATFUNC(nfGetMultiplyBySkyColor);
	DEF_NATFUNC(nfSetMultiplyBySkyColor);
	
	DEF_NATFUNC(nfGetTarget);
	
	DEF_NATFUNC(nfEquals);
#undef DEF_NATFUNC
};

#endif
