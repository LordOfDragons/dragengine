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
#include <libdscript/libdscript.h>

#include "deClassAudioSystem.h"
#include "../sound/deClassMicrophone.h"
#include "../deClassModuleParameter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deAudioSystem.h>
#include <dragengine/systems/modules/audio/deBaseAudioModule.h>

#include <libdscript/exceptions.h>


// Management
///////////////

// public static func Microphone getActiveMicrophone()
deClassAudioSystem::nfGetActiveMicrophone::nfGetActiveMicrophone( const sInitData &init ) :
dsFunction( init.clsAudSys, "getActiveMicrophone", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsMic ){
}
void deClassAudioSystem::nfGetActiveMicrophone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAudioSystem *clsAudSys = ( deClassAudioSystem* )GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deClassMicrophone *clsMic = clsAudSys->GetDS().GetClassMicrophone();
	deMicrophone *microphone = audSys->GetActiveMicrophone();
	
	if( microphone ){
		clsMic->PushMicrophone( rt, microphone );
		
	}else{
		rt->PushObject( NULL, clsMic );
	}
}

// public static func setActiveMicrophone( Microphone microphone )
deClassAudioSystem::nfSetActiveMicrophone::nfSetActiveMicrophone( const sInitData &init ) :
dsFunction( init.clsAudSys, "setActiveMicrophone", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsMic ); // microphone
}
void deClassAudioSystem::nfSetActiveMicrophone::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAudioSystem *clsAudSys = ( deClassAudioSystem* )GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deClassMicrophone *clsMic = clsAudSys->GetDS().GetClassMicrophone();
	dsRealObject *object = rt->GetValue( 0 )->GetRealObject();
	
	if( object ){
		audSys->SetActiveMicrophone( clsMic->GetMicrophone( object ) );
		
	}else{
		audSys->SetActiveMicrophone( NULL );
	}
}



// Parameters
///////////////

// public static func int getParameterCount()
deClassAudioSystem::nfGetParameterCount::nfGetParameterCount( const sInitData &init ) :
dsFunction( init.clsAudSys, "getParameterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassAudioSystem::nfGetParameterCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAudioSystem *clsAudSys = ( deClassAudioSystem* )GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deBaseAudioModule *module = audSys->GetActiveModule();
	
	rt->PushInt( module->GetParameterCount() );
}

// public static func ModuleParameter getParameterInfo( int index )
deClassAudioSystem::nfGetParameterInfo::nfGetParameterInfo( const sInitData &init ) :
dsFunction( init.clsAudSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter( init.clsInteger ); // index
}
void deClassAudioSystem::nfGetParameterInfo::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAudioSystem *clsAudSys = ( deClassAudioSystem* )GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deBaseAudioModule *module = audSys->GetActiveModule();
	int index = rt->GetValue( 0 )->GetInt();
	
	if( index < 0 || index >= module->GetParameterCount() ) DSTHROW( dueInvalidParam );
	
	clsAudSys->GetDS().GetClassModuleParameter()->PushParameter( rt, module, index );
}

// public static func ModuleParameter getParameterInfo( String name )
deClassAudioSystem::nfGetParameterInfo2::nfGetParameterInfo2( const sInitData &init ) :
dsFunction( init.clsAudSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar ){
	p_AddParameter( init.clsString ); // name
}
void deClassAudioSystem::nfGetParameterInfo2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassAudioSystem* )GetOwnerClass() )->GetDS();
	const deAudioSystem &audSys = *ds.GetGameEngine()->GetAudioSystem();
	deBaseAudioModule * const module = audSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter( rt, module,
		module->IndexOfParameterNamed( rt->GetValue( 0 )->GetString() ) );
}

// public static func String getParameterValue( String name )
deClassAudioSystem::nfGetParameterValue::nfGetParameterValue( const sInitData &init ) :
dsFunction( init.clsAudSys, "getParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // name
}
void deClassAudioSystem::nfGetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAudioSystem *clsAudSys = ( deClassAudioSystem* )GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deBaseAudioModule *module = audSys->GetActiveModule();
	rt->PushString( module->GetParameterValue( rt->GetValue( 0 )->GetString() ) );
}

// public static func void setParameterValue( String name, String value )
deClassAudioSystem::nfSetParameterValue::nfSetParameterValue( const sInitData &init ) :
dsFunction( init.clsAudSys, "setParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsString ); // value
}
void deClassAudioSystem::nfSetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAudioSystem *clsAudSys = ( deClassAudioSystem* )GetOwnerClass();
	deAudioSystem *audSys = clsAudSys->GetDS().GetGameEngine()->GetAudioSystem();
	deBaseAudioModule *module = audSys->GetActiveModule();
	
	module->SetParameterValue( rt->GetValue( 0 )->GetString(), rt->GetValue( 1 )->GetString() );
}



// public static func String sendCommand( String command )
deClassAudioSystem::nfSendCommand::nfSendCommand( const sInitData &init ) : dsFunction( init.clsAudSys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // command
}
void deClassAudioSystem::nfSendCommand::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassAudioSystem* )GetOwnerClass() )->GetDS();
	const deAudioSystem &audioSystem = *ds.GetGameEngine()->GetAudioSystem();
	deBaseAudioModule &module = *audioSystem.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8( rt->GetValue( 0 )->GetString() );
	argList.ParseCommand( command );
	module.SendCommand( argList, answer );
	rt->PushString( answer.ToUTF8() );
}

// public static func int getFPSRate()
deClassAudioSystem::nfGetFPSRate::nfGetFPSRate( const sInitData &init ) :
dsFunction( init.clsAudSys, "getFPSRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassAudioSystem::nfGetFPSRate::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassAudioSystem* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetGameEngine()->GetAudioSystem()->GetActiveModule()->GetFPSRate() );
}



// Class deClassAudioSystem
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassAudioSystem::deClassAudioSystem( deScriptingDragonScript &ds ) :
dsClass( "AudioSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( 0 );
}

deClassAudioSystem::~deClassAudioSystem(){
}



// Management
///////////////

void deClassAudioSystem::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsAudSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsMic = pDS.GetClassMicrophone();
	init.clsModPar = pDS.GetClassModuleParameter();
	
	// add functions
	AddFunction( new nfGetActiveMicrophone( init ) );
	AddFunction( new nfSetActiveMicrophone( init ) );
	
	AddFunction( new nfGetParameterCount( init ) );
	AddFunction( new nfGetParameterInfo( init ) );
	AddFunction( new nfGetParameterInfo2( init ) );
	AddFunction( new nfGetParameterValue( init ) );
	AddFunction( new nfSetParameterValue( init ) );
	
	AddFunction( new nfSendCommand( init ) );
	AddFunction( new nfGetFPSRate( init ) );
}
