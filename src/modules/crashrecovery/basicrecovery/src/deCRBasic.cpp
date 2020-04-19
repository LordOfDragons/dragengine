/* 
 * Drag[en]gine Basic Crash Recovery Module
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

#include "deCRBasic.h"
#include "decrbWindowMain.h"
#include "corefault/decrbCoreFault.h"

#include <dragengine/deEngine.h>
#include <dragengine/app/deOS.h>
#include <dragengine/systems/deGraphicSystem.h>
#include <dragengine/systems/deInputSystem.h>
#include <dragengine/systems/modules/deLoadableModule.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>



#ifdef __cplusplus
extern "C" {
#endif
MOD_ENTRY_POINT_ATTR deBaseModule *BRCreateModule( deLoadableModule *loadableModule );
#ifdef  __cplusplus
}
#endif


// Entry Point
////////////////

deBaseModule *BRCreateModule( deLoadableModule *loadableModule ){
	deBaseModule *module = NULL;
	
	try{
		module = new deCRBasic( *loadableModule );
		
	}catch( const deException & ){
		return NULL;
	}
	
	return module;
}



// class deCRBasic
//////////////////////////

// constructor, destructor
deCRBasic::deCRBasic( deLoadableModule &loadableModule ) :
deBaseCrashRecoveryModule( loadableModule ){
	pQuitEngine = true;
	pCoreFault = NULL;
}
deCRBasic::~deCRBasic(){
}



// Management
///////////////

bool deCRBasic::Init(){
	try{
		pCoreFault = new decrbCoreFault( this );
		
	}catch( const deException &e ){
		LogException( e );
		return false;
	}
	
	return true;
}

void deCRBasic::CleanUp(){
	if( pCoreFault ){
		delete pCoreFault;
		pCoreFault = NULL;
	}
}



// crash management
bool deCRBasic::RecoverFromError(){
	char argBuffer[ 256 ];
	sprintf( argBuffer, "dummy" );
	int argc = 1;
	char *args[ 1 ] = { argBuffer };
	deEngine *engine = GetGameEngine();
	FXApp *app = NULL;
	
	// print error to console in case stopping graphic system crashes due to very ugly circumstances
	//LogTrace();
	
	// shut down graphic system
	engine->GetGraphicSystem()->Stop();
	//engine->GetOS()->OnFrameUpdate();
	//engine->GetInputSystem()->ProcessEvents();
	// show window
	try{
		app = new FXApp( "Drag[en]gine Crash Recovery", "RPTD" );
		if( ! app ) DETHROW( deeOutOfMemory );
		app->init( argc, args );
		new decrbWindowMain( app, this );
		app->create();
		app->run();
		delete app;
	}catch( const deException &e ){
		if( app ) delete app;
		e.PrintError();
		pQuitEngine = true;
	}
	// if we are not quitting restart system
	if( pQuitEngine ){
		LogInfo( "shuting down engine." );
		return false;
	}else{
		try{
			engine->GetGraphicSystem()->Start();
		}catch( const deException &e ){
			engine->GetGraphicSystem()->Stop();
			LogError( "restarting engine systems failed, quitting." );
			return false;
		}
		return true;
	}
}

// internal functions for module classes only
void deCRBasic::SetQuitEngine( bool quitEngine ){
	pQuitEngine = quitEngine;
}

void deCRBasic::LogTrace(){
	const deErrorTrace &trace = *GetGameEngine()->GetErrorTrace();
	const int pointCount = trace.GetPointCount();
	int i;
	
	LogError( decrbWindowMain::GetTextForError( trace.GetError() ) );
	
	for( i=0; i<pointCount; i++ ){
		const deErrorTracePoint &tracePoint = *trace.GetPoint( i );
		
		if( tracePoint.GetSourceModule() ){
			LogErrorFormat( "Trace %i: %s, %s at %i", i + 1,
				tracePoint.GetSourceModule()->GetName().GetString(),
				tracePoint.GetSourceFunction(),
				tracePoint.GetSourceLine() );
			
		}else{
			LogErrorFormat( "Trace %i: Game Engine, %s at %i", i + 1,
				tracePoint.GetSourceFunction(),
				tracePoint.GetSourceLine() );
		}
		
		const int valueCount = tracePoint.GetValueCount();
		int j;
		for( j=0; j<valueCount; j++ ){
			LogTraceSubValues( *tracePoint.GetValue( j ), "  " );
		}
	}
}

void deCRBasic::LogTraceSubValues( const deErrorTraceValue &traceValue, const char *indent ){
	LogErrorFormat( "- %s = %s", traceValue.GetName(), traceValue.GetValue() );
	
	const int valueCount = traceValue.GetSubValueCount();
	if( valueCount == 0 ){
		return;
	}
	
	const decString childIndent( decString( "  " ) + indent );
	int i;
	
	for( i=0; i<valueCount; i++ ){
		LogTraceSubValues( *traceValue.GetSubValue( i ), indent );
	}
}
