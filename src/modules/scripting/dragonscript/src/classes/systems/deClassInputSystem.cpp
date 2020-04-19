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

#include <libdscript/exceptions.h>

#include "deClassInputSystem.h"
#include "../deClassModuleParameter.h"
#include "../input/deClassInputDevice.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/input/deInputDevice.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/modules/input/deBaseInputModule.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>



// Native functions
/////////////////////

// public static func bool getCaptureInputDevices()
deClassInputSystem::nfGetCaptureInputDevices::nfGetCaptureInputDevices( const sInitData &init ) :
dsFunction( init.clsInpSys, "getCaptureInputDevices", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
}
void deClassInputSystem::nfGetCaptureInputDevices::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	const deInputSystem &inpsys = *ds.GetGameEngine()->GetInputSystem();
	rt->PushBool( inpsys.GetCaptureInputDevices() );
}

// public static func void setCaptureInputDevices( bool captureInputDevices )
deClassInputSystem::nfSetCaptureInputDevices::nfSetCaptureInputDevices( const sInitData &init ) :
dsFunction( init.clsInpSys, "setCaptureInputDevices", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsBool ); // captureInputDevices
}
void deClassInputSystem::nfSetCaptureInputDevices::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deInputSystem &inpsys = *ds.GetGameEngine()->GetInputSystem();
	
	inpsys.SetCaptureInputDevices( rt->GetValue( 0 )->GetBool() );
}



// public static func int getDeviceCount()
deClassInputSystem::nfGetDeviceCount::nfGetDeviceCount( const sInitData &init ) :
dsFunction( init.clsInpSys, "getDeviceCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassInputSystem::nfGetDeviceCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassInputSystem &clsInpSys = *( ( deClassInputSystem* )GetOwnerClass() );
	rt->PushInt( clsInpSys.GetCachedDeviceCount() );
}

// public static func InputDevice getDeviceAt( int index )
deClassInputSystem::nfGetDeviceAt::nfGetDeviceAt( const sInitData &init ) :
dsFunction( init.clsInpSys, "getDeviceAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInputDevice ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputSystem::nfGetDeviceAt::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassInputSystem &clsInpSys = *( ( deClassInputSystem* )GetOwnerClass() );
	const int index = rt->GetValue( 0 )->GetInt();
	
	clsInpSys.GetDS().GetClassInputDevice()->PushInputDevice( rt,
		clsInpSys.GetCachedDeviceAt( index ), index );
}

// public static func int indexOfDeviceWithID( String id )
deClassInputSystem::nfIndexOfDeviceWithID::nfIndexOfDeviceWithID( const sInitData &init ) :
dsFunction( init.clsInpSys, "indexOfDeviceWithID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsString ); // id
}
void deClassInputSystem::nfIndexOfDeviceWithID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const char * const id = rt->GetValue( 0 )->GetString();
	rt->PushInt( module.IndexOfDeviceWithID( id ) );
}

// public static func int indexOfButtonWithID( int device, String id )
deClassInputSystem::nfIndexOfButtonWithID::nfIndexOfButtonWithID( const sInitData &init ) :
dsFunction( init.clsInpSys, "indexOfButtonWithID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsString ); // id
}
void deClassInputSystem::nfIndexOfButtonWithID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const char * const id = rt->GetValue( 1 )->GetString();
	rt->PushInt( module.IndexOfButtonWithID( device, id ) );
}

// public static func int indexOfAxisWithID( int device, String id )
deClassInputSystem::nfIndexOfAxisWithID::nfIndexOfAxisWithID( const sInitData &init ) :
dsFunction( init.clsInpSys, "indexOfAxisWithID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsString ); // id
}
void deClassInputSystem::nfIndexOfAxisWithID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const char * const id = rt->GetValue( 1 )->GetString();
	rt->PushInt( module.IndexOfAxisWithID( device, id ) );
}

// public static func int indexOfFeedbackWithID( int device, String id )
deClassInputSystem::nfIndexOfFeedbackWithID::nfIndexOfFeedbackWithID( const sInitData &init ) :
dsFunction( init.clsInpSys, "indexOfFeedbackWithID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsString ); // id
}
void deClassInputSystem::nfIndexOfFeedbackWithID::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const char * const id = rt->GetValue( 1 )->GetString();
	rt->PushInt( module.IndexOfFeedbackWithID( device, id ) );
}

