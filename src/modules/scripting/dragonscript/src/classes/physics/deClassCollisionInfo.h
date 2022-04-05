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

#ifndef _DECLASSCOLLISIONINFO_H_
#define _DECLASSCOLLISIONINFO_H_

#include <libdscript/libdscript.h>

class deEngine;
class deClassCollider;
class deCollisionInfo;
class deScriptingDragonScript;



// collision response script class
class deClassCollisionInfo : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	deClassCollider *pClsCol;
	dsClass *pClsParticleCollisionResponse;
	
public:
	// constructor
	deClassCollisionInfo( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassCollisionInfo();
	
	// management
	void CreateClassMembers( dsEngine *engine );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	inline deClassCollider *GetClassCollider() const{ return pClsCol; }
	
	deCollisionInfo *GetInfo( dsRealObject *myself ) const;
	void PushInfo( dsRunTime *rt, deCollisionInfo *info );
	inline dsClass *GetClassParticleCollisionResponse() const{ return pClsParticleCollisionResponse; }

private:
	struct sInitData{
		dsClass *clsCI, *clsVoid, *clsBool, *clsStr, *clsInt, *clsFlt, *clsObj;
		dsClass *clsVec, *clsDVec, *clsCol, *clsPt3;
		dsClass *clsParticleCollisionResponse;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetOwnerBone );
	DEF_NATFUNC( nfGetOwnerShape );
	DEF_NATFUNC( nfGetOwnerFace );
	
	DEF_NATFUNC( nfGetCollider );
	DEF_NATFUNC( nfGetBone );
	DEF_NATFUNC( nfGetShape );
	DEF_NATFUNC( nfGetFace );
	DEF_NATFUNC( nfIsCollider );
	DEF_NATFUNC( nfHasCollision );
	DEF_NATFUNC( nfHasBone );
	DEF_NATFUNC( nfHasShape );
	DEF_NATFUNC( nfHasFace );
	
	DEF_NATFUNC( nfGetParticleLifetime );
	DEF_NATFUNC( nfGetParticleMass );
	DEF_NATFUNC( nfGetParticlePosition );
	DEF_NATFUNC( nfSetParticlePosition );
	DEF_NATFUNC( nfGetParticleVelocity );
	DEF_NATFUNC( nfSetParticleVelocity );
	DEF_NATFUNC( nfGetParticleResponse );
	DEF_NATFUNC( nfSetParticleResponse );
	
	DEF_NATFUNC( nfGetDistance );
	DEF_NATFUNC( nfGetNormal );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfGetImpulse );
	
	DEF_NATFUNC( nfGetStopTesting );
	DEF_NATFUNC( nfSetStopTesting );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

#endif
