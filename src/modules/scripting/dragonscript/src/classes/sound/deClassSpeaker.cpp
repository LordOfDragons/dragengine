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

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "deClassSpeaker.h"
#include "deClassSound.h"
#include "deClassMicrophone.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../physics/deClassLayerMask.h"
#include "../synthesizer/deClassSynthesizerInstance.h"
#include "../video/deClassVideoPlayer.h"
#include "../utils/deClassShapeList.h"
#include "../world/deClassWorld.h"
#include "../../peers/dedsSpeaker.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/deResource.h>
#include <dragengine/resources/deResourceManager.h>
#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSpeaker.h>
#include <dragengine/resources/sound/deSpeakerManager.h>

#include <libdscript/libdscript.h>
#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// native structure
struct sSpkNatDat{
	deSpeaker *speaker;
};



// Native functions
/////////////////////

// public func new()
deClassSpeaker::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSpk,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSpeaker::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSpkNatDat *nd = (sSpkNatDat*)p_GetNativeData(myself);
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	deSpeakerManager *spkMgr = clsSpk->GetGameEngine()->GetSpeakerManager();
	
	// clear (important)
	nd->speaker = NULL;
	
	// create speaker
	nd->speaker = spkMgr->CreateSpeaker();
	if(!nd->speaker) DSTHROW(dueOutOfMemory);
}

// public func destructor()
deClassSpeaker::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSpk,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSpeaker::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSpkNatDat *nd = (sSpkNatDat*)p_GetNativeData(myself);
	
	if(nd->speaker){
		nd->speaker->FreeReference();
		nd->speaker = NULL;
	}
}



// public func SpeakerType getType()
deClassSpeaker::nfGetType::nfGetType(const sInitData &init) : dsFunction(init.clsSpk,
"getType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSpeakerType){
}
void deClassSpeaker::nfGetType::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushValue(((deClassSpeaker*)GetOwnerClass())->GetClassSpeakerType()
		->GetVariable(speaker->GetType())->GetStaticValue());
}

// public func void setType( SpeakerType type )
deClassSpeaker::nfSetType::nfSetType(const sInitData &init) : dsFunction(init.clsSpk,
"setType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSpeakerType); // type
}
void deClassSpeaker::nfSetType::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	speaker->SetType((deSpeaker::eSpeakerType)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Sound getSound()
deClassSpeaker::nfGetSound::nfGetSound(const sInitData &init) : dsFunction(init.clsSpk,
"getSound", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSnd){
}
void deClassSpeaker::nfGetSound::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	deClassSound *clsSnd = clsSpk->GetScriptModule()->GetClassSound();
	
	if(speaker->GetSound()){
		clsSnd->PushSound(rt, speaker->GetSound());
		
	}else{
		rt->PushObject(NULL, clsSnd);
	}
}

// public func void setSound( Sound sound )
deClassSpeaker::nfSetSound::nfSetSound(const sInitData &init) : dsFunction(init.clsSpk,
"setSound", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSnd); // sound
}
void deClassSpeaker::nfSetSound::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	deClassSound *clsSnd = clsSpk->GetScriptModule()->GetClassSound();
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	
	if(object){
		speaker->SetSound(clsSnd->GetSound(object));
		
	}else{
		speaker->SetSound(NULL);
	}
}

// public func SynthesizerInstance getSynthesizer()
deClassSpeaker::nfGetSynthesizer::nfGetSynthesizer(const sInitData &init) : dsFunction(init.clsSpk,
"getSynthesizer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSynthInst){
}
void deClassSpeaker::nfGetSynthesizer::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	const deScriptingDragonScript &ds = *(((deClassSpeaker*)GetOwnerClass())->GetScriptModule());
	
	ds.GetClassSynthesizerInstance()->PushSynthesizerInstance(rt, speaker.GetSynthesizer());
}

