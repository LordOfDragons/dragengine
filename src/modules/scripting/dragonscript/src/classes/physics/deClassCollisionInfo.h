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
