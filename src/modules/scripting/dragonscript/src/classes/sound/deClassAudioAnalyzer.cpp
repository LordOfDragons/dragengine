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

// func int getResolution()
deClassAudioAnalyzer::nfGetResolution::nfGetResolution(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getResolution", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassAudioAnalyzer::nfGetResolution::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushInt(analyzer.GetResolution());
}

// func void setResolution(int resolution)
deClassAudioAnalyzer::nfSetResolution::nfSetResolution(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setResolution", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // resolution
}
void deClassAudioAnalyzer::nfSetResolution::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetResolution(rt->GetValue(0)->GetInt());
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

// func float getLowestFrequency()
deClassAudioAnalyzer::nfGetLowestFrequency::nfGetLowestFrequency(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getLowestFrequency", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetLowestFrequency::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetLowestFrequency());
}

// func float getHighestFrequency()
deClassAudioAnalyzer::nfGetHighestFrequency::nfGetHighestFrequency(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getHighestFrequency", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetHighestFrequency::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetHighestFrequency());
}

// func void setFrequencyRange(float lowestFrequency, float highestFrequency)
deClassAudioAnalyzer::nfSetFrequencyRange::nfSetFrequencyRange(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setFrequencyRange", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // lowestFrequency
	p_AddParameter(init.clsFloat); // highestFrequency
}
void deClassAudioAnalyzer::nfSetFrequencyRange::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetFrequencyRange(rt->GetValue(0)->GetFloat(), rt->GetValue(1)->GetFloat());
}

// func bool getEnablePreEmphasis()
deClassAudioAnalyzer::nfGetEnablePreEmphasis::nfGetEnablePreEmphasis(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getEnablePreEmphasis", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAudioAnalyzer::nfGetEnablePreEmphasis::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushBool(analyzer.GetEnablePreEmphasis());
}

// func void setEnablePreEmphasis(bool enable)
deClassAudioAnalyzer::nfSetEnablePreEmphasis::nfSetEnablePreEmphasis(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setEnablePreEmphasis", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enable
}
void deClassAudioAnalyzer::nfSetEnablePreEmphasis::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetEnablePreEmphasis(rt->GetValue(0)->GetBool());
}

// func float getPreEmphasisFactor()
deClassAudioAnalyzer::nfGetPreEmphasisFactor::nfGetPreEmphasisFactor(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getPreEmphasisFactor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetPreEmphasisFactor::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetPreEmphasisFactor());
}

// func void setPreEmphasisFactor(float factor)
deClassAudioAnalyzer::nfSetPreEmphasisFactor::nfSetPreEmphasisFactor(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setPreEmphasisFactor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsFloat); // factor
}
void deClassAudioAnalyzer::nfSetPreEmphasisFactor::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetPreEmphasisFactor(rt->GetValue(0)->GetFloat());
}

// func bool getEnableMelFiltering()
deClassAudioAnalyzer::nfGetEnableMelFiltering::nfGetEnableMelFiltering(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getEnableMelFiltering", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAudioAnalyzer::nfGetEnableMelFiltering::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushBool(analyzer.GetEnableMelFiltering());
}

// func void setEnableMelFiltering(bool enable)
deClassAudioAnalyzer::nfSetEnableMelFiltering::nfSetEnableMelFiltering(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setEnableMelFiltering", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // enable
}
void deClassAudioAnalyzer::nfSetEnableMelFiltering::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetEnableMelFiltering(rt->GetValue(0)->GetBool());
}

// func int getMelFilterCount()
deClassAudioAnalyzer::nfGetMelFilterCount::nfGetMelFilterCount(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getMelFilterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger){
}
void deClassAudioAnalyzer::nfGetMelFilterCount::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushInt(analyzer.GetMelFilterCount());
}

// func void setMelFilterCount(int count)
deClassAudioAnalyzer::nfSetMelFilterCount::nfSetMelFilterCount(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setMelFilterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsInteger); // count
}
void deClassAudioAnalyzer::nfSetMelFilterCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetMelFilterCount(rt->GetValue(0)->GetInt());
}

