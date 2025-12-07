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
#ifndef _DECLASSCOLOR_H_
#define _DECLASSCOLOR_H_

// includes
#include <libdscript/libdscript.h>
#include "dragengine/common/math/decMath.h"

// predefinitions
class deEngine;
class deScriptingDragonScript;

// color script class
class deClassColor : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
public:
	// constructor
	deClassColor(deEngine *gameEngine, deScriptingDragonScript *scriptManager);
	~deClassColor();
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	void InitStatics(dsRunTime *RT);
	inline deEngine *GetGameEngine() const{return pGameEngine;}
	inline deScriptingDragonScript *GetScriptModule() const{return pScrMgr;}
	const decColor &GetColor(dsRealObject *This) const;
	void PushColor(dsRunTime *RT, const decColor &color);
	void AddColorConstant(dsRunTime *RT, const char *name, const decColor &color);

private:
	struct sInitData{
		dsClass *clsClr, *clsVoid, *clsBool, *clsStr, *clsObj, *clsInt, *clsFlt;
		dsClass *clsFileReader, *clsFileWriter;
		dsClass *clsArray;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfNew2);
	DEF_NATFUNC(nfNew3);
	DEF_NATFUNC(nfNewRGB);
	DEF_NATFUNC(nfNewRGBA);
	DEF_NATFUNC(nfNewHSV);
	DEF_NATFUNC(nfNewHSL);
	DEF_NATFUNC(nfNewCMYK);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetRed);
	DEF_NATFUNC(nfGetGreen);
	DEF_NATFUNC(nfGetBlue);
	DEF_NATFUNC(nfGetAlpha);
	DEF_NATFUNC(nfClamp);
	DEF_NATFUNC(nfInvert);
	
	DEF_NATFUNC(nfIsEqual);
	DEF_NATFUNC(nfIsEqual2);
	
	DEF_NATFUNC(nfReadFromFile);
	DEF_NATFUNC(nfWriteToFile);
	
	DEF_NATFUNC(nfOpAdd);
	DEF_NATFUNC(nfOpSubtract);
	DEF_NATFUNC(nfOpScale);
	DEF_NATFUNC(nfOpDivide);
	DEF_NATFUNC(nfOpMultiply);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
	DEF_NATFUNC(nfToString);
	DEF_NATFUNC(nfToStringPrecision);
	DEF_NATFUNC(nfToHSV);
	DEF_NATFUNC(nfToHSL);
	DEF_NATFUNC(nfToCMYK);
#undef DEF_NATFUNC
};

// end of include only once
#endif
