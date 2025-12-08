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

#include "deClassSynthesizer.h"
#include "deClassSynthesizerSource.h"
#include "sources/deClassSSSound.h"
#include "sources/deClassSSWave.h"
#include "sources/deClassSSSynthesizer.h"
#include "sources/deClassSSGroup.h"
#include "deClassSynthesizerEffect.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSound.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceWave.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSynthesizer.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceGroup.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



struct sSynSNatDat{
	deSynthesizer *synthesizer;
	deSynthesizerSource *source;
};



// Native Functions
/////////////////////

// protected func new()
deClassSynthesizerSource::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSynS,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PROTECTED | DSTM_NATIVE, init.clsVoid){
}
void deClassSynthesizerSource::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	nd.synthesizer = NULL;
	nd.source = NULL;
}

// public func destructor()
deClassSynthesizerSource::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSynS,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSynthesizerSource::nfDestructor::RunFunction(dsRunTime *rt, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	
	if(nd.synthesizer){
		nd.synthesizer->FreeReference();
		nd.synthesizer = NULL;
	}
	
	if(nd.source){
		nd.source->FreeReference();
		nd.source = NULL;
	}
}



// public func void setEnabled( bool enabled )
deClassSynthesizerSource::nfSetEnabled::nfSetEnabled(const sInitData &init) : dsFunction(init.clsSynS,
"setEnabled", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enabled
}
void deClassSynthesizerSource::nfSetEnabled::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	const bool enabled = rt->GetValue(0)->GetBool();
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	if(nd.source->GetEnabled() == enabled){
		return;
	}
	
	nd.source->SetEnabled(enabled);
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setMixMode( SynthesizerSourceMix mixMode )
deClassSynthesizerSource::nfSetMixMode::nfSetMixMode(const sInitData &init) : dsFunction(init.clsSynS,
"setMixMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSynthesizerSourceMix); // mixMode
}
void deClassSynthesizerSource::nfSetMixMode::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	
	if(!nd.source || !rt->GetValue(0)->GetRealObject()){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->SetMixMode((deSynthesizerSource::eMixModes)
		((dsClassEnumeration*)rt->GetEngine()->GetClassEnumeration())->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setBlendFactor( float factor )
deClassSynthesizerSource::nfSetBlendFactor::nfSetBlendFactor(const sInitData &init) : dsFunction(init.clsSynS,
"setBlendFactor", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // factor
}
void deClassSynthesizerSource::nfSetBlendFactor::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->SetBlendFactor(rt->GetValue(0)->GetFloat());
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}




// public func void setMinVolume( float volume )
deClassSynthesizerSource::nfSetMinVolume::nfSetMinVolume(const sInitData &init) : dsFunction(init.clsSynS,
"setMinVolume", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // volume
}
void deClassSynthesizerSource::nfSetMinVolume::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->SetMinVolume(rt->GetValue(0)->GetFloat());
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setMaxVolume( float volume )
deClassSynthesizerSource::nfSetMaxVolume::nfSetMaxVolume(const sInitData &init) : dsFunction(init.clsSynS,
"setMaxVolume", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // volume
}
void deClassSynthesizerSource::nfSetMaxVolume::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->SetMaxVolume(rt->GetValue(0)->GetFloat());
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setMinPanning( float panning )
deClassSynthesizerSource::nfSetMinPanning::nfSetMinPanning(const sInitData &init) : dsFunction(init.clsSynS,
"setMinPanning", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // panning
}
void deClassSynthesizerSource::nfSetMinPanning::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->SetMinPanning(rt->GetValue(0)->GetFloat());
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void setMaxPanning( float panning )
deClassSynthesizerSource::nfSetMaxPanning::nfSetMaxPanning(const sInitData &init) : dsFunction(init.clsSynS,
"setMaxPanning", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // panning
}
void deClassSynthesizerSource::nfSetMaxPanning::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->SetMaxPanning(rt->GetValue(0)->GetFloat());
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// public func void addEffect( SynthesizerEffect effect )
deClassSynthesizerSource::nfAddEffect::nfAddEffect(const sInitData &init) : dsFunction(init.clsSynS,
"addEffect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSynEff); // effect
}
void deClassSynthesizerSource::nfAddEffect::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassSynthesizerSource*)GetOwnerClass())->GetDS();
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	dsRealObject * const objEffect = rt->GetValue(0)->GetRealObject();
	deSynthesizerEffect * const effect = ds.GetClassSynthesizerEffect()->GetEffect(objEffect);
	if(!effect){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->AddEffect(effect);
	
	ds.GetClassSynthesizerEffect()->AssignSource(objEffect, nd.source);
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void removeEffect( SynthesizerEffect effect )
deClassSynthesizerSource::nfRemoveEffect::nfRemoveEffect(const sInitData &init) : dsFunction(init.clsSynS,
"removeEffect", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSynEff); // effect
}
void deClassSynthesizerSource::nfRemoveEffect::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassSynthesizerSource*)GetOwnerClass())->GetDS();
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	dsRealObject * const objEffect = rt->GetValue(0)->GetRealObject();
	deSynthesizerEffect * const effect = ds.GetClassSynthesizerEffect()->GetEffect(objEffect);
	if(!effect){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->RemoveEffect(effect);
	
	// TODO: reverse AssignSource but how?
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}