// public func void setSynthesizer( SynthesizerInstance synthesizer )
deClassSpeaker::nfSetSynthesizer::nfSetSynthesizer(const sInitData &init) : dsFunction(init.clsSpk,
"setSynthesizer", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSynthInst); // synthesizer
}
void deClassSpeaker::nfSetSynthesizer::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	const deScriptingDragonScript &ds = *(((deClassSpeaker*)GetOwnerClass())->GetScriptModule());
	
	deSynthesizerInstance * const synthInstance = ds.GetClassSynthesizerInstance()->
		GetSynthesizerInstance(rt->GetValue(0)->GetRealObject());
		
	speaker.SetSynthesizer(synthInstance);
}

// public func VideoPlayer getVideoPlayer()
deClassSpeaker::nfGetVideoPlayer::nfGetVideoPlayer(const sInitData &init) :
dsFunction(init.clsSpk, "getVideoPlayer", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVideoPlayer){
}
void deClassSpeaker::nfGetVideoPlayer::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	const deScriptingDragonScript &ds = *(((deClassSpeaker*)GetOwnerClass())->GetScriptModule());
	
	ds.GetClassVideoPlayer()->PushVideoPlayer(rt, speaker.GetVideoPlayer());
}

// public func void setVideoPlayer( VideoPlayer videoPlayer )
deClassSpeaker::nfSetVideoPlayer::nfSetVideoPlayer(const sInitData &init) :
dsFunction(init.clsSpk, "setVideoPlayer", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVideoPlayer); // videoPlayer
}
void deClassSpeaker::nfSetVideoPlayer::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	const deScriptingDragonScript &ds = *(((deClassSpeaker*)GetOwnerClass())->GetScriptModule());
	
	speaker.SetVideoPlayer(ds.GetClassVideoPlayer()->GetVideoPlayer(rt->GetValue(0)->GetRealObject()));
}

// public func DVector getPosition()
deClassSpeaker::nfGetPosition::nfGetPosition(const sInitData &init) : dsFunction(init.clsSpk,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec){
}
void deClassSpeaker::nfGetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	
	clsSpk->GetClassDVector()->PushDVector(rt, speaker->GetPosition());
}

// public func void setPosition( DVector position )
deClassSpeaker::nfSetPosition::nfSetPosition(const sInitData &init) : dsFunction(init.clsSpk,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsDVec); // position
}
void deClassSpeaker::nfSetPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	
	dsRealObject *obj = rt->GetValue(0)->GetRealObject();
	if(!obj) DSTHROW(dueNullPointer);
	
	speaker->SetPosition(clsSpk->GetClassDVector()->GetDVector(obj));
}

// public func Quaternion getOrientation()
deClassSpeaker::nfGetOrientation::nfGetOrientation(const sInitData &init) : dsFunction(init.clsSpk,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat){
}
void deClassSpeaker::nfGetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	deClassQuaternion *clsQuat = clsSpk->GetScriptModule()->GetClassQuaternion();
	
	clsQuat->PushQuaternion(rt, speaker->GetOrientation());
}

// public func void setOrientation( Quaternion orientation )
deClassSpeaker::nfSetOrientation::nfSetOrientation(const sInitData &init) : dsFunction(init.clsSpk,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsQuat); // orientation
}
void deClassSpeaker::nfSetOrientation::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	deClassQuaternion *clsQuat = clsSpk->GetScriptModule()->GetClassQuaternion();
	dsRealObject *obj = rt->GetValue(0)->GetRealObject();
	
	if(!obj) DSTHROW(dueNullPointer);
	
	speaker->SetOrientation(clsQuat->GetQuaternion(obj));
}

// public func Vector getVelocity()
deClassSpeaker::nfGetVelocity::nfGetVelocity(const sInitData &init) : dsFunction(init.clsSpk,
"getVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec){
}
void deClassSpeaker::nfGetVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	deClassVector *clsVec = clsSpk->GetScriptModule()->GetClassVector();
	
	clsVec->PushVector(rt, speaker->GetVelocity());
}

// public func void setVelocity( Vector velocity )
deClassSpeaker::nfSetVelocity::nfSetVelocity(const sInitData &init) : dsFunction(init.clsSpk,
"setVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsVec); // velocity
}
void deClassSpeaker::nfSetVelocity::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	deClassVector *clsVec = clsSpk->GetScriptModule()->GetClassVector();
	dsRealObject *obj = rt->GetValue(0)->GetRealObject();
	
	if(!obj) DSTHROW(dueNullPointer);
	
	speaker->SetVelocity(clsVec->GetVector(obj));
}

