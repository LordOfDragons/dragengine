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
#ifndef _DECLASSMICROPHONE_H_
#define _DECLASSMICROPHONE_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deMicrophone;
class deClassSpeaker;
class deClassVector;
class deClassDVector;
class deScriptingDragonScript;



// camera script class
class deClassMicrophone : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassVector *pClsVec;
	deClassDVector *pClsDVec;
	deClassSpeaker *pClsSpk;
	dsClass *pClsMicrophoneType;
	
public:
	// constructor
	deClassMicrophone( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassMicrophone();
	
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	deMicrophone *GetMicrophone( dsRealObject *myself ) const;
	void PushMicrophone( dsRunTime *rt, deMicrophone *camera );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }
	inline deClassSpeaker *GetClassSpeaker() const{ return pClsSpk; }
	inline dsClass *GetClassMicrophoneType() const{ return pClsMicrophoneType; }
	
private:
	struct sInitData{
		dsClass *clsMic, *clsVoid, *clsInt, *clsFlt, *clsBool;
		dsClass *clsDVec, *clsVec, *clsQuat, *clsObj, *clsSpk;
		dsClass *clsLayerMask;
		dsClass *clsMicrophoneType;
	};
#define DEF_NATFUNC(name) \
	class name : public dsFunction{ \
	public: \
		name(const sInitData &init); \
		void RunFunction(dsRunTime *RT, dsValue *This); \
	}
	DEF_NATFUNC( nfNew );
	DEF_NATFUNC( nfDestructor );
	
	DEF_NATFUNC( nfGetType );
	DEF_NATFUNC( nfSetType );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetVelocity );
	DEF_NATFUNC( nfSetVelocity );
	
	DEF_NATFUNC( nfGetMuted );
	DEF_NATFUNC( nfSetMuted );
	DEF_NATFUNC( nfGetVolume );
	DEF_NATFUNC( nfSetVolume );
	
	DEF_NATFUNC( nfGetLayerMask );
	DEF_NATFUNC( nfSetLayerMask );
	
	DEF_NATFUNC( nfAddSpeaker );
	DEF_NATFUNC( nfRemoveSpeaker );
	DEF_NATFUNC( nfRemoveAllSpeakers );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
