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
	deClassServer( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassServer();
	
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	deServer *GetServer( dsRealObject *myself ) const;
	void PushServer( dsRunTime *rt, deServer *server );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassServerListener *GetClassServerListener() const{ return pClsSvrL; }
	
private:
	struct sInitData{
		dsClass *clsSvr, *clsVoid, *clsInt, *clsBool, *clsStr, *clsObj, *clsSvrL;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetAddress );
	
	DEF_NATFUNC( nfListenOn );
	DEF_NATFUNC( nfStopListening );
	
	DEF_NATFUNC( nfGetServerListener );
	DEF_NATFUNC( nfSetServerListener );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
