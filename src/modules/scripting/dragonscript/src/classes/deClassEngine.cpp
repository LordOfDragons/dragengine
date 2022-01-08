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

// includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libdscript/libdscript.h>
#include "deClassEngine.h"
#include "deClassGame.h"
#include "string/deClassStringID.h"
#include "../deScriptingDragonScript.h"
#include "../resourceloader/dedsResourceLoader.h"
#include "../deClassPathes.h"

#include "dragengine/deEngine.h"
#include "dragengine/systems/deScriptingSystem.h"
#include "dragengine/errortracing/deErrorTrace.h"
#include "dragengine/errortracing/deErrorTracePoint.h"
#include "dragengine/errortracing/deErrorTraceValue.h"

#include <libdscript/exceptions.h>
#include <libdscript/dsMemoryManager.h>


// native functions
/////////////////////

// static public func float getElapsedTime()
deClassEngine::nfGetElapsedTime::nfGetElapsedTime( const sInitData &init ) :
dsFunction( init.clsEngine, "getElapsedTime", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsFloat ){
}
void deClassEngine::nfGetElapsedTime::RunFunction( dsRunTime *rt, dsValue* ){
	const deEngine &gameEngine = *( ( deClassEngine* )GetOwnerClass() )->GetDS().GetGameEngine();
	rt->PushFloat(gameEngine.GetElapsedTime());
}

// static public func void resetTimers()
deClassEngine::nfResetTimers::nfResetTimers( const sInitData &init ) :
dsFunction( init.clsEngine, "resetTimers", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
}
void deClassEngine::nfResetTimers::RunFunction( dsRunTime*, dsValue* ){
	deEngine &gameEngine = *( ( deClassEngine* )GetOwnerClass() )->GetDS().GetGameEngine();
	gameEngine.ResetTimers();
}

// static public func Game getGame()
deClassEngine::nfGetGame::nfGetGame( const sInitData &init ) :
dsFunction( init.clsEngine, "getGame", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsGame ){
}
void deClassEngine::nfGetGame::RunFunction( dsRunTime *rt, dsValue* ){
	deScriptingDragonScript &ds = ((deClassEngine*)GetOwnerClass())->GetDS();
	rt->PushValue( ds.GetGameObject() );
}

// static public func int getFPSRate()
deClassEngine::nfGetFPSRate::nfGetFPSRate( const sInitData &init ) :
dsFunction( init.clsEngine, "getFPSRate", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassEngine::nfGetFPSRate::RunFunction( dsRunTime *rt, dsValue* ){
	const deEngine &gameEngine = *( ( deClassEngine* )GetOwnerClass() )->GetDS().GetGameEngine();
	rt->PushInt( gameEngine.GetFPSRate() );
}

// static public func void quit()
deClassEngine::nfQuit::nfQuit( const sInitData &init ) :
dsFunction( init.clsEngine, "quit", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
}
void deClassEngine::nfQuit::RunFunction( dsRunTime*, dsValue* ){
	( ( deClassEngine* )GetOwnerClass() )->GetDS().GetGameEngine()->Quit();
}



// static public func void log( String message )
deClassEngine::nfLog::nfLog( const sInitData &init ) :
dsFunction( init.clsEngine, "log", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString );
}
void deClassEngine::nfLog::RunFunction( dsRunTime *rt, dsValue* ){
	( ( deClassEngine* )GetOwnerClass() )->GetDS().LogInfo( rt->GetValue( 0 )->GetString() );
}



// static public func bool getEditMode()
deClassEngine::nfGetEditMode::nfGetEditMode( const sInitData &init ) :
dsFunction( init.clsEngine, "getEditMode", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBoolean ){
}
void deClassEngine::nfGetEditMode::RunFunction( dsRunTime *rt, dsValue* ){
	const deEngine &gameEngine = *( ( deClassEngine* )GetOwnerClass() )->GetDS().GetGameEngine();
	rt->PushBool( gameEngine.GetScriptingSystem()->GetEditMode() );
}



// public static func int loadingResourceCount()
deClassEngine::nfLoadingResourceCount::nfLoadingResourceCount( const sInitData &init ) : dsFunction( init.clsEngine,
"loadingResourceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassEngine::nfLoadingResourceCount::RunFunction( dsRunTime *rt, dsValue* ){
	rt->PushInt( ( ( deClassEngine* )GetOwnerClass() )->GetDS().GetResourceLoader()->GetTaskCount() );
}