// func World getParentWorld()
deClassSpeaker::nfGetParentWorld::nfGetParentWorld(const sInitData &init) :
dsFunction(init.clsSpk, "getParentWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld){
}
void deClassSpeaker::nfGetParentWorld::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	deClassSpeaker &clsSpk = *((deClassSpeaker*)GetOwnerClass());
	deClassWorld &clsWorld = *(clsSpk.GetScriptModule()->GetClassWorld());
	
	clsWorld.PushWorld(rt, speaker.GetParentWorld());
}

// func Microphone getParentMicrophone()
deClassSpeaker::nfGetParentMicrophone::nfGetParentMicrophone(const sInitData &init) :
dsFunction(init.clsSpk, "getParentMicrophone", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsMicrophone){
}
void deClassSpeaker::nfGetParentMicrophone::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	deClassSpeaker &clsSpk = *((deClassSpeaker*)GetOwnerClass());
	deClassMicrophone &clsMic = *(clsSpk.GetScriptModule()->GetClassMicrophone());
	
	clsMic.PushMicrophone(rt, speaker.GetParentMicrophone());
}

// public func bool getMuted()
deClassSpeaker::nfGetMuted::nfGetMuted(const sInitData &init) : dsFunction(init.clsSpk,
"getMuted", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSpeaker::nfGetMuted::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushBool(speaker->GetMuted());
}

// public func void setMuted( bool muted )
deClassSpeaker::nfSetMuted::nfSetMuted(const sInitData &init) : dsFunction(init.clsSpk,
"setMuted", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // muted
}
void deClassSpeaker::nfSetMuted::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	speaker->SetMuted(rt->GetValue(0)->GetBool());
}

// public func bool getLooping()
deClassSpeaker::nfGetLooping::nfGetLooping(const sInitData &init) : dsFunction(init.clsSpk,
"getLooping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSpeaker::nfGetLooping::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushBool(speaker->GetLooping());
}

// public func void setLooping( bool looping )
deClassSpeaker::nfSetLooping::nfSetLooping(const sInitData &init) : dsFunction(init.clsSpk,
"setLooping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // looping
}
void deClassSpeaker::nfSetLooping::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	speaker->SetLooping(rt->GetValue(0)->GetBool());
}

// public func int getPlayFrom()
deClassSpeaker::nfGetPlayFrom::nfGetPlayFrom(const sInitData &init) : dsFunction(init.clsSpk,
"getPlayFrom", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSpeaker::nfGetPlayFrom::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushInt(speaker->GetPlayFrom());
}

// public func int getPlayTo()
deClassSpeaker::nfGetPlayTo::nfGetPlayTo(const sInitData &init) : dsFunction(init.clsSpk,
"getPlayTo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSpeaker::nfGetPlayTo::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushInt(speaker->GetPlayTo());
}

// public func void setPlayPosition( int playFrom, int playTo )
deClassSpeaker::nfSetPlayPosition::nfSetPlayPosition(const sInitData &init) : dsFunction(init.clsSpk,
"setPlayPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // playFrom
	p_AddParameter(init.clsInt); // playTo
}
void deClassSpeaker::nfSetPlayPosition::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	const int playFrom = rt->GetValue(0)->GetInt();
	const int playTo = rt->GetValue(1)->GetInt();
	speaker->SetPlayPosition(playFrom, playTo);
}

// public func float getPlaySpeed()
deClassSpeaker::nfGetPlaySpeed::nfGetPlaySpeed(const sInitData &init) : dsFunction(init.clsSpk,
"getPlaySpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSpeaker::nfGetPlaySpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushFloat(speaker->GetPlaySpeed());
}

// public func void setPlaySpeed( float playSpeed )
deClassSpeaker::nfSetPlaySpeed::nfSetPlaySpeed(const sInitData &init) : dsFunction(init.clsSpk,
"setPlaySpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // playSpeed
}
void deClassSpeaker::nfSetPlaySpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	speaker->SetPlaySpeed(rt->GetValue(0)->GetFloat());
}

