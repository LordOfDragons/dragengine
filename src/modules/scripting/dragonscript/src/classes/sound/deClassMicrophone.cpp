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

#include "deClassMicrophone.h"
#include "deClassSpeaker.h"
#include "../math/deClassVector.h"
#include "../math/deClassDVector.h"
#include "../math/deClassQuaternion.h"
#include "../physics/deClassLayerMask.h"
#include "../world/deClassWorld.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/resources/sound/deMicrophoneManager.h>

#include <libdscript/libdscript.h>
#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// native structure
struct sMicNatDat{
	deMicrophone *microphone;
};



// Native functions
/////////////////////

// public func new()
deClassMicrophone::nfNew::nfNew( const sInitData &init ) : dsFunction( init.clsMic,
DSFUNC_CONSTRUCTOR, DSFT_CONSTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMicrophone::nfNew::RunFunction(dsRunTime *rt, dsValue *myself){
	sMicNatDat *nd = ( sMicNatDat* )p_GetNativeData( myself );
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	deMicrophoneManager *spkMgr = clsMic->GetGameEngine()->GetMicrophoneManager();
	
	// clear (important)
	nd->microphone = NULL;
	
	// create microphone
	nd->microphone = spkMgr->CreateMicrophone();
	if( ! nd->microphone ) DSTHROW( dueOutOfMemory );
}

// public func destructor()
deClassMicrophone::nfDestructor::nfDestructor( const sInitData &init ) : dsFunction( init.clsMic,
DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassMicrophone::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sMicNatDat *nd = ( sMicNatDat* )p_GetNativeData( myself );
	
	if( nd->microphone ){
		nd->microphone->FreeReference();
		nd->microphone = NULL;
	}
}



// public func MicrophoneType getType()
deClassMicrophone::nfGetType::nfGetType( const sInitData &init ) : dsFunction( init.clsMic,
"getType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsMicrophoneType ){
}
void deClassMicrophone::nfGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	
	rt->PushValue( ( ( deClassMicrophone* )GetOwnerClass() )->GetClassMicrophoneType()
		->GetVariable( microphone->GetType() )->GetStaticValue() );
}

// public func void setType( MicrophoneType type )
deClassMicrophone::nfSetType::nfSetType( const sInitData &init ) : dsFunction( init.clsMic,
"setType", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsMicrophoneType ); // type
}
void deClassMicrophone::nfSetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( ! rt->GetValue( 0 )->GetRealObject() ){
		DSTHROW( dueNullPointer );
	}
	
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	
	microphone->SetType( ( deMicrophone::eMicrophoneType )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() ) );
}

// public func DVector getPosition()
deClassMicrophone::nfGetPosition::nfGetPosition( const sInitData &init ) : dsFunction( init.clsMic,
"getPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsDVec ){
}
void deClassMicrophone::nfGetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	
	clsMic->GetClassDVector()->PushDVector( rt, microphone->GetPosition() );
}

// public func void setPosition( DVector position )
deClassMicrophone::nfSetPosition::nfSetPosition( const sInitData &init ) : dsFunction( init.clsMic,
"setPosition", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsDVec ); // position
}
void deClassMicrophone::nfSetPosition::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	
	if( ! obj ) DSTHROW( dueNullPointer );
	
	microphone->SetPosition( clsMic->GetClassDVector()->GetDVector( obj ) );
}

// public func Quaternion getOrientation()
deClassMicrophone::nfGetOrientation::nfGetOrientation( const sInitData &init ) : dsFunction( init.clsMic,
"getOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsQuat ){
}
void deClassMicrophone::nfGetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	deClassQuaternion *clsQuat = clsMic->GetScriptModule()->GetClassQuaternion();
	
	clsQuat->PushQuaternion( rt, microphone->GetOrientation() );
}

// public func void setOrientation( Quaternion orientation )
deClassMicrophone::nfSetOrientation::nfSetOrientation( const sInitData &init ) : dsFunction( init.clsMic,
"setOrientation", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsQuat ); // orientation
}
void deClassMicrophone::nfSetOrientation::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	deClassQuaternion *clsQuat = clsMic->GetScriptModule()->GetClassQuaternion();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	
	if( ! obj ) DSTHROW( dueNullPointer );
	
	microphone->SetOrientation( clsQuat->GetQuaternion( obj ) );
}

// public func Vector getVelocity()
deClassMicrophone::nfGetVelocity::nfGetVelocity( const sInitData &init ) : dsFunction( init.clsMic,
"getVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVec ){
}
void deClassMicrophone::nfGetVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	deClassVector *clsVec = clsMic->GetScriptModule()->GetClassVector();
	
	clsVec->PushVector( rt, microphone->GetVelocity() );
}

