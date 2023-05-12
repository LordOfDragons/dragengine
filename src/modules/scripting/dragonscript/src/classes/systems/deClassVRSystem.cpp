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
#include <string.h>

#include "deClassVRSystem.h"
#include "../deClassModuleParameter.h"
#include "../graphics/deClassCamera.h"
#include "../input/deClassInputDevice.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"
#include "../../utils/dedsInputDevice.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/deVRSystem.h>
#include <dragengine/systems/modules/vr/deBaseVRModule.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>

#include <libdscript/exceptions.h>
#include <libdscript/packages/default/dsClassEnumeration.h>



// Native functions
/////////////////////

// public static func bool runtimeUsable
deClassVRSystem::nfRuntimeUsable::nfRuntimeUsable( const sInitData &init ) :
dsFunction( init.clsVRSystem, "runtimeUsable", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
}
void deClassVRSystem::nfRuntimeUsable::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	rt->PushBool( ds.GetGameEngine()->GetVRSystem()->RuntimeUsable() );
}

// public static func void requestFeatureEyeGazeTracking(VRFeatureSupportLevel level)
deClassVRSystem::nfRequestFeatureEyeGazeTracking::nfRequestFeatureEyeGazeTracking( const sInitData &init ) :
dsFunction( init.clsInputDevice, "requestFeatureEyeGazeTracking", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsVRFeatureSupportLevel ); // level
}
void deClassVRSystem::nfRequestFeatureEyeGazeTracking::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	
	const deBaseVRModule::eFeatureSupportLevel level = ( deBaseVRModule::eFeatureSupportLevel )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	ds.GetGameEngine()->GetVRSystem()->RequestFeatureEyeGazeTracking( level );
}

// public static func void requestFeatureFacialTracking(VRFeatureSupportLevel level)
deClassVRSystem::nfRequestFeatureFacialTracking::nfRequestFeatureFacialTracking( const sInitData &init ) :
dsFunction( init.clsInputDevice, "requestFeatureFacialTracking", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsVRFeatureSupportLevel ); // level
}
void deClassVRSystem::nfRequestFeatureFacialTracking::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	
	const deBaseVRModule::eFeatureSupportLevel level = ( deBaseVRModule::eFeatureSupportLevel )
		( ( dsClassEnumeration* )rt->GetEngine()->GetClassEnumeration() )->GetConstantOrder(
			*rt->GetValue( 0 )->GetRealObject() );
	
	ds.GetGameEngine()->GetVRSystem()->RequestFeatureFacialTracking( level );
}

// public static func void startRuntime
deClassVRSystem::nfStartRuntime::nfStartRuntime( const sInitData &init ) :
dsFunction( init.clsVRSystem, "startRuntime", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
}
void deClassVRSystem::nfStartRuntime::RunFunction( dsRunTime*, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	ds.GetGameEngine()->GetVRSystem()->StartRuntime();
}

// public static func void stopRuntime()
deClassVRSystem::nfStopRuntime::nfStopRuntime( const sInitData &init ) :
dsFunction( init.clsVRSystem, "stopRuntime", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
}
void deClassVRSystem::nfStopRuntime::RunFunction( dsRunTime*, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	ds.GetGameEngine()->GetVRSystem()->StopRuntime();
}

// public static func bool isRuntimeRunning()
deClassVRSystem::nfIsRuntimeRunning::nfIsRuntimeRunning( const sInitData &init ) :
dsFunction( init.clsVRSystem, "isRuntimeRunning", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
}
void deClassVRSystem::nfIsRuntimeRunning::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	rt->PushBool( ds.GetGameEngine()->GetVRSystem()->GetIsRunning() );
}

// public static func Camera getCamera()
deClassVRSystem::nfGetCamera::nfGetCamera( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getCamera", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsCamera ){
}
void deClassVRSystem::nfGetCamera::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	ds.GetClassCamera()->PushCamera( rt, ds.GetGameEngine()->GetVRSystem()->GetCamera() );
}

