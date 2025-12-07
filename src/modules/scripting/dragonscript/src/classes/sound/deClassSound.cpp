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

#include "deClassSound.h"
#include "../resources/deClassResourceListener.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../resourceloader/dedsResourceLoader.h"

#include <dragengine/resources/sound/deSound.h>
#include <dragengine/resources/sound/deSoundManager.h>
#include <dragengine/deEngine.h>
#include <dragengine/resources/loader/deResourceLoader.h>

#include <libdscript/exceptions.h>



// Native Structure
/////////////////////

struct sSndNatDat{
	deSound *sound;
};



// Constructors, Destructors
//////////////////////////////

// public func new( String filename )
deClassSound::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSound, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsString); // filename
}
void deClassSound::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSndNatDat &nd = *((sSndNatDat*)p_GetNativeData(myself));
	
	// prepare
	nd.sound = NULL;
	
	// load sound
	deScriptingDragonScript &ds = ((deClassSound*)GetOwnerClass())->GetDS();
	deSoundManager &sndMgr = *ds.GetGameEngine()->GetSoundManager();
	
	const char * const filename = rt->GetValue(0)->GetString();
	
	nd.sound = sndMgr.LoadSound(filename, "/", false);
}

// static public func void loadAsynchron( String filename, ResourceListener listener )
deClassSound::nfLoadAsynchron::nfLoadAsynchron(const sInitData &init) :
dsFunction(init.clsSound, "loadAsynchron", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsString); // filename
	p_AddParameter(init.clsResNot); // listener
}
void deClassSound::nfLoadAsynchron::RunFunction(dsRunTime *rt, dsValue *myself){
	deScriptingDragonScript &ds = ((deClassSound*)GetOwnerClass())->GetDS();
	
	const char * const filename = rt->GetValue(0)->GetString();
	dsRealObject * const listener = rt->GetValue(1)->GetRealObject();
	
	if(!listener){
		DSTHROW(dueInvalidParam);
	}
	
	ds.GetResourceLoader()->AddRequest(filename, deResourceLoader::ertSound, listener);
}

// public func destructor()
deClassSound::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSound, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSound::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSndNatDat &nd = *((sSndNatDat*)p_GetNativeData(myself));
	
	if(nd.sound){
		nd.sound->FreeReference();
		nd.sound = NULL;
	}
}



// Management
///////////////

// public func String getFilename()
deClassSound::nfGetFilename::nfGetFilename(const sInitData &init) :
dsFunction(init.clsSound, "getFilename", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString){
}
void deClassSound::nfGetFilename::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSound &sound = *(((sSndNatDat*)p_GetNativeData(myself))->sound);
	
	rt->PushString(sound.GetFilename());
}

// public func int getBytesPerSample()
deClassSound::nfGetBytesPerSample::nfGetBytesPerSample(const sInitData &init) :
dsFunction(init.clsSound, "getBytesPerSample", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassSound::nfGetBytesPerSample::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSound &sound = *(((sSndNatDat*)p_GetNativeData(myself))->sound);
	
	rt->PushInt(sound.GetBytesPerSample());
}

// public func int getSampleCount()
deClassSound::nfGetSampleCount::nfGetSampleCount(const sInitData &init) :
dsFunction(init.clsSound, "getSampleCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassSound::nfGetSampleCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSound &sound = *(((sSndNatDat*)p_GetNativeData(myself))->sound);
	
	rt->PushInt(sound.GetSampleCount());
}

// public func int getChannelCount()
deClassSound::nfGetChannelCount::nfGetChannelCount(const sInitData &init) :
dsFunction(init.clsSound, "getChannelCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassSound::nfGetChannelCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSound &sound = *(((sSndNatDat*)p_GetNativeData(myself))->sound);
	
	rt->PushInt(sound.GetChannelCount());
}

// public func int getSampleRate()
deClassSound::nfGetSampleRate::nfGetSampleRate(const sInitData &init) :
dsFunction(init.clsSound, "getSampleRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassSound::nfGetSampleRate::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSound &sound = *(((sSndNatDat*)p_GetNativeData(myself))->sound);
	
	rt->PushInt(sound.GetSampleRate());
}

// public func float getPlayTime()
deClassSound::nfGetPlayTime::nfGetPlayTime(const sInitData &init) :
dsFunction(init.clsSound, "getPlayTime", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSound::nfGetPlayTime::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSound &sound = *(((sSndNatDat*)p_GetNativeData(myself))->sound);
	
	rt->PushFloat(sound.GetPlayTime());
}



// Common
///////////

// public func int hashCode()
deClassSound::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsSound, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}

void deClassSound::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSound * const sound = ((sSndNatDat*)p_GetNativeData(myself))->sound;
	
	rt->PushInt((int)(intptr_t)sound);
}

// public func bool equals( Object object )
deClassSound::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsSound, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // object
}
void deClassSound::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSound * const sound = ((sSndNatDat*)p_GetNativeData(myself))->sound;
	deClassSound * const clsSound = (deClassSound*)GetOwnerClass();
	dsValue * const object = rt->GetValue(0);
	
	if(!p_IsObjOfType(object, clsSound)){
		rt->PushBool(false);
		
	}else{
		deSound * const otherSound = ((sSndNatDat*)p_GetNativeData(object))->sound;
		
		rt->PushBool(sound == otherSound);
	}
}



// Class deClassSound
///////////////////////

// Constructor, Destructor
////////////////////////////

deClassSound::deClassSound(deScriptingDragonScript &ds) :
dsClass("Sound", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSndNatDat));
}

deClassSound::~deClassSound(){
}



// Management
///////////////

void deClassSound::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	init.clsSound = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsObject = engine->GetClassObject();
	
	init.clsResNot = pDS.GetClassResourceListener();
	
	AddFunction(new nfNew (init));
	AddFunction(new nfLoadAsynchron(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetFilename(init));
	AddFunction(new nfGetBytesPerSample(init));
	AddFunction(new nfGetSampleRate(init));
	AddFunction(new nfGetChannelCount(init));
	AddFunction(new nfGetSampleCount(init));
	AddFunction(new nfGetPlayTime(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
}

deSound *deClassSound::GetSound(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sSndNatDat*)p_GetNativeData(myself->GetBuffer()))->sound;
}

void deClassSound::PushSound(dsRunTime *rt, deSound *sound){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!sound){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sSndNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->sound = sound;
	sound->AddReference();
}
