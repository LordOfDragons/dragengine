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
#ifndef _DECLASSCONNECTIONLISTENER_H_
#define _DECLASSCONNECTIONLISTENER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deClassNetworkState;
class deClassNetworkMessage;
class deScriptingDragonScript;



// connection listener class
class deClassConnectionListener : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassNetworkState *pClsNS;
	deClassNetworkMessage *pClsNM;
	
public:
	// constructor
	deClassConnectionListener( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassConnectionListener();
	
	// internal functions
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	void CreateClassMembers( dsEngine *engine );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	
	inline deClassNetworkMessage *GetClassNetworkMessage() const{ return pClsNM; }
	inline deClassNetworkState *GetClassNetworkState() const{ return pClsNS; }
	
	/** \brief Function index for messageReceived(). */
	inline int GetFuncIndexMessageReceived() const{ return pFuncIndexMessageReceived; }
	
	/** \brief Function index for linkState(). */
	inline int GetFuncIndexLinkState() const{ return pFuncIndexLinkState; }
	
private:
	int pFuncIndexMessageReceived;
	int pFuncIndexLinkState;
	
	struct sInitData{
		dsClass *clsConL, *clsVoid, *clsInt, *clsBool, *clsNM, *clsNS;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	
	DEF_NATFUNC( nfConnectionClosed );
	DEF_NATFUNC( nfMessageProgress );
	DEF_NATFUNC( nfMessageReceived );
	DEF_NATFUNC( nfLinkState );
#undef DEF_NATFUNC
};

// end of include only once
#endif
