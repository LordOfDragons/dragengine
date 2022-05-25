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
#ifndef _DECLASSSPEAKER_H_
#define _DECLASSSPEAKER_H_

// includes
#include <libdscript/libdscript.h>

// predefinitions
class deEngine;
class deSpeaker;
class deClassSound;
class deClassVector;
class deClassDVector;
class deScriptingDragonScript;



// camera script class
class deClassSpeaker : public dsClass{
private:
	deEngine *pGameEngine;
	deScriptingDragonScript *pScrMgr;
	
	deClassVector *pClsVec;
	deClassDVector *pClsDVec;
	deClassSound *pClsSnd;
	dsClass *pClsSpeakerType;
	
public:
	// constructor
	deClassSpeaker( deEngine *gameEngine, deScriptingDragonScript *scrMgr );
	~deClassSpeaker();
	
	// internal functions
	void CreateClassMembers( dsEngine *engine );
	deSpeaker *GetSpeaker( dsRealObject *myself ) const;
	void PushSpeaker( dsRunTime *rt, deSpeaker *speaker );
	inline deEngine *GetGameEngine() const{ return pGameEngine; }
	inline deScriptingDragonScript *GetScriptModule() const{ return pScrMgr; }
	
	inline deClassVector *GetClassVector() const{ return pClsVec; }
	inline deClassDVector *GetClassDVector() const{ return pClsDVec; }
	inline deClassSound *GetClassSound() const{ return pClsSnd; }
	inline dsClass *GetClassSpeakerType() const{ return pClsSpeakerType; }
	
private:
	struct sInitData{
		dsClass *clsSpk;
		
		dsClass *clsDVec;
		dsClass *clsSnd;
		dsClass *clsSynthInst;
		dsClass *clsVec;
		dsClass *clsQuat;
		dsClass *clsLayerMask;
		
		dsClass *clsVoid;
		dsClass *clsInt;
		dsClass *clsFlt;
		dsClass *clsBool;
		dsClass *clsObj;
		dsClass *clsShapeList;
		dsClass *clsSpeakerType;
		dsClass *clsVideoPlayer;
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
	DEF_NATFUNC( nfGetSound );
	DEF_NATFUNC( nfSetSound );
	DEF_NATFUNC( nfGetSynthesizer );
	DEF_NATFUNC( nfSetSynthesizer );
	DEF_NATFUNC( nfGetVideoPlayer );
	DEF_NATFUNC( nfSetVideoPlayer );
	DEF_NATFUNC( nfGetPosition );
	DEF_NATFUNC( nfSetPosition );
	DEF_NATFUNC( nfGetOrientation );
	DEF_NATFUNC( nfSetOrientation );
	DEF_NATFUNC( nfGetVelocity );
	DEF_NATFUNC( nfSetVelocity );
	
	DEF_NATFUNC( nfGetMuted );
	DEF_NATFUNC( nfSetMuted );
	DEF_NATFUNC( nfGetLooping );
	DEF_NATFUNC( nfSetLooping );
	DEF_NATFUNC( nfGetPlayFrom );
	DEF_NATFUNC( nfGetPlayTo );
	DEF_NATFUNC( nfSetPlayPosition );
	DEF_NATFUNC( nfGetPlaySpeed );
	DEF_NATFUNC( nfSetPlaySpeed );
	DEF_NATFUNC( nfGetVolume );
	DEF_NATFUNC( nfSetVolume );
	DEF_NATFUNC( nfGetRange );
	DEF_NATFUNC( nfSetRange );
	DEF_NATFUNC( nfGetRollOff );
	DEF_NATFUNC( nfSetRollOff );
	DEF_NATFUNC( nfGetDistanceOffset );
	DEF_NATFUNC( nfSetDistanceOffset );
	DEF_NATFUNC( nfGetShape );
	DEF_NATFUNC( nfSetShape );
	DEF_NATFUNC( nfGetLayerMask );
	DEF_NATFUNC( nfSetLayerMask );
	
	DEF_NATFUNC( nfGetOwner );
	DEF_NATFUNC( nfSetOwner );
	
	DEF_NATFUNC( nfGetPlaying );
	DEF_NATFUNC( nfGetPaused );
	DEF_NATFUNC( nfGetStopped );
	DEF_NATFUNC( nfPlay );
	DEF_NATFUNC( nfPause );
	DEF_NATFUNC( nfStop );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
