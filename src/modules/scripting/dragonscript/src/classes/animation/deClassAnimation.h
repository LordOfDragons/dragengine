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
#ifndef _DECLASSANIMATION_H_
#define _DECLASSANIMATION_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deAnimation;
class deScriptingDragonScript;
class deClassResourceListener;

// animation script class
class deClassAnimation : public dsClass{
private:
	deEngine *p_gameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassResourceListener *pClsRN;
	
public:
	// constructor
	deClassAnimation(deEngine *GameEngine, deScriptingDragonScript *ScrMgr);
	~deClassAnimation();
	// internal functions
	void CreateClassMembers(dsEngine *engine);
	inline deEngine *GetGameEngine() const{ return p_gameEngine; }
	
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassResourceListener *GetClassResourceListener() const{ return pClsRN; }
	
	deAnimation *GetAnimation(dsRealObject *This) const;
	void PushAnimation( dsRunTime *rt, deAnimation *anim );
	
private:
	struct sInitData{
		dsClass *clsAnim, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj, *clsRN;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC(nfLoad);
	DEF_NATFUNC( nfLoadAsynchron );
	DEF_NATFUNC(nfSave);
	DEF_NATFUNC(nfDestructor);
	DEF_NATFUNC(nfGetFilename);
	DEF_NATFUNC(nfGetMovePlaytime);
	DEF_NATFUNC( nfGetMoveFPS );
	DEF_NATFUNC( nfGetMoveCount );
	DEF_NATFUNC( nfGetMoveName );
	DEF_NATFUNC(nfEquals);
	DEF_NATFUNC(nfHashCode);
#undef DEF_NATFUNC
};

// end of include only once
#endif
