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

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/service/deServiceManager.h>
#include <dragengine/systems/deScriptingSystem.h>

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

// static public func void restart(String info)
deClassEngine::nfRestart::nfRestart( const sInitData &init ) :
dsFunction( init.clsEngine, "restart", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsString );
}
void deClassEngine::nfRestart::RunFunction( dsRunTime *rt, dsValue* ){
	( ( deClassEngine* )GetOwnerClass() )->GetDS().RequestRestart( rt->GetValue( 0 )->GetString() );
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



// public static func bool getAppActive()
deClassEngine::nfGetAppActive::nfGetAppActive( const sInitData &init ) :
dsFunction( init.clsEngine, "getAppActive", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBoolean ){
}
void deClassEngine::nfGetAppActive::RunFunction( dsRunTime *rt, dsValue* ){
	rt->PushBool( ( ( deClassEngine* )GetOwnerClass() )->GetDS().GetGameEngine()->GetOS()->GetAppActive() );
}



// public static func void updateResourceLoading()
deClassEngine::nfUpdateResourceLoading::nfUpdateResourceLoading( const sInitData &init ) :
dsFunction( init.clsEngine, "updateResourceLoading", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
}
void deClassEngine::nfUpdateResourceLoading::RunFunction( dsRunTime*, dsValue* ){
	( ( deClassEngine* )GetOwnerClass() )->GetDS().GetResourceLoader()->OnFrameUpdate();
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

// public static func bool isCompatibleVersionOlder(int major, int minor)
deClassEngine::nfIsCompatibleVersionOlder::nfIsCompatibleVersionOlder( const sInitData &init ) :
dsFunction( init.clsEngine, "isCompatibleVersionOlder", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBoolean ){
	p_AddParameter( init.clsInteger );
	p_AddParameter( init.clsInteger );
}
void deClassEngine::nfIsCompatibleVersionOlder::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	const deScriptingDragonScript::sModuleVersion &version = ds.GetCompatibleVersion();
	const int major = rt->GetValue( 0 )->GetInt();
	const int minor = rt->GetValue( 1 )->GetInt();
	rt->PushBool( version.major < major || ( version.major == major && version.minor < minor ) );
}

// public static func bool isCompatibleVersionNewer(int major, int minor)
deClassEngine::nfIsCompatibleVersionNewer::nfIsCompatibleVersionNewer( const sInitData &init ) :
dsFunction( init.clsEngine, "isCompatibleVersionNewer", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBoolean ){
	p_AddParameter( init.clsInteger );
	p_AddParameter( init.clsInteger );
}
void deClassEngine::nfIsCompatibleVersionNewer::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	const deScriptingDragonScript::sModuleVersion &version = ds.GetCompatibleVersion();
	const int major = rt->GetValue( 0 )->GetInt();
	const int minor = rt->GetValue( 1 )->GetInt();
	rt->PushBool( version.major > major || ( version.major == major && version.minor > minor ) );
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

// public static func bool isModuleVersionOlder(int major, int minor)
deClassEngine::nfIsModuleVersionOlder::nfIsModuleVersionOlder( const sInitData &init ) :
dsFunction( init.clsEngine, "isModuleVersionOlder", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBoolean ){
	p_AddParameter( init.clsInteger );
	p_AddParameter( init.clsInteger );
}
void deClassEngine::nfIsModuleVersionOlder::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	const deScriptingDragonScript::sModuleVersion &version = ds.GetModuleVersion();
	const int major = rt->GetValue( 0 )->GetInt();
	const int minor = rt->GetValue( 1 )->GetInt();
	rt->PushBool( version.major < major || ( version.major == major && version.minor < minor ) );
}

// public static func bool isModuleVersionNewer(int major, int minor)
deClassEngine::nfIsModuleVersionNewer::nfIsModuleVersionNewer( const sInitData &init ) :
dsFunction( init.clsEngine, "isModuleVersionNewer", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBoolean ){
	p_AddParameter( init.clsInteger );
	p_AddParameter( init.clsInteger );
}
void deClassEngine::nfIsModuleVersionNewer::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	const deScriptingDragonScript::sModuleVersion &version = ds.GetModuleVersion();
	const int major = rt->GetValue( 0 )->GetInt();
	const int minor = rt->GetValue( 1 )->GetInt();
	rt->PushBool( version.major > major || ( version.major == major && version.minor > minor ) );
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

// static public func bool getDefaultEnableAuralization()
deClassEngine::nfGetDefaultEnableAuralization::nfGetDefaultEnableAuralization( const sInitData &init ) :
dsFunction( init.clsEngine, "getDefaultEnableAuralization", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBoolean ){
}
void deClassEngine::nfGetDefaultEnableAuralization::RunFunction( dsRunTime *rt, dsValue* ){
	rt->PushBool( ( ( ( deClassEngine* )GetOwnerClass() )->GetDefaultEnableAuralization() ) );
}

// static public func void setDefaultEnableAuralization(bool enable)
deClassEngine::nfSetDefaultEnableAuralization::nfSetDefaultEnableAuralization( const sInitData &init ) :
dsFunction( init.clsEngine, "setDefaultEnableAuralization", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsBoolean ); // enable
}
void deClassEngine::nfSetDefaultEnableAuralization::RunFunction( dsRunTime *rt, dsValue* ){
	( ( deClassEngine* )GetOwnerClass() )->SetDefaultEnableAuralization( rt->GetValue( 0 )->GetBool() );
}



