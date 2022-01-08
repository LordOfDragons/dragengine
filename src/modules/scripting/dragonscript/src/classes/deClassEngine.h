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

#ifndef _DECLASSENGINE_H_
#define _DECLASSENGINE_H_

#include <libdscript/libdscript.h>
#include <dragengine/common/utils/decTimer.h>

class deScriptingDragonScript;


/**
 * Game engine.
 */
class deClassEngine : public dsClass{
private:
	deScriptingDragonScript &pDS;
	decTimer pDebugTimer;
	bool pDefaultEnableGI;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** Create script class. */
	deClassEngine( deScriptingDragonScript &ds );
	
	/** Clean up script class. */
	virtual ~deClassEngine();
	
	
	
	/** \name Management */
	/*@{*/
	/** Script module. */
	inline deScriptingDragonScript &GetDS() const{ return pDS; }
	
	/** Create class members. */
	void CreateClassMembers( dsEngine *engine );
	
	/** Debug timer. */
	inline decTimer &GetDebugTimer(){ return pDebugTimer; }
	
	/** Default enable GI. */
	inline bool GetDefaultEnableGI() const{ return pDefaultEnableGI; }
	
	/** Set default enable GI. */
	void SetDefaultEnableGI( bool enable );
	/*@}*/
	
	
	
private:
	struct sInitData{
		dsClass *clsEngine;
		
		dsClass *clsVoid;
		dsClass *clsInteger;
		dsClass *clsFloat;
		dsClass *clsString;
		dsClass *clsBoolean;
		dsClass *clsDictionary;
		
		dsClass *clsWindow;
		dsClass *clsGame;
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
	
	DEF_NATFUNC( nfGetCompatibleVersion );
	DEF_NATFUNC( nfGetCompatibleVersionMajor );
	DEF_NATFUNC( nfGetCompatibleVersionMinor );
	DEF_NATFUNC( nfGetCompatibleVersionPatch );
	
	DEF_NATFUNC( nfGetModuleVersion );
	DEF_NATFUNC( nfGetModuleVersionMajor );
	DEF_NATFUNC( nfGetModuleVersionMinor );
	DEF_NATFUNC( nfGetModuleVersionPatch );
	
	DEF_NATFUNC( nfGetDefaultEnableGI );
	DEF_NATFUNC( nfSetDefaultEnableGI );
#undef DEF_NATFUNC
};

#endif
