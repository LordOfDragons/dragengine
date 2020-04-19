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
#ifndef _DECLASSCOLLIDERLISTENER_H_
#define _DECLASSCOLLIDERLISTENER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deScriptingDragonScript;

// component listener class
class deClassColliderListener : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
public:
	// constructor
	deClassColliderListener( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassColliderListener();
	// internal functions
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	void CreateClassMembers( dsEngine *engine );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	
	/** \brief Function index for collisionResponse(). */
	inline int GetFuncIndexCollisionResponse() const{ return pFuncIndexCollisionResponse; }
	
	/** \brief Function index for canHitCollider(). */
	inline int GetFuncIndexCanHitCollider() const{ return pFuncIndexCanHitCollider; }
	
	/** \brief Function index for colliderChanged(). */
	inline int GetFuncIndexColliderChanged() const{ return pFuncIndexColliderChanged; }
	
private:
	int pFuncIndexCollisionResponse;
	int pFuncIndexCanHitCollider;
	int pFuncIndexColliderChanged;
	
	struct sInitData{
		dsClass *clsCLL, *clsVoid, *clsBool, *clsFlt, *clsInt, *clsCI, *clsCol, *clsComp;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfCollisionResponse );
	DEF_NATFUNC( nfCanHitCollider );
	DEF_NATFUNC( nfColliderChanged );
#undef DEF_NATFUNC
};

// end of include only once
#endif
