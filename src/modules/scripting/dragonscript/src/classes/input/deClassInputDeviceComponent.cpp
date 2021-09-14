/* 
 * Drag[en]gine DragonScript Script Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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
#include "deClassInputDeviceComponent.h"
#include "../graphics/deClassImage.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsInputDevice.h"

#include <dragengine/deEngine.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDeviceComponent.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sIDComponentNatDat{
	dedsInputDevice *device;
	int componentIndex;
};



// Native functions
/////////////////////

// public func destructor()
deClassInputDeviceComponent::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsIDComponent, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassInputDeviceComponent::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sIDComponentNatDat &nd = *( ( sIDComponentNatDat* )p_GetNativeData( myself ) );
	if( nd.device ){
		nd.device->FreeReference();
		nd.device = NULL;
	}
	nd.componentIndex = -1;
}



// Management
///////////////

// public func InputDevice getInputDevice()
deClassInputDeviceComponent::nfGetInputDevice::nfGetInputDevice( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getInputDevice", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDevice ){
}
void deClassInputDeviceComponent::nfGetInputDevice::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceComponent* )GetOwnerClass() )->GetDS();
	
	ds.GetClassInputDevice()->PushInputDevice( rt, nd.device );
}

// public func int getComponentIndex()
deClassInputDeviceComponent::nfGetComponentIndex::nfGetComponentIndex( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getComponentIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDeviceComponent::nfGetComponentIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	
	rt->PushInt( nd.componentIndex );
}



// public func String getID()
deClassInputDeviceComponent::nfGetID::nfGetID( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceComponent::nfGetID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &component = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	
	rt->PushString( component.GetID() );
}

// public func String getName()
deClassInputDeviceComponent::nfGetName::nfGetName( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceComponent::nfGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &component = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	
	rt->PushString( component.GetName() );
}

// public func InputDeviceComponentType getType()
deClassInputDeviceComponent::nfGetType::nfGetType( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDeviceComponentType ){
}
void deClassInputDeviceComponent::nfGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &component = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	
	rt->PushValue( ( ( deClassInputDeviceComponent* )GetOwnerClass() )->GetClassInputDeviceComponentType()
		->GetVariable( component.GetType() )->GetStaticValue() );
}

// public func Image getDisplayImage()
deClassInputDeviceComponent::nfGetDisplayImage::nfGetDisplayImage( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getDisplayIconAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
}
void deClassInputDeviceComponent::nfGetDisplayImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &component = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceComponent* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, component.GetDisplayImage() );
}

// public func int getDisplayIconCount()
deClassInputDeviceComponent::nfGetDisplayIconCount::nfGetDisplayIconCount( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getDisplayIconCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDeviceComponent::nfGetDisplayIconCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &component = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	rt->PushInt( component.GetDisplayIconCount() );
}

// public func Image getDisplayIconAt( int index )
deClassInputDeviceComponent::nfGetDisplayIconAt::nfGetDisplayIconAt( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getDisplayIconAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDeviceComponent::nfGetDisplayIconAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &component = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceComponent* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, component.GetDisplayIconAt( rt->GetValue( 0 )->GetInt() ) );
}

// public func Image getLargestDisplayIconX( int maxWidth )
deClassInputDeviceComponent::nfGetLargestDisplayIconX::nfGetLargestDisplayIconX( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getLargestDisplayIconX", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // maxWidth
}
void deClassInputDeviceComponent::nfGetLargestDisplayIconX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &component = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceComponent* )GetOwnerClass() )->GetDS();
	const int count = component.GetDisplayIconCount();
	const int maxWidth = rt->GetValue( 0 )->GetInt();
	deImage *bestIcon = NULL;
	int bestWidth = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		deImage * const icon = component.GetDisplayIconAt( i );
		if( icon->GetWidth() <= maxWidth && ( ! bestIcon || icon->GetWidth() >= bestWidth ) ){
			bestIcon = icon;
			bestWidth = icon->GetWidth();
		}
	}
	
	ds.GetClassImage()->PushImage( rt, bestIcon );
}

// public func Image getLargestDisplayIconY( int maxHeight )
deClassInputDeviceComponent::nfGetLargestDisplayIconY::nfGetLargestDisplayIconY( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getLargestDisplayIconY", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // maxHeight
}
void deClassInputDeviceComponent::nfGetLargestDisplayIconY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &component = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceComponent* )GetOwnerClass() )->GetDS();
	const int count = component.GetDisplayIconCount();
	const int maxHeight = rt->GetValue( 0 )->GetInt();
	deImage *bestIcon = NULL;
	int bestHeight = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		deImage * const icon = component.GetDisplayIconAt( i );
		if( icon->GetHeight() <= maxHeight && ( ! bestIcon || icon->GetHeight() >= bestHeight ) ){
			bestIcon = icon;
			bestHeight = icon->GetHeight();
		}
	}
	
	ds.GetClassImage()->PushImage( rt, bestIcon );
}

// public func String getDisplayText()
deClassInputDeviceComponent::nfGetDisplayText::nfGetDisplayText( const sInitData &init ) :
dsFunction( init.clsIDComponent, "getDisplayText", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceComponent::nfGetDisplayText::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceComponent &axis = nd.device->GetDevice()->GetComponentAt( nd.componentIndex );
	
	rt->PushString( axis.GetDisplayText() );
}



// public func bool equals( Object obj )
deClassInputDeviceComponent::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsIDComponent, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassInputDeviceComponent::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassInputDeviceComponent * const clsIDComponent = ( deClassInputDeviceComponent* )GetOwnerClass();
	const sIDComponentNatDat &nd = *( ( const sIDComponentNatDat* )p_GetNativeData( myself ) );
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsIDComponent ) ){
		rt->PushBool( false );
		
	}else{
		const sIDComponentNatDat &other = *( ( const sIDComponentNatDat* )p_GetNativeData( obj ) );
		rt->PushBool( nd.device == other.device && nd.componentIndex == other.componentIndex );
	}
}



// Class deClassInputDeviceComponent
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassInputDeviceComponent::deClassInputDeviceComponent( deScriptingDragonScript &ds ) :
dsClass( "InputDeviceComponent", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sIDComponentNatDat ) );
}

deClassInputDeviceComponent::~deClassInputDeviceComponent(){
}



// Management
///////////////

void deClassInputDeviceComponent::CreateClassMembers( dsEngine *engine ){
	pClsInputDeviceComponentType = engine->GetClass( "Dragengine.InputDeviceComponentType" );
	
	sInitData init;
	
	init.clsIDComponent = this;
	
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	
	init.clsInputDevice = pDS.GetClassInputDevice();
	init.clsInputDeviceComponentType = pClsInputDeviceComponentType;
	init.clsImage = pDS.GetClassImage();
	
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetInputDevice( init ) );
	AddFunction( new nfGetComponentIndex( init ) );
	
	AddFunction( new nfGetID( init ) );
	AddFunction( new nfGetName( init ) );
	AddFunction( new nfGetType( init ) );
	AddFunction( new nfGetDisplayImage( init ) );
	AddFunction( new nfGetDisplayIconCount( init ) );
	AddFunction( new nfGetDisplayIconAt( init ) );
	AddFunction( new nfGetLargestDisplayIconX( init ) );
	AddFunction( new nfGetLargestDisplayIconY( init ) );
	AddFunction( new nfGetDisplayText( init ) );
	
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

void deClassInputDeviceComponent::PushComponent( dsRunTime *rt, dedsInputDevice *device, int index ){
	if( ! rt || ! device || index < 0 || index >= device->GetDevice()->GetComponentCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sIDComponentNatDat &nd = *( ( sIDComponentNatDat* )p_GetNativeData(
		rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.device = device;
	device->AddReference();
	nd.componentIndex = index;
}
