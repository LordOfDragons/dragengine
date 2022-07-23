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

#include "deClassPhysicsSystem.h"
#include "../deClassModuleParameter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/systems/dePhysicsSystem.h>
#include <dragengine/systems/modules/physics/deBasePhysicsModule.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <libdscript/exceptions.h>



// Native functions
/////////////////////

// public static func int getParameterCount()
deClassPhysicsSystem::nfGetParameterCount::nfGetParameterCount( const sInitData &init ) : dsFunction( init.clsPhySys,
"getParameterCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassPhysicsSystem::nfGetParameterCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dePhysicsSystem &phySys = *( ( ( deClassPhysicsSystem* )GetOwnerClass() )->GetDS().GetGameEngine()->GetPhysicsSystem() );
	rt->PushInt( phySys.GetActiveModule()->GetParameterCount() );
}

// public static func ModuleParameter getParameterInfo( int index )
deClassPhysicsSystem::nfGetParameterInfo::nfGetParameterInfo( const sInitData &init ) : dsFunction(init.clsPhySys,
"getParameterInfo", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModParam ){
	p_AddParameter( init.clsInteger ); // index
}
void deClassPhysicsSystem::nfGetParameterInfo::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassPhysicsSystem* )GetOwnerClass() )->GetDS();
	const dePhysicsSystem &phySys = *( ds.GetGameEngine()->GetPhysicsSystem() );
	deBasePhysicsModule * const module = phySys.GetActiveModule();
	
	const int index = rt->GetValue( 0 )->GetInt();
	if( index < 0 || index >= module->GetParameterCount() ){
		DSTHROW( dueInvalidParam );
	}
	
	ds.GetClassModuleParameter()->PushParameter( rt, module, index );
}

// public static func ModuleParameter getParameterInfo( String name )
deClassPhysicsSystem::nfGetParameterInfo2::nfGetParameterInfo2( const sInitData &init ) :
dsFunction( init.clsPhySys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModParam ){
	p_AddParameter( init.clsString ); // name
}
void deClassPhysicsSystem::nfGetParameterInfo2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassPhysicsSystem* )GetOwnerClass() )->GetDS();
	const dePhysicsSystem &phySys = *ds.GetGameEngine()->GetPhysicsSystem();
	deBasePhysicsModule * const module = phySys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter( rt, module,
		module->IndexOfParameterNamed( rt->GetValue( 0 )->GetString() ) );
}

// public static func String getParameterValue( String name )
deClassPhysicsSystem::nfGetParameterValue::nfGetParameterValue( const sInitData &init ) : dsFunction( init.clsPhySys,
"getParameterValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // name
}
void deClassPhysicsSystem::nfGetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassPhysicsSystem* )GetOwnerClass() )->GetDS();
	const dePhysicsSystem &phySys = *ds.GetGameEngine()->GetPhysicsSystem();
	const deBasePhysicsModule &module = *phySys.GetActiveModule();
	
	rt->PushString( module.GetParameterValue( rt->GetValue( 0 )->GetString() ) );
}

// public static func void setParameterValue( String name, String value )
deClassPhysicsSystem::nfSetParameterValue::nfSetParameterValue( const sInitData &init ) : dsFunction( init.clsPhySys,
"setParameterValue", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsString ); // value
}
void deClassPhysicsSystem::nfSetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	const dePhysicsSystem &phySys = *( ( ( deClassPhysicsSystem* )GetOwnerClass() )
		->GetDS().GetGameEngine()->GetPhysicsSystem() );
	deBasePhysicsModule &module = *phySys.GetActiveModule();
	
	module.SetParameterValue( rt->GetValue( 0 )->GetString(), rt->GetValue( 1 )->GetString() );
}



// public static func String sendCommand( String command )
deClassPhysicsSystem::nfSendCommand::nfSendCommand( const sInitData &init ) : dsFunction( init.clsPhySys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // command
}
void deClassPhysicsSystem::nfSendCommand::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassPhysicsSystem* )GetOwnerClass() )->GetDS();
	const dePhysicsSystem &phySys = *ds.GetGameEngine()->GetPhysicsSystem();
	deBasePhysicsModule &module = *phySys.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8( rt->GetValue( 0 )->GetString() );
	argList.ParseCommand( command );
	module.SendCommand( argList, answer );
	rt->PushString( answer.ToUTF8().GetString() );
}

// public static func int getFPSRate()
deClassPhysicsSystem::nfGetFPSRate::nfGetFPSRate( const sInitData &init ) :
dsFunction( init.clsPhySys, "getFPSRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassPhysicsSystem::nfGetFPSRate::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassPhysicsSystem* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetGameEngine()->GetPhysicsSystem()->GetActiveModule()->GetFPSRate() );
}



// Class deClassPhysicsSystem
///////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassPhysicsSystem::deClassPhysicsSystem( deScriptingDragonScript &ds ) :
dsClass( "PhysicsSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	p_SetNativeDataSize( 0 );
}

deClassPhysicsSystem::~deClassPhysicsSystem(){
}



// Management
///////////////

void deClassPhysicsSystem::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	init.clsPhySys = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsModParam = pDS.GetClassModuleParameter();
	
	AddFunction( new nfGetParameterCount( init ) );
	AddFunction( new nfGetParameterInfo( init ) );
	AddFunction( new nfGetParameterInfo2( init ) );
	AddFunction( new nfGetParameterValue( init ) );
	AddFunction( new nfSetParameterValue( init ) );
	
	AddFunction( new nfSendCommand( init ) );
	AddFunction( new nfGetFPSRate( init ) );
}
