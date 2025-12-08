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
#include "deClassSynthesizerController.h"
#include "deClassSynthesizerSource.h"
#include "../curve/deClassCurveBezier.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/synthesizer/deSynthesizer.h>
#include <dragengine/resources/synthesizer/deSynthesizerManager.h>
#include <dragengine/resources/synthesizer/deSynthesizerLink.h>
#include <dragengine/resources/synthesizer/deSynthesizerController.h>
#include <dragengine/resources/synthesizer/deSynthesizerControllerTarget.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSource.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceSynthesizer.h>
#include <dragengine/resources/synthesizer/source/deSynthesizerSourceVisitorIdentify.h>



// Native Sstructure
struct sSynNatDat{
	deSynthesizer::Ref synthesizer;
};



// Native Functions
/////////////////////

// public func new()
deClassSynthesizer::nfNew::nfNew(const sInitData &init) : dsFunction(init.clsSyn,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSynthesizer::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	sSynNatDat &nd = *((sSynNatDat*)p_GetNativeData(myself));
	const deScriptingDragonScript &ds = ((deClassSynthesizer*)GetOwnerClass())->GetDS();
	deSynthesizerManager &synmgr = *ds.GetGameEngine()->GetSynthesizerManager();
	
	// clear ( important )
	nd.synthesizer = NULL;
	
	// create synthesizer
	nd.synthesizer = synmgr.CreateSynthesizer();
}

// public func destructor()
deClassSynthesizer::nfDestructor::nfDestructor(const sInitData &init) : dsFunction(init.clsSyn,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSynthesizer::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	sSynNatDat &nd = *((sSynNatDat*)p_GetNativeData(myself));
	
	if(nd.synthesizer){
		nd.synthesizer->FreeReference();
		nd.synthesizer = NULL;
	}
}



// public func int getChannelCount()
deClassSynthesizer::nfGetChannelCount::nfGetChannelCount(const sInitData &init) : dsFunction(init.clsSyn,
"getChannelCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSynthesizer::nfGetChannelCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	rt->PushInt(synthesizer.GetChannelCount());
}

// public func void setChannelCount( int channelCount )
deClassSynthesizer::nfSetChannelCount::nfSetChannelCount(const sInitData &init) : dsFunction(init.clsSyn,
"setChannelCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // channelCount
}
void deClassSynthesizer::nfSetChannelCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	synthesizer.SetChannelCount(rt->GetValue(0)->GetInt());
}

// public func int getSampleRate()
deClassSynthesizer::nfGetSampleRate::nfGetSampleRate(const sInitData &init) : dsFunction(init.clsSyn,
"getSampleRate", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSynthesizer::nfGetSampleRate::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	rt->PushInt(synthesizer.GetSampleRate());
}

// public func void setSampleRate( int sampleRate )
deClassSynthesizer::nfSetSampleRate::nfSetSampleRate(const sInitData &init) : dsFunction(init.clsSyn,
"setSampleRate", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sampleRate
}
void deClassSynthesizer::nfSetSampleRate::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	synthesizer.SetSampleRate(rt->GetValue(0)->GetInt());
}

// public func int getBytesPerSample()
deClassSynthesizer::nfGetBytesPerSample::nfGetBytesPerSample(const sInitData &init) : dsFunction(init.clsSyn,
"getBytesPerSample", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSynthesizer::nfGetBytesPerSample::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	rt->PushInt(synthesizer.GetBytesPerSample());
}

// public func void setBytesPerSample( int bytesPerSample )
deClassSynthesizer::nfSetBytesPerSample::nfSetBytesPerSample(const sInitData &init) : dsFunction(init.clsSyn,
"setBytesPerSample", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // bytesPerSample
}
void deClassSynthesizer::nfSetBytesPerSample::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	synthesizer.SetBytesPerSample(rt->GetValue(0)->GetInt());
}

// public func int getSampleCount()
deClassSynthesizer::nfGetSampleCount::nfGetSampleCount(const sInitData &init) : dsFunction(init.clsSyn,
"getSampleCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSynthesizer::nfGetSampleCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	rt->PushInt(synthesizer.GetSampleCount());
}

// public func void setSampleCount( int sampleCount )
deClassSynthesizer::nfSetSampleCount::nfSetSampleCount(const sInitData &init) : dsFunction(init.clsSyn,
"setSampleCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // sampleCount
}
void deClassSynthesizer::nfSetSampleCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	synthesizer.SetSampleCount(rt->GetValue(0)->GetInt());
}



