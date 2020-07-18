/* 
 * Drag[en]gine Game Engine
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

#include "deBaseSystem.h"
#include "deModuleSystem.h"
#include "modules/deLoadableModule.h"
#include "../deEngine.h"
#include "../common/exceptions.h"
#include "../logger/deLogger.h"
#include "../parallel/deParallelProcessing.h"



// Definitions
////////////////

#define LOGSOURCE "Dragengine"



// Class deBaseSystem
///////////////////////

// Constructor, destructor
////////////////////////////

deBaseSystem::deBaseSystem( deEngine *engine, const char *systemName, int requiredModuleType ){
	if( ! engine || ! systemName ) DETHROW( deeInvalidParam );
	
	pEngine = engine;
	pSystemName = NULL;
	pActiveLoadableModule = NULL;
	pRequiredModuleType = requiredModuleType;
	pFailed = false;
	pRunning = false;
	
	try{
		pSystemName = new char[ strlen( systemName ) + 1 ];
		if( ! pSystemName ) DETHROW( deeOutOfMemory );
		strcpy( pSystemName, systemName );
		
	}catch( const deException & ){
		if( pSystemName ) delete [] pSystemName;
		throw;
	}
}

deBaseSystem::~deBaseSystem(){
	if( pRunning ) Stop();
	
	ClearPermanents();
	
	if( pActiveLoadableModule ){
		pActiveLoadableModule->FreeReference();
		pActiveLoadableModule = NULL;
	}
	
	if( pSystemName ) delete [] pSystemName;
}



// Management
///////////////

void deBaseSystem::SetHasFailed( bool hasFailed ){
	pFailed = hasFailed;
}

bool deBaseSystem::CanStart(){
	return pActiveLoadableModule && pActiveLoadableModule->IsLoaded();
}

void deBaseSystem::Start(){
	if( pRunning || ! CanStart() ){
		DETHROW( deeInvalidAction );
	}
	
	LogInfoFormat( "Starting %s module %s", GetSystemName(), pActiveLoadableModule->GetName().GetString() );
	
	deParallelProcessing &parallelProcessing = pEngine->GetParallelProcessing();
	const bool resumeParallelProcessing = ! parallelProcessing.GetPaused();
	
	if( resumeParallelProcessing ){
		parallelProcessing.Pause();
	}
	
	try{
		// switch to running mode and lock module
		pActiveLoadableModule->Lock();
		pRunning = true;
		
		// post start
		PostStart();
		
	}catch( const deException & ){
		Stop();
		if( resumeParallelProcessing ){
			parallelProcessing.Resume();
		}
		throw;
	}
	
	if( resumeParallelProcessing ){
		parallelProcessing.Resume();
	}
}

void deBaseSystem::Stop(){
	if( ! pRunning || ! pActiveLoadableModule ){
		return;
	}
	
	LogInfoFormat( "Stopping %s module %s", GetSystemName(), pActiveLoadableModule->GetName().GetString() );
	
	deParallelProcessing &parallelProcessing = pEngine->GetParallelProcessing();
	const bool resumeParallelProcessing = ! parallelProcessing.GetPaused();
	
	if( resumeParallelProcessing ){
		parallelProcessing.Pause();
	}
	
	try{
		PreStop();
		
		// switch to non-running mode and unlock module
		pRunning = false;
		pActiveLoadableModule->Unlock();
		
	}catch( const deException & ){
		if( resumeParallelProcessing ){
			parallelProcessing.Resume();
		}
		throw;
	}
	
	if( resumeParallelProcessing ){
		parallelProcessing.Resume();
	}
}



// Helper Functions
/////////////////////

void deBaseSystem::CheckAndActivateFirst( deModuleSystem::eModuleTypes type ){
	deModuleSystem *moduleSystem = pEngine->GetModuleSystem();
	
	// check for modules
	if( moduleSystem->GetLoadedModuleCountFor( type ) == 0 ){
		LogErrorFormat( "No loaded %s modules found", GetSystemName() );
		DETHROW( deeNoModuleFound );
	}
	
	// select first module as default one
	SetActiveModule( moduleSystem->GetFirstLoadedModuleFor( type ) );
}



// Extend those
/////////////////

void deBaseSystem::ClearPermanents(){
}

void deBaseSystem::SetActiveModule( deLoadableModule *module ){
	if( ! module || ! module->IsLoaded() || module->GetType() != pRequiredModuleType ){
		DETHROW( deeInvalidParam );
	}
	if( pRunning ){
		DETHROW( deeInvalidAction );
	}
	
	LogInfoFormat( "Activating %s module %s %s", GetSystemName(),
		module->GetName().GetString(), module->GetVersion().GetString() );
	
	// set new module
	if( pActiveLoadableModule ){
		pActiveLoadableModule->FreeReference();
	}
	
	pActiveLoadableModule = module;
	
	if( pActiveLoadableModule ){
		pActiveLoadableModule->AddReference();
	}
}



// Overload those
///////////////////

void deBaseSystem::PostStart(){
}

void deBaseSystem::PreStop(){
}



// Debugging
//////////////

void deBaseSystem::LogInfo( const char *message ){
	pEngine->GetLogger()->LogInfo( LOGSOURCE, message );
}

void deBaseSystem::LogInfoFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	pEngine->GetLogger()->LogInfoFormatUsing( LOGSOURCE, message, list );
	va_end( list );
}

void deBaseSystem::LogInfoFormatUsing( const char *message, va_list args ){
	pEngine->GetLogger()->LogInfoFormatUsing( LOGSOURCE, message, args );
}

void deBaseSystem::LogWarn( const char *message ){
	pEngine->GetLogger()->LogWarn( LOGSOURCE, message );
}

void deBaseSystem::LogWarnFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	pEngine->GetLogger()->LogWarnFormatUsing( LOGSOURCE, message, list );
	va_end( list );
}

void deBaseSystem::LogWarnFormatUsing( const char *message, va_list args ){
	pEngine->GetLogger()->LogWarnFormatUsing( LOGSOURCE, message, args );
}

void deBaseSystem::LogError( const char *message ){
	pEngine->GetLogger()->LogError( LOGSOURCE, message );
}

void deBaseSystem::LogErrorFormat( const char *message, ... ){
	va_list list;
	va_start( list, message );
	pEngine->GetLogger()->LogErrorFormatUsing( LOGSOURCE, message, list );
	va_end( list );
}

void deBaseSystem::LogErrorFormatUsing( const char *message, va_list args ){
	pEngine->GetLogger()->LogErrorFormatUsing( LOGSOURCE, message, args );
}

void deBaseSystem::LogException( const deException &exception ){
	pEngine->GetLogger()->LogException( LOGSOURCE, exception );
}
