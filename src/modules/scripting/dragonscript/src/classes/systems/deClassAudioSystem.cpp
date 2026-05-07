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
#include <string.h>
#include <libdscript/libdscript.h>

#include "deClassAudioSystem.h"
#include "../sound/deClassMicrophone.h"
#include "../deClassModuleParameter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/modules/audio/deBaseAudioModule.h>

#include <libdscript/exceptions.h>


// Management
///////////////

// static func Microphone getActiveMicrophone()
deClassAudioSystem::nfGetActiveMicrophone::nfGetActiveMicrophone(const sInitData &init) :
dsFunction(init.clsAudSys, "getActiveMicrophone", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsMic){
}
void deClassAudioSystem::nfGetActiveMicrophone::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAudioSystem *clsAudSys = (deClassAudioSystem*)GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deClassMicrophone *clsMic = clsAudSys->GetDS().GetClassMicrophone();
	deMicrophone *microphone = audSys->GetActiveMicrophone();
	
	if(microphone){
		clsMic->PushMicrophone(rt, microphone);
		
	}else{
		rt->PushObject(nullptr, clsMic);
	}
}

// static func setActiveMicrophone(Microphone microphone)
deClassAudioSystem::nfSetActiveMicrophone::nfSetActiveMicrophone(const sInitData &init) :
dsFunction(init.clsAudSys, "setActiveMicrophone", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsMic); // microphone
}
void deClassAudioSystem::nfSetActiveMicrophone::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAudioSystem *clsAudSys = (deClassAudioSystem*)GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deClassMicrophone *clsMic = clsAudSys->GetDS().GetClassMicrophone();
	dsRealObject *object = rt->GetValue(0)->GetRealObject();
	
	if(object){
		audSys->SetActiveMicrophone(clsMic->GetMicrophone(object));
		
	}else{
		audSys->SetActiveMicrophone(nullptr);
	}
}



// Audio capture
//////////////////

// static func bool canCaptureAudio()
deClassAudioSystem::nfCanCaptureAudio::nfCanCaptureAudio(const sInitData &init) :
dsFunction(init.clsAudSys, "canCaptureAudio", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool){
}
void deClassAudioSystem::nfCanCaptureAudio::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = (static_cast<deClassAudioSystem*>(GetOwnerClass()))->GetDS();
	rt->PushBool(ds.GetGameEngine()->GetAudioSystem()->CanCaptureAudio());
}

// static func void startAudioCapture()
deClassAudioSystem::nfStartAudioCapture::nfStartAudioCapture(const sInitData &init) :
dsFunction(init.clsAudSys, "startAudioCapture", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
}
void deClassAudioSystem::nfStartAudioCapture::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = (static_cast<deClassAudioSystem*>(GetOwnerClass()))->GetDS();
	ds.GetGameEngine()->GetAudioSystem()->StartAudioCapture();
}

// static func void stopAudioCapture()
deClassAudioSystem::nfStopAudioCapture::nfStopAudioCapture(const sInitData &init) :
dsFunction(init.clsAudSys, "stopAudioCapture", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
}
void deClassAudioSystem::nfStopAudioCapture::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = (static_cast<deClassAudioSystem*>(GetOwnerClass()))->GetDS();
	ds.GetGameEngine()->GetAudioSystem()->StopAudioCapture();
}

// static func bool isCapturingAudio()
deClassAudioSystem::nfIsCapturingAudio::nfIsCapturingAudio(const sInitData &init) :
dsFunction(init.clsAudSys, "isCapturingAudio", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool){
}
void deClassAudioSystem::nfIsCapturingAudio::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = (static_cast<deClassAudioSystem*>(GetOwnerClass()))->GetDS();
	rt->PushBool(ds.GetGameEngine()->GetAudioSystem()->IsCapturingAudio());
}

// static func int getAudioCaptureSampleRate()
deClassAudioSystem::nfGetAudioCaptureSampleRate::nfGetAudioCaptureSampleRate(const sInitData &init) :
dsFunction(init.clsAudSys, "getAudioCaptureSampleRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger){
}
void deClassAudioSystem::nfGetAudioCaptureSampleRate::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = (static_cast<deClassAudioSystem*>(GetOwnerClass()))->GetDS();
	deAudioSystem::AudioCaptureFormat format;
	ds.GetGameEngine()->GetAudioSystem()->GetAudioCaptureFormat(format);
	rt->PushInt(format.sampleRate);
}