// public static func bool getButtonPressed( int device, int button )
deClassInputSystem::nfGetButtonPressed::nfGetButtonPressed( const sInitData &init ) :
dsFunction( init.clsInpSys, "getButtonPressed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // button
}
void deClassInputSystem::nfGetButtonPressed::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int button  = rt->GetValue( 1 )->GetInt();
	rt->PushBool( module.GetButtonPressed( device, button ) );
}

// public static func int getAxisValue( int device, int axis )
deClassInputSystem::nfGetAxisValue::nfGetAxisValue( const sInitData &init ) :
dsFunction( init.clsInpSys, "getAxisValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // axis
}
void deClassInputSystem::nfGetAxisValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int axis  = rt->GetValue( 1 )->GetInt();
	rt->PushInt( module.GetAxisValue( device, axis ) );
}

// public static func int getFeedbackValue( int device, int feedback )
deClassInputSystem::nfGetFeedbackValue::nfGetFeedbackValue( const sInitData &init ) :
dsFunction( init.clsInpSys, "getFeedbackValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // feedback
}
void deClassInputSystem::nfGetFeedbackValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int feedback  = rt->GetValue( 1 )->GetInt();
	rt->PushInt( module.GetFeedbackValue( device, feedback ) );
}

// public static func void setFeedbackValue( int device, int feedback, int value )
deClassInputSystem::nfSetFeedbackValue::nfSetFeedbackValue( const sInitData &init ) :
dsFunction( init.clsInpSys, "setFeedbackValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // feedback
	p_AddParameter( init.clsInteger ); // value
}
void deClassInputSystem::nfSetFeedbackValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int feedback  = rt->GetValue( 1 )->GetInt();
	const int value  = rt->GetValue( 2 )->GetInt();
	module.SetFeedbackValue( device, feedback, value );
}

// public static func int buttonMatchingKeyCode( int device, int keyCode )
deClassInputSystem::nfButtonMatchingKeyCode::nfButtonMatchingKeyCode( const sInitData &init ) :
dsFunction( init.clsInpSys, "buttonMatchingKeyCode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // keyCode
}
void deClassInputSystem::nfButtonMatchingKeyCode::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const deInputEvent::eKeyCodes keyCode =
		( deInputEvent::eKeyCodes )rt->GetValue( 1 )->GetInt();
	rt->PushInt( module.ButtonMatchingKeyCode( device, keyCode ) );
}

// public static func int buttonMatchingKeyChar( int device, int keyCode )
deClassInputSystem::nfButtonMatchingKeyChar::nfButtonMatchingKeyChar( const sInitData &init ) :
dsFunction( init.clsInpSys, "buttonMatchingKeyChar", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // keyChar
}
void deClassInputSystem::nfButtonMatchingKeyChar::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deBaseInputModule &module = *ds.GetGameEngine()->GetInputSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int keyChar = rt->GetValue( 1 )->GetInt();
	rt->PushInt( module.ButtonMatchingKeyChar( device, keyChar ) );
}



// public static func int getParameterCount()
deClassInputSystem::nfGetParameterCount::nfGetParameterCount( const sInitData &init ) :
dsFunction( init.clsInpSys, "getParameterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassInputSystem::nfGetParameterCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	const deInputSystem &inpsys = *ds.GetGameEngine()->GetInputSystem();
	rt->PushInt( inpsys.GetActiveModule()->GetParameterCount() );
}

// public static func ModuleParameter getParameterInfo(int index)
deClassInputSystem::nfGetParameterInfo::nfGetParameterInfo( const sInitData &init ) :
dsFunction( init.clsInpSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassInputSystem::nfGetParameterInfo::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	const deInputSystem &inpsys = *ds.GetGameEngine()->GetInputSystem();
	deBaseInputModule * const module = inpsys.GetActiveModule();
	
	const int index = rt->GetValue( 0 )->GetInt();
	if( index < 0 || index >= module->GetParameterCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	ds.GetClassModuleParameter()->PushParameter( rt, module, index );
}

// public static func ModuleParameter getParameterInfo( String name )
deClassInputSystem::nfGetParameterInfo2::nfGetParameterInfo2( const sInitData &init ) :
dsFunction( init.clsInpSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar ){
	p_AddParameter( init.clsString ); // name
}
void deClassInputSystem::nfGetParameterInfo2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	deInputSystem &inpSys = *ds.GetGameEngine()->GetInputSystem();
	deBaseInputModule * const module = inpSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter( rt, module,
		module->IndexOfParameterNamed( rt->GetValue( 0 )->GetString() ) );
}