// public func int getControllerCount()
deClassSynthesizer::nfGetControllerCount::nfGetControllerCount(const sInitData &init) : dsFunction(init.clsSyn,
"getControllerCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSynthesizer::nfGetControllerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer::Ref synthesizer = ((sSynNatDat*)p_GetNativeData(myself))->synthesizer;
	rt->PushInt(synthesizer->GetControllerCount());
}

// public func void setControllerCount( int count )
deClassSynthesizer::nfSetControllerCount::nfSetControllerCount(const sInitData &init) : dsFunction(init.clsSyn,
"setControllerCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // count
}
void deClassSynthesizer::nfSetControllerCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer::Ref synthesizer = ((sSynNatDat*)p_GetNativeData(myself))->synthesizer;
	int count = rt->GetValue(0)->GetInt();
	
	if(count < 0) DSTHROW(dueInvalidParam);
	
	while(synthesizer->GetControllerCount() > count){
		synthesizer->RemoveController(synthesizer->GetControllerAt(synthesizer->GetControllerCount() - 1));
	}
	
	while(synthesizer->GetControllerCount() < count){
		synthesizer->AddController(deSynthesizerController::Ref::NewWith());
	}
}

// public func SynthesizerController getControllerAt( int index )
deClassSynthesizer::nfGetControllerAt::nfGetControllerAt(const sInitData &init) : dsFunction(init.clsSyn,
"getControllerAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSynController){
	p_AddParameter(init.clsInt); // controller
}
void deClassSynthesizer::nfGetControllerAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer * const synthesizer = ((sSynNatDat*)p_GetNativeData(myself))->synthesizer;
	const deScriptingDragonScript &ds = ((deClassSynthesizer*)GetOwnerClass())->GetDS();
	const int index = rt->GetValue(0)->GetInt();
	
	if(index < 0){
		ds.GetClassSynthesizerController()->PushController(rt, synthesizer,
			synthesizer->GetControllerCount() + index);
		
	}else{
		ds.GetClassSynthesizerController()->PushController(rt, synthesizer, index);
	}
}

// public func SynthesizerController getControllerNamed( String name )
deClassSynthesizer::nfGetControllerNamed::nfGetControllerNamed(const sInitData &init) :
dsFunction(init.clsSyn, "getControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSynController){
	p_AddParameter(init.clsString); // name
}
void deClassSynthesizer::nfGetControllerNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer * const synthesizer = ((sSynNatDat*)p_GetNativeData(myself))->synthesizer;
	const deScriptingDragonScript &ds = ((deClassSynthesizer*)GetOwnerClass())->GetDS();
	const int index = synthesizer->IndexOfControllerNamed(rt->GetValue(0)->GetString());
	
	if(index == -1){
		rt->PushObject(NULL, ds.GetClassSynthesizerController());
		
	}else{
		ds.GetClassSynthesizerController()->PushController(rt, synthesizer, index);
	}
}

// public func int indexOfControllerNamed( String name )
deClassSynthesizer::nfIndexOfControllerNamed::nfIndexOfControllerNamed(const sInitData &init) :
dsFunction(init.clsSyn, "indexOfControllerNamed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsString); // name
}
void deClassSynthesizer::nfIndexOfControllerNamed::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer * const synthesizer = ((sSynNatDat*)p_GetNativeData(myself))->synthesizer;
	rt->PushInt(synthesizer->IndexOfControllerNamed(rt->GetValue(0)->GetString()));
}



// public func int getLinkCount()
deClassSynthesizer::nfGetLinkCount::nfGetLinkCount(const sInitData &init) : dsFunction(init.clsSyn,
"getLinkCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSynthesizer::nfGetLinkCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	rt->PushInt(synthesizer.GetLinkCount());
}

// public func int addLink( int controller )
deClassSynthesizer::nfAddLink::nfAddLink(const sInitData &init) : dsFunction(init.clsSyn,
"addLink", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
	p_AddParameter(init.clsInt); // controller
}
void deClassSynthesizer::nfAddLink::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	
	deSynthesizerLink::Ref link = NULL;
	
	try{
		link.TakeOver(new deSynthesizerLink);
		link->SetController(rt->GetValue(0)->GetInt());
		synthesizer.AddLink(link);
	}catch(...){
		throw;
	}
	
	rt->PushInt(synthesizer.GetLinkCount() - 1);
}

