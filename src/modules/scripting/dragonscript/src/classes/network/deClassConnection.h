/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
	deClassConnection( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassConnection();
	
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	deConnection *GetConnection( dsRealObject *myself ) const;
	void PushConnection( dsRunTime *rt, deConnection *connection );
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
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetLocalAddress );
	DEF_NATFUNC( nfGetRemoteAddress );
	DEF_NATFUNC( nfGetConnected );
	
	DEF_NATFUNC( nfConnectTo );
	DEF_NATFUNC( nfDisconnect );
	DEF_NATFUNC( nfSendMessage );
	DEF_NATFUNC( nfSendReliableMessage );
	DEF_NATFUNC( nfLinkState );
	
	DEF_NATFUNC( nfGetConnectionListener );
	DEF_NATFUNC( nfSetConnectionListener );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
