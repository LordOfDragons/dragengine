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

#include "deoalATDebug.h"
#include "deoalAudioThread.h"
#include "deoalDebugInfo.h"
#include "../deAudioOpenAL.h"
#include "../devmode/deoalDevMode.h"
#include "../environment/deoalEnvironment.h"
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



// Class deoalATDebug
////////////////////////

// Constructor, destructor
////////////////////////////

deoalATDebug::deoalATDebug( deoalAudioThread &audioThread ) :
pAudioThread( audioThread ),

pEnabled( false ),

pLogCalcEnvProbe( false ),
pShowSpeakerEnvInfo( false ),
pShowSpeakerEnvInfoAt( false ){
}

deoalATDebug::~deoalATDebug(){
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