// public func void removeAllLinks()
deClassSynthesizer::nfRemoveAllLinks::nfRemoveAllLinks(const sInitData &init) : dsFunction(init.clsSyn,
"removeAllLinks", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSynthesizer::nfRemoveAllLinks::RunFunction(dsRunTime*, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	synthesizer.RemoveAllLinks();
}

// public func void linkSetController( int link, int controller );
deClassSynthesizer::nfLinkSetController::nfLinkSetController(const sInitData &init) : dsFunction(init.clsSyn,
"linkSetController", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // link
	p_AddParameter(init.clsInt); // controller
}
void deClassSynthesizer::nfLinkSetController::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	
	const int index = rt->GetValue(0)->GetInt();
	deSynthesizerLink &link = *synthesizer.GetLinkAt(index);
	
	link.SetController(rt->GetValue(1)->GetInt());
	
	synthesizer.NotifyLinksChanged();
}

// public func void linkSetRepeat( int link, int repeat );
deClassSynthesizer::nfLinkSetRepeat::nfLinkSetRepeat(const sInitData &init) : dsFunction(init.clsSyn,
"linkSetRepeat", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // link
	p_AddParameter(init.clsInt); // repeat
}
void deClassSynthesizer::nfLinkSetRepeat::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	
	const int index = rt->GetValue(0)->GetInt();
	deSynthesizerLink &link = *synthesizer.GetLinkAt(index);
	
	link.SetRepeat(rt->GetValue(1)->GetInt());
	
	synthesizer.NotifyLinksChanged();
}

// public func void linkSetCurve( int link, CurveBezier curve );
deClassSynthesizer::nfLinkSetCurve::nfLinkSetCurve(const sInitData &init) : dsFunction(init.clsSyn,
"linkSetCurve", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInt); // link
	p_AddParameter(init.clsCurveBezier); // curve
}
void deClassSynthesizer::nfLinkSetCurve::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	const deScriptingDragonScript &ds = ((deClassSynthesizer*)GetOwnerClass())->GetDS();
	
	const int index = rt->GetValue(0)->GetInt();
	deSynthesizerLink &link = *synthesizer.GetLinkAt(index);
	const decCurveBezier &curve = ds.GetClassCurveBezier()->GetCurve(rt->GetValue(1)->GetRealObject());
	
	link.GetCurve() = curve;
	
	synthesizer.NotifyLinksChanged();
}



// public func int getSourceCount()
deClassSynthesizer::nfGetSourceCount::nfGetSourceCount(const sInitData &init) : dsFunction(init.clsSyn,
"getSourceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}
void deClassSynthesizer::nfGetSourceCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	rt->PushInt(synthesizer.GetSourceCount());
}

// public func void addSource( SynthesizerSource source )
deClassSynthesizer::nfAddSource::nfAddSource(const sInitData &init) : dsFunction(init.clsSyn,
"addSource", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsSynS); // source
}
void deClassSynthesizer::nfAddSource::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	const deScriptingDragonScript &ds = ((deClassSynthesizer*)GetOwnerClass())->GetDS();
	deClassSynthesizerSource &clsSynS = *ds.GetClassSynthesizerSource();
	
	dsRealObject * const objSource = rt->GetValue(0)->GetRealObject();
	deSynthesizerSource *source = clsSynS.GetSource(objSource);
	if(!source){
		DSTHROW(dueNullPointer);
	}
	
	synthesizer.AddSource(source);
	
	clsSynS.AssignSynthesizer(objSource, &synthesizer);
}

// public func SynthesizerSource getSourceAt( int position )
deClassSynthesizer::nfGetSourceAt::nfGetSourceAt(const sInitData &init) : dsFunction(init.clsSyn,
"getSourceAt", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsSynS){
	p_AddParameter(init.clsInt); // position
}
void deClassSynthesizer::nfGetSourceAt::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	const deScriptingDragonScript &ds = ((deClassSynthesizer*)GetOwnerClass())->GetDS();
	deClassSynthesizerSource &clsSynS = *ds.GetClassSynthesizerSource();
	
	const int position = rt->GetValue(0)->GetInt();
	
	clsSynS.PushSource(rt, &synthesizer, synthesizer.GetSourceAt(position));
}

// public func void removeAllSources()
deClassSynthesizer::nfRemoveAllSources::nfRemoveAllSources(const sInitData &init) : dsFunction(init.clsSyn,
"removeAllSources", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassSynthesizer::nfRemoveAllSources::RunFunction(dsRunTime*, dsValue *myself){
	deSynthesizer &synthesizer = *(((sSynNatDat*)p_GetNativeData(myself))->synthesizer);
	synthesizer.RemoveAllSources();
}



// public func int hashCode()
deClassSynthesizer::nfHashCode::nfHashCode(const sInitData &init) : dsFunction(init.clsSyn,
"hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt){
}

void deClassSynthesizer::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer * const synthesizer = ((sSynNatDat*)p_GetNativeData(myself))->synthesizer;
	// hash code = memory location
	rt->PushInt((int)(intptr_t)synthesizer);
}

