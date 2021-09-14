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

#include "deClassInputDevice.h"
#include "deClassInputDeviceButton.h"
#include "../graphics/deClassImage.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsInputDevice.h"

#include <dragengine/deEngine.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDeviceButton.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sIDButtonNatDat{
	dedsInputDevice *device;
	int buttonIndex;
};



// Native functions
/////////////////////

// public func destructor()
deClassInputDeviceButton::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsIDButton, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassInputDeviceButton::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sIDButtonNatDat &nd = *( ( sIDButtonNatDat* )p_GetNativeData( myself ) );
	if( nd.device ){
		nd.device->FreeReference();
		nd.device = NULL;
	}
	nd.buttonIndex = -1;
}



// Management
///////////////

// public func InputDevice getInputDevice()
deClassInputDeviceButton::nfGetInputDevice::nfGetInputDevice( const sInitData &init ) :
dsFunction( init.clsIDButton, "getInputDevice", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDevice ){
}
void deClassInputDeviceButton::nfGetInputDevice::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceButton* )GetOwnerClass() )->GetDS();
	
	ds.GetClassInputDevice()->PushInputDevice( rt, nd.device );
}

// public func int getButtonIndex()
deClassInputDeviceButton::nfGetButtonIndex::nfGetButtonIndex( const sInitData &init ) :
dsFunction( init.clsIDButton, "getButtonIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDeviceButton::nfGetButtonIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	
	rt->PushInt( nd.buttonIndex );
}



// public func String getID()
deClassInputDeviceButton::nfGetID::nfGetID( const sInitData &init ) :
dsFunction( init.clsIDButton, "getID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceButton::nfGetID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	
	rt->PushString( button.GetID() );
}

// public func String getName()
deClassInputDeviceButton::nfGetName::nfGetName( const sInitData &init ) :
dsFunction( init.clsIDButton, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceButton::nfGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	
	rt->PushString( button.GetName() );
}

// public func InputDeviceButtonType getType()
deClassInputDeviceButton::nfGetType::nfGetType( const sInitData &init ) :
dsFunction( init.clsIDButton, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDeviceButtonType ){
}
void deClassInputDeviceButton::nfGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	
	rt->PushValue( ( ( deClassInputDeviceButton* )GetOwnerClass() )->GetClassInputDeviceButtonType()
		->GetVariable( button.GetType() )->GetStaticValue() );
}

// public func String getComponent()
deClassInputDeviceButton::nfGetComponent::nfGetComponent( const sInitData &init ) :
dsFunction( init.clsIDButton, "getComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceButton::nfGetComponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	
	rt->PushString( button.GetComponent() );
}

// public func Image getDisplayImage()
deClassInputDeviceButton::nfGetDisplayImage::nfGetDisplayImage( const sInitData &init ) :
dsFunction( init.clsIDButton, "getDisplayIconAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
}
void deClassInputDeviceButton::nfGetDisplayImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceButton* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, button.GetDisplayImage() );
}

// public func int getDisplayIconCount()
deClassInputDeviceButton::nfGetDisplayIconCount::nfGetDisplayIconCount( const sInitData &init ) :
dsFunction( init.clsIDButton, "getDisplayIconCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDeviceButton::nfGetDisplayIconCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	rt->PushInt( button.GetDisplayIconCount() );
}

// public func Image getDisplayIconAt( int index )
deClassInputDeviceButton::nfGetDisplayIconAt::nfGetDisplayIconAt( const sInitData &init ) :
dsFunction( init.clsIDButton, "getDisplayIconAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDeviceButton::nfGetDisplayIconAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceButton* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, button.GetDisplayIconAt( rt->GetValue( 0 )->GetInt() ) );
}

// public func Image getLargestDisplayIconX( int maxWidth )
deClassInputDeviceButton::nfGetLargestDisplayIconX::nfGetLargestDisplayIconX( const sInitData &init ) :
dsFunction( init.clsIDButton, "getLargestDisplayIconX", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // maxWidth
}
void deClassInputDeviceButton::nfGetLargestDisplayIconX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceButton* )GetOwnerClass() )->GetDS();
	const int count = button.GetDisplayIconCount();
	const int maxWidth = rt->GetValue( 0 )->GetInt();
	deImage *bestIcon = NULL;
	int bestWidth = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		deImage * const icon = button.GetDisplayIconAt( i );
		if( icon->GetWidth() <= maxWidth && ( ! bestIcon || icon->GetWidth() >= bestWidth ) ){
			bestIcon = icon;
			bestWidth = icon->GetWidth();
		}
	}
	
	ds.GetClassImage()->PushImage( rt, bestIcon );
}

// public func Image getLargestDisplayIconY( int maxHeight )
deClassInputDeviceButton::nfGetLargestDisplayIconY::nfGetLargestDisplayIconY( const sInitData &init ) :
dsFunction( init.clsIDButton, "getLargestDisplayIconY", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // maxHeight
}
void deClassInputDeviceButton::nfGetLargestDisplayIconY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &button = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceButton* )GetOwnerClass() )->GetDS();
	const int count = button.GetDisplayIconCount();
	const int maxHeight = rt->GetValue( 0 )->GetInt();
	deImage *bestIcon = NULL;
	int bestHeight = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		deImage * const icon = button.GetDisplayIconAt( i );
		if( icon->GetHeight() <= maxHeight && ( ! bestIcon || icon->GetHeight() >= bestHeight ) ){
			bestIcon = icon;
			bestHeight = icon->GetHeight();
		}
	}
	
	ds.GetClassImage()->PushImage( rt, bestIcon );
}

