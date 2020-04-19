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
#include <stdint.h>
#include <string.h>

#include <libdscript/libdscript.h>

#include "deClassSoundLevelMeterSpeaker.h"
#include "deClassSpeaker.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/sound/deSpeaker.h>



struct sSLMSNatDat{
	deSpeaker *speaker;
	float volume;
};



// Constructors, Destructors
//////////////////////////////

// private func new()
deClassSoundLevelMeterSpeaker::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeterSpeaker, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PRIVATE | DSTM_NATIVE, init.clsVoid ){
}
void deClassSoundLevelMeterSpeaker::nfNew::RunFunction( dsRunTime*, dsValue* ){
	DSTHROW( dueInvalidAction );
}

// public func destructor()
deClassSoundLevelMeterSpeaker::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeterSpeaker, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSoundLevelMeterSpeaker::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	sSLMSNatDat &nd = *( ( sSLMSNatDat* )p_GetNativeData( myself ) );
	
	if( nd.speaker ){
		nd.speaker->FreeReference();
		nd.speaker = NULL;
	}
}



// Management
///////////////

// public func Speaker getSpeaker()
deClassSoundLevelMeterSpeaker::nfGetSpeaker::nfGetSpeaker( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeterSpeaker, "getSpeaker", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSpeaker ){
}
void deClassSoundLevelMeterSpeaker::nfGetSpeaker::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSLMSNatDat &nd = *( ( sSLMSNatDat* )p_GetNativeData( myself ) );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeterSpeaker* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSpeaker()->PushSpeaker( rt, nd.speaker );
}

// public func float getVolume()
deClassSoundLevelMeterSpeaker::nfGetVolume::nfGetVolume( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeterSpeaker, "getVolume", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassSoundLevelMeterSpeaker::nfGetVolume::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sSLMSNatDat &nd = *( ( sSLMSNatDat* )p_GetNativeData( myself ) );
	
	rt->PushFloat( nd.volume );
}



// Class deClassSoundLevelMeterSpeaker
////////////////////////////////////////

// Constructor
////////////////

deClassSoundLevelMeterSpeaker::deClassSoundLevelMeterSpeaker( deScriptingDragonScript &ds ) :
dsClass( "SoundLevelMeterSpeaker", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSLMSNatDat ) );
}

deClassSoundLevelMeterSpeaker::~deClassSoundLevelMeterSpeaker(){
}



// Management
///////////////

void deClassSoundLevelMeterSpeaker::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsSoundLevelMeterSpeaker = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	init.clsSpeaker = pDS.GetClassSpeaker();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetSpeaker( init ) );
	AddFunction( new nfGetVolume( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

void deClassSoundLevelMeterSpeaker::PushSoundLevelMeterSpeaker( dsRunTime* rt,
const deSoundLevelMeter::cAudibleSpeaker &speaker ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	
	sSLMSNatDat &nd = *( ( sSLMSNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	
	nd.speaker = speaker.GetSpeaker();
	if( nd.speaker ){
		nd.speaker->AddReference();
	}
	
	nd.volume = speaker.GetVolume();
}