// public static func Dictionary getScriptModuleStats()
deClassEngine::nfGetScriptModuleStats::nfGetScriptModuleStats( const sInitData &init ) :
dsFunction( init.clsEngine, "getScriptModuleStats", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsDictionary ){
}
void deClassEngine::nfGetScriptModuleStats::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	const dsEngine &sengine = *ds.GetScriptEngine();
	
	dsValue * const valueStats = rt->CreateValue( sengine.GetClassDictionary() );
	
	try{
		rt->CreateObject( valueStats, sengine.GetClassDictionary(), 0 );
		
		// StringID table string count
		rt->PushInt( ds.GetClassStringID()->GetStringCount() );
		rt->PushString( "StringID.StringCount" );
		rt->RunFunction( valueStats, "setAt", 2 );
		
		// Module package class count
		const dsPackage &packageDE = *sengine.GetPackage( "Drag[en]gine Classes" );
		
		rt->PushInt( packageDE.GetClassCount() );
		rt->PushString( "ScriptEngine.Packages.Dragengine.ClassCount" );
		rt->RunFunction( valueStats, "setAt", 2 );
		
		rt->PushInt( packageDE.GetScriptCount() );
		rt->PushString( "ScriptEngine.Packages.Dragengine.ScriptClassCount" );
		rt->RunFunction( valueStats, "setAt", 2 );
		
		rt->PushInt( packageDE.GetHostClassCount() );
		rt->PushString( "ScriptEngine.Packages.Dragengine.HostClassCount" );
		rt->RunFunction( valueStats, "setAt", 2 );
		
		// Game package class count
		const dsPackage &packageGame = *sengine.GetPackage( "GamePackage" );
		
		rt->PushInt( packageGame.GetClassCount() );
		rt->PushString( "ScriptEngine.Packages.Game.ClassCount" );
		rt->RunFunction( valueStats, "setAt", 2 );
		
		rt->PushInt( packageGame.GetScriptCount() );
		rt->PushString( "ScriptEngine.Packages.Game.ScriptClassCount" );
		rt->RunFunction( valueStats, "setAt", 2 );
		
		rt->PushInt( packageGame.GetHostClassCount() );
		rt->PushString( "ScriptEngine.Packages.Game.HostClassCount" );
		rt->RunFunction( valueStats, "setAt", 2 );
		
		// memory manager
		//dsMemoryManager &memoryManager = *sengine.GetMemoryManager();
		
		// finished
		rt->PushValue( valueStats );
		rt->FreeValue( valueStats );
		
	}catch( ... ){
		if( valueStats ){
			rt->FreeValue( valueStats );
		}
		throw;
	}
}



// public static func String getCompatibleVersion()
deClassEngine::nfGetCompatibleVersion::nfGetCompatibleVersion( const sInitData &init ) :
dsFunction( init.clsEngine, "getCompatibleVersion", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
}
void deClassEngine::nfGetCompatibleVersion::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushString( ds.GetCompatibleVersion().version );
}

// public static func int getCompatibleVersionMajor()
deClassEngine::nfGetCompatibleVersionMajor::nfGetCompatibleVersionMajor( const sInitData &init ) :
dsFunction( init.clsEngine, "getCompatibleVersionMajor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassEngine::nfGetCompatibleVersionMajor::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetCompatibleVersion().major );
}

// public static func int getCompatibleVersionMinor()
deClassEngine::nfGetCompatibleVersionMinor::nfGetCompatibleVersionMinor( const sInitData &init ) :
dsFunction( init.clsEngine, "getCompatibleVersionMinor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassEngine::nfGetCompatibleVersionMinor::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetCompatibleVersion().minor );
}

// public static func int getCompatibleVersionPatch()
deClassEngine::nfGetCompatibleVersionPatch::nfGetCompatibleVersionPatch( const sInitData &init ) :
dsFunction( init.clsEngine, "getCompatibleVersionPatch", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassEngine::nfGetCompatibleVersionPatch::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetCompatibleVersion().patch );
}


// public static func String getModuleVersion()
deClassEngine::nfGetModuleVersion::nfGetModuleVersion( const sInitData &init ) :
dsFunction( init.clsEngine, "getModuleVersion", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
}
void deClassEngine::nfGetModuleVersion::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushString( ds.GetModuleVersion().version );
}

