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
	deClassTouchSensorListener( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassTouchSensorListener();
	// internal functions
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	void CreateClassMembers( dsEngine *engine );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deClassCollider *GetClassCollider() const{ return pClsCol; }
	
	/** \brief Function index for colliderEntered(). */
	inline int GetFuncIndexColliderEntered() const{ return pFuncIndexColliderEntered; }
	
	/** \brief Function index for colliderLeft(). */
	inline int GetFuncIndexColliderLeft() const{ return pFuncIndexColliderLeft; }

private:
	int pFuncIndexColliderEntered;
	int pFuncIndexColliderLeft;
	
	struct sInitData{
		dsClass *clsTSL, *clsVoid, *clsBool, *clsInt, *clsCol;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfColliderEntered );
	DEF_NATFUNC( nfColliderLeft );
#undef DEF_NATFUNC
};

// end of include only once
#endif
