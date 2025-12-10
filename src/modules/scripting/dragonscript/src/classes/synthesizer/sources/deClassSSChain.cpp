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

#include "deClassSSChain.h"
#include "../deClassSynthesizer.h"
#include "../deClassSynthesizerSource.h"
#include "../../sound/deClassSound.h"
#include "../../../deScriptingDragonScript.h"
#include "../../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceChain.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native structure
/////////////////////

struct sSSSoundNatDat{
	deSynthesizer::Ref synthesizer;
	deSynthesizerSourceChain::Ref source;
};



// Native Functions
/////////////////////

// public func new()
deClassSSChain::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSSSound,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSSChain::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSSSoundNatDat * const nd = new (p_GetNativeData(myself)) sSSSoundNatDat;
	
	// super call
	deClassSynthesizerSource * const baseClass = (deClassSynthesizerSource*)GetOwnerClass()->GetBaseClass();
	baseClass->CallBaseClassConstructor(rt, myself, baseClass->GetFirstConstructor(), 0);
	
	// create synthesizer source
	nd->source = new deSynthesizerSourceChain;
	baseClass->AssignSource(myself->GetRealObject(), nd->source);
}

// public func destructor()
deClassSSChain::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSSSound,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSSChain::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	static_cast<sSSSoundNatDat*>(p_GetNativeData(myself))->~sSSSoundNatDat();
}