// public func String getDisplayText()
deClassInputDeviceButton::nfGetDisplayText::nfGetDisplayText( const sInitData &init ) :
dsFunction( init.clsIDButton, "getDisplayText", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceButton::nfGetDisplayText::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceButton &axis = nd.device->GetDevice()->GetButtonAt( nd.buttonIndex );
	
	rt->PushString( axis.GetDisplayText() );
}



// public func bool isPressed()
deClassInputDeviceButton::nfIsPressed::nfIsPressed( const sInitData &init ) :
dsFunction( init.clsIDButton, "isPressed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassInputDeviceButton::nfIsPressed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceButton* )GetOwnerClass() )->GetDS();
	
	switch( nd.device->GetDeviceSource() ){
	case deInputEvent::esInput:{
		deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
		const int deviceIndex = module.IndexOfDeviceWithID( nd.device->GetDevice()->GetID() );
		rt->PushBool( deviceIndex != -1 && module.GetButtonPressed( deviceIndex, nd.buttonIndex ) );
		}break;
		
	case deInputEvent::esVR:{
		deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
		const int deviceIndex = module.IndexOfDeviceWithID( nd.device->GetDevice()->GetID() );
		rt->PushBool( deviceIndex != -1 && module.GetButtonPressed( deviceIndex, nd.buttonIndex ) );
		}break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "input source" );
	}
}

// public func bool isTouched()
deClassInputDeviceButton::nfIsTouched::nfIsTouched( const sInitData &init ) :
dsFunction( init.clsIDButton, "isTouched", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
}
void deClassInputDeviceButton::nfIsTouched::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceButton* )GetOwnerClass() )->GetDS();
	
	switch( nd.device->GetDeviceSource() ){
	case deInputEvent::esInput:{
		deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
		const int deviceIndex = module.IndexOfDeviceWithID( nd.device->GetDevice()->GetID() );
		rt->PushBool( deviceIndex != -1 && module.GetButtonTouched( deviceIndex, nd.buttonIndex ) );
		}break;
		
	case deInputEvent::esVR:{
		deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
		const int deviceIndex = module.IndexOfDeviceWithID( nd.device->GetDevice()->GetID() );
		rt->PushBool( deviceIndex != -1 && module.GetButtonTouched( deviceIndex, nd.buttonIndex ) );
		}break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "input source" );
	}
}



// public func bool equals( Object obj )
deClassInputDeviceButton::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsIDButton, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassInputDeviceButton::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassInputDeviceButton * const clsIDButton = ( deClassInputDeviceButton* )GetOwnerClass();
	const sIDButtonNatDat &nd = *( ( const sIDButtonNatDat* )p_GetNativeData( myself ) );
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsIDButton ) ){
		rt->PushBool( false );
		
	}else{
		const sIDButtonNatDat &other = *( ( const sIDButtonNatDat* )p_GetNativeData( obj ) );
		rt->PushBool( nd.device == other.device && nd.buttonIndex == other.buttonIndex );
	}
}



// Class deClassInputDeviceButton
///////////////////////////////

// Constructor, destructor
////////////////////////////

deClassInputDeviceButton::deClassInputDeviceButton( deScriptingDragonScript &ds ) :
dsClass( "InputDeviceButton", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sIDButtonNatDat ) );
}

deClassInputDeviceButton::~deClassInputDeviceButton(){
}



// Management
///////////////

void deClassInputDeviceButton::CreateClassMembers( dsEngine *engine ){
	pClsInputDeviceButtonType = engine->GetClass( "Dragengine.InputDeviceButtonType" );
	
	sInitData init;
	
	init.clsIDButton = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	init.clsInputDevice = pDS.GetClassInputDevice();
	init.clsInputDeviceButtonType = pClsInputDeviceButtonType;
	init.clsImage = pDS.GetClassImage();
	
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetInputDevice( init ) );
	AddFunction( new nfGetButtonIndex( init ) );
	
	AddFunction( new nfGetID( init ) );
	AddFunction( new nfGetName( init ) );
	AddFunction( new nfGetType( init ) );
	AddFunction( new nfGetComponent( init ) );
	AddFunction( new nfGetDisplayImage( init ) );
	AddFunction( new nfGetDisplayIconCount( init ) );
	AddFunction( new nfGetDisplayIconAt( init ) );
	AddFunction( new nfGetLargestDisplayIconX( init ) );
	AddFunction( new nfGetLargestDisplayIconY( init ) );
	AddFunction( new nfGetDisplayText( init ) );
	
	AddFunction( new nfIsPressed( init ) );
	AddFunction( new nfIsTouched( init ) );
	
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

void deClassInputDeviceButton::PushButton( dsRunTime *rt, dedsInputDevice *device, int index ){
	if( ! rt || ! device || index < 0 || index >= device->GetDevice()->GetButtonCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sIDButtonNatDat &nd = *( ( sIDButtonNatDat* )p_GetNativeData(
		rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.device = device;
	device->AddReference();
	nd.buttonIndex = index;
}
