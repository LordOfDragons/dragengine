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

// include only once
#ifndef _DECLASSLUMIMETER_H_
#define _DECLASSLUMIMETER_H_

// includes
#include <libdscript/libdscript.h>



// predefinitions
class deEngine;
class deLumimeter;
class deClassVector;
class deClassDVector;
class deClassColor;
class deScriptingDragonScript;



// lumimeter script class
class deClassLumimeter : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassVector *pClsVec;
	deClassDVector *pClsDVec;
	deClassColor *pClsClr;
	
public:
	// constructor
	deClassLumimeter(deEngine *gameEngine, deScriptingDragonScript *scrMgr);
	virtual ~deClassLumimeter();
	
	// internal functions
	virtual void CreateClassMembers(dsEngine *engine);
	
	deLumimeter *GetLumimeter(dsRealObject *myself) const;
	void PushLumimeter(dsRunTime *rt, deLumimeter::Ref lumimeter);
	
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassColor *GetClassColor() const{ return pClsClr; }
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }

private:
	struct sInitData{
		dsClass *clsLM, *clsVoid, *clsInt, *clsFlt, *clsBool, *clsObj;
		dsClass *clsVec, *clsDVec, *clsClr;
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
	
	DEF_NATFUNC(nfGetPosition);
	DEF_NATFUNC(nfSetPosition);
	DEF_NATFUNC(nfGetDirection);
	DEF_NATFUNC(nfSetDirection);
	DEF_NATFUNC(nfGetConeInnerAngle);
	DEF_NATFUNC(nfSetConeInnerAngle);
	DEF_NATFUNC(nfGetConeOuterAngle);
	DEF_NATFUNC(nfSetConeOuterAngle);
	DEF_NATFUNC(nfGetConeExponent);
	DEF_NATFUNC(nfSetConeExponent);
	DEF_NATFUNC(nfGetParentWorld);
	
	DEF_NATFUNC(nfMeasureLuminance);
	DEF_NATFUNC(nfMeasureColor);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
