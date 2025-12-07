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
#ifndef _DECLASSTOUCHSENSORLISTENER_H_
#define _DECLASSTOUCHSENSORLISTENER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deClassCollider;
class deScriptingDragonScript;

// component listener class
class deClassTouchSensorListener : public dsClass{
private:
	deEngine *pGameEngine;
	deClassCollider *pClsCol;
	deScriptingDragonScript *pScrMgr;
public:
	// constructor
	deClassTouchSensorListener(deEngine *gameEngine, deScriptingDragonScript *scrMgr);
	~deClassTouchSensorListener();
	// internal functions
	inline deScriptingDragonScript *GetScriptModule() const{return pScrMgr;}
	void CreateClassMembers(dsEngine *engine);
	inline deEngine *GetGameEngine() const{return pGameEngine;}
	inline deClassCollider *GetClassCollider() const{return pClsCol;}
	
	/** \brief Function index for colliderEntered(). */
	inline int GetFuncIndexColliderEntered() const{return pFuncIndexColliderEntered;}
	
	/** \brief Function index for colliderLeft(). */
	inline int GetFuncIndexColliderLeft() const{return pFuncIndexColliderLeft;}

private:
	int pFuncIndexColliderEntered;
	int pFuncIndexColliderLeft;
	
	struct sInitData{
		dsClass *clsTSL, *clsVoid, *clsBool, *clsInt, *clsCol;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfColliderEntered);
	DEF_NATFUNC(nfColliderLeft);
#undef DEF_NATFUNC
};

// end of include only once
#endif
