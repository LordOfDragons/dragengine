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

#include "deClassNetworkSystem.h"
#include "../deClassModuleParameter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deNetworkSystem.h>
#include <dragengine/systems/modules/network/deBaseNetworkModule.h>

#include <libdscript/exceptions.h>


// Management
///////////////



// Parameters
///////////////

// public static func int getParameterCount()
deClassNetworkSystem::nfGetParameterCount::nfGetParameterCount( const sInitData &init ) :
dsFunction( init.clsNetSys, "getParameterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassNetworkSystem::nfGetParameterCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassNetworkSystem *clsNetSys = ( deClassNetworkSystem* )GetOwnerClass();
	deNetworkSystem *audSys = clsNetSys->GetDS().GetGameEngine()->GetNetworkSystem();
	deBaseNetworkModule *module = audSys->GetActiveModule();
	
	rt->PushInt( module->GetParameterCount() );
}

// public static func ModuleParameter getParameterInfo( int index )
deClassNetworkSystem::nfGetParameterInfo::nfGetParameterInfo( const sInitData &init ) :
dsFunction( init.clsNetSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter( init.clsInteger ); // index
}
void deClassNetworkSystem::nfGetParameterInfo::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassNetworkSystem *clsNetSys = ( deClassNetworkSystem* )GetOwnerClass();
	deNetworkSystem *audSys = clsNetSys->GetDS().GetGameEngine()->GetNetworkSystem();
	deBaseNetworkModule *module = audSys->GetActiveModule();
	int index = rt->GetValue( 0 )->GetInt();
	
	if( index < 0 || index >= module->GetParameterCount() ) DSTHROW( dueInvalidParam );
	
	clsNetSys->GetDS().GetClassModuleParameter()->PushParameter( rt, module, index );
}

// public static func ModuleParameter getParameterInfo( String name )
deClassNetworkSystem::nfGetParameterInfo2::nfGetParameterInfo2( const sInitData &init ) :
dsFunction( init.clsNetSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar ){
	p_AddParameter( init.clsString ); // name
}
void deClassNetworkSystem::nfGetParameterInfo2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassNetworkSystem* )GetOwnerClass() )->GetDS();
	const deNetworkSystem &audSys = *ds.GetGameEngine()->GetNetworkSystem();
	deBaseNetworkModule * const module = audSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter( rt, module,
		module->IndexOfParameterNamed( rt->GetValue( 0 )->GetString() ) );
}

// public static func String getParameterValue( String name )
deClassNetworkSystem::nfGetParameterValue::nfGetParameterValue( const sInitData &init ) :
dsFunction( init.clsNetSys, "getParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // name
}
void deClassNetworkSystem::nfGetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassNetworkSystem *clsNetSys = ( deClassNetworkSystem* )GetOwnerClass();
	deNetworkSystem *audSys = clsNetSys->GetDS().GetGameEngine()->GetNetworkSystem();
	deBaseNetworkModule *module = audSys->GetActiveModule();
	rt->PushString( module->GetParameterValue( rt->GetValue( 0 )->GetString() ) );
}

// public static func void setParameterValue( String name, String value )
deClassNetworkSystem::nfSetParameterValue::nfSetParameterValue( const sInitData &init ) :
dsFunction( init.clsNetSys, "setParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsString ); // value
}
void deClassNetworkSystem::nfSetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassNetworkSystem *clsNetSys = ( deClassNetworkSystem* )GetOwnerClass();
	deNetworkSystem *audSys = clsNetSys->GetDS().GetGameEngine()->GetNetworkSystem();
	deBaseNetworkModule *module = audSys->GetActiveModule();
	
	module->SetParameterValue( rt->GetValue( 0 )->GetString(), rt->GetValue( 1 )->GetString() );
}



// public static func String sendCommand( String command )
deClassNetworkSystem::nfSendCommand::nfSendCommand( const sInitData &init ) : dsFunction( init.clsNetSys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // command
}
void deClassNetworkSystem::nfSendCommand::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassNetworkSystem* )GetOwnerClass() )->GetDS();
	const deNetworkSystem &networkSystem = *ds.GetGameEngine()->GetNetworkSystem();
	deBaseNetworkModule &module = *networkSystem.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8( rt->GetValue( 0 )->GetString() );
	argList.ParseCommand( command );
	module.SendCommand( argList, answer );
	rt->PushString( answer.ToUTF8() );
}

// public static func int getFPSRate()
deClassNetworkSystem::nfGetFPSRate::nfGetFPSRate( const sInitData &init ) :
dsFunction( init.clsNetSys, "getFPSRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassNetworkSystem::nfGetFPSRate::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassNetworkSystem* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetGameEngine()->GetNetworkSystem()->GetActiveModule()->GetFPSRate() );
}



// Class deClassNetworkSystem
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassNetworkSystem::deClassNetworkSystem( deScriptingDragonScript &ds ) :
dsClass( "NetworkSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( 0 );
}

deClassNetworkSystem::~deClassNetworkSystem(){
}



// Management
///////////////

void deClassNetworkSystem::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsNetSys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsModPar = pDS.GetClassModuleParameter();
	
	// add functions
	AddFunction( new nfGetParameterCount( init ) );
	AddFunction( new nfGetParameterInfo( init ) );
	AddFunction( new nfGetParameterInfo2( init ) );
	AddFunction( new nfGetParameterValue( init ) );
	AddFunction( new nfSetParameterValue( init ) );
	
	AddFunction( new nfSendCommand( init ) );
	AddFunction( new nfGetFPSRate( init ) );
}
