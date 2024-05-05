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
#ifndef _DECLASSANIMATION_H_
#define _DECLASSANIMATION_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deAnimation;
class deScriptingDragonScript;
class deClassResourceListener;

// animation script class
class deClassAnimation : public dsClass{
private:
	deEngine *p_gameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassResourceListener *pClsRN;
	
public:
	// constructor
	deClassAnimation(deEngine *GameEngine, deScriptingDragonScript *ScrMgr);
	~deClassAnimation();
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	inline deEngine *GetGameEngine() const{ return p_gameEngine; }
	
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassResourceListener *GetClassResourceListener() const{ return pClsRN; }
	
	deAnimation *GetAnimation(dsRealObject *This) const;
	void PushAnimation( dsRunTime *rt, deAnimation *anim );
	
private:
	struct sInitData{
		dsClass *clsAnim, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj, *clsRN;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfLoad);
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC(nfSave);
	DEF_NATFUNC(nfDestructor);
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetMovePlaytime);
	DEF_NATFUNC( nfGetBoneCount );
	DEF_NATFUNC( nfIndexOfBoneNamed );
	DEF_NATFUNC( nfGetBoneName );
	DEF_NATFUNC( nfGetMoveFPS );
	DEF_NATFUNC( nfGetMoveCount );
	DEF_NATFUNC( nfGetMoveName );
	DEF_NATFUNC( nfIndexOfMoveNamed );
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
