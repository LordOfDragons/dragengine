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
	DEF_NATFUNC( nfGetSpeakerGain );
	DEF_NATFUNC( nfSetSpeakerGain);
	DEF_NATFUNC( nfGetParentWorld );
	
	DEF_NATFUNC( nfAddSpeaker );
	DEF_NATFUNC( nfRemoveSpeaker );
	DEF_NATFUNC( nfRemoveAllSpeakers );
	
	DEF_NATFUNC( nfEquals );
	DEF_NATFUNC( nfHashCode );
#undef DEF_NATFUNC
};

// end of include only once
#endif