// public func bool equals( Object obj )
deClassSynthesizer::nfEquals::nfEquals(const sInitData &init) : dsFunction(init.clsSyn,
"equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassSynthesizer::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	deSynthesizer * const synthesizer = ((sSynNatDat*)p_GetNativeData(myself))->synthesizer;
	deClassSynthesizer * const clsSyn = (deClassSynthesizer*)GetOwnerClass();
	dsValue * const obj = rt->GetValue(0);
	
	if(!p_IsObjOfType(obj, clsSyn)){
		rt->PushBool(false);
		
	}else{
		deSynthesizer * const otherSynthesizer = ((sSynNatDat*)p_GetNativeData(obj))->synthesizer;
		rt->PushBool(synthesizer == otherSynthesizer);
	}
}

// public static func bool equals( Synthesizer synthesizer1, Synthesizer synthesizer2 )
deClassSynthesizer::nfEquals2::nfEquals2(const sInitData &init) :
dsFunction(init.clsSyn, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool){
	p_AddParameter(init.clsSyn); // synthesizer1
	p_AddParameter(init.clsSyn); // synthesizer2
}
void deClassSynthesizer::nfEquals2::RunFunction(dsRunTime *rt, dsValue*){
	deClassSynthesizer &clsSyn = *((deClassSynthesizer*)GetOwnerClass());
	deSynthesizer * const synthesizer1 = clsSyn.GetSynthesizer(rt->GetValue(0)->GetRealObject());
	deSynthesizer * const synthesizer2 = clsSyn.GetSynthesizer(rt->GetValue(1)->GetRealObject());
	
	rt->PushBool(synthesizer1 == synthesizer2);
}



// class deClassSynthesizer
/////////////////////////////

// Constructor
////////////////

deClassSynthesizer::deClassSynthesizer(deScriptingDragonScript &ds) :
dsClass("Synthesizer", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	// store information into parser info
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	// do the rest
	p_SetNativeDataSize(sizeof(sSynNatDat));
}

deClassSynthesizer::~deClassSynthesizer(){
}



// Management
///////////////

void deClassSynthesizer::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsSyn = this;
	init.clsSynS = pDS.GetClassSynthesizerSource();
	init.clsSynController = pDS.GetClassSynthesizerController();
	init.clsCurveBezier = pDS.GetClassCurveBezier();
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetChannelCount(init));
	AddFunction(new nfSetChannelCount(init));
	AddFunction(new nfGetSampleRate(init));
	AddFunction(new nfSetSampleRate(init));
	AddFunction(new nfGetBytesPerSample(init));
	AddFunction(new nfSetBytesPerSample(init));
	AddFunction(new nfGetSampleCount(init));
	AddFunction(new nfSetSampleCount(init));
	
	AddFunction(new nfGetControllerCount(init));
	AddFunction(new nfSetControllerCount(init));
	AddFunction(new nfGetControllerAt(init));
	AddFunction(new nfGetControllerNamed(init));
	AddFunction(new nfIndexOfControllerNamed(init));
	
	AddFunction(new nfGetLinkCount(init));
	AddFunction(new nfAddLink(init));
	AddFunction(new nfRemoveAllLinks(init));
	AddFunction(new nfLinkSetController(init));
	AddFunction(new nfLinkSetRepeat(init));
	AddFunction(new nfLinkSetCurve(init));
	
	AddFunction(new nfGetSourceCount(init));
	AddFunction(new nfAddSource(init));
	AddFunction(new nfGetSourceAt(init));
	AddFunction(new nfRemoveAllSources(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfEquals2(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSynthesizer *deClassSynthesizer::GetSynthesizer(dsRealObject *myself) const{
	if(!myself){
		return NULL;
	}
	
	return ((sSynNatDat*)p_GetNativeData(myself->GetBuffer()))->synthesizer;
}

void deClassSynthesizer::PushSynthesizer(dsRunTime *rt, deSynthesizer *synthesizer){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!synthesizer){
		rt->PushObject(NULL, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	((sSynNatDat*)p_GetNativeData(rt->GetValue(0)->GetRealObject()->GetBuffer()))->synthesizer = synthesizer;
}
