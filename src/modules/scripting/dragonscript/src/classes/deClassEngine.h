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
#ifndef _DECLASSENGINE_H_
#define _DECLASSENGINE_H_

// includes
#include <libdscript/libdscript.h>
#include <dragengine/common/utils/decTimer.h>

// predefinitions
class deEngine;
class deScriptingDragonScript;

// engine script class
class deClassEngine : public dsClass{
private:
	deEngine *p_gameEngine;
	deScriptingDragonScript *p_scrMgr;
	decTimer pDebugTimer;
	
public:
	// constructor
	deClassEngine(deEngine *GameEngine, deScriptingDragonScript *ScrMgr);
	~deClassEngine();
	// management
	void CreateClassMembers(dsEngine *engine);
	inline deEngine *GetGameEngine() const{ return p_gameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return p_scrMgr; }
	inline decTimer &GetDebugTimer(){ return pDebugTimer; }

private:
	struct sInitData{
		dsClass *clsEng, *clsVoid, *clsInt, *clsFlt, *clsStr;
		dsClass *clsWin, *clsGame, *clsBool;
		dsClass *clsDict;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	// engine
	DEF_NATFUNC( nfGetElapsedTime );
	DEF_NATFUNC( nfResetTimers );
	DEF_NATFUNC( nfGetGame );
	DEF_NATFUNC( nfGetFPSRate );
	DEF_NATFUNC( nfQuit );
	DEF_NATFUNC( nfGetEditMode );
	DEF_NATFUNC( nfLoadingResourceCount );
	DEF_NATFUNC( nfLog );
	DEF_NATFUNC( nfGetScriptModuleStats );
#undef DEF_NATFUNC
};

// end of include only once
#endif
