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

#include "deResourceLoaderTask.h"
#include "../deResourceLoader.h"
#include "../../deFileResource.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../filesystem/deVirtualFileSystem.h"
#include "../../../logger/deLogger.h"



// Definitions
////////////////

#define LOGSOURCE "Resource Loader"



// Class deResourceLoaderTask
///////////////////////////////

// Constructor, destructor
////////////////////////////

deResourceLoaderTask::deResourceLoaderTask( deEngine &engine,
deResourceLoader &resourceLoader, deVirtualFileSystem *vfs,
const char *path, deResourceLoader::eResourceType resourceType ) :
deParallelTask( NULL ),
pEngine( engine ),
pResourceLoader( resourceLoader ),
pVFS( vfs ),
pPath( path ),
pResourceType( resourceType ),
pState( esPending ),
pType( etRead )
{
	if( ! vfs ){
		DETHROW( deeInvalidParam );
	}
	
	SetMarkFinishedAfterRun( false );
}

deResourceLoaderTask::~deResourceLoaderTask(){
}



// Management
///////////////

bool deResourceLoaderTask::Matches( deVirtualFileSystem *vfs, const char *path,
deResourceLoader::eResourceType resourceType ) const{
	if( ! vfs || ! path ){
		DETHROW( deeInvalidParam );
	}
	return pVFS == vfs && pPath == path && resourceType == pResourceType;
}



// Debugging
//////////////

decString deResourceLoaderTask::GetDebugName() const{
	return "ResLoad";
}

decString deResourceLoaderTask::GetDebugDetails() const{
	return pPath;
}



// Protected Functions
////////////////////////

void deResourceLoaderTask::SetResource( deFileResource *resource ){
	pResource = resource;
}

void deResourceLoaderTask::SetState( eStates state ){
	pState = state;
}

void deResourceLoaderTask::SetType( eTypes type ){
	pType = type;
}

void deResourceLoaderTask::LogCreateEnter(){
	if( ! pResourceLoader.GetOutputDebugMessages() ){
		return;
	}
	
	const decString debugName( GetDebugName() );
	pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Task(%s)[%s] Create Enter",
		debugName.GetString(), pPath.GetString() );
	pDebugTimer.Reset();
}

void deResourceLoaderTask::LogCreateExit(){
	if( ! pResourceLoader.GetOutputDebugMessages() ){
		return;
	}
	
	const decString debugName( GetDebugName() );
	const float debugElapsed( pDebugTimer.GetElapsedTime() );
	pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Task(%s)[%s] Create Exit (%.2fs)",
		debugName.GetString(), pPath.GetString(), debugElapsed );
}

void deResourceLoaderTask::LogRunEnter(){
	if( ! pResourceLoader.GetOutputDebugMessages() ){
		return;
	}
	
	const decString debugName( GetDebugName() );
	pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Task(%s)[%s] Run Enter",
		debugName.GetString(), pPath.GetString() );
	pDebugTimer.Reset();
}

void deResourceLoaderTask::LogRunExit(){
	if( ! pResourceLoader.GetOutputDebugMessages() ){
		return;
	}
	
	const decString debugName( GetDebugName() );
	const float debugElapsed( pDebugTimer.GetElapsedTime() );
	pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Task(%s)[%s] Run Exit (%.2fs)",
		debugName.GetString(), pPath.GetString(), debugElapsed );
}

void deResourceLoaderTask::LogFinishedEnter(){
	if( ! pResourceLoader.GetOutputDebugMessages() ){
		return;
	}
	
	const decString debugName( GetDebugName() );
	pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Task(%s)[%s] Finished Enter",
		debugName.GetString(), pPath.GetString() );
	pDebugTimer.Reset();
}

void deResourceLoaderTask::LogFinishedExit(){
	if( ! pResourceLoader.GetOutputDebugMessages() ){
		LogFinished();
		return;
	}
	
	const decString debugName( GetDebugName() );
	const float debugElapsed( pDebugTimer.GetElapsedTime() );
	pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Task(%s)[%s] Finished Exit (%.2fs)",
		debugName.GetString(), pPath.GetString(), debugElapsed );
	
	LogFinished();
}

void deResourceLoaderTask::LogFinished(){
	if( ! pResourceLoader.GetOutputDebugMessages() && pState != esFailed && ! IsCancelled() ){
		return;
	}
	
	const decString debugName( GetDebugName() );
	
	const char *stateName = "?";
	switch( pState ){
	case esPending:
		stateName = "Pending";
		break;
		
	case esSucceeded:
		stateName = "Succeeded";
		break;
		
	case esFailed:
		stateName = "Failed";
		break;
	}
	
	pEngine.GetLogger()->LogInfoFormat( LOGSOURCE, "Task(%s)[%s] Finished: res=%p state=%s",
		debugName.GetString(), pPath.GetString(), ( deResource* )pResource, stateName );
}
