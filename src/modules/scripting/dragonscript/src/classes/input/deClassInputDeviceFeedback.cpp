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
#include <stdint.h>
#include <string.h>

#include "deClassInputDevice.h"
#include "deClassInputDeviceFeedback.h"
#include "deClassInputDeviceComponent.h"
#include "../graphics/deClassImage.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsInputDevice.h"

#include <dragengine/deEngine.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/input/deInputDeviceFeedback.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sIDFeedbackNatDat{
	dedsInputDevice *device;
	int feedbackIndex;
};



// Native functions
/////////////////////

// public func destructor()
deClassInputDeviceFeedback::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsIDFeedback, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassInputDeviceFeedback::nfDestructor::RunFunction( dsRunTime*, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sIDFeedbackNatDat &nd = *( ( sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	if( nd.device ){
		nd.device->FreeReference();
		nd.device = NULL;
	}
	nd.feedbackIndex = -1;
}



// Management
///////////////

// public func InputDevice getInputDevice()
deClassInputDeviceFeedback::nfGetInputDevice::nfGetInputDevice( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getInputDevice", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDevice ){
}
void deClassInputDeviceFeedback::nfGetInputDevice::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceFeedback* )GetOwnerClass() )->GetDS();
	
	ds.GetClassInputDevice()->PushInputDevice( rt, nd.device );
}

// public func int getFeedbackIndex()
deClassInputDeviceFeedback::nfGetFeedbackIndex::nfGetFeedbackIndex( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getFeedbackIndex", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDeviceFeedback::nfGetFeedbackIndex::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	
	rt->PushInt( nd.feedbackIndex );
}



// public func String getID()
deClassInputDeviceFeedback::nfGetID::nfGetID( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceFeedback::nfGetID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	
	rt->PushString( feedback.GetID() );
}

// public func String getName()
deClassInputDeviceFeedback::nfGetName::nfGetName( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getName", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceFeedback::nfGetName::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	
	rt->PushString( feedback.GetName() );
}

// public func InputDeviceFeedbackType getType()
deClassInputDeviceFeedback::nfGetType::nfGetType( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputDeviceFeedbackType ){
}
void deClassInputDeviceFeedback::nfGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	
	rt->PushValue( ( ( deClassInputDeviceFeedback* )GetOwnerClass() )->GetClassInputDeviceFeedbackType()
		->GetVariable( feedback.GetType() )->GetStaticValue() );
}

// public func String getComponent()
deClassInputDeviceFeedback::nfGetComponent::nfGetComponent( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getComponent", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceFeedback::nfGetComponent::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	
	rt->PushString( feedback.GetComponent() );
}

// public func Image getDisplayImage()
deClassInputDeviceFeedback::nfGetDisplayImage::nfGetDisplayImage( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getDisplayImage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
}
void deClassInputDeviceFeedback::nfGetDisplayImage::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceFeedback* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, feedback.GetDisplayImage() );
}

// public func int getDisplayIconCount()
deClassInputDeviceFeedback::nfGetDisplayIconCount::nfGetDisplayIconCount( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getDisplayIconCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputDeviceFeedback::nfGetDisplayIconCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	rt->PushInt( feedback.GetDisplayIconCount() );
}

// public func Image getDisplayIconAt( int index )
deClassInputDeviceFeedback::nfGetDisplayIconAt::nfGetDisplayIconAt( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getDisplayIconAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputDeviceFeedback::nfGetDisplayIconAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceFeedback* )GetOwnerClass() )->GetDS();
	
	ds.GetClassImage()->PushImage( rt, feedback.GetDisplayIconAt( rt->GetValue( 0 )->GetInt() ) );
}

// public func Image getLargestDisplayIconX( int maxWidth )
deClassInputDeviceFeedback::nfGetLargestDisplayIconX::nfGetLargestDisplayIconX( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getLargestDisplayIconX", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // maxWidth
}
void deClassInputDeviceFeedback::nfGetLargestDisplayIconX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceFeedback* )GetOwnerClass() )->GetDS();
	const int count = feedback.GetDisplayIconCount();
	const int maxWidth = rt->GetValue( 0 )->GetInt();
	deImage *bestIcon = NULL;
	int bestWidth = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		deImage * const icon = feedback.GetDisplayIconAt( i );
		if( icon->GetWidth() <= maxWidth && ( ! bestIcon || icon->GetWidth() >= bestWidth ) ){
			bestIcon = icon;
			bestWidth = icon->GetWidth();
		}
	}
	
	ds.GetClassImage()->PushImage( rt, bestIcon );
}

// public func Image getLargestDisplayIconY( int maxHeight )
deClassInputDeviceFeedback::nfGetLargestDisplayIconY::nfGetLargestDisplayIconY( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getLargestDisplayIconY", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsImage ){
	p_AddParameter( init.clsInteger ); // maxHeight
}
void deClassInputDeviceFeedback::nfGetLargestDisplayIconY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &feedback = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceFeedback* )GetOwnerClass() )->GetDS();
	const int count = feedback.GetDisplayIconCount();
	const int maxHeight = rt->GetValue( 0 )->GetInt();
	deImage *bestIcon = NULL;
	int bestHeight = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		deImage * const icon = feedback.GetDisplayIconAt( i );
		if( icon->GetHeight() <= maxHeight && ( ! bestIcon || icon->GetHeight() >= bestHeight ) ){
			bestIcon = icon;
			bestHeight = icon->GetHeight();
		}
	}
	
	ds.GetClassImage()->PushImage( rt, bestIcon );
}

