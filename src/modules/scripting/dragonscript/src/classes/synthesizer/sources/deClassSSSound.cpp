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

#include "deClassSSSound.h"
#include "../deClassSynthesizer.h"
#include "../deClassSynthesizerSource.h"
#include "../../sound/deClassSound.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSound.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sSSSoundNatDat{
	deSynthesizer *synthesizer;
	deSynthesizerSourceSound *source;
};



// Native Functions
/////////////////////

// public func new()
deClassSSSound::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSSSound,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSSSound::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself));
	
	// clear ( important )
	nd.synthesizer = NULL;
	nd.source = NULL;
	
	// super call
	deClassSynthesizerSource * const baseClass = (deClassSynthesizerSource*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create synthesizer source
	nd.source = new deSynthesizerSourceSound;
	baseClass->AssignSource(myself->GetRealObject(), nd.source);
}

// public func destructor()
deClassSSSound::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSSSound,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSSSound::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself));
	
	if(nd.synthesizer){
		nd.synthesizer->FreeReference();
		nd.synthesizer = NULL;
	}
	
	if(nd.source){
		nd.source->FreeReference();
		nd.source = NULL;
	}
}



// public func void targetAddLink( SSSoundTarget target, int link )
deClassSSSound::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsSSSound,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSSSoundTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassSSSound::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(! rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself));
	const deClassSSSound::eTargets target = (deClassSSSound::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassSSSound::etBlendFactor:
		nd.source->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassSSSound::etVolume:
		nd.source->GetTargetVolume().AddLink(link);
		break;
		
	case deClassSSSound::etPanning:
		nd.source->GetTargetPanning().AddLink(link);
		break;
		
	case deClassSSSound::etSpeed:
		nd.source->GetTargetSpeed().AddLink(link);
		break;
		
	case deClassSSSound::etPlay:
		nd.source->GetTargetPlay().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void targetRemoveAllLinks( SSSoundTarget target )
deClassSSSound::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsSSSound,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSSSoundTarget); // target
}
void deClassSSSound::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(! rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself));
	const deClassSSSound::eTargets target = (deClassSSSound::eTargets)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassSSSound::etBlendFactor:
		nd.source->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassSSSound::etVolume:
		nd.source->GetTargetVolume().RemoveAllLinks();
		break;
		
	case deClassSSSound::etPanning:
		nd.source->GetTargetPanning().RemoveAllLinks();
		break;
		
	case deClassSSSound::etSpeed:
		nd.source->GetTargetSpeed().RemoveAllLinks();
		break;
		
	case deClassSSSound::etPlay:
		nd.source->GetTargetPlay().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// public func void setSound( Sound sound )
deClassSSSound::nfSetSound::nfSetSound(const sInitData &init) : dsFunction(init.clsSSSound,
"setSound", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSound); // sound
}
void deClassSSSound::nfSetSound::RunFunction(dsRunTime *rt, dsValue *myself){
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassSSSound*)GetOwnerClass())->GetDS();
	deSound * const sound = ds.GetClassSound()->GetSound(rt->GetValue(0)->GetRealObject());
	
	if(sound == nd.source->GetSound()){
		return;
	}
	
	nd.source->SetSound(sound);
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setMinSpeed( float speed )
deClassSSSound::nfSetMinSpeed::nfSetMinSpeed(const sInitData &init) : dsFunction(init.clsSSSound,
"setMinSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // speed
}
void deClassSSSound::nfSetMinSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself));
	const float speed = rt->GetValue(0)->GetFloat();
	
	if(fabsf(speed - nd.source->GetMinSpeed()) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	nd.source->SetMinSpeed(speed);
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setMaxSpeed( float speed )
deClassSSSound::nfSetMaxSpeed::nfSetMaxSpeed(const sInitData &init) : dsFunction(init.clsSSSound,
"setMaxSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // speed
}
void deClassSSSound::nfSetMaxSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself));
	const float speed = rt->GetValue(0)->GetFloat();
	
	if(fabsf(speed - nd.source->GetMaxSpeed()) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	nd.source->SetMaxSpeed(speed);
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setLooping( bool looping )
deClassSSSound::nfSetLooping::nfSetLooping(const sInitData &init) : dsFunction(init.clsSSSound,
"setLooping", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // looping
}
void deClassSSSound::nfSetLooping::RunFunction(dsRunTime *rt, dsValue *myself){
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself));
	const bool looping = rt->GetValue(0)->GetBool();
	
	if(looping == nd.source->GetLooping()){
		return;
	}
	
	nd.source->SetLooping(looping);
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// Class deClassSSSound
/////////////////////////

// Constructor
////////////////

deClassSSSound::deClassSSSound(deScriptingDragonScript &ds) :
dsClass("SSSound", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("SynthesizerSource");
	
	p_SetNativeDataSize(sizeof(sSSSoundNatDat));
}

deClassSSSound::~deClassSSSound(){
}



// Management
///////////////

void deClassSSSound::CreateClassMembers(dsEngine *engine){
	pClsSSSoundTarget = engine->GetClass("Dragengine.Scenery.SSSoundTarget");
	
	sInitData init;
	init.clsSSSound = this;
	init.clsSound = pDS.GetClassSound();
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsSSSoundTarget = pClsSSSoundTarget;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	AddFunction(new nfSetSound(init));
	AddFunction(new nfSetMinSpeed(init));
	AddFunction(new nfSetMaxSpeed(init));
	AddFunction(new nfSetLooping(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerSourceSound *deClassSSSound::GetSource(dsRealObject *myself) const{
	if(! myself){
		return NULL;
	}
	
	return ((sSSSoundNatDat*)p_GetNativeData(myself->GetBuffer()))->source;
}

void deClassSSSound::AssignSynthesizer(dsRealObject *myself, deSynthesizer *synthesizer){
	if(! myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassSynthesizerSource()->AssignSynthesizer(myself, synthesizer);
	
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(myself->GetBuffer()));
	
	if(synthesizer == nd.synthesizer){
		return;
	}
	
	if(nd.synthesizer){
		nd.synthesizer->FreeReference();
	}
	
	nd.synthesizer = synthesizer;
	
	if(synthesizer){
		synthesizer->AddReference();
	}
}

void deClassSSSound::PushSource(dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSourceSound *source){
	if(! rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(! source){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassSynthesizerSource * const baseClass = (deClassSynthesizerSource*)GetBaseClass();
	rt->CreateObjectNakedOnStack(this);
	sSSSoundNatDat &nd = *((sSSSoundNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
	nd.synthesizer = NULL;
	nd.source = NULL;
	
	try{
		baseClass->CallBaseClassConstructor(rt, rt->GetValue(0), baseClass->GetFirstConstructor(), 0);
		
		nd.synthesizer = synthesizer;
		if(synthesizer){
			synthesizer->AddReference();
		}
		
		nd.source = source;
		source->AddReference();
		
		baseClass->AssignSource(rt->GetValue(0)->GetRealObject(), source);
		baseClass->AssignSynthesizer(rt->GetValue(0)->GetRealObject(), synthesizer);
		
	}catch(...){
		rt->RemoveValues(1); // remove pushed object
		throw;
	}
}