// public static func void setCamera( Camera camera )
deClassVRSystem::nfSetCamera::nfSetCamera( const sInitData &init ) :
dsFunction( init.clsVRSystem, "setCamera", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsCamera ); // camera
}
void deClassVRSystem::nfSetCamera::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	ds.GetGameEngine()->GetVRSystem()->SetCamera(
		ds.GetClassCamera()->GetCamera( rt->GetValue( 0 )->GetRealObject() ) );
}

// public static func bool supportsPassthrough()
deClassVRSystem::nfSupportsPassthrough::nfSupportsPassthrough( const sInitData &init ) :
dsFunction( init.clsVRSystem, "supportsPassthrough", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
}
void deClassVRSystem::nfSupportsPassthrough::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	rt->PushBool( ds.GetGameEngine()->GetVRSystem()->SupportsPassthrough() );
}

// public static func bool getEnablePassthrough()
deClassVRSystem::nfGetEnablePassthrough::nfGetEnablePassthrough( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getEnablePassthrough", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
}
void deClassVRSystem::nfGetEnablePassthrough::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	rt->PushBool( ds.GetGameEngine()->GetVRSystem()->GetEnablePassthrough() );
}

// public static func void setEnablePassthrough(bool enable)
deClassVRSystem::nfSetEnablePassthrough::nfSetEnablePassthrough( const sInitData &init ) :
dsFunction( init.clsVRSystem, "setEnablePassthrough", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsBool ); // enable
}
void deClassVRSystem::nfSetEnablePassthrough::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	ds.GetGameEngine()->GetVRSystem()->SetEnablePassthrough( rt->GetValue( 0 )->GetBool() );
}

// public static func float getPassthroughTransparency()
deClassVRSystem::nfGetPassthroughTransparency::nfGetPassthroughTransparency( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getPassthroughTransparency", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFloat ){
}
void deClassVRSystem::nfGetPassthroughTransparency::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	rt->PushFloat( ds.GetGameEngine()->GetVRSystem()->GetPassthroughTransparency() );
}

// public static func void setPassthroughTransparency(float transparency)
deClassVRSystem::nfSetPassthroughTransparency::nfSetPassthroughTransparency( const sInitData &init ) :
dsFunction( init.clsVRSystem, "setPassthroughTransparency", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsFloat ); // transparency
}
void deClassVRSystem::nfSetPassthroughTransparency::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	ds.GetGameEngine()->GetVRSystem()->SetPassthroughTransparency( rt->GetValue( 0 )->GetFloat() );
}



// public static func int getDeviceCount()
deClassVRSystem::nfGetDeviceCount::nfGetDeviceCount( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getDeviceCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassVRSystem::nfGetDeviceCount::RunFunction( dsRunTime *rt, dsValue* ){
	deClassVRSystem &clsVRSystem = *( ( deClassVRSystem* )GetOwnerClass() );
	rt->PushInt( clsVRSystem.GetCachedDeviceCount() );
}

// public static func InputDevice getDeviceAt( int index )
deClassVRSystem::nfGetDeviceAt::nfGetDeviceAt( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getDeviceAt", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInputDevice ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassVRSystem::nfGetDeviceAt::RunFunction( dsRunTime *rt, dsValue* ){
	deClassVRSystem &clsVRSystem = *( ( deClassVRSystem* )GetOwnerClass() );
	const int index = rt->GetValue( 0 )->GetInt();
	
	clsVRSystem.GetDS().GetClassInputDevice()->PushInputDevice( rt, clsVRSystem.GetCachedDeviceAt( index ) );
}

// public static func int indexOfDeviceWithID( String id )
deClassVRSystem::nfIndexOfDeviceWithID::nfIndexOfDeviceWithID( const sInitData &init ) :
dsFunction( init.clsVRSystem, "indexOfDeviceWithID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsString ); // id
}
void deClassVRSystem::nfIndexOfDeviceWithID::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const char * const id = rt->GetValue( 0 )->GetString();
	rt->PushInt( module.IndexOfDeviceWithID( id ) );
}