// static func int getAudioCaptureBitRate()
deClassAudioSystem::nfGetAudioCaptureBitRate::nfGetAudioCaptureBitRate(const sInitData &init) :
dsFunction(init.clsAudSys, "getAudioCaptureBitRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger){
}
void deClassAudioSystem::nfGetAudioCaptureBitRate::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = (static_cast<deClassAudioSystem*>(GetOwnerClass()))->GetDS();
	deAudioSystem::AudioCaptureFormat format;
	ds.GetGameEngine()->GetAudioSystem()->GetAudioCaptureFormat(format);
	rt->PushInt(format.bitRate);
}

// static func float getAudioCapturePeak()
deClassAudioSystem::nfGetAudioCapturePeak::nfGetAudioCapturePeak(const sInitData &init) :
dsFunction(init.clsAudSys, "getAudioCapturePeak", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFloat){
}
void deClassAudioSystem::nfGetAudioCapturePeak::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = (static_cast<deClassAudioSystem*>(GetOwnerClass()))->GetDS();
	deAudioSystem::AudioCaptureLevels levels;
	ds.GetGameEngine()->GetAudioSystem()->GetAudioCaptureLevels(levels);
	rt->PushFloat(levels.peak);
}

// static func float getAudioCaptureRMS()
deClassAudioSystem::nfGetAudioCaptureRMS::nfGetAudioCaptureRMS(const sInitData &init) :
dsFunction(init.clsAudSys, "getAudioCaptureRMS", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFloat){
}
void deClassAudioSystem::nfGetAudioCaptureRMS::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = (static_cast<deClassAudioSystem*>(GetOwnerClass()))->GetDS();
	deAudioSystem::AudioCaptureLevels levels;
	ds.GetGameEngine()->GetAudioSystem()->GetAudioCaptureLevels(levels);
	rt->PushFloat(levels.rms);
}


// static func float magnitudeToDb(float level)
deClassAudioSystem::nfMagnitudeToDb::nfMagnitudeToDb(const sInitData &init) :
dsFunction(init.clsAudSys, "magnitudeToDb", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFloat){
	p_AddParameter(init.clsFloat); // level
}
void deClassAudioSystem::nfMagnitudeToDb::RunFunction(dsRunTime *rt, dsValue*){
	const float level = rt->GetValue(0)->GetFloat();
	const float db = 20.0f * log10f(decMath::clamp(level, 1e-3f, 1.0f));
	rt->PushFloat(db);
}

// static func float magnitudeToDbPercentage(float level)
deClassAudioSystem::nfMagnitudeToDbPercentage::nfMagnitudeToDbPercentage(const sInitData &init) :
dsFunction(init.clsAudSys, "magnitudeToDbPercentage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFloat){
	p_AddParameter(init.clsFloat); // level
}
void deClassAudioSystem::nfMagnitudeToDbPercentage::RunFunction(dsRunTime *rt, dsValue*){
	const float level = rt->GetValue(0)->GetFloat();
	const float db = 20.0f * log10f(decMath::clamp(level, 1e-3f, 1.0f));
	rt->PushFloat(decMath::linearStep(db, -60.0f, 0.0f));
}


// Parameters
///////////////

// static func int getParameterCount()
deClassAudioSystem::nfGetParameterCount::nfGetParameterCount(const sInitData &init) :
dsFunction(init.clsAudSys, "getParameterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger){
}
void deClassAudioSystem::nfGetParameterCount::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAudioSystem *clsAudSys = (deClassAudioSystem*)GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deBaseAudioModule *module = audSys->GetActiveModule();
	
	rt->PushInt(module->GetParameterCount());
}

// static func ModuleParameter getParameterInfo(int index)
deClassAudioSystem::nfGetParameterInfo::nfGetParameterInfo(const sInitData &init) :
dsFunction(init.clsAudSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsInteger); // index
}
void deClassAudioSystem::nfGetParameterInfo::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAudioSystem *clsAudSys = (deClassAudioSystem*)GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deBaseAudioModule *module = audSys->GetActiveModule();
	int index = rt->GetValue(0)->GetInt();
	
	if(index < 0 || index >= module->GetParameterCount()) DSTHROW(dueInvalidParam);
	
	clsAudSys->GetDS().GetClassModuleParameter()->PushParameter(rt, module, index);
}