// public func float getVolume()
deClassSpeaker::nfGetVolume::nfGetVolume(const sInitData &init) : dsFunction(init.clsSpk,
"getVolume", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSpeaker::nfGetVolume::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushFloat(speaker->GetVolume());
}

// public func void setVolume( float volume )
deClassSpeaker::nfSetVolume::nfSetVolume(const sInitData &init) : dsFunction(init.clsSpk,
"setVolume", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // volume
}
void deClassSpeaker::nfSetVolume::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	speaker->SetVolume(rt->GetValue(0)->GetFloat());
}

// public func float getRange()
deClassSpeaker::nfGetRange::nfGetRange(const sInitData &init) : dsFunction(init.clsSpk,
"getRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSpeaker::nfGetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	rt->PushFloat(speaker.GetRange());
}

// public func void setRange( float volume )
deClassSpeaker::nfSetRange::nfSetRange(const sInitData &init) : dsFunction(init.clsSpk,
"setRange", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // volume
}
void deClassSpeaker::nfSetRange::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	speaker.SetRange(rt->GetValue(0)->GetFloat());
}

// public func float getRollOff()
deClassSpeaker::nfGetRollOff::nfGetRollOff(const sInitData &init) : dsFunction(init.clsSpk,
"getRollOff", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSpeaker::nfGetRollOff::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	rt->PushFloat(speaker.GetRollOff());
}

// public func void setRollOff( float volume )
deClassSpeaker::nfSetRollOff::nfSetRollOff(const sInitData &init) : dsFunction(init.clsSpk,
"setRollOff", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // volume
}
void deClassSpeaker::nfSetRollOff::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	speaker.SetRollOff(rt->GetValue(0)->GetFloat());
}

// public func float getDistanceOffset()
deClassSpeaker::nfGetDistanceOffset::nfGetDistanceOffset(const sInitData &init) :
dsFunction(init.clsSpk, "getDistanceOffset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt){
}
void deClassSpeaker::nfGetDistanceOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	rt->PushFloat(speaker.GetDistanceOffset());
}

// public func void setDistanceOffset( float distanceOffset )
deClassSpeaker::nfSetDistanceOffset::nfSetDistanceOffset(const sInitData &init) :
dsFunction(init.clsSpk, "setDistanceOffset", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFlt); // distanceOffset
}
void deClassSpeaker::nfSetDistanceOffset::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	speaker.SetDistanceOffset(rt->GetValue(0)->GetFloat());
}

// public func ShapeList getShape()
deClassSpeaker::nfGetShape::nfGetShape(const sInitData &init) :
dsFunction(init.clsSpk, "getShape", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsShapeList){
}
void deClassSpeaker::nfGetShape::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	const deScriptingDragonScript &ds = *((deClassSpeaker*)GetOwnerClass())->GetScriptModule();
	ds.GetClassShapeList()->PushShapeList(rt, speaker.GetShape());
}

// public func void setShape( ShapeList shape )
deClassSpeaker::nfSetShape::nfSetShape(const sInitData &init) :
dsFunction(init.clsSpk, "setShape", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsShapeList); // shape
}
void deClassSpeaker::nfSetShape::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	const deScriptingDragonScript &ds = *((deClassSpeaker*)GetOwnerClass())->GetScriptModule();
	speaker.SetShape(ds.GetClassShapeList()->GetShapeList(rt->GetValue(0)->GetRealObject()));
}

// public func LayerMask getLayerMask()
deClassSpeaker::nfGetLayerMask::nfGetLayerMask(const sInitData &init) : dsFunction(init.clsSpk,
"getLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask){
}
void deClassSpeaker::nfGetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	const deScriptingDragonScript &ds = *((deClassSpeaker*)GetOwnerClass())->GetScriptModule();
	
	ds.GetClassLayerMask()->PushLayerMask(rt, speaker.GetLayerMask());
}

