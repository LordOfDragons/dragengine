/*
 * MIT License
 *
 * Copyright (C) 2026, DragonDreams GmbH (info@dragondreams.ch)
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

#include "deClassAudioAnalyzer.h"
#include "../dedsHelpers.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/sound/deAudioAnalyzer.h>
#include <dragengine/resources/sound/deAudioAnalyzerManager.h>

#include <libdscript/libdscript.h>



struct sAANatDat{
	deAudioAnalyzer::Ref audioAnalyze;
};



// Constructors, Destructors
//////////////////////////////

// func new()
deClassAudioAnalyzer::nfNew::nfNew(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAudioAnalyzer::nfNew::RunFunction(dsRunTime*, dsValue *myself){
	sAANatDat &nd = dedsNewNativeData<sAANatDat>(p_GetNativeData(myself));
	
	nd.audioAnalyze = static_cast<deClassAudioAnalyzer*>(GetOwnerClass())->GetDS().
		GetGameEngine()->GetAudioAnalyzerManager()->CreateAudioAnalyzer();
}

// func destructor()
deClassAudioAnalyzer::nfDestructor::nfDestructor(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAudioAnalyzer::nfDestructor::RunFunction(dsRunTime*, dsValue *myself){
	if(myself->GetRealObject()->GetRefCount() != 1){
		return; // protected against GC cleaning up leaking
	}
	
	dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).~sAANatDat();
}



// Management
///////////////

// func bool getUseAudioCapture()
deClassAudioAnalyzer::nfGetUseAudioCapture::nfGetUseAudioCapture(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getUseAudioCapture", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAudioAnalyzer::nfGetUseAudioCapture::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushBool(analyzer.GetUseAudioCapture());
}

// func void setUseAudioCapture(bool useAudioCapture)
deClassAudioAnalyzer::nfSetUseAudioCapture::nfSetUseAudioCapture(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setUseAudioCapture", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // useAudioCapture
}
void deClassAudioAnalyzer::nfSetUseAudioCapture::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetUseAudioCapture(rt->GetValue(0)->GetBool());
}

// func int getFrequencyBandCount()
deClassAudioAnalyzer::nfGetFrequencyBandCount::nfGetFrequencyBandCount(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getFrequencyBandCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassAudioAnalyzer::nfGetFrequencyBandCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushInt(analyzer.GetFrequencyBandCount());
}

// func void setFrequencyBandCount(int count)
deClassAudioAnalyzer::nfSetFrequencyBandCount::nfSetFrequencyBandCount(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setFrequencyBandCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // count
}
void deClassAudioAnalyzer::nfSetFrequencyBandCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetFrequencyBandCount(rt->GetValue(0)->GetInt());
}

// func void updateResults()
deClassAudioAnalyzer::nfUpdateResults::nfUpdateResults(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "updateResults", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassAudioAnalyzer::nfUpdateResults::RunFunction(dsRunTime*, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.UpdateResults();
}



// Time-Domain Results
///////////////////////

// func float getRMS()
deClassAudioAnalyzer::nfGetRMS::nfGetRMS(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getRMS", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetRMS::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetRMS());
}

// func float getPeak()
deClassAudioAnalyzer::nfGetPeak::nfGetPeak(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getPeak", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetPeak::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetPeak());
}

// func float getZeroCrossingRate()
deClassAudioAnalyzer::nfGetZeroCrossingRate::nfGetZeroCrossingRate(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getZeroCrossingRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetZeroCrossingRate::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetZeroCrossingRate());
}



// Frequency-Domain Results
////////////////////////////

// func float getSpectralCentroid()
deClassAudioAnalyzer::nfGetSpectralCentroid::nfGetSpectralCentroid(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getSpectralCentroid", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetSpectralCentroid::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetSpectralCentroid());
}

// func float getSpectralFlatness()
deClassAudioAnalyzer::nfGetSpectralFlatness::nfGetSpectralFlatness(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getSpectralFlatness", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetSpectralFlatness::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetSpectralFlatness());
}

// func float getSpectralFlux()
deClassAudioAnalyzer::nfGetSpectralFlux::nfGetSpectralFlux(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getSpectralFlux", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetSpectralFlux::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetSpectralFlux());
}

// func float getSpectralRolloff()
deClassAudioAnalyzer::nfGetSpectralRolloff::nfGetSpectralRolloff(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getSpectralRolloff", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetSpectralRolloff::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetSpectralRolloff());
}

// func int getSpectralPeakCount()
deClassAudioAnalyzer::nfGetSpectralPeakCount::nfGetSpectralPeakCount(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getSpectralPeakCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassAudioAnalyzer::nfGetSpectralPeakCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushInt(analyzer.GetSpectralPeaks().GetCount());
}

// func float getSpectralPeakFrequencyAt(int index)
deClassAudioAnalyzer::nfGetSpectralPeakFrequencyAt::nfGetSpectralPeakFrequencyAt(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getSpectralPeakFrequencyAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // index
}
void deClassAudioAnalyzer::nfGetSpectralPeakFrequencyAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetSpectralPeaks()[rt->GetValue(0)->GetInt()].frequency);
}

// func float getSpectralPeakMagnitudeAt(int index)
deClassAudioAnalyzer::nfGetSpectralPeakMagnitudeAt::nfGetSpectralPeakMagnitudeAt(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getSpectralPeakMagnitudeAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // index
}
void deClassAudioAnalyzer::nfGetSpectralPeakMagnitudeAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetSpectralPeaks()[rt->GetValue(0)->GetInt()].magnitude);
}

// func float getFrequencyBandAt(int index)
deClassAudioAnalyzer::nfGetFrequencyBandAt::nfGetFrequencyBandAt(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getFrequencyBandAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // index
}
void deClassAudioAnalyzer::nfGetFrequencyBandAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetFrequencyBands()[rt->GetValue(0)->GetInt()]);
}

// func float getPitch()
deClassAudioAnalyzer::nfGetPitch::nfGetPitch(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getPitch", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetPitch::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetPitch());
}



// func int hashCode()
deClassAudioAnalyzer::nfHashCode::nfHashCode(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassAudioAnalyzer::nfHashCode::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer * const analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushInt((int)(intptr_t)analyzer);
}

// func bool equals(Object obj)
deClassAudioAnalyzer::nfEquals::nfEquals(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
	p_AddParameter(init.clsObject); // obj
}
void deClassAudioAnalyzer::nfEquals::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer * const analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	deClassAudioAnalyzer * const clsAudioAnalyze = static_cast<deClassAudioAnalyzer*>(GetOwnerClass());
	
	dsValue * const object = rt->GetValue(0);
	if(!p_IsObjOfType(object, clsAudioAnalyze)){
		rt->PushBool(false);
		
	}else{
		rt->PushBool(dedsGetNativeData<sAANatDat>(p_GetNativeData(object)).audioAnalyze == analyzer);
	}
}



// Class deClassAudioAnalyzer
//////////////////////////////

// Constructor
////////////////

deClassAudioAnalyzer::deClassAudioAnalyzer(deScriptingDragonScript &ds) :
dsClass("AudioAnalyzer", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_SCENERY);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(dedsNativeDataSize<sAANatDat>());
}

deClassAudioAnalyzer::~deClassAudioAnalyzer() = default;



// Management
///////////////

void deClassAudioAnalyzer::CreateClassMembers(dsEngine *engine){
	sInitData init;
	init.clsAudioAnalyzer = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	// add functions
	AddFunction(new nfNew(init));
	AddFunction(new nfDestructor(init));
	
	AddFunction(new nfGetUseAudioCapture(init));
	AddFunction(new nfSetUseAudioCapture(init));
	AddFunction(new nfGetFrequencyBandCount(init));
	AddFunction(new nfSetFrequencyBandCount(init));
	AddFunction(new nfUpdateResults(init));
	
	AddFunction(new nfGetRMS(init));
	AddFunction(new nfGetPeak(init));
	AddFunction(new nfGetZeroCrossingRate(init));
	AddFunction(new nfGetSpectralCentroid(init));
	AddFunction(new nfGetSpectralFlatness(init));
	AddFunction(new nfGetSpectralFlux(init));
	AddFunction(new nfGetSpectralRolloff(init));
	AddFunction(new nfGetSpectralPeakCount(init));
	AddFunction(new nfGetSpectralPeakFrequencyAt(init));
	AddFunction(new nfGetSpectralPeakMagnitudeAt(init));
	AddFunction(new nfGetFrequencyBandAt(init));
	AddFunction(new nfGetPitch(init));
	
	AddFunction(new nfEquals(init));
	AddFunction(new nfHashCode(init));
	
	// calculate member offsets
	CalcMemberOffsets();
}

deAudioAnalyzer *deClassAudioAnalyzer::GetAudioAnalyze(dsRealObject *myself) const{
	if(!myself){
		return nullptr;
	}
	return dedsGetNativeData<sAANatDat>(p_GetNativeData(myself->GetBuffer())).audioAnalyze;
}

void deClassAudioAnalyzer::PushAudioAnalyze(dsRunTime *rt, deAudioAnalyzer *analyzer){
	if(!rt){
		DSTHROW(dueInvalidParam);
	}
	
	if(!analyzer){
		rt->PushObject(nullptr, this);
		return;
	}
	
	rt->CreateObjectNakedOnStack(this);
	dedsNewNativeData<sAANatDat>(p_GetNativeData(
		rt->GetValue(0)->GetRealObject()->GetBuffer())).audioAnalyze = analyzer;
}
