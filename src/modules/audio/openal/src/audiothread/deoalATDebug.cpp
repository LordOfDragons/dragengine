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

#include "deoalATDebug.h"
#include "deoalAudioThread.h"
#include "deoalDebugInfo.h"
#include "deoalATLogger.h"
#include "../deAudioOpenAL.h"
#include "../devmode/deoalDevMode.h"
#include "../environment/deoalEnvironment.h"
#include "../extensions/deoalExtensions.h"
#include "../microphone/deoalMicrophone.h"
#include "../microphone/deoalAMicrophone.h"
#include "../speaker/deoalASpeaker.h"
#include "../world/deoalWorld.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/debug/deDebugBlockInfo.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerManager.h>
#include <dragengine/resources/world/deWorld.h>



// Callbacks
//////////////

void fDebugOutput(ALenum source, ALenum type, ALuint id, ALenum severity,
ALsizei length, const ALchar *message, void *userParam) AL_API_NOEXCEPT17{
	deoalAudioThread &audioThread = *((deoalAudioThread*)userParam);
	if(!audioThread.HasDebug() || !audioThread.GetDebug().GetEnabled()){
		return;
	}
	
	bool stackTrace = true;
	
	const char *strSource;
	switch(source){
	case AL_DEBUG_SOURCE_API_EXT:
		strSource = "API";
		break;
		
	case AL_DEBUG_SOURCE_AUDIO_SYSTEM_EXT:
		strSource = "Audio System";
		break;
		
	case AL_DEBUG_SOURCE_THIRD_PARTY_EXT:
		strSource = "Third Party";
		break;
		
	case AL_DEBUG_SOURCE_APPLICATION_EXT:
		strSource = "Application";
		break;
		
	case AL_DEBUG_SOURCE_OTHER_EXT:
		strSource = "Other";
		break;
		
	default:
		strSource = "Unknown";
	}
	
	const char *strType;
	switch(type){
	case AL_DEBUG_TYPE_ERROR_EXT:
		strType = "Error";
		break;
		
	case AL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_EXT:
		strType = "Deprecated Behavior";
		break;
		
	case AL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_EXT:
		strType = "Undefined Behavior";
		break;
		
	case AL_DEBUG_TYPE_PORTABILITY_EXT:
		strType = "Portability";
		break;
		
	case AL_DEBUG_TYPE_PERFORMANCE_EXT:
		strType = "Performance";
		break;
		
	case AL_DEBUG_TYPE_MARKER_EXT:
		strType = "Marker";
		break;
		
	case AL_DEBUG_TYPE_OTHER_EXT:
		strType = "Other";
		break;
		
	default:
		strType = "Unknown";
	}
	
	switch(severity){
	case AL_DEBUG_SEVERITY_HIGH_EXT:
		audioThread.GetLogger().LogErrorFormat("Source(%s) Type(%s) ID(0x%x): %s",
			strSource, strType, id, message);
		if(stackTrace){
			audioThread.GetLogger().LogException(deeInvalidAction(__FILE__, __LINE__));
		}
		break;
		
	case AL_DEBUG_SEVERITY_MEDIUM_EXT:
		if(audioThread.GetConfiguration().GetDoLogWarn()){
			audioThread.GetLogger().LogWarnFormat("Source(%s) Type(%s) ID(0x%x): %s",
				strSource, strType, id, message);
			if(stackTrace){
				audioThread.GetLogger().LogException(deeInvalidAction(__FILE__, __LINE__));
			}
		}
		break;
		
	case AL_DEBUG_SEVERITY_LOW_EXT:
		if(audioThread.GetConfiguration().GetDoLogDebug()){
			audioThread.GetLogger().LogInfoFormat("Source(%s) Type(%s) ID(0x%x): %s",
				strSource, strType, id, message);
			if(stackTrace){
				audioThread.GetLogger().LogException(deeInvalidAction(__FILE__, __LINE__));
			}
		}
		break;
		
	case AL_DEBUG_SEVERITY_NOTIFICATION_EXT:
		if(audioThread.GetConfiguration().GetDoLogDebug()){
			audioThread.GetLogger().LogInfoFormat("Source(%s) Type(%s) ID(0x%x): %s",
				strSource, strType, id, message);
		}
		break;
		
	default:
		break;
	}
}


// Class deoalATDebug
////////////////////////

// Constructor, destructor
////////////////////////////

deoalATDebug::deoalATDebug( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),

pEnabled( false ),

pLogCalcEnvProbe( false ),
pShowSpeakerEnvInfo( false ),
pShowSpeakerEnvInfoAt( false )
{
#ifdef WITH_DEBUG
	if(audioThread.GetExtensions().GetHasExtension(deoalExtensions::ext_ALC_EXT_debug)){
		palDebugMessageCallback(fDebugOutput, &audioThread);
		alEnable(AL_DEBUG_OUTPUT_EXT);
		
		palDebugMessageControl(AL_DONT_CARE_EXT, AL_DONT_CARE_EXT, AL_DONT_CARE_EXT, 0, NULL, AL_TRUE);
		
		audioThread.GetLogger().LogInfo("Debugging callback using ALC_EXT_debug activated");
	}
#endif
}

deoalATDebug::~deoalATDebug(){
	if(palDebugMessageCallback){
		palDebugMessageCallback(nullptr, nullptr);
	}
}



// Management
///////////////

void deoalATDebug::Synchronize(){
	const deoalDevMode * const devmode = pAudioThread.GetOal().GetDevMode();
	deoalDebugInfo &debugInfo = pAudioThread.GetDebugInfo();
	
	if( devmode && devmode->GetEnabled() ){
		pEnabled = true;
		pLogCalcEnvProbe = devmode->GetLogCalcEnvProbe();
		pShowSpeakerEnvInfo = devmode->GetShowSpeakerEnvInfo();
		pShowSpeakerEnvInfoAt = devmode->GetShowSpeakerEnvInfoAt();
		pShowSpeakerEnvInfoAtPosition = devmode->GetShowSpeakerEnvInfoAtPosition();
		
		if( devmode->GetShowModuleInfo() ){
			debugInfo.ShowDIModule();
			
		}else{
			debugInfo.HideDIModule();
		}
		
		if( devmode->GetShowActiveMicInfo() ){
			debugInfo.ShowDIActiveMic();
			
		}else{
			debugInfo.HideDIActiveMic();
		}
		
		if( devmode->GetCaptureMicRays() ){
			debugInfo.ShowDDActiveMicRays();
		}
		
		if( devmode->GetShowSpeakerEnvInfo() ){
			debugInfo.ShowDISpeakerAtPosition();
			
		}else{
			debugInfo.HideDISpeakerAtPosition();
		}
		
		debugInfo.VisAudSpeakers( devmode->GetVisualizeAudibleSpeakers() );
		
		debugInfo.PrepareDIClosestSpeakers( devmode->GetShowSpeakerEnvInfoClosest() );
		
	}else if( pEnabled ){
		pLogCalcEnvProbe = false;
		pShowSpeakerEnvInfo = false;
		pShowSpeakerEnvInfoAt = false;
		
		debugInfo.HideDIModule();
		debugInfo.HideDIActiveMic();
		debugInfo.HideDDActiveMicRays();
		debugInfo.HideDISpeakerAtPosition();
		debugInfo.PrepareDIClosestSpeakers( 0 );
		debugInfo.PrepareDDClosestSpeakersDirect( 0 );
		
		pEnabled = false;
	}
}