// public func void setLayerMask( LayerMask layerMask )
deClassSpeaker::nfSetLayerMask::nfSetLayerMask(const sInitData &init) : dsFunction(init.clsSpk,
"setLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsLayerMask); // layerMask
}
void deClassSpeaker::nfSetLayerMask::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker &speaker = *(((sSpkNatDat*)p_GetNativeData(myself))->speaker);
	const deScriptingDragonScript &ds = *((deClassSpeaker*)GetOwnerClass())->GetScriptModule();
	
	speaker.SetLayerMask(ds.GetClassLayerMask()->GetLayerMask(rt->GetValue(0)->GetRealObject()));
}



// public func Object getOwner()
deClassSpeaker::nfGetOwner::nfGetOwner(const sInitData &init) : dsFunction(init.clsSpk,
"getOwner", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsObj){
}
void deClassSpeaker::nfGetOwner::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSpkNatDat &nd = *((sSpkNatDat*)p_GetNativeData(myself));
	if(!nd.speaker){
		DSTHROW(dueNullPointer);
	}
	
	dedsSpeaker * const scrCol = (dedsSpeaker*)nd.speaker->GetPeerScripting();
	
	if(scrCol){
		rt->PushObject(scrCol->GetOwner());
		
	}else{
		rt->PushObject(NULL);
	}
}

// public func void setOwner( Object owner )
deClassSpeaker::nfSetOwner::nfSetOwner(const sInitData &init) : dsFunction(init.clsSpk,
"setOwner", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsObj); // owner
}
void deClassSpeaker::nfSetOwner::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSpkNatDat &nd = *((sSpkNatDat*)p_GetNativeData(myself));
	if(!nd.speaker){
		DSTHROW(dueNullPointer);
	}
	
	dedsSpeaker * const scrCol = (dedsSpeaker*)nd.speaker->GetPeerScripting();
	if(scrCol){
		scrCol->SetOwner(rt->GetValue(0)->GetRealObject());
	}
}



// public func bool getPlaying()
deClassSpeaker::nfGetPlaying::nfGetPlaying(const sInitData &init) : dsFunction(init.clsSpk,
"getPlaying", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSpeaker::nfGetPlaying::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushBool(speaker->GetPlaying());
}

// public func bool getPaused()
deClassSpeaker::nfGetPaused::nfGetPaused(const sInitData &init) : dsFunction(init.clsSpk,
"getPaused", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSpeaker::nfGetPaused::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushBool(speaker->GetPaused());
}

// public func bool getStopped()
deClassSpeaker::nfGetStopped::nfGetStopped(const sInitData &init) : dsFunction(init.clsSpk,
"getStopped", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassSpeaker::nfGetStopped::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushBool(speaker->GetStopped());
}

// public func void play()
deClassSpeaker::nfPlay::nfPlay(const sInitData &init) : dsFunction(init.clsSpk,
"play", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSpeaker::nfPlay::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	speaker->Play();
}

// public func void stop()
deClassSpeaker::nfStop::nfStop(const sInitData &init) : dsFunction(init.clsSpk,
"stop", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSpeaker::nfStop::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	speaker->Stop();
}

// public func void pause()
deClassSpeaker::nfPause::nfPause(const sInitData &init) : dsFunction(init.clsSpk,
"pause", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSpeaker::nfPause::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	speaker->Pause();
}



// public func int hashCode()
deClassSpeaker::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsSpk, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassSpeaker::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	
	rt->PushInt((int)(intptr_t)speaker);
}

// public func bool equals( Object object )
deClassSpeaker::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSpk, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObj); // object
}
void deClassSpeaker::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deSpeaker *speaker = ((sSpkNatDat*)p_GetNativeData(myself))->speaker;
	deClassSpeaker *clsSpk = (deClassSpeaker*)GetOwnerClass();
	dsValue *obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSpk)){
		rt->PushBool(false);
	}else{
		deSpeaker *otherSpeaker = ((sSpkNatDat*)p_GetNativeData(obj))->speaker;
		rt->PushBool(speaker == otherSpeaker);
	}
}



// Class deClassSpeaker
/////////////////////////

// Constructor
////////////////