// public func void targetAddLink( SSChainTarget target, int link )
deClassSSChain::nfTargetAddLink::nfTargetAddLink(const sInitData &init) : dsFunction(init.clsSSSound,
"targetAddLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSSChainTarget); // target
	p_AddParameter(init.clsInt); // link
}
void deClassSSChain::nfTargetAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(myself));
	const deClassSSChain::eTargets target = (deClassSSChain::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	const int link = rt->GetValue(1)->GetInt();
	
	switch(target){
	case deClassSSChain::etBlendFactor:
		nd.source->GetTargetBlendFactor().AddLink(link);
		break;
		
	case deClassSSChain::etVolume:
		nd.source->GetTargetVolume().AddLink(link);
		break;
		
	case deClassSSChain::etPanning:
		nd.source->GetTargetPanning().AddLink(link);
		break;
		
	case deClassSSChain::etSpeed:
		nd.source->GetTargetSpeed().AddLink(link);
		break;
		
	case deClassSSChain::etSelection:
		nd.source->GetTargetSelect().AddLink(link);
		break;
		
	case deClassSSChain::etPlay:
		nd.source->GetTargetPlay().AddLink(link);
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void targetRemoveAllLinks( SSChainTarget target )
deClassSSChain::nfTargetRemoveAllLinks::nfTargetRemoveAllLinks(const sInitData &init) : dsFunction(init.clsSSSound,
"targetRemoveAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSSChainTarget); // target
}
void deClassSSChain::nfTargetRemoveAllLinks::RunFunction(dsRunTime *rt, dsValue *myself){
	if(!rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(myself));
	const deClassSSChain::eTargets target = (deClassSSChain::eTargets)
		static_cast<dsClassEnumeration*>(rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	switch(target){
	case deClassSSChain::etBlendFactor:
		nd.source->GetTargetBlendFactor().RemoveAllLinks();
		break;
		
	case deClassSSChain::etVolume:
		nd.source->GetTargetVolume().RemoveAllLinks();
		break;
		
	case deClassSSChain::etPanning:
		nd.source->GetTargetPanning().RemoveAllLinks();
		break;
		
	case deClassSSChain::etSpeed:
		nd.source->GetTargetSpeed().RemoveAllLinks();
		break;
		
	case deClassSSChain::etSelection:
		nd.source->GetTargetSelect().RemoveAllLinks();
		break;
		
	case deClassSSChain::etPlay:
		nd.source->GetTargetPlay().RemoveAllLinks();
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// public func int getSoundCount()
deClassSSChain::nfGetSoundCount::nfGetSoundCount(const sInitData &init) : dsFunction(init.clsSSSound,
"getSoundCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSSChain::nfGetSoundCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(myself));
	rt->PushInt(nd.source->GetSoundCount());
}

// public func void addSound( Sound sound )
deClassSSChain::nfAddSound::nfAddSound(const sInitData &init) : dsFunction(init.clsSSSound,
"addSound", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSound); // sound
}
void deClassSSChain::nfAddSound::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassSSChain*)GetOwnerClass())->GetDS();
	deSound * const sound = ds.GetClassSound()->GetSound(rt->GetValue(0)->GetRealObject());
	
	nd.source->AddSound(sound);
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void removeAllSounds()
deClassSSChain::nfRemoveAllSounds::nfRemoveAllSounds(const sInitData &init) : dsFunction(init.clsSSSound,
"removeAllSounds", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSSChain::nfRemoveAllSounds::RunFunction(dsRunTime *rt, dsValue *myself){
	const sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(myself));
	
	if(nd.source->GetSoundCount() == 0){
		return;
	}
	
	nd.source->RemoveAllSounds();
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// public func void setMinSpeed( float speed )
deClassSSChain::nfSetMinSpeed::nfSetMinSpeed(const sInitData &init) : dsFunction(init.clsSSSound,
"setMinSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // speed
}
void deClassSSChain::nfSetMinSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(myself));
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
deClassSSChain::nfSetMaxSpeed::nfSetMaxSpeed(const sInitData &init) : dsFunction(init.clsSSSound,
"setMaxSpeed", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // speed
}
void deClassSSChain::nfSetMaxSpeed::RunFunction(dsRunTime *rt, dsValue *myself){
	sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(myself));
	const float speed = rt->GetValue(0)->GetFloat();
	
	if(fabsf(speed - nd.source->GetMaxSpeed()) <= FLOAT_SAFE_EPSILON){
		return;
	}
	
	nd.source->SetMaxSpeed(speed);
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// Class deClassSSChain
/////////////////////////

// Constructor
////////////////

deClassSSChain::deClassSSChain(deScriptingDragonScript &ds) :
dsClass("SSChain", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("SynthesizerSource");
	
	p_SetNativeDataSize(sizeof(sSSSoundNatDat));
}

deClassSSChain::~deClassSSChain(){
}



// Management
///////////////

void deClassSSChain::CreateClassMembers(dsEngine *engine){
	pClsSSChainTarget = engine->GetClass("Dragengine.Scenery.SSChainTarget");
	
	sInitData init;
	init.clsSSSound = this;
	init.clsSound = pDS.GetClassSound();
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsSSChainTarget = pClsSSChainTarget;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfTargetAddLink(init));
	AddFunction(new nfTargetRemoveAllLinks(init));
	
	AddFunction(new nfGetSoundCount(init));
	AddFunction(new nfAddSound(init));
	AddFunction(new nfRemoveAllSounds(init));
	
	AddFunction(new nfSetMinSpeed(init));
	AddFunction(new nfSetMaxSpeed(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerSourceChain *deClassSSChain::GetSource(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return static_cast<sSSSoundNatDat*>(p_GetNativeData(myself->GetBuffer()))->source;
}

void deClassSSChain::AssignSynthesizer(dsRealObject *myself, deSynthesizer *synthesizer){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	pDS.GetClassSynthesizerSource()->AssignSynthesizer(myself, synthesizer);
	
	sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(myself->GetBuffer()));
	
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

void deClassSSChain::PushSource(dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSourceChain *source){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!source){
		rt->PushObject(NULL, this);
		return;
	}
	
	deClassSynthesizerSource * const baseClass = static_cast<deClassSynthesizerSource*>(GetBaseClass());
	rt->CreateObjectNakedOnStack(this);
	sSSSoundNatDat &nd = *static_cast<sSSSoundNatDat*>(p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()));
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
