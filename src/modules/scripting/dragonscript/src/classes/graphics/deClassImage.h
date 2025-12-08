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
#ifndef _DECLASSIMAGE_H_
#define _DECLASSIMAGE_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deImage;
class deScriptingDragonScript;
class deClassResourceListener;

// image script class
class deClassImage : public dsClass{
private:
	deEngine *p_gameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassResourceListener *pClsRN;
	
public:
	// constructor
	deClassImage(deEngine *GameEngine, deScriptingDragonScript *ScrMgr);
	~deClassImage();
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	inline deEngine *GetGameEngine() const{ return p_gameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	deImage *GetImage(dsRealObject *This) const;
	void PushImage(dsRunTime *rt, deImage::Ref image);
	
	inline deClassResourceListener *GetClassResourceListener() const{ return pClsRN; }

private:
	struct sInitData{
		dsClass *clsImg, *clsVoid, *clsBool, *clsStr, *clsInt, *clsObj, *clsRN;
		dsClass *clsPt;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfLoadAsynchron);
	DEF_NATFUNC(nfDestructor);
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetWidth);
	DEF_NATFUNC(nfGetHeight);
	DEF_NATFUNC(nfGetDepth);
	DEF_NATFUNC(nfGetComponentCount);
	DEF_NATFUNC(nfGetBitCount);
	DEF_NATFUNC(nfGetSize);
	DEF_NATFUNC(nfSaveToFile);
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfEquals2);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