// public static func String getParameterValue( String name )
deClassInputSystem::nfGetParameterValue::nfGetParameterValue( const sInitData &init ) :
dsFunction( init.clsInpSys, "getParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // name
}
void deClassInputSystem::nfGetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	const deInputSystem &inpsys = *ds.GetGameEngine()->GetInputSystem();
	const deBaseInputModule &module = *inpsys.GetActiveModule();
	
	rt->PushString( module.GetParameterValue( rt->GetValue( 0 )->GetString() ) );
}

// public static func void setParameterValue( String name, String value )
deClassInputSystem::nfSetParameterValue::nfSetParameterValue( const sInitData &init ) :
dsFunction( init.clsInpSys, "setParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsString ); // value
}
void deClassInputSystem::nfSetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	const deInputSystem &inpsys = *ds.GetGameEngine()->GetInputSystem();
	deBaseInputModule &module = *inpsys.GetActiveModule();
	
	module.SetParameterValue( rt->GetValue( 0 )->GetString(), rt->GetValue( 1 )->GetString() );
}



// public static func String sendCommand( String command )
deClassInputSystem::nfSendCommand::nfSendCommand( const sInitData &init ) :
dsFunction( init.clsInpSys, "sendCommand", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // command
}
void deClassInputSystem::nfSendCommand::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassInputSystem* )GetOwnerClass() )->GetDS();
	const deInputSystem &inpsys = *ds.GetGameEngine()->GetInputSystem();
	deBaseInputModule &module = *inpsys.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8( rt->GetValue( 0 )->GetString() );
	argList.ParseCommand( command );
	module.SendCommand( argList, answer );
	rt->PushString( answer.ToUTF8().GetString() );
}



// Class deClassInputSystem
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassInputSystem::deClassInputSystem( deScriptingDragonScript &ds ) :
dsClass( "InputSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ),
pCacheDirty( true )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	p_SetNativeDataSize( 0 );
}

deClassInputSystem::~deClassInputSystem(){
}



// Management
///////////////

void deClassInputSystem::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsInpSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsModPar = pDS.GetClassModuleParameter();
	init.clsInputDevice = pDS.GetClassInputDevice();
	
	AddFunction( new nfGetCaptureInputDevices( init ) );
	AddFunction( new nfSetCaptureInputDevices( init ) );
	
	AddFunction( new nfGetDeviceCount( init ) );;
	AddFunction( new nfGetDeviceAt( init ) );;
	AddFunction( new nfIndexOfDeviceWithID( init ) );;
	AddFunction( new nfIndexOfButtonWithID( init ) );;
	AddFunction( new nfIndexOfAxisWithID( init ) );;
	AddFunction( new nfIndexOfFeedbackWithID( init ) );;
	AddFunction( new nfGetButtonPressed( init ) );;
	AddFunction( new nfGetAxisValue( init ) );;
	AddFunction( new nfGetFeedbackValue( init ) );;
	AddFunction( new nfSetFeedbackValue( init ) );;
	AddFunction( new nfButtonMatchingKeyCode( init ) );;
	AddFunction( new nfButtonMatchingKeyChar( init ) );;
	
	AddFunction( new nfGetParameterCount( init ) );
	AddFunction( new nfGetParameterInfo( init ) );
	AddFunction( new nfGetParameterInfo2( init ) );
	AddFunction( new nfGetParameterValue( init ) );
	AddFunction( new nfSetParameterValue( init ) );
	
	AddFunction( new nfSendCommand( init ) );
}



int deClassInputSystem::GetCachedDeviceCount(){
	pUpdateCachedDevices();
	return pCachedDevices.GetCount();
}

deInputDevice *deClassInputSystem::GetCachedDeviceAt( int index ){
	pUpdateCachedDevices();
	return ( deInputDevice* )pCachedDevices.GetAt( index );
}

void deClassInputSystem::InvalidCachedDevices(){
	pCachedDevices.RemoveAll();
	pCacheDirty = true;
}



// Private Functions
//////////////////////

void deClassInputSystem::pUpdateCachedDevices(){
	if( ! pCacheDirty ){
		return;
	}
	
	deBaseInputModule &module = *pDS.GetGameEngine()->GetInputSystem()->GetActiveModule();
	const int count = module.GetDeviceCount();
	deInputDevice *device = NULL;
	int i;
	
	pCachedDevices.RemoveAll();
	
	try{
		for( i=0; i<count; i++ ){
			device = module.GetDeviceAt( i );
			pCachedDevices.Add( device );
			device->FreeReference();
			device = NULL;
		}
		
	}catch( ... ){
		if( device ){
			device->FreeReference();
		}
		throw;
	}
	
	pCacheDirty = false;
}
