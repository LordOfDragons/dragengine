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
deClassEngine::nfGetElapsedTime::nfGetElapsedTime(const sInitData &init) : dsFunction(
init.clsEng, "getElapsedTime", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC,
init.clsFlt){
}
void deClassEngine::nfGetElapsedTime::RunFunction(dsRunTime *RT, dsValue *This){
	deEngine *gameEngine = ((deClassEngine*)GetOwnerClass())->GetGameEngine();
	RT->PushFloat(gameEngine->GetElapsedTime());
}

// static public func void resetTimers()
deClassEngine::nfResetTimers::nfResetTimers( const sInitData &init ) : dsFunction( init.clsEng,
"resetTimers", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
}
void deClassEngine::nfResetTimers::RunFunction(dsRunTime *RT, dsValue *This){
	deEngine *gameEngine = ( ( deClassEngine* )GetOwnerClass() )->GetGameEngine();
	gameEngine->ResetTimers();
}

// static public func Game getGame()
deClassEngine::nfGetGame::nfGetGame(const sInitData &init) : dsFunction(
init.clsEng, "getGame", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsGame){
}
void deClassEngine::nfGetGame::RunFunction(dsRunTime *RT, dsValue *This){
	deScriptingDragonScript *scrMgr = ((deClassEngine*)GetOwnerClass())->GetScriptModule();
	RT->PushValue(scrMgr->GetGameObject());
}

// static public func int getFPSRate()
deClassEngine::nfGetFPSRate::nfGetFPSRate(const sInitData &init) : dsFunction(
init.clsEng, "getFPSRate", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC,
init.clsInt){
}
void deClassEngine::nfGetFPSRate::RunFunction(dsRunTime *RT, dsValue *This){
	deEngine *gameEngine = ((deClassEngine*)GetOwnerClass())->GetGameEngine();
	RT->PushInt(gameEngine->GetFPSRate());
}

// static public func void quit()
deClassEngine::nfQuit::nfQuit(const sInitData &init) : dsFunction(
init.clsEng, "quit", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid){
}
void deClassEngine::nfQuit::RunFunction(dsRunTime *RT, dsValue *This){
	((deClassEngine*)GetOwnerClass())->GetGameEngine()->Quit();
}



// static public func void log( String message )
deClassEngine::nfLog::nfLog( const sInitData &init ) :
dsFunction( init.clsEng, "log", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsVoid ){
	p_AddParameter( init.clsStr );
}
void deClassEngine::nfLog::RunFunction( dsRunTime *rt, dsValue* ){
	( ( deClassEngine* )GetOwnerClass() )->GetScriptModule()->LogInfo( rt->GetValue( 0 )->GetString() );
}



// static public func bool getEditMode()
deClassEngine::nfGetEditMode::nfGetEditMode( const sInitData &init ) : dsFunction( init.clsEng,
"getEditMode", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsBool ){
}
void deClassEngine::nfGetEditMode::RunFunction( dsRunTime *rt, dsValue *myself ){
	deEngine *gameEngine = ( ( deClassEngine* )GetOwnerClass() )->GetGameEngine();
	rt->PushBool( gameEngine->GetScriptingSystem()->GetEditMode() );
}



// public static func int loadingResourceCount()
deClassEngine::nfLoadingResourceCount::nfLoadingResourceCount( const sInitData &init ) : dsFunction( init.clsEng,
"loadingResourceCount", DSFT_FUNCTION, DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsInt ){
}
void deClassEngine::nfLoadingResourceCount::RunFunction( dsRunTime *rt, dsValue *myself ){
	rt->PushInt( ( ( deClassEngine* )GetOwnerClass() )->GetScriptModule()->GetResourceLoader()->GetTaskCount() );
}



// public static func Dictionary getScriptModuleStats()
deClassEngine::nfGetScriptModuleStats::nfGetScriptModuleStats( const sInitData &init ) :
dsFunction( init.clsEng, "getScriptModuleStats", DSFT_FUNCTION,
DSTM_PUBLIC | DSTM_NATIVE | DSTM_STATIC, init.clsDict ){
}
void deClassEngine::nfGetScriptModuleStats::RunFunction( dsRunTime *rt, dsValue *myself ){
	const deScriptingDragonScript &ds = *( ( ( deClassEngine* )GetOwnerClass() )->GetScriptModule() );
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



// class deClassEngine
////////////////////////
// constructor
deClassEngine::deClassEngine(deEngine *GameEngine, deScriptingDragonScript *ScrMgr) :
dsClass("Engine", DSCT_CLASS, DSTM_PUBLIC | DSTM_NATIVE){
	if(!GameEngine || !ScrMgr) DSTHROW(dueInvalidParam);
	// prepare
	p_gameEngine = GameEngine;
	p_scrMgr = ScrMgr;
	// set parser info
	GetParserInfo()->SetParent(DENS_DRAGENGINE);
	GetParserInfo()->SetBase("Object");
	// do the rest
	p_SetNativeDataSize(0);
}
deClassEngine::~deClassEngine(){
}

// management
void deClassEngine::CreateClassMembers(dsEngine *engine){
	sInitData init;
	
	// store classes
	init.clsEng = this;
	init.clsVoid = engine->GetClassVoid();
	init.clsInt = engine->GetClassInt();
	init.clsFlt = engine->GetClassFloat();
	init.clsBool = engine->GetClassBool();
	init.clsStr = engine->GetClassString();
	init.clsDict = engine->GetClassDictionary();
	init.clsWin = engine->GetClass(DECN_WINDOW);
	init.clsGame = p_scrMgr->GetClassGame();
	
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

	// calculate member offsets
	CalcMemberOffsets();
}