// public func String getDisplayText()
deClassInputDeviceFeedback::nfGetDisplayText::nfGetDisplayText( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getDisplayText", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsString ){
}
void deClassInputDeviceFeedback::nfGetDisplayText::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	const deInputDeviceFeedback &axis = nd.device->GetDevice()->GetFeedbackAt( nd.feedbackIndex );
	
	rt->PushString( axis.GetDisplayText() );
}



// public func float getValue()
deClassInputDeviceFeedback::nfGetValue::nfGetValue( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "getValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassInputDeviceFeedback::nfGetValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceFeedback* )GetOwnerClass() )->GetDS();
	
	switch( nd.device->GetDeviceSource() ){
	case deInputEvent::esInput:{
		deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
		
		const int deviceIndex = module.IndexOfDeviceWithID( nd.device->GetDevice()->GetID() );
		if( deviceIndex != -1 ){
			rt->PushFloat( module.GetFeedbackValue( deviceIndex, nd.feedbackIndex ) );
			
		}else{
			rt->PushFloat( 0.0f );
		}
		}break;
		
	case deInputEvent::esVR:{
		deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
		
		const int deviceIndex = module.IndexOfDeviceWithID( nd.device->GetDevice()->GetID() );
		if( deviceIndex != -1 ){
			rt->PushFloat( module.GetFeedbackValue( deviceIndex, nd.feedbackIndex ) );
			
		}else{
			rt->PushFloat( 0.0f );
		}
		}break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "input source" );
	}
}

// public func void setValue( float value )
deClassInputDeviceFeedback::nfSetValue::nfSetValue( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "setValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // value
}
void deClassInputDeviceFeedback::nfSetValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	deScriptingDragonScript &ds = ( ( deClassInputDeviceFeedback* )GetOwnerClass() )->GetDS();
	
	switch( nd.device->GetDeviceSource() ){
	case deInputEvent::esInput:{
		deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
		
		const int deviceIndex = module.IndexOfDeviceWithID( nd.device->GetDevice()->GetID() );
		if( deviceIndex != -1 ){
			module.SetFeedbackValue( deviceIndex, nd.feedbackIndex, rt->GetValue( 0 )->GetFloat() );
		}
		}break;
		
	case deInputEvent::esVR:{
		deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
		
		const int deviceIndex = module.IndexOfDeviceWithID( nd.device->GetDevice()->GetID() );
		if( deviceIndex != -1 ){
			module.SetFeedbackValue( deviceIndex, nd.feedbackIndex, rt->GetValue( 0 )->GetFloat() );
		}
		}break;
		
	default:
		DETHROW_INFO( deeInvalidParam, "input source" );
	}
}



// public func bool equals( Object obj )
deClassInputDeviceFeedback::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsIDFeedback, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassInputDeviceFeedback::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassInputDeviceFeedback * const clsIDFeedback = ( deClassInputDeviceFeedback* )GetOwnerClass();
	const sIDFeedbackNatDat &nd = *( ( const sIDFeedbackNatDat* )p_GetNativeData( myself ) );
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsIDFeedback ) ){
		rt->PushBool( false );
		
	}else{
		const sIDFeedbackNatDat &other = *( ( const sIDFeedbackNatDat* )p_GetNativeData( obj ) );
		rt->PushBool( nd.device == other.device && nd.feedbackIndex == other.feedbackIndex );
	}
}



// Class deClassInputDeviceFeedback
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deClassInputDeviceFeedback::deClassInputDeviceFeedback( deScriptingDragonScript &ds ) :
dsClass( "InputDeviceFeedback", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sIDFeedbackNatDat ) );
}

deClassInputDeviceFeedback::~deClassInputDeviceFeedback(){
}



// Management
///////////////

void deClassInputDeviceFeedback::CreateClassMembers( dsEngine *engine ){
	pClsInputDeviceFeedbackType = engine->GetClass( "Dragengine.InputDeviceFeedbackType" );
	
	sInitData init;
	init.clsIDFeedback = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsInputDevice = pDS.GetClassInputDevice();
	init.clsInputDeviceFeedbackType = pClsInputDeviceFeedbackType;
	init.clsImage = pDS.GetClassImage();
	
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetInputDevice( init ) );
	AddFunction( new nfGetFeedbackIndex( init ) );
	
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
	
	AddFunction( new nfGetValue( init ) );
	AddFunction( new nfSetValue( init ) );
	
	AddFunction( new nfEquals( init ) );
	
	CalcMemberOffsets();
}

void deClassInputDeviceFeedback::PushFeedback( dsRunTime *rt, dedsInputDevice *device, int index ){
	if( ! rt || ! device || index < 0 || index >= device->GetDevice()->GetFeedbackCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sIDFeedbackNatDat &nd = *( ( sIDFeedbackNatDat* )p_GetNativeData(
		rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.device = device;
	device->AddReference();
	nd.feedbackIndex = index;
}