// func bool getNormalizeMelEnergies()
deClassAudioAnalyzer::nfGetNormalizeMelEnergies::nfGetNormalizeMelEnergies(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getNormalizeMelEnergies", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool){
}
void deClassAudioAnalyzer::nfGetNormalizeMelEnergies::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushBool(analyzer.GetNormalizeMelEnergies());
}

// func void setNormalizeMelEnergies(bool normalize)
deClassAudioAnalyzer::nfSetNormalizeMelEnergies::nfSetNormalizeMelEnergies(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "setNormalizeMelEnergies", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter(init.clsBool); // normalize
}
void deClassAudioAnalyzer::nfSetNormalizeMelEnergies::RunFunction(dsRunTime *rt, dsValue *myself){
	deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	analyzer.SetNormalizeMelEnergies(rt->GetValue(0)->GetBool());
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

// func float getFrequencyBandEnergyAt(int index)
deClassAudioAnalyzer::nfGetFrequencyBandEnergyAt::nfGetFrequencyBandEnergyAt(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getFrequencyBandEnergyAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // index
}
void deClassAudioAnalyzer::nfGetFrequencyBandEnergyAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetFrequencyBands()[rt->GetValue(0)->GetInt()].energy);
}

// func float getFrequencyBandLowestFrequencyAt(int index)
deClassAudioAnalyzer::nfGetFrequencyBandLowestFrequencyAt::nfGetFrequencyBandLowestFrequencyAt(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getFrequencyBandLowestFrequencyAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // index
}
void deClassAudioAnalyzer::nfGetFrequencyBandLowestFrequencyAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetFrequencyBands()[rt->GetValue(0)->GetInt()].lowestFrequency);
}

// func float getFrequencyBandHighestFrequencyAt(int index)
deClassAudioAnalyzer::nfGetFrequencyBandHighestFrequencyAt::nfGetFrequencyBandHighestFrequencyAt(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getFrequencyBandHighestFrequencyAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
	p_AddParameter(init.clsInteger); // index
}
void deClassAudioAnalyzer::nfGetFrequencyBandHighestFrequencyAt::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetFrequencyBands()[rt->GetValue(0)->GetInt()].highestFrequency);
}

// func float getMaxBandEnergy()
deClassAudioAnalyzer::nfGetMaxBandEnergy::nfGetMaxBandEnergy(const sInitData &init) :
dsFunction(init.clsAudioAnalyzer, "getMaxBandEnergy", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat){
}
void deClassAudioAnalyzer::nfGetMaxBandEnergy::RunFunction(dsRunTime *rt, dsValue *myself){
	const deAudioAnalyzer &analyzer = dedsGetNativeData<sAANatDat>(p_GetNativeData(myself)).audioAnalyze;
	rt->PushFloat(analyzer.GetFrequencyBands().Inject(0.0f, [](float acc, const deAudioAnalyzer::FrequencyBand &band){
		return decMath::max(acc, band.energy);
	}));
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
	AddFunction(new nfGetResolution(init));
	AddFunction(new nfSetResolution(init));
	AddFunction(new nfGetFrequencyBandCount(init));
	AddFunction(new nfSetFrequencyBandCount(init));
	AddFunction(new nfGetLowestFrequency(init));
	AddFunction(new nfGetHighestFrequency(init));
	AddFunction(new nfSetFrequencyRange(init));
	AddFunction(new nfGetEnablePreEmphasis(init));
	AddFunction(new nfSetEnablePreEmphasis(init));
	AddFunction(new nfGetPreEmphasisFactor(init));
	AddFunction(new nfSetPreEmphasisFactor(init));
	AddFunction(new nfGetEnableMelFiltering(init));
	AddFunction(new nfSetEnableMelFiltering(init));
	AddFunction(new nfGetMelFilterCount(init));
	AddFunction(new nfSetMelFilterCount(init));
	AddFunction(new nfGetNormalizeMelEnergies(init));
	AddFunction(new nfSetNormalizeMelEnergies(init));
	
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
	AddFunction(new nfGetFrequencyBandEnergyAt(init));
	AddFunction(new nfGetFrequencyBandLowestFrequencyAt(init));
	AddFunction(new nfGetFrequencyBandHighestFrequencyAt(init));
	AddFunction(new nfGetMaxBandEnergy(init));
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