// static public func String getUserLocaleLanguage()
deClassEngine::nfGetUserLocaleLanguage::nfGetUserLocaleLanguage( const sInitData &init ) :
dsFunction( init.clsEngine, "getUserLocaleLanguage", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
}
void deClassEngine::nfGetUserLocaleLanguage::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushString( ds.GetGameEngine()->GetOS()->GetUserLocaleLanguage() );
}

// static public func String getUserLocaleTerritory()
deClassEngine::nfGetUserLocaleTerritory::nfGetUserLocaleTerritory( const sInitData &init ) :
dsFunction( init.clsEngine, "getUserLocaleTerritory", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsString ){
}
void deClassEngine::nfGetUserLocaleTerritory::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	rt->PushString( ds.GetGameEngine()->GetOS()->GetUserLocaleTerritory() );
}



// static public func Set getSupportedServices()
deClassEngine::nfGetSupportedServices::nfGetSupportedServices( const sInitData &init ) :
dsFunction( init.clsEngine, "getSupportedServices", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsSet ){
}
void deClassEngine::nfGetSupportedServices::RunFunction( dsRunTime *rt, dsValue* ){
	const deScriptingDragonScript &ds = ( ( deClassEngine* )GetOwnerClass() )->GetDS();
	const decStringSet names( ds.GetGameEngine()->GetServiceManager()->GetAllSupportedSerices() );
	const dsEngine &sengine = *ds.GetScriptEngine();
	const int count = names.GetCount();
	int i;
	
	dsValue * const valueNames = rt->CreateValue( sengine.GetClassSet() );
	
	try{
		rt->CreateObject( valueNames, sengine.GetClassSet(), 0 );
		
		for( i=0; i<count; i++ ){
			rt->PushString( names.GetAt( i ) );
			rt->RunFunction( valueNames, "add", 1 );
		}
		
		rt->PushValue( valueNames );
		rt->FreeValue( valueNames );
		
	}catch( ... ){
		if( valueNames ){
			rt->FreeValue( valueNames );
		}
		throw;
	}
}



// Class deClassEngine
////////////////////////

// Constructor, Destructor
////////////////////////////

deClassEngine::deClassEngine( deScriptingDragonScript &ds ) :
dsClass("Engine", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE),
pDS( ds ),
pDefaultEnableGI( false ),
pDefaultEnableAuralization( true )
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
	init.clsSet = engine->GetClassSet();
	init.clsDictionary = engine->GetClassDictionary();
	init.clsWindow = engine->GetClass( DECN_WINDOW );
	init.clsGame = pDS.GetClassGame();
	
	// add functions
	AddFunction( new nfGetElapsedTime( init ) );
	AddFunction( new nfResetTimers( init ) );
	AddFunction( new nfGetGame( init ) );
	AddFunction( new nfGetFPSRate( init ) );
	AddFunction( new nfQuit( init ) );
	AddFunction( new nfRestart( init ) );
	AddFunction( new nfGetEditMode( init ) );
	AddFunction( new nfLoadingResourceCount( init ) );
	AddFunction( new nfLog( init ) );
	AddFunction( new nfGetScriptModuleStats( init ) );
	AddFunction( new nfGetAppActive( init ) );
	AddFunction( new nfUpdateResourceLoading( init ) );
	
	AddFunction( new nfGetCompatibleVersion( init ) );
	AddFunction( new nfGetCompatibleVersionMajor( init ) );
	AddFunction( new nfGetCompatibleVersionMinor( init ) );
	AddFunction( new nfGetCompatibleVersionPatch( init ) );
	AddFunction( new nfIsCompatibleVersionOlder( init ) );
	AddFunction( new nfIsCompatibleVersionNewer( init ) );
	
	AddFunction( new nfGetModuleVersion( init ) );
	AddFunction( new nfGetModuleVersionMajor( init ) );
	AddFunction( new nfGetModuleVersionMinor( init ) );
	AddFunction( new nfGetModuleVersionPatch( init ) );
	AddFunction( new nfIsModuleVersionOlder( init ) );
	AddFunction( new nfIsModuleVersionNewer( init ) );
	
	AddFunction( new nfGetDefaultEnableGI( init ) );
	AddFunction( new nfSetDefaultEnableGI( init ) );
	AddFunction( new nfGetDefaultEnableAuralization( init ) );
	AddFunction( new nfSetDefaultEnableAuralization( init ) );
	
	AddFunction( new nfGetUserLocaleLanguage( init ) );
	AddFunction( new nfGetUserLocaleTerritory( init ) );
	
	AddFunction( new nfGetSupportedServices( init ) );

	// calculate member offsets
	CalcMemberOffsets();
}

void deClassEngine::SetDefaultEnableGI( bool enable ){
	pDefaultEnableGI = enable;
}

void deClassEngine::SetDefaultEnableAuralization( bool enable ){
	pDefaultEnableAuralization = enable;
}
