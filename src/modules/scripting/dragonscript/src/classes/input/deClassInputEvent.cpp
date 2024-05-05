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

#include "deClassInputEvent.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/input/deInputEvent.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native Structure
/////////////////////

struct sInpEvNatDat{
	deInputEvent *event;
};



// Native functions
/////////////////////


// Constructors, Destructor
/////////////////////////////

// public func destructor()
deClassInputEvent::nfDestructor::nfDestructor( const sInitData &init ) :
dsFunction( init.clsInputEvent, DSFUNC_DESTRUCTOR, DSFT_DESTRUCTOR,
DSTM_PUBLIC | DSTM_NATIVE, init.clsVoid ){
}
void deClassInputEvent::nfDestructor::RunFunction( dsRunTime *rt, dsValue *myself ){
	if( myself->GetRealObject()->GetRefCount() != 1 ){
		return; // protected against GC cleaning up leaking
	}
	
	sInpEvNatDat &nd = *( ( sInpEvNatDat* )p_GetNativeData( myself ) );
	if( ! nd.event ){
		return;
	}
	
	delete nd.event;
	nd.event = NULL;
}



// Management
///////////////

// public func InputEventType getType()
deClassInputEvent::nfGetType::nfGetType( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getType", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputEventType ){
}
void deClassInputEvent::nfGetType::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	
	rt->PushValue( ( ( deClassInputEvent* )GetOwnerClass() )->GetClassInputEventType()
		->GetVariable( event.GetType() )->GetStaticValue() );
}

// public func int getDevice()
deClassInputEvent::nfGetDevice::nfGetDevice( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getDevice", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputEvent::nfGetDevice::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushInt( event.GetDevice() );
}

// public func int getCode()
deClassInputEvent::nfGetCode::nfGetCode( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputEvent::nfGetCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushInt( event.GetCode() );
}

// public func int getState()
deClassInputEvent::nfGetState::nfGetState( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getState", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputEvent::nfGetState::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushInt( event.GetState() );
}

// public func int getKeyCode()
deClassInputEvent::nfGetKeyCode::nfGetKeyCode( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getKeyCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputEvent::nfGetKeyCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushInt( event.GetKeyCode() );
}

// public func int getKeyChar()
deClassInputEvent::nfGetKeyChar::nfGetKeyChar( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getKeyChar", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputEvent::nfGetKeyChar::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushInt( event.GetKeyChar() );
}

// public func int getX()
deClassInputEvent::nfGetX::nfGetX( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getX", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputEvent::nfGetX::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushInt( event.GetX() );
}

// public func int getY()
deClassInputEvent::nfGetY::nfGetY( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getY", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputEvent::nfGetY::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushInt( event.GetY() );
}

// public func float getValue()
deClassInputEvent::nfGetValue::nfGetValue( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsFloat ){
}
void deClassInputEvent::nfGetValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushFloat( event.GetValue() );
}

// public func int getTime()
deClassInputEvent::nfGetTime::nfGetTime( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getTime", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}
void deClassInputEvent::nfGetTime::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	rt->PushInt( event.GetTime().tv_sec * 1000 + event.GetTime().tv_usec / 1000 );  // temp hack
}

// public func InputEventSource getSource()
deClassInputEvent::nfGetSource::nfGetSource( const sInitData &init ) :
dsFunction( init.clsInputEvent, "getSource", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE, init.clsInputEventSource ){
}
void deClassInputEvent::nfGetSource::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	
	rt->PushValue( ( ( deClassInputEvent* )GetOwnerClass() )->GetClassInputEventSource()
		->GetVariable( event.GetSource() )->GetStaticValue() );
}



// public func int hashCode()
deClassInputEvent::nfHashCode::nfHashCode( const sInitData &init ) :
dsFunction( init.clsInputEvent, "hashCode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsInteger ){
}

void deClassInputEvent::nfHashCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent * const event = ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event;
	
	rt->PushInt( ( int )( intptr_t )event );
}

// public func bool equals( Object obj )
deClassInputEvent::nfEquals::nfEquals( const sInitData &init ) :
dsFunction( init.clsInputEvent, "equals", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE, init.clsBool ){
	p_AddParameter( init.clsObject ); // obj
}
void deClassInputEvent::nfEquals::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deInputEvent &event = *( ( ( sInpEvNatDat* )p_GetNativeData( myself ) )->event );
	deClassInputEvent * const clsInputEvent = ( deClassInputEvent* )GetOwnerClass();
	dsValue * const obj = rt->GetValue( 0 );
	
	if( ! p_IsObjOfType( obj, clsInputEvent ) ){
		rt->PushBool( false );
		
	}else{
		const deInputEvent &other = *( ( ( sInpEvNatDat* )p_GetNativeData( obj ) )->event );
		rt->PushBool( event == other );
	}
}



// Class deClassInputEvent
////////////////////////////

// Constructor, destructor
////////////////////////////

deClassInputEvent::deClassInputEvent( deScriptingDragonScript &ds ) :
dsClass( "InputEvent", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE | DSTM_FIXED ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( sizeof( sInpEvNatDat ) );
}

deClassInputEvent::~deClassInputEvent(){
}



// Management
///////////////

void deClassInputEvent::CreateClassMembers( dsEngine *engine ){
	pClsInputEventType = engine->GetClass( "Dragengine.InputEventType" );
	pClsInputEventSource = engine->GetClass( "Dragengine.InputEventSource" );
	
	sInitData init;
	init.clsInputEvent = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsString = engine->GetClassString();
	init.clsBool = engine->GetClassBool();
	init.clsObject = engine->GetClassObject();
	init.clsInputEventType = pClsInputEventType;
	init.clsInputEventSource = pClsInputEventSource;
	
	AddFunction( new nfDestructor( init ) );
	
	AddFunction( new nfGetType( init ) );
	AddFunction( new nfGetDevice( init ) );
	AddFunction( new nfGetCode( init ) );
	AddFunction( new nfGetState( init ) );
	AddFunction( new nfGetKeyCode( init ) );
	AddFunction( new nfGetKeyChar( init ) );
	AddFunction( new nfGetX( init ) );
	AddFunction( new nfGetY( init ) );
	AddFunction( new nfGetValue( init ) );
	AddFunction( new nfGetTime( init ) );
	AddFunction( new nfGetSource( init ) );
	
	AddFunction( new nfEquals( init ) );
	AddFunction( new nfHashCode( init ) );
	
	CalcMemberOffsets();
}

const deInputEvent &deClassInputEvent::GetInputEvent( dsRealObject *myself ) const{
	if( ! myself ){
		DSTHROW( dueInvalidParam );
	}
	
	return *( ( ( sInpEvNatDat* )p_GetNativeData( myself->GetBuffer() ) )->event );
}

void deClassInputEvent::PushInputEvent( dsRunTime *rt, const deInputEvent &event ){
	if( ! rt ){
		DSTHROW( dueInvalidParam );
	}
	
	rt->CreateObjectNakedOnStack( this );
	sInpEvNatDat &nd = *( ( sInpEvNatDat* )p_GetNativeData( rt->GetValue( 0 )->GetRealObject()->GetBuffer() ) );
	nd.event = NULL;
	
	try{
		nd.event = new deInputEvent( event );
		
	}catch( ... ){
		rt->RemoveValues( 1 ); // remove pushed object
		throw;
	}
}
