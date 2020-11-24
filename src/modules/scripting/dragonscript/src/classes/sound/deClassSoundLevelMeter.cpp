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

#include "deClassSoundLevelMeter.h"
#include "deClassSoundLevelMeterSpeaker.h"
#include "deClassSoundLevelMeterListener.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../physics/deClassLayerMask.h"
#include "../../peers/dedsSoundLevelMeter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/sensor/deSoundLevelMeter.h>
#include <dragengine/resources/sensor/deSoundLevelMeterManager.h>

#include <libdscript/libdscript.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



struct sSLMNatDat{
	deSoundLevelMeter *soundLevelMeter;
};



// Constructors, Destructors
//////////////////////////////

// public func new( int type )
deClassSoundLevelMeter::nfNew::nfNew( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSoundLevelMeter::nfNew::RunFunction( dsRunTime*, dsValue *myself ){
	sSLMNatDat &nd = *( ( sSLMNatDat* )p_GetNativeData( myself ) );
	deSoundLevelMeterManager *tsMgr = ( ( deClassSoundLevelMeter* )GetOwnerClass() )
		->GetDS().GetGameEngine()->GetSoundLevelMeterManager();
	
	// clear ( important )
	nd.soundLevelMeter = NULL;
	
	// create soundLevelMeter
	nd.soundLevelMeter = tsMgr->CreateSoundLevelMeter();
}

// public func destructor()
deClassSoundLevelMeter::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassSoundLevelMeter::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sSLMNatDat &nd = *( ( sSLMNatDat* )p_GetNativeData( myself ) );
	
	if( nd.soundLevelMeter ){
		nd.soundLevelMeter->FreeReference();
		nd.soundLevelMeter = NULL;
	}
}



// Management
///////////////

// public func SoundLevelMeterType getType()
deClassSoundLevelMeter::nfGetType::nfGetType( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSoundLevelMeterType ){
}
void deClassSoundLevelMeter::nfGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	rt->PushValue( ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetClassSoundLevelMeterType()
		->GetVariable( soundLevelMeter.GetType() )->GetStaticValue() );
}

// public func setType( SoundLevelMeterType type )
deClassSoundLevelMeter::nfSetType::nfSetType( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "setType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSoundLevelMeterType ); // type
}
void deClassSoundLevelMeter::nfSetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	soundLevelMeter.SetType( ( deSoundLevelMeter::eMeterTypes )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func DVector getPosition()
deClassSoundLevelMeter::nfGetPosition::nfGetPosition( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsDVector ){
}
void deClassSoundLevelMeter::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetDS();
	
	ds.GetClassDVector()->PushDVector( rt, soundLevelMeter.GetPosition() );
}

// public func setPosition( DVector position )
deClassSoundLevelMeter::nfSetPosition::nfSetPosition( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "setPosition", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVector ); // position
}
void deClassSoundLevelMeter::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetDS();
	
	soundLevelMeter.SetPosition( ds.GetClassDVector()->GetDVector( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func Quaternion getOrientation()
deClassSoundLevelMeter::nfGetOrientation::nfGetOrientation( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsQuaternion ){
}
void deClassSoundLevelMeter::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetDS();
	
	ds.GetClassQuaternion()->PushQuaternion( rt, soundLevelMeter.GetOrientation() );
}

// public func setOrientation( Quaternion orientation )
deClassSoundLevelMeter::nfSetOrientation::nfSetOrientation( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "setOrientation", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuaternion ); // orientation
}
void deClassSoundLevelMeter::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetDS();
	
	soundLevelMeter.SetOrientation( ds.GetClassQuaternion()->GetQuaternion( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func float getConeAngle()
deClassSoundLevelMeter::nfGetConeAngle::nfGetConeAngle( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getConeAngle", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassSoundLevelMeter::nfGetConeAngle::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	rt->PushFloat( soundLevelMeter.GetConeAngle() / DEG2RAD );
}

// public func setConeAngle( float angle )
deClassSoundLevelMeter::nfSetConeAngle::nfSetConeAngle( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "setConeAngle", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // angle
}
void deClassSoundLevelMeter::nfSetConeAngle::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	soundLevelMeter.SetConeAngle( rt->GetValue( 0 )->GetFloat() * DEG2RAD );
}

// public func float getAudibleDistance()
deClassSoundLevelMeter::nfGetAudibleDistance::nfGetAudibleDistance( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getAudibleDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassSoundLevelMeter::nfGetAudibleDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	rt->PushFloat( soundLevelMeter.GetAudibleDistance() );
}

// public func setAudibleDistance( float audibleDistance )
deClassSoundLevelMeter::nfSetAudibleDistance::nfSetAudibleDistance( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "setAudibleDistance", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // audibleDistance
}
void deClassSoundLevelMeter::nfSetAudibleDistance::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	soundLevelMeter.SetAudibleDistance( rt->GetValue( 0 )->GetFloat() );
}

