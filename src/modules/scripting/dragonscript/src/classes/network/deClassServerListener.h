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
#ifndef _DECLASSSERVERLISTENER_H_
#define _DECLASSSERVERLISTENER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deClassConnection;
class deScriptingDragonScript;



// component listener class
class deClassServerListener : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassConnection *pClsCon;
	
public:
	// constructor
	deClassServerListener(deEngine *gameEngine, deScriptingDragonScript *scrMgr);
	~deClassServerListener();
	// internal functions
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	void CreateClassMembers(dsEngine *engine);
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	
	inline deClassConnection *GetClassConnection() const{ return pClsCon; }
	
private:
	struct sInitData{
		dsClass *clsSvrL, *clsVoid, *clsBool, *clsInt, *clsStr, *clsCon;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfClientConnected);
#undef DEF_NATFUNC
};

// end of include only once
#endif
