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
#ifndef _DECLASSNETWORK_H_
#define _DECLASSNETWORK_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deServer;
class deScriptingDragonScript;
class deClassServerListener;



// network script class
class deClassServer : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassServerListener *pClsSvrL;
	
public:
	// constructor
	deClassServer(deEngine *gameEngine, deScriptingDragonScript *scrMgr);
	~deClassServer();
	
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	deServer *GetServer(dsRealObject *myself) const;
	void PushServer(dsRunTime *rt, deServer *server);
	inline deEngine *GetGameEngine() const{return pGameEngine;}
	inline deScriptingDragonScript *GetScriptModule() const{return pScrMgr;}
	
	inline deClassServerListener *GetClassServerListener() const{return pClsSvrL;}
	
private:
	struct sInitData{
		dsClass *clsSvr, *clsVoid, *clsInt, *clsBool, *clsStr, *clsObj, *clsSvrL;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetAddress);
	
	DEF_NATFUNC(nfListenOn);
	DEF_NATFUNC(nfStopListening);
	
	DEF_NATFUNC(nfGetServerListener);
	DEF_NATFUNC(nfSetServerListener);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