// public static func int indexOfButtonWithID( int device, String id )
deClassVRSystem::nfIndexOfButtonWithID::nfIndexOfButtonWithID( const sInitData &init ) :
dsFunction( init.clsVRSystem, "indexOfButtonWithID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsString ); // id
}
void deClassVRSystem::nfIndexOfButtonWithID::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const char * const id = rt->GetValue( 1 )->GetString();
	rt->PushInt( module.IndexOfButtonWithID( device, id ) );
}

// public static func int indexOfAxisWithID( int device, String id )
deClassVRSystem::nfIndexOfAxisWithID::nfIndexOfAxisWithID( const sInitData &init ) :
dsFunction( init.clsVRSystem, "indexOfAxisWithID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsString ); // id
}
void deClassVRSystem::nfIndexOfAxisWithID::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const char * const id = rt->GetValue( 1 )->GetString();
	rt->PushInt( module.IndexOfAxisWithID( device, id ) );
}

// public static func int indexOfFeedbackWithID( int device, String id )
deClassVRSystem::nfIndexOfFeedbackWithID::nfIndexOfFeedbackWithID( const sInitData &init ) :
dsFunction( init.clsVRSystem, "indexOfFeedbackWithID", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsString ); // id
}
void deClassVRSystem::nfIndexOfFeedbackWithID::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const char * const id = rt->GetValue( 1 )->GetString();
	rt->PushInt( module.IndexOfFeedbackWithID( device, id ) );
}

// public static func bool getButtonPressed( int device, int button )
deClassVRSystem::nfGetButtonPressed::nfGetButtonPressed( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getButtonPressed", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // button
}
void deClassVRSystem::nfGetButtonPressed::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int button  = rt->GetValue( 1 )->GetInt();
	rt->PushBool( module.GetButtonPressed( device, button ) );
}

// public static func bool getButtonTouched( int device, int button )
deClassVRSystem::nfGetButtonTouched::nfGetButtonTouched( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getButtonTouched", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // button
}
void deClassVRSystem::nfGetButtonTouched::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int button  = rt->GetValue( 1 )->GetInt();
	rt->PushBool( module.GetButtonTouched( device, button ) );
}

// public static func float getAxisValue( int device, int axis )
deClassVRSystem::nfGetAxisValue::nfGetAxisValue( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getAxisValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFloat ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // axis
}
void deClassVRSystem::nfGetAxisValue::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int axis  = rt->GetValue( 1 )->GetInt();
	rt->PushFloat( module.GetAxisValue( device, axis ) );
}

// public static func float getFeedbackValue( int device, int feedback )
deClassVRSystem::nfGetFeedbackValue::nfGetFeedbackValue( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getFeedbackValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsFloat ); // feedback
}
void deClassVRSystem::nfGetFeedbackValue::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int feedback  = rt->GetValue( 1 )->GetInt();
	rt->PushFloat( module.GetFeedbackValue( device, feedback ) );
}

// public static func void setFeedbackValue( int device, int feedback, float value )
deClassVRSystem::nfSetFeedbackValue::nfSetFeedbackValue( const sInitData &init ) :
dsFunction( init.clsVRSystem, "setFeedbackValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsInteger ); // device
	p_AddParameter( init.clsInteger ); // feedback
	p_AddParameter( init.clsFloat ); // value
}
void deClassVRSystem::nfSetFeedbackValue::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deBaseVRModule &module = *ds.GetGameEngine()->GetVRSystem()->GetActiveModule();
	
	const int device = rt->GetValue( 0 )->GetInt();
	const int feedback  = rt->GetValue( 1 )->GetInt();
	const float value  = rt->GetValue( 2 )->GetFloat();
	module.SetFeedbackValue( device, feedback, value );
}



