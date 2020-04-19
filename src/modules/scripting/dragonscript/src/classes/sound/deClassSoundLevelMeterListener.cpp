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
#include <string.h>

#include "deClassSoundLevelMeterSpeaker.h"
#include "deClassSoundLevelMeterListener.h"
#include "deClassSpeaker.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/modules/scripting/deBaseScriptingSoundLevelMeter.h>

#include <libdscript/exceptions.h>



// public func void speakerAudible( SoundLevelMeterSpeaker speaker )
deClassSoundLevelMeterListener::nfSpeakerAudible::nfSpeakerAudible( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeterListener, "speakerAudible", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSoundLevelMeterSpeaker ); // speaker
}
void deClassSoundLevelMeterListener::nfSpeakerAudible::RunFunction( dsRunTime*, dsValue* ){
}

// public func void speakerInaudible( Speaker speaker )
deClassSoundLevelMeterListener::nfSpeakerInaudible::nfSpeakerInaudible( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeterListener, "speakerInaudible", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_ABSTRACT | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSpeaker ); // speaker
}
void deClassSoundLevelMeterListener::nfSpeakerInaudible::RunFunction( dsRunTime*, dsValue* ){
}



// Class deClassSoundLevelMeterListener
/////////////////////////////////////////

// Constructor
////////////////

deClassSoundLevelMeterListener::deClassSoundLevelMeterListener( deScriptingDragonScript &ds ) :
dsClass( "SoundLevelMeterListener", DSCT_INTERFACE, DSTM_PUBLIC | DSTM_NATIVE | DSTM_ABSTRACT ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	
	p_SetNativeDataSize( 0 );
}

deClassSoundLevelMeterListener::~deClassSoundLevelMeterListener(){
}


// Management
///////////////

void deClassSoundLevelMeterListener::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsSoundLevelMeterListener = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsBool = engine->GetClassBool();
	init.clsInteger = engine->GetClassInt();
	
	init.clsSoundLevelMeterSpeaker = pDS.GetClassSoundLevelMeterSpeaker();
	init.clsSpeaker = pDS.GetClassSpeaker();
	
	// add functions
	AddFunction( new nfSpeakerAudible( init ) ); // function 0
	AddFunction( new nfSpeakerInaudible( init ) ); // function 1
	
	// calculate member offsets
	CalcMemberOffsets();
	
	// store function indices for fast calling
	const dsFuncList &funcList = *GetFuncList();
	pFuncIndexSpeakerAudible = funcList.GetIndexOf( GetFunction( 0 ) );
	pFuncIndexSpeakerInaudible = funcList.GetIndexOf( GetFunction( 1 ) );
}
