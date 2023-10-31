/* 
 * Drag[en]gine DragonScript Script Module
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

#include "dedsSoundLevelMeter.h"
#include "../deScriptingDragonScript.h"
#include "../classes/sound/deClassSpeaker.h"
#include "../classes/sound/deClassSoundLevelMeter.h"
#include "../classes/sound/deClassSoundLevelMeterSpeaker.h"
#include "../classes/sound/deClassSoundLevelMeterListener.h"

#include <libdscript/exceptions.h>
#include <libdscript/libdscript.h>

#include <dragengine/resources/sensor/deSoundLevelMeter.h>



// Class dedsSoundLevelMeter
//////////////////////////////

// Constructor, destructor
////////////////////////////

dedsSoundLevelMeter::dedsSoundLevelMeter( deScriptingDragonScript &ds,
deSoundLevelMeter *soundLevelMeter ) :
pDS( ds ),
pSoundLevelMeter( soundLevelMeter ),
pValCB( NULL ),
pHasCB( false )
{
	if( ! soundLevelMeter ){
		DSTHROW( dueInvalidParam );
	}
	
	pValCB = ds.GetScriptEngine()->GetMainRunTime()->CreateValue( ds.GetClassSoundLevelMeterListener() );
}

dedsSoundLevelMeter::~dedsSoundLevelMeter(){
	if( ! pValCB ){
		return;
	}
	
	// check if the resource is in progress of being deleted. if this is not
	// the case we can end up re-entering this destructor due to the resource
	// being deleted due to links breaking while freeing the value. if this
	// is the case delay the deletion until a safe time
	if( pSoundLevelMeter && pSoundLevelMeter->GetRefCount() > 0 ){
		pDS.AddValueDeleteLater( pValCB );
		
	}else{
		pDS.GetScriptEngine()->GetMainRunTime()->FreeValue( pValCB );
	}
	
	pValCB = NULL;
	pHasCB = false;
}



// Management
///////////////

dsRealObject *dedsSoundLevelMeter::GetCallback() const{
	return pValCB->GetRealObject();
}

void dedsSoundLevelMeter::SetCallback( dsRealObject *object ){
	if( ! pValCB ){
		return;
	}
	
	dsRunTime &rt = *pDS.GetScriptEngine()->GetMainRunTime();
	
	if( object ){
		rt.SetObject( pValCB, object );
		rt.CastValueTo( pValCB, pValCB, pDS.GetClassSoundLevelMeterListener() );
		pHasCB = true;
		
	}else{
		rt.SetNull( pValCB, pDS.GetClassSoundLevelMeterListener() );
		pHasCB = false;
	}
}



// Notifications
//////////////////

void dedsSoundLevelMeter::SpeakerAudible( const deSoundLevelMeter::cAudibleSpeaker &speaker ){
	if( ! pHasCB ){
		return;
	}
	
	const int funcIndex = pDS.GetClassSoundLevelMeterListener()->GetFuncIndexSpeakerAudible();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	
	try{
		pDS.GetClassSoundLevelMeterSpeaker()->PushSoundLevelMeterSpeaker( rt, speaker ); // speaker
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}

void dedsSoundLevelMeter::SpeakerInaudible( deSpeaker *speaker ){
	if( ! speaker ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! pHasCB ){
		return;
	}
	
	const int funcIndex = pDS.GetClassSoundLevelMeterListener()->GetFuncIndexSpeakerInaudible();
	dsRunTime * const rt = pDS.GetScriptEngine()->GetMainRunTime();
	
	try{
		pDS.GetClassSpeaker()->PushSpeaker( rt, speaker ); // speaker
		rt->RunFunctionFast( pValCB, funcIndex );
		
	}catch( const duException &e ){
		rt->PrintExceptionTrace();
		e.PrintError();
	}
}
