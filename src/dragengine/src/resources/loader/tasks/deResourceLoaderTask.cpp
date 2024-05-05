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
