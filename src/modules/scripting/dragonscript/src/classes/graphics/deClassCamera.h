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
#ifndef _DECLASSCAMERA_H_
#define _DECLASSCAMERA_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deCamera;
class deClassVector;
class deClassDVector;
class deClassEffect;
class deScriptingDragonScript;



// camera script class
class deClassCamera : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassEffect *pClsEff;
	deClassVector *pClsVec;
	deClassDVector *pClsDVec;
	
public:
	// constructor
	deClassCamera( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassCamera();
	
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	deCamera *GetCamera( dsRealObject *myself ) const;
	void PushCamera( dsRunTime *rt, deCamera *camera );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassEffect *GetClassEffect() const{ return pClsEff; }
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }
	
private:
	struct sInitData{
		dsClass *clsCam;
		
		dsClass *clsVoid;
		dsClass *clsInt;
		dsClass *clsFlt;
		dsClass *clsBool;
		dsClass *clsObj;
		
		dsClass *clsDVec;
		dsClass *clsVec;
		dsClass *clsQuat;
		dsClass *clsEff;
		dsClass *clsPoint;
		dsClass *clsLayerMask;
		dsClass *clsWorld;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetFov );
	DEF_NATFUNC( nfSetFov );
	DEF_NATFUNC( nfGetFovRatio );
	DEF_NATFUNC( nfSetFovRatio );
	DEF_NATFUNC( nfGetImageDistance );
	DEF_NATFUNC( nfSetImageDistance );
	DEF_NATFUNC( nfGetViewDistance );
	DEF_NATFUNC( nfSetViewDistance );
	DEF_NATFUNC( nfGetExposure );
	DEF_NATFUNC( nfSetExposure );
	DEF_NATFUNC( nfGetLowestIntensity );
	DEF_NATFUNC( nfSetLowestIntensity );
	DEF_NATFUNC( nfGetHighestIntensity );
	DEF_NATFUNC( nfSetHighestIntensity );
	DEF_NATFUNC( nfGetAdaptionTime );
	DEF_NATFUNC( nfSetAdaptionTime );
	DEF_NATFUNC( nfResetAdaptedIntensity );
	
	DEF_NATFUNC( nfProject );
	DEF_NATFUNC( nfBackProject );
	DEF_NATFUNC( nfBackProject2 );
	
	DEF_NATFUNC( nfGetLayerMask );
	DEF_NATFUNC( nfSetLayerMask );
	DEF_NATFUNC( nfGetParentWorld );
	
	DEF_NATFUNC( nfGetEffectCount );
	DEF_NATFUNC( nfGetEffectAt );
	DEF_NATFUNC( nfAddEffect );
	DEF_NATFUNC( nfRemoveEffect );
	DEF_NATFUNC( nfRemoveAllEffects );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
