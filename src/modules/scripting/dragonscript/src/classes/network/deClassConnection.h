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
#ifndef _DECLASSCONNECTION_H_
#define _DECLASSCONNECTION_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deConnection;
class deClassNetworkState;
class deClassNetworkMessage;
class deClassConnectionListener;
class deScriptingDragonScript;



// connection script class
class deClassConnection : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassNetworkState *pClsNS;
	deClassNetworkMessage *pClsNM;
	deClassConnectionListener *pClsConL;
	
public:
	// constructor
	deClassConnection(deEngine *gameEngine, deScriptingDragonScript *scrMgr);
	~deClassConnection();
	
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	deConnection *GetConnection(dsRealObject *myself) const;
	void PushConnection(dsRunTime *rt, deConnection *connection);
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassNetworkState *GetClassNetworkState() const{ return pClsNS; }
	inline deClassNetworkMessage *GetClassNetworkMessage() const{ return pClsNM; }
	inline deClassConnectionListener *GetClassConnectionListener() const{ return pClsConL; }
	
private:
	struct sInitData{
		dsClass *clsCon, *clsVoid, *clsInt, *clsBool, *clsObj, *clsStr;
		dsClass *clsNS, *clsNM, *clsConL;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{\
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfNew);
	DEF_NATFUNC(nfDestructor);
	
	DEF_NATFUNC(nfGetLocalAddress);
	DEF_NATFUNC(nfGetRemoteAddress);
	DEF_NATFUNC(nfGetConnected);
	
	DEF_NATFUNC(nfConnectTo);
	DEF_NATFUNC(nfDisconnect);
	DEF_NATFUNC(nfSendMessage);
	DEF_NATFUNC(nfSendReliableMessage);
	DEF_NATFUNC(nfLinkState);
	
	DEF_NATFUNC(nfGetConnectionListener);
	DEF_NATFUNC(nfSetConnectionListener);
	
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