// public func void removeAllEffects()
deClassSynthesizerSource::nfRemoveAllEffects::nfRemoveAllEffects(const sInitData &init) : dsFunction(init.clsSynS,
"removeAllEffects", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSynthesizerSource::nfRemoveAllEffects::RunFunction(dsRunTime *rt, dsValue *myself){
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself));
	
	if(!nd.source){
		DSTHROW(dueNullPointer);
	}
	
	nd.source->RemoveAllEffects();
	
	// TODO: reverse AssignSource but how?
	
	if(nd.synthesizer){
		nd.synthesizer->NotifySourcesChanged();
	}
}



// Class deClassSynthesizerSource
///////////////////////////////////

// Constructor
////////////////

deClassSynthesizerSource::deClassSynthesizerSource(deScriptingDragonScript &ds) :
dsClass("SynthesizerSource", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds){
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(sizeof(sSynSNatDat));
}

deClassSynthesizerSource::~deClassSynthesizerSource(){
}



// Management
///////////////

void deClassSynthesizerSource::CreateClassMembers(dsEngine *engine){
	pClsSynthesizerSourceMix = engine->GetClass("Dragengine.Scenery.SynthesizerSourceMix");
	
	sInitData init;
	init.clsSynS = this;
	init.clsSynEff = pDS.GetClassSynthesizerEffect();
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsSynthesizerSourceMix = pClsSynthesizerSourceMix;
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfSetEnabled(init));
	AddFunction(new nfSetMixMode(init));
	AddFunction(new nfSetBlendFactor(init));
	
	AddFunction(new nfSetMinVolume(init));
	AddFunction(new nfSetMaxVolume(init));
	AddFunction(new nfSetMinPanning(init));
	AddFunction(new nfSetMaxPanning(init));
	
	AddFunction(new nfAddEffect(init));
	AddFunction(new nfRemoveEffect(init));
	AddFunction(new nfRemoveAllEffects(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizerSource *deClassSynthesizerSource::GetSource(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sSynSNatDat*)p_GetNativeData(myself->GetBuffer()))->source;
}

void deClassSynthesizerSource::AssignSource(dsRealObject *myself, deSynthesizerSource *source){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself->GetBuffer()));
	
	if(source == nd.source){
		return;
	}
	
	if(nd.source){
		nd.source->FreeReference();
	}
	
	nd.source = source;
}

void deClassSynthesizerSource::AssignSynthesizer(dsRealObject *myself, deSynthesizer *synthesizer){
	if(!myself){
		DSTHROW(dueInvalidParam);
	}
	
	sSynSNatDat &nd = *((sSynSNatDat*)p_GetNativeData(myself->GetBuffer()));
	
	if(synthesizer == nd.synthesizer){
		return;
	}
	
	if(nd.synthesizer){
		nd.synthesizer->FreeReference();
	}
	
	nd.synthesizer = synthesizer;
}

void deClassSynthesizerSource::PushSource(dsRunTime *rt, deSynthesizer *synthesizer, deSynthesizerSource *source){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!source){
		rt->PushObject(NULL, this);
		return;
	}
	
	deSynthesizerSourceVisitorIdentify visitor;
	source->Visit(visitor);
	
	switch(visitor.GetType()){
	case deSynthesizerSourceVisitorIdentify::estSound:
		pDS.GetClassSSSound()->PushSource(rt, synthesizer, &visitor.CastToSound());
		break;
		
	case deSynthesizerSourceVisitorIdentify::estWave:
		pDS.GetClassSSWave()->PushSource(rt, synthesizer, &visitor.CastToWave());
		break;
		
	case deSynthesizerSourceVisitorIdentify::estSynthesizer:
		pDS.GetClassSSSynthesizer()->PushSource(rt, synthesizer, &visitor.CastToSynthesizer());
		break;
		
	case deSynthesizerSourceVisitorIdentify::estGroup:
		pDS.GetClassSSGroup()->PushSource(rt, synthesizer, &visitor.CastToGroup());
		break;
		
	default:
		DSTHROW(dueInvalidParam);
	}
}
