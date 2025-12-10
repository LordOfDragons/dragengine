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

#include <new>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <libdscript/libdscript.h>

#include "deClassSoundLevelMeterSpeaker.h"
#include "deClassSpeaker.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/sound/deSpeaker.h>



struct sSLMSNatDat{
	deSpeaker::Ref speaker;
};



// Constructors, Destructors
//////////////////////////////

// private func new()
deClassSoundLevelMeterSpeaker::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsSoundLevelMeterSpeaker, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid){
}
void deClassSoundLevelMeterSpeaker::nfNew::RunFunction(dsRunTime*, dsValue*){
	DSTHROW(dueInvalidAction);
}

// public func destructor()
deClassSoundLevelMeterSpeaker::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsSoundLevelMeterSpeaker, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSoundLevelMeterSpeaker::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSLMSNatDat*>(p_GetNativeData(myself))->~sSLMSNatDat();
}



// Management
///////////////

// public func Speaker getSpeaker()
deClassSoundLevelMeterSpeaker::nfGetSpeaker::nfGetSpeaker(const sInitData &init) :
dsFunction(init.clsSoundLevelMeterSpeaker, "getSpeaker", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSpeaker){
}
void deClassSoundLevelMeterSpeaker::nfGetSpeaker::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSLMSNatDat &nd = *static_cast<sSLMSNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = (static_cast<deClassSoundLevelMeterSpeaker*>(GetOwnerClass()))->GetDS();
	
	ds.GetClassSpeaker()->PushSpeaker(rt, nd.speaker);
}

// public func float getVolume()
deClassSoundLevelMeterSpeaker::nfGetVolume::nfGetVolume(const sInitData &init) :
dsFunction(init.clsSoundLevelMeterSpeaker, "getVolume", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassSoundLevelMeterSpeaker::nfGetVolume::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSLMSNatDat &nd = *static_cast<sSLMSNatDat*>(p_GetNativeData(myself));
	
	rt->PushFloat(nd.volume);
}



// Class deClassSoundLevelMeterSpeaker
////////////////////////////////////////

// Constructor
////////////////

deClassSoundLevelMeterSpeaker::deClassSoundLevelMeterSpeaker(deScriptingDragonScript &ds) :
dsClass("SoundLevelMeterSpeaker", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSLMSNatDat));
}

deClassSoundLevelMeterSpeaker::~deClassSoundLevelMeterSpeaker(){
}



// Management
///////////////

void deClassSoundLevelMeterSpeaker::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsSoundLevelMeterSpeaker = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	init.clsSpeaker = pDS.GetClassSpeaker();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetSpeaker(init));
	AddFunction(new nfGetVolume(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

void deClassSoundLevelMeterSpeaker::PushSoundLevelMeterSpeaker(dsRunTime* rt,
const deSoundLevelMeter::cAudibleSpeaker &speaker){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	rt->CreateObjectNakedOnStack(this);
	
	sSLMSNatDat * const nd = new (rt->GetValue(0)->GetRealObject()->GetBuffer()) sSLMSNatDat;
	
	nd->speaker = speaker.GetSpeaker();
	if(nd->speaker){
		nd->speaker->AddReference();
	}
	
	nd->volume = speaker.GetVolume();
}
