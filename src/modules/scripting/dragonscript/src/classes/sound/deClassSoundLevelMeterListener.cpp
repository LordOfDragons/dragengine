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