// public static func int getParameterCount()
deClassVRSystem::nfGetParameterCount::nfGetParameterCount( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getParameterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassVRSystem::nfGetParameterCount::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	const deVRSystem &vrsys = *ds.GetGameEngine()->GetVRSystem();
	rt->PushInt( vrsys.GetActiveModule()->GetParameterCount() );
}

// public static func ModuleParameter getParameterInfo(int index)
deClassVRSystem::nfGetParameterInfo::nfGetParameterInfo( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassVRSystem::nfGetParameterInfo::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	const deVRSystem &vrsys = *ds.GetGameEngine()->GetVRSystem();
	deBaseVRModule * const module = vrsys.GetActiveModule();
	
	const int index = rt->GetValue( 0 )->GetInt();
	if( index < 0 || index >= module->GetParameterCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	ds.GetClassModuleParameter()->PushParameter( rt, module, index );
}

// public static func ModuleParameter getParameterInfo( String name )
deClassVRSystem::nfGetParameterInfo2::nfGetParameterInfo2( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar ){
	p_AddParameter( init.clsString ); // name
}
void deClassVRSystem::nfGetParameterInfo2::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	deVRSystem &inpSys = *ds.GetGameEngine()->GetVRSystem();
	deBaseVRModule * const module = inpSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter( rt, module,
		module->IndexOfParameterNamed( rt->GetValue( 0 )->GetString() ) );
}

// public static func String getParameterValue( String name )
deClassVRSystem::nfGetParameterValue::nfGetParameterValue( const sInitData &init ) :
dsFunction( init.clsVRSystem, "getParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // name
}
void deClassVRSystem::nfGetParameterValue::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	const deVRSystem &vrsys = *ds.GetGameEngine()->GetVRSystem();
	const deBaseVRModule &module = *vrsys.GetActiveModule();
	
	rt->PushString( module.GetParameterValue( rt->GetValue( 0 )->GetString() ) );
}

// public static func void setParameterValue( String name, String value )
deClassVRSystem::nfSetParameterValue::nfSetParameterValue( const sInitData &init ) :
dsFunction( init.clsVRSystem, "setParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsString ); // value
}
void deClassVRSystem::nfSetParameterValue::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	const deVRSystem &vrsys = *ds.GetGameEngine()->GetVRSystem();
	deBaseVRModule &module = *vrsys.GetActiveModule();
	
	module.SetParameterValue( rt->GetValue( 0 )->GetString(), rt->GetValue( 1 )->GetString() );
}



// public static func String sendCommand( String command )
deClassVRSystem::nfSendCommand::nfSendCommand( const sInitData &init ) :
dsFunction( init.clsVRSystem, "sendCommand", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // command
}
void deClassVRSystem::nfSendCommand::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassVRSystem* )GetOwnerClass() )->GetDS();
	const deVRSystem &vrsys = *ds.GetGameEngine()->GetVRSystem();
	deBaseVRModule &module = *vrsys.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8( rt->GetValue( 0 )->GetString() );
	argList.ParseCommand( command );
	module.SendCommand( argList, answer );
	rt->PushString( answer.ToUTF8().GetString() );
}



// Class deClassVRSystem
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassVRSystem::deClassVRSystem( deScriptingDragonScript &ds ) :
dsClass( "VRSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds ),
pCacheDirty( true ),
pClsVRFeatureSupportLevel( nullptr )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	p_SetNativeDataSize( 0 );
}

deClassVRSystem::~deClassVRSystem(){
}



// Management
///////////////