deClassSpeaker::deClassSpeaker(deEngine *gameEngine, deScriptingDragonScript *scrMgr) :
dsClass("Speaker", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE){
	if(!gameEngine || !scrMgr) DSTHROW(dueInvalidParam);
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// store information into parser info
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	// do the rest
	p_SetNativeDataSize(sizeof(sSpkNatDat));
}

deClassSpeaker::~deClassSpeaker(){
}



// Management
///////////////

void deClassSpeaker::CreateClassMembers(dsEngine *engine){
	pClsDVec = pScrMgr->GetClassDVector();
	pClsSnd = pScrMgr->GetClassSound();
	pClsVec = pScrMgr->GetClassVector();
	pClsSpeakerType = engine->GetClass("Dragengine.Scenery.SpeakerType");
	
	sInitData init;
	init.clsSpk = this;
	init.clsSnd = pClsSnd;
	init.clsSynthInst = pScrMgr->GetClassSynthesizerInstance();
	init.clsVec = pClsVec;
	init.clsDVec = pClsDVec;
	init.clsQuat = pScrMgr->GetClassQuaternion();
	init.clsLayerMask = pScrMgr->GetClassLayerMask();
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsShapeList = pScrMgr->GetClassShapeList();
	init.clsSpeakerType = pClsSpeakerType;
	init.clsVideoPlayer = pScrMgr->GetClassVideoPlayer();
	init.clsWorld = pScrMgr->GetClassWorld();
	init.clsMicrophone = pScrMgr->GetClassMicrophone();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetType(init));
	AddFunction(new nfSetType(init));
	AddFunction(new nfGetSound(init));
	AddFunction(new nfSetSound(init));
	AddFunction(new nfGetSynthesizer(init));
	AddFunction(new nfSetSynthesizer(init));
	AddFunction(new nfGetVideoPlayer(init));
	AddFunction(new nfSetVideoPlayer(init));
	AddFunction(new nfGetPosition(init));
	AddFunction(new nfSetPosition(init));
	AddFunction(new nfGetOrientation(init));
	AddFunction(new nfSetOrientation(init));
	AddFunction(new nfGetVelocity(init));
	AddFunction(new nfSetVelocity(init));
	AddFunction(new nfGetParentWorld(init));
	AddFunction(new nfGetParentMicrophone(init));
	
	AddFunction(new nfGetMuted(init));
	AddFunction(new nfSetMuted(init));
	AddFunction(new nfGetLooping(init));
	AddFunction(new nfSetLooping(init));
	AddFunction(new nfGetPlayFrom(init));
	AddFunction(new nfGetPlayTo(init));
	AddFunction(new nfSetPlayPosition(init));
	AddFunction(new nfGetPlaySpeed(init));
	AddFunction(new nfSetPlaySpeed(init));
	AddFunction(new nfGetVolume(init));
	AddFunction(new nfSetVolume(init));
	AddFunction(new nfGetRange(init));
	AddFunction(new nfSetRange(init));
	AddFunction(new nfGetRollOff(init));
	AddFunction(new nfSetRollOff(init));
	AddFunction(new nfGetDistanceOffset(init));
	AddFunction(new nfSetDistanceOffset(init));
	AddFunction(new nfGetShape(init));
	AddFunction(new nfSetShape(init));
	AddFunction(new nfGetLayerMask(init));
	AddFunction(new nfSetLayerMask(init));
	
	AddFunction(new nfGetOwner(init));
	AddFunction(new nfSetOwner(init));
	
	AddFunction(new nfGetPlaying(init));
	AddFunction(new nfGetPaused(init));
	AddFunction(new nfGetStopped(init));
	AddFunction(new nfPlay(init));
	AddFunction(new nfPause(init));
	AddFunction(new nfStop(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSpeaker *deClassSpeaker::GetSpeaker(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sSpkNatDat*)p_GetNativeData(myself->GetBuffer()))->speaker;
}

void deClassSpeaker::PushSpeaker(dsRunTime *rt, deSpeaker *speaker){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!speaker){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sSpkNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->speaker = speaker;
	speaker->AddReference();
}
