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

#include "deClassAnimatorSystem.h"
#include "../deClassModuleParameter.h"
#include "../../deScriptingDragonScript.h"
#include "../../deClassPathes.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>
#include <dragengine/common/string/unicode/decUnicodeArgumentList.h>
#include <dragengine/resources/sound/deMicrophone.h>
#include <dragengine/systems/deAnimatorSystem.h>
#include <dragengine/systems/modules/animator/deBaseAnimatorModule.h>

#include <libdscript/exceptions.h>


// Management
///////////////



// Parameters
///////////////

// public static func int getParameterCount()
deClassAnimatorSystem::nfGetParameterCount::nfGetParameterCount( const sInitData &init ) :
dsFunction( init.clsAmrSys, "getParameterCount", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassAnimatorSystem::nfGetParameterCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAnimatorSystem *clsAmrSys = ( deClassAnimatorSystem* )GetOwnerClass();
	deAnimatorSystem *amrSys = clsAmrSys->GetDS().GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule *module = amrSys->GetActiveModule();
	
	rt->PushInt( module->GetParameterCount() );
}

// public static func ModuleParameter getParameterInfo( int index )
deClassAnimatorSystem::nfGetParameterInfo::nfGetParameterInfo( const sInitData &init ) :
dsFunction( init.clsAmrSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar){
	p_AddParameter( init.clsInteger ); // index
}
void deClassAnimatorSystem::nfGetParameterInfo::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAnimatorSystem *clsAmrSys = ( deClassAnimatorSystem* )GetOwnerClass();
	deAnimatorSystem *amrSys = clsAmrSys->GetDS().GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule *module = amrSys->GetActiveModule();
	int index = rt->GetValue( 0 )->GetInt();
	
	if( index < 0 || index >= module->GetParameterCount() ) DSTHROW( dueInvalidParam );
	
	clsAmrSys->GetDS().GetClassModuleParameter()->PushParameter( rt, module, index );
}

// public static func ModuleParameter getParameterInfo( String name )
deClassAnimatorSystem::nfGetParameterInfo2::nfGetParameterInfo2( const sInitData &init ) :
dsFunction( init.clsAmrSys, "getParameterInfo", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsModPar ){
	p_AddParameter( init.clsString ); // name
}
void deClassAnimatorSystem::nfGetParameterInfo2::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassAnimatorSystem* )GetOwnerClass() )->GetDS();
	const deAnimatorSystem &amrSys = *ds.GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule * const module = amrSys.GetActiveModule();
	
	ds.GetClassModuleParameter()->PushParameter( rt, module,
		module->IndexOfParameterNamed( rt->GetValue( 0 )->GetString() ) );
}

// public static func String getParameterValue( String name )
deClassAnimatorSystem::nfGetParameterValue::nfGetParameterValue( const sInitData &init ) :
dsFunction( init.clsAmrSys, "getParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // name
}
void deClassAnimatorSystem::nfGetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAnimatorSystem *clsAmrSys = ( deClassAnimatorSystem* )GetOwnerClass();
	deAnimatorSystem *amrSys = clsAmrSys->GetDS().GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule *module = amrSys->GetActiveModule();
	rt->PushString( module->GetParameterValue( rt->GetValue( 0 )->GetString() ) );
}

// public static func void setParameterValue( String name, String value )
deClassAnimatorSystem::nfSetParameterValue::nfSetParameterValue( const sInitData &init ) :
dsFunction( init.clsAmrSys, "setParameterValue", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString ); // name
	p_AddParameter( init.clsString ); // value
}
void deClassAnimatorSystem::nfSetParameterValue::RunFunction( dsRunTime *rt, dsValue *myself ){
	deClassAnimatorSystem *clsAmrSys = ( deClassAnimatorSystem* )GetOwnerClass();
	deAnimatorSystem *amrSys = clsAmrSys->GetDS().GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule *module = amrSys->GetActiveModule();
	
	module->SetParameterValue( rt->GetValue( 0 )->GetString(), rt->GetValue( 1 )->GetString() );
}



// public static func String sendCommand( String command )
deClassAnimatorSystem::nfSendCommand::nfSendCommand( const sInitData &init ) : dsFunction( init.clsAmrSys,
"sendCommand", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
	p_AddParameter( init.clsString ); // command
}
void deClassAnimatorSystem::nfSendCommand::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = ( ( deClassAnimatorSystem* )GetOwnerClass() )->GetDS();
	const deAnimatorSystem &animatorSystem = *ds.GetGameEngine()->GetAnimatorSystem();
	deBaseAnimatorModule &module = *animatorSystem.GetActiveModule();
	decUnicodeArgumentList argList;
	decUnicodeString command;
	decUnicodeString answer;
	
	command.SetFromUTF8( rt->GetValue( 0 )->GetString() );
	argList.ParseCommand( command );
	module.SendCommand( argList, answer );
	rt->PushString( answer.ToUTF8() );
}



// Class deClassAnimatorSystem
/////////////////////////////

// Constructor, Destructor
////////////////////////////

deClassAnimatorSystem::deClassAnimatorSystem( deScriptingDragonScript &ds ) :
dsClass( "AnimatorSystem", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE ),
pDS( ds )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( 0 );
}

deClassAnimatorSystem::~deClassAnimatorSystem(){
}



// Management
///////////////

void deClassAnimatorSystem::CreateClassMembers( dsEngine *engine ){
	sInitData init;
	
	// store classes
	init.clsAmrSys = this;
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
}