void deClassVRSystem::CreateClassMembers( dsEngine *engine ){
	pClsVRFeatureSupportLevel = engine->GetClass( "Dragengine.VRFeatureSupportLevel" );
	
	sInitData init;
	
	init.clsVRSystem = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsModPar = pDS.GetClassModuleParameter();
	init.clsInputDevice = pDS.GetClassInputDevice();
	init.clsCamera = pDS.GetClassCamera();
	init.clsVRFeatureSupportLevel = pClsVRFeatureSupportLevel;
	
	AddFunction( new nfRuntimeUsable( init ) );
	AddFunction( new nfRequestFeatureEyeGazeTracking( init ) );
	AddFunction( new nfRequestFeatureFacialTracking( init ) );
	AddFunction( new nfStartRuntime( init ) );
	AddFunction( new nfStopRuntime( init ) );
	AddFunction( new nfIsRuntimeRunning( init ) );
	
	AddFunction( new nfGetCamera( init ) );
	AddFunction( new nfSetCamera( init ) );
	AddFunction( new nfSupportsPassthrough( init ) );
	AddFunction( new nfGetEnablePassthrough( init ) );
	AddFunction( new nfSetEnablePassthrough( init ) );
	AddFunction( new nfGetPassthroughTransparency( init ) );
	AddFunction( new nfSetPassthroughTransparency( init ) );
	
	AddFunction( new nfGetDeviceCount( init ) );
	AddFunction( new nfGetDeviceAt( init ) );
	AddFunction( new nfIndexOfDeviceWithID( init ) );
	AddFunction( new nfIndexOfButtonWithID( init ) );
	AddFunction( new nfIndexOfAxisWithID( init ) );
	AddFunction( new nfIndexOfFeedbackWithID( init ) );
	AddFunction( new nfGetButtonPressed( init ) );
	AddFunction( new nfGetButtonTouched( init ) );
	AddFunction( new nfGetAxisValue( init ) );
	AddFunction( new nfGetFeedbackValue( init ) );
	AddFunction( new nfSetFeedbackValue( init ) );
	
	AddFunction( new nfGetParameterCount( init ) );
	AddFunction( new nfGetParameterInfo( init ) );
	AddFunction( new nfGetParameterInfo2( init ) );
	AddFunction( new nfGetParameterValue( init ) );
	AddFunction( new nfSetParameterValue( init ) );
	
	AddFunction( new nfSendCommand( init ) );
}



int deClassVRSystem::GetCachedDeviceCount(){
	pUpdateCachedDevices();
	return pCachedDevices.GetCount();
}

dedsInputDevice *deClassVRSystem::GetCachedDeviceAt( int index ){
	pUpdateCachedDevices();
	return ( dedsInputDevice* )pCachedDevices.GetAt( index );
}

void deClassVRSystem::InvalidCachedDevices(){
	pCachedDevices.RemoveAll();
	pCacheDirty = true;
}

void deClassVRSystem::OnFrameUpdate(){
	const int count = pCachedDevices.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		( ( dedsInputDevice* )pCachedDevices.GetAt( i ) )->OnFrameUpdate();
	}
}



// Private Functions
//////////////////////

void deClassVRSystem::pUpdateCachedDevices(){
	if( ! pCacheDirty ){
		return;
	}
	
	deBaseVRModule &module = *pDS.GetGameEngine()->GetVRSystem()->GetActiveModule();
	const decObjectList oldDevices( pCachedDevices );
	const int oldDeviceCount = oldDevices.GetCount();
	const int count = module.GetDeviceCount();
	dedsInputDevice::Ref device;
	int i, j;
	
	pCachedDevices.RemoveAll();
	
	for( i=0; i<count; i++ ){
		device.TakeOver( new dedsInputDevice( pDS, module, i ) );
		
		dedsInputDevice *reuseDevice = nullptr;
		for( j=0; j<oldDeviceCount; j++ ){
			dedsInputDevice * const oldDevice = ( dedsInputDevice* )oldDevices.GetAt( j );
			if( oldDevice->GetDevice()->GetID() == device->GetDevice()->GetID() ){
				reuseDevice = oldDevice;
				break;
			}
		}
		
		if( reuseDevice ){
			reuseDevice->Update( device );
			pCachedDevices.Add( reuseDevice );
			
		}else{
			pCachedDevices.Add( device );
		}
	}
	
	pCacheDirty = false;
}