// static func ModuleParameter getParameterInfo(String name)
deClassAudioSystem::nfGetParameterInfo2::nfGetParameterInfo2(const sInitData &init) :
dsFunction(init.clsAudSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter(init.clsString); // name
}
void deClassAudioSystem::nfGetParameterInfo2::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassAudioSystem*)GetOwnerClass())->GetDS();
	const deAudioSystem &audSys = *ds.GetGameEngine()->GetAudioSystem();
	deBaseAudioModule * const module = audSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter(rt, module,
		module->IndexOfParameterNamed(rt->GetValue(0)->GetString()));
}

// static func String getParameterValue(String name)
deClassAudioSystem::nfGetParameterValue::nfGetParameterValue(const sInitData &init) :
dsFunction(init.clsAudSys, "getParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString){
	p_AddParameter(init.clsString); // name
}
void deClassAudioSystem::nfGetParameterValue::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAudioSystem *clsAudSys = (deClassAudioSystem*)GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deBaseAudioModule *module = audSys->GetActiveModule();
	rt->PushString(module->GetParameterValue(rt->GetValue(0)->GetString()));
}

// static func void setParameterValue(String name, String value)
deClassAudioSystem::nfSetParameterValue::nfSetParameterValue(const sInitData &init) :
dsFunction(init.clsAudSys, "setParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
	p_AddParameter(init.clsString); // name
	p_AddParameter(init.clsString); // value
}
void deClassAudioSystem::nfSetParameterValue::RunFunction(dsRunTime *rt, dsValue *myself){
	deClassAudioSystem *clsAudSys = (deClassAudioSystem*)GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deBaseAudioModule *module = audSys->GetActiveModule();
	
	module->SetParameterValue(rt->GetValue(0)->GetString(), rt->GetValue(1)->GetString());
}



// static func String sendCommand(String command)
deClassAudioSystem::nfSendCommand::nfSendCommand(const sInitData &init) : dsFunction(init.clsAudSys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString){
	p_AddParameter(init.clsString); // command
}
void deClassAudioSystem::nfSendCommand::RunFunction(dsRunTime *rt, dsValue *myself){
	const deScriptingDragonScript &ds = ((deClassAudioSystem*)GetOwnerClass())->GetDS();
	const deAudioSystem &audioSystem = *ds.GetGameEngine()->GetAudioSystem();
	deBaseAudioModule &module = *audioSystem.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8(rt->GetValue(0)->GetString());
	argList.ParseCommand(command);
	module.SendCommand(argList, answer);
	rt->PushString(answer.ToUTF8());
}

// static func int getFPSRate()
deClassAudioSystem::nfGetFPSRate::nfGetFPSRate(const sInitData &init) :
dsFunction(init.clsAudSys, "getFPSRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger){
}
void deClassAudioSystem::nfGetFPSRate::RunFunction(dsRunTime *rt, dsValue*){
	const deScriptingDragonScript &ds = ((deClassAudioSystem*)GetOwnerClass())->GetDS();
	rt->PushInt(ds.GetGameEngine()->GetAudioSystem()->GetActiveModule()->GetFPSRate());
}



// Class deClassAudioSystem
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassAudioSystem::deClassAudioSystem(deScriptingDragonScript &ds) :
dsClass("AudioSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS(ds)
{
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	
	p_SetNativeDataSize(0);
}

deClassAudioSystem::~deClassAudioSystem(){
}



// Management
///////////////

void deClassAudioSystem::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsAudSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsMic = pDS.GetClassMicrophone();
	init.clsModPar = pDS.GetClassModuleParameter();
	
	// add functions
	AddFunction(new nfGetActiveMicrophone(init));
	AddFunction(new nfSetActiveMicrophone(init));
	
	AddFunction(new nfCanCaptureAudio(init));
	AddFunction(new nfStartAudioCapture(init));
	AddFunction(new nfStopAudioCapture(init));
	AddFunction(new nfIsCapturingAudio(init));
	AddFunction(new nfGetAudioCaptureSampleRate(init));
	AddFunction(new nfGetAudioCaptureBitRate(init));
	AddFunction(new nfGetAudioCapturePeak(init));
	AddFunction(new nfGetAudioCaptureRMS(init));
	
	AddFunction(new nfMagnitudeToDb(init));
	AddFunction(new nfMagnitudeToDbPercentage(init));
	
	AddFunction(new nfGetParameterCount(init));
	AddFunction(new nfGetParameterInfo(init));
	AddFunction(new nfGetParameterInfo2(init));
	AddFunction(new nfGetParameterValue(init));
	AddFunction(new nfSetParameterValue(init));
	
	AddFunction(new nfSendCommand(init));
	AddFunction(new nfGetFPSRate(init));
}