// public func void setVelocity( Vector velocity )
deClassMicrophone::nfSetVelocity::nfSetVelocity( const sInitData &init ) : dsFunction( init.clsMic,
"setVelocity", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsVec ); // velocity
}
void deClassMicrophone::nfSetVelocity::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	deClassVector *clsVec = clsMic->GetScriptModule()->GetClassVector();
	dsRealObject *obj = rt->GetValue( 0 )->GetRealObject();
	
	if( ! obj ) DSTHROW( dueNullPointer );
	
	microphone->SetVelocity( clsVec->GetVector( obj ) );
}



// public func float getMuted()
deClassMicrophone::nfGetMuted::nfGetMuted( const sInitData &init ) : dsFunction( init.clsMic,
"getMuted", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassMicrophone::nfGetMuted::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	
	rt->PushBool( microphone->GetMuted() );
}

// public func void setMuted( bool muted )
deClassMicrophone::nfSetMuted::nfSetMuted( const sInitData &init ) : dsFunction( init.clsMic,
"setMuted", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsBool ); // muted
}
void deClassMicrophone::nfSetMuted::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	
	microphone->SetMuted( rt->GetValue( 0 )->GetBool() );
}

// public func float getVolume()
deClassMicrophone::nfGetVolume::nfGetVolume( const sInitData &init ) : dsFunction( init.clsMic,
"getVolume", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMicrophone::nfGetVolume::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	
	rt->PushFloat( microphone->GetVolume() );
}

// public func void setVolume( float volume )
deClassMicrophone::nfSetVolume::nfSetVolume( const sInitData &init ) : dsFunction( init.clsMic,
"setVolume", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // volume
}
void deClassMicrophone::nfSetVolume::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	
	microphone->SetVolume( rt->GetValue( 0 )->GetFloat() );
}



// public func LayerMask getLayerMask()
deClassMicrophone::nfGetLayerMask::nfGetLayerMask( const sInitData &init ) : dsFunction( init.clsMic,
"getLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsLayerMask ){
}
void deClassMicrophone::nfGetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deMicrophone &microphone = *( ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone );
	const deScriptingDragonScript &ds = *( ( deClassMicrophone* )GetOwnerClass() )->GetScriptModule();
	
	ds.GetClassLayerMask()->PushLayerMask( rt, microphone.GetLayerMask() );
}

// public func void setLayerMask( LayerMask layerMask )
deClassMicrophone::nfSetLayerMask::nfSetLayerMask( const sInitData &init ) : dsFunction( init.clsMic,
"setLayerMask", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsLayerMask ); // layerMask
}
void deClassMicrophone::nfSetLayerMask::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone &microphone = *( ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone );
	const deScriptingDragonScript &ds = *( ( deClassMicrophone* )GetOwnerClass() )->GetScriptModule();
	
	microphone.SetLayerMask( ds.GetClassLayerMask()->GetLayerMask( rt->GetValue( 0 )->GetRealObject() ) );
}

// public func float getSpeakerGain()
deClassMicrophone::nfGetSpeakerGain::nfGetSpeakerGain( const sInitData &init ) :
dsFunction( init.clsMic, "getSpeakerGain", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsFlt ){
}
void deClassMicrophone::nfGetSpeakerGain::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deMicrophone &microphone = *( ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone );
	rt->PushFloat( microphone.GetSpeakerGain() );
}

// public func void setSpeakerGain(float gain)
deClassMicrophone::nfSetSpeakerGain::nfSetSpeakerGain(const sInitData &init) :
dsFunction( init.clsMic, "setSpeakerGain", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFlt ); // gain
}
void deClassMicrophone::nfSetSpeakerGain::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone &microphone = *( ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone );
	microphone.SetSpeakerGain( rt->GetValue( 0 )->GetFloat() );
}

// public func World getParentWorld()
deClassMicrophone::nfGetParentWorld::nfGetParentWorld( const sInitData &init ) :
dsFunction( init.clsMic, "getParentWorld", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsWorld ){
}
void deClassMicrophone::nfGetParentWorld::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deMicrophone &microphone = *( ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone );
	const deScriptingDragonScript &ds = *( ( deClassMicrophone* )GetOwnerClass() )->GetScriptModule();
	
	ds.GetClassWorld()->PushWorld( rt, microphone.GetParentWorld() );
}



// Speakers
/////////////

// public func void addSpeaker( Speaker speaker )
deClassMicrophone::nfAddSpeaker::nfAddSpeaker( const sInitData &init ) : dsFunction(init.clsMic,
"addSpeaker", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsSpk ); // speaker
}
void deClassMicrophone::nfAddSpeaker::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	
	if( ! object ) DSTHROW( dueNullPointer );
	
	microphone->AddSpeaker( clsMic->GetClassSpeaker()->GetSpeaker( object ) );
}

