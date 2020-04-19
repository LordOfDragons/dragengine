/* 
 * Drag[en]gine OpenAL Audio Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
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