// public func LayerMask getLayerMask()
deClassSoundLevelMeter::nfGetLayerMask::nfGetLayerMask( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask ){
}
void deClassSoundLevelMeter::nfGetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetDS();
	
	ds.GetClassLayerMask()->PushLayerMask( rt, soundLevelMeter.GetLayerMask() );
}

// public func void setLayerMask( LayerMask layerMask )
deClassSoundLevelMeter::nfSetLayerMask::nfSetLayerMask( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "setLayerMask", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLayerMask ); // layerMask
}
void deClassSoundLevelMeter::nfSetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetDS();
	
	soundLevelMeter.SetLayerMask( ds.GetClassLayerMask()->GetLayerMask( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func bool getEnabled()
deClassSoundLevelMeter::nfGetEnabled::nfGetEnabled( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getEnabled", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassSoundLevelMeter::nfGetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	
	rt->PushBool( soundLevelMeter.GetEnabled() );
}

// public func void setEnabled( bool enabled )
deClassSoundLevelMeter::nfSetEnabled::nfSetEnabled( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "setEnabled", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enabled
}
void deClassSoundLevelMeter::nfSetEnabled::RunFunction( dsRunTime *rt, dsValue *myself ){
	deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	
	soundLevelMeter.SetEnabled( rt->GetValue( 0 )->GetBool() );
}



// public func int getAudibleSpeakerCount()
deClassSoundLevelMeter::nfGetAudibleSpeakerCount::nfGetAudibleSpeakerCount( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getAudibleSpeakerCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassSoundLevelMeter::nfGetAudibleSpeakerCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	
	rt->PushInt( soundLevelMeter.GetAudibleSpeakerCount() );
}

// public func SoundLevelMeterSpeaker getAudibleSpeakerAt( int index )
deClassSoundLevelMeter::nfGetAudibleSpeakerAt::nfGetAudibleSpeakerAt( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getAudibleSpeakerAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSoundLevelMeterSpeaker ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassSoundLevelMeter::nfGetAudibleSpeakerAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetDS();
	
	ds.GetClassSoundLevelMeterSpeaker()->PushSoundLevelMeterSpeaker( rt,
		soundLevelMeter.GetAudibleSpeakerAt( rt->GetValue( 0 )->GetInt() ) );
}



// public func SoundLevelMeterListener getListener()
deClassSoundLevelMeter::nfGetListener::nfGetListener( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "getListener", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsSoundLevelMeterListener ){
}
void deClassSoundLevelMeter::nfGetListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	const deScriptingDragonScript &ds = ( ( deClassSoundLevelMeter* )GetOwnerClass() )->GetDS();
	const dedsSoundLevelMeter * const peer = ( dedsSoundLevelMeter* )soundLevelMeter.GetPeerScripting();
	
	if( peer ){
		rt->PushObject( peer->GetCallback(), ds.GetClassSoundLevelMeterListener() );
		
	}else{
		rt->PushObject( NULL, ds.GetClassSoundLevelMeterListener() );
	}
}

// public func void setListener( SoundLevelMeterListener listener )
deClassSoundLevelMeter::nfSetListener::nfSetListener( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "setListener", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSoundLevelMeterListener ); // listener
}
void deClassSoundLevelMeter::nfSetListener::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter &soundLevelMeter = *( ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter );
	dedsSoundLevelMeter * const peer = ( dedsSoundLevelMeter* )soundLevelMeter.GetPeerScripting();
	if( peer ){
		peer->SetCallback( rt->GetValue( 0 )->GetRealObject() );
	}
}