// public func void removeSpeaker(Speaker speaker)
deClassMicrophone::nfRemoveSpeaker::nfRemoveSpeaker( const sInitData &init ) : dsFunction(init.clsMic,
"removeSpeaker", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
	p_AddParameter( init.clsSpk ); // speaker
}
void deClassMicrophone::nfRemoveSpeaker::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	
	if( ! object ) DSTHROW( dueNullPointer );
	
	microphone->RemoveSpeaker( clsMic->GetClassSpeaker()->GetSpeaker( object ) );
}

// public func void removeAllSpeakers()
deClassMicrophone::nfRemoveAllSpeakers::nfRemoveAllSpeakers( const sInitData &init ) : dsFunction(init.clsMic,
"removeAllSpeakers", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid){
}
void deClassMicrophone::nfRemoveAllSpeakers::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	
	microphone->RemoveAllSpeakers();
}



// public func int hashCode()
deClassMicrophone::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsMic, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInt ){
}

void deClassMicrophone::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	
	rt->PushInt( ( int )( intptr_t )microphone );
}

// public func bool equals( Object object )
deClassMicrophone::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsMic, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObj ); // object
}
void deClassMicrophone::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deMicrophone *microphone = ( ( sMicNatDat* )p_GetNativeData( myself ) )->microphone;
	deClassMicrophone *clsMic = ( deClassMicrophone* )GetOwnerClass();
	dsValue *obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsMic ) ){
		rt->PushBool( false );
	}else{
		deMicrophone *otherMicrophone = ( ( sMicNatDat* )p_GetNativeData( obj ) )->microphone;
		rt->PushBool( microphone == otherMicrophone );
	}
}



// Class deClassMicrophone
////////////////////////////

// Constructor
////////////////

deClassMicrophone::deClassMicrophone( deEngine *gameEngine, deScriptingDragonScript *scrMgr ) :
dsClass( "Microphone", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ){
	if( ! gameEngine || ! scrMgr ) DSTHROW( dueInvalidParam );
	
	// prepare
	pGameEngine = gameEngine;
	pScrMgr = scrMgr;
	
	// store information into parser info
	GetParserInfo()->SetParent( DENS_SCENERY );
	GetParserInfo()->SetBase( "Object" );
	
	// do the rest
	p_SetNativeDataSize( sizeof( sMicNatDat ) );
}

deClassMicrophone::~deClassMicrophone(){
}



// Management
///////////////

void deClassMicrophone::CreateClassMembers( dsEngine *engine ){
	pClsVec = pScrMgr->GetClassVector();
	pClsDVec = pScrMgr->GetClassDVector();
	pClsSpk = pScrMgr->GetClassSpeaker();
	pClsMicrophoneType = engine->GetClass( "Dragengine.Scenery.MicrophoneType" );
	
	sInitData init;
	init.clsMic = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsObj = engine->GetClassObject();
	init.clsVec = pClsVec;
	init.clsQuat = pScrMgr->GetClassQuaternion();
	init.clsDVec = pClsDVec;
	init.clsSpk = pClsSpk;
	init.clsLayerMask = pScrMgr->GetClassLayerMask();
	init.clsMicrophoneType = pClsMicrophoneType;
	init.clsWorld = pScrMgr->GetClassWorld();
	
	// add functions
	AddFunction( new nfNew( init ) );
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetType( init ) );
	AddFunction( new nfSetType( init ) );
	AddFunction( new nfGetPosition( init ) );
	AddFunction( new nfSetPosition( init ) );
	AddFunction( new nfGetOrientation( init ) );
	AddFunction( new nfSetOrientation( init ) );
	AddFunction( new nfGetVelocity( init ) );
	AddFunction( new nfSetVelocity( init ) );
	
	AddFunction( new nfGetMuted( init ) );
	AddFunction( new nfSetMuted( init ) );
	AddFunction( new nfGetVolume( init ) );
	AddFunction( new nfSetVolume( init ) );
	
	AddFunction( new nfGetLayerMask( init ) );
	AddFunction( new nfSetLayerMask( init ) );
	
	AddFunction( new nfGetSpeakerGain( init ) );
	AddFunction( new nfSetSpeakerGain( init ) );
	AddFunction( new nfGetParentWorld( init ) );
	
	AddFunction( new nfAddSpeaker( init ) );
	AddFunction( new nfRemoveSpeaker( init ) );
	AddFunction( new nfRemoveAllSpeakers( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	// calculate member offsets
	CalcMemberOffsets();
}

deMicrophone *deClassMicrophone::GetMicrophone( dsRealObject *myself ) const{
	if( ! myself ){
		return NULL;
	}
	
	return ( ( sMicNatDat* )p_GetNativeData( myself->GetBuffer() ) )->microphone;
}

void deClassMicrophone::PushMicrophone( dsRunTime *rt, deMicrophone *microphone ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	if( ! microphone ){
		rt->PushObject( NULL, this );
		return;
	}
	
	rt->CreateObjectNakedOnStack( this );
	( ( sMicNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) )->microphone = microphone;
	microphone->AddReference();
}