// public static func int getModuleVersionMajor()
deClassEngine::nfGetModuleVersionMajor::nfGetModuleVersionMajor( const sInitData &init ) :
dsFunction( init.clsEngine, "getModuleVersionMajor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassEngine::nfGetModuleVersionMajor::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetModuleVersion().major );
}

// public static func int getModuleVersionMinor()
deClassEngine::nfGetModuleVersionMinor::nfGetModuleVersionMinor( const sInitData &init ) :
dsFunction( init.clsEngine, "getModuleVersionMinor", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassEngine::nfGetModuleVersionMinor::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetModuleVersion().minor );
}

// public static func int getModuleVersionPatch()
deClassEngine::nfGetModuleVersionPatch::nfGetModuleVersionPatch( const sInitData &init ) :
dsFunction( init.clsEngine, "getModuleVersionPatch", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInteger ){
}
void deClassEngine::nfGetModuleVersionPatch::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushInt( ds.GetModuleVersion().patch );
}




// static public func bool getDefaultEnableGI()
deClassEngine::nfGetDefaultEnableGI::nfGetDefaultEnableGI( const sInitData &init ) :
dsFunction( init.clsEngine, "getDefaultEnableGI", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBoolean ){
}
void deClassEngine::nfGetDefaultEnableGI::RunFunction( dsRunTime *rt, dsValue* ){
	rt->PushBool( ( ( ( deClassEngine* )GetOwnerClass() )->GetDefaultEnableGI() ) );
}

// static public func void setDefaultEnableGI( bool enable )
deClassEngine::nfSetDefaultEnableGI::nfSetDefaultEnableGI( const sInitData &init ) :
dsFunction( init.clsEngine, "setDefaultEnableGI", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsBoolean ); // enable
}
void deClassEngine::nfSetDefaultEnableGI::RunFunction( dsRunTime *rt, dsValue* ){
	( ( deClassEngine* )GetOwnerClass() )->SetDefaultEnableGI( rt->GetValue( 0 )->GetBool() );
}



// Class deClassEngine
////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEngine::deClassEngine( deScriptingDragonScript &ds ) :
dsClass("Engine", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS( ds ),
pDefaultEnableGI( false )
{
	GetParserInfo()->SetParent( DENS_DRAGENGINE );
	GetParserInfo()->SetBase( "Object" );
	
	p_SetNativeDataSize( 0 );
}

deClassEngine::~deClassEngine(){
}



// Management
///////////////

void deClassEngine::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsEngine = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInteger = engine->GetClassInt();
	init.clsFloat = engine->GetClassFloat();
	init.clsBoolean = engine->GetClassBool();
	init.clsString = engine->GetClassString();
	init.clsDictionary = engine->GetClassDictionary();
	init.clsWindow = engine->GetClass( DECN_WINDOW );
	init.clsGame = pDS.GetClassGame();
	
	// add functions
	AddFunction( new nfGetElapsedTime( init ) );
	AddFunction( new nfResetTimers( init ) );
	AddFunction( new nfGetGame( init ) );
	AddFunction( new nfGetFPSRate( init ) );
	AddFunction( new nfQuit( init ) );
	AddFunction( new nfGetEditMode( init ) );
	AddFunction( new nfLoadingResourceCount( init ) );
	AddFunction( new nfLog( init ) );
	AddFunction( new nfGetScriptModuleStats( init ) );
	
	AddFunction( new nfGetCompatibleVersion( init ) );
	AddFunction( new nfGetCompatibleVersionMajor( init ) );
	AddFunction( new nfGetCompatibleVersionMinor( init ) );
	AddFunction( new nfGetCompatibleVersionPatch( init ) );
	
	AddFunction( new nfGetModuleVersion( init ) );
	AddFunction( new nfGetModuleVersionMajor( init ) );
	AddFunction( new nfGetModuleVersionMinor( init ) );
	AddFunction( new nfGetModuleVersionPatch( init ) );
	
	AddFunction( new nfGetDefaultEnableGI( init ) );
	AddFunction( new nfSetDefaultEnableGI( init ) );

	// calculate member offsets
	CalcMemberOffsets();
}

void deClassEngine::SetDefaultEnableGI( bool enable ){
	pDefaultEnableGI = enable;
}