// public func int hashCode()
deClassSoundLevelMeter::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "hashCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassSoundLevelMeter::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter * const soundLevelMeter = ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter;
	
	// hash code = memory location
	rt->PushInt( ( intptr_t )soundLevelMeter );
}

// public func bool equals( Object obj )
deClassSoundLevelMeter::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsSoundLevelMeter, "equals", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassSoundLevelMeter::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deSoundLevelMeter * const soundLevelMeter = ( ( sSLMNatDat* )p_GetNativeData( myself ) )->soundLevelMeter;
	deClassSoundLevelMeter * const clsSoundLevelMeter = ( deClassSoundLevelMeter* )GetOwnerClass();
	
	dsValue * const object = rt->GetValue( 0 );
	if( ! p_IsObjOfType( object, clsSoundLevelMeter ) ){
		rt->PushBool( false );
		
	}else{
		rt->PushBool( soundLevelMeter == ( ( sSLMNatDat* )p_GetNativeData( object ) )->soundLevelMeter );
	}
}



// Class deClassSoundLevelMeter
/////////////////////////////////

// Constructor
////////////////

deClassSoundLevelMeter::deClassSoundLevelMeter( deScriptingDragonScript &ds ) :
dsClass( "SoundLevelMeter", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sSLMNatDat ) );
}

deClassSoundLevelMeter::~deClassSoundLevelMeter(){
}



// Management
///////////////

void deClassSoundLevelMeter::CreateClassMembers( dsEngine *engine ){
	pClsSoundLevelMeterType = engine->GetClass( "Dragengine.Scenery.SoundLevelMeterType" );
	
	sInitData init;
	init.clsSoundLevelMeter = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsQuaternion = pDS.GetClassQuaternion();
	init.clsDVector = pDS.GetClassDVector();
	init.clsLayerMask = pDS.GetClassLayerMask();
	init.clsSoundLevelMeterSpeaker = pDS.GetClassSoundLevelMeterSpeaker();
	init.clsSoundLevelMeterListener = pDS.GetClassSoundLevelMeterListener();
	init.clsSoundLevelMeterType = pClsSoundLevelMeterType;
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetType( init ) );
	AddFunction( new nfSetType( init ) );
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfGetConeAngle( init ) );
	AddFunction( new nfSetConeAngle( init ) );
	AddFunction( new nfGetAudibleDistance( init ) );
	AddFunction( new nfSetAudibleDistance( init ) );
	AddFunction( new nfGetLayerMask( init ) );
	AddFunction( new nfSetLayerMask( init ) );
	AddFunction( new nfGetEnabled( init ) );
	AddFunction( new nfSetEnabled( init ) );
	
	AddFunction( new nfGetAudibleSpeakerCount( init ) );
	AddFunction( new nfGetAudibleSpeakerAt( init ) );
	
	AddFunction( new nfGetListener( init ) );
	AddFunction( new nfSetListener( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deSoundLevelMeter *deClassSoundLevelMeter::GetSoundLevelMeter( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	return ( ( sSLMNatDat* )p_GetNativeData( myself->GetBuffer() ) )->soundLevelMeter;
}

void deClassSoundLevelMeter::PushSoundLevelMeter( dsRunTime *rt, deSoundLevelMeter *soundLevelMeter ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! soundLevelMeter ){
		rt->PushObject( NULL, this );
		return;
	}
	
	// create new value
	rt->CreateObjectNakedOnStack( this );
	( ( sSLMNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )
		->soundLevelMeter = soundLevelMeter;
	soundLevelMeter->AddReference();
}
