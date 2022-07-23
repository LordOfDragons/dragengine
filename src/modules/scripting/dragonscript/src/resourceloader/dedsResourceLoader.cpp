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
#include <libdscript/exceptions.h>

#include "dedsResourceLoader.h"
#include "dedsResourceLoaderTask.h"
#include "../deScriptingDragonScript.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decTimer.h>
#include <dragengine/errortracing/deErrorTrace.h>
#include <dragengine/errortracing/deErrorTracePoint.h>
#include <dragengine/errortracing/deErrorTraceValue.h>
#include <dragengine/resources/deFileResource.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/loader/deResourceLoaderInfo.h>



// Class dedsResourceLoader
/////////////////////////////

// Constructor, destructor
////////////////////////////

dedsResourceLoader::dedsResourceLoader( deScriptingDragonScript *ds ){
	if( ! ds ) DSTHROW( dueInvalidParam );
	
	pDS = ds;
	
	pTasks = NULL;
	pTaskCount = 0;
	pTaskSize = 0;
}

dedsResourceLoader::~dedsResourceLoader(){
	CancelAllRequests();
	if( pTasks ) delete [] pTasks;
}



// Management
///////////////

void dedsResourceLoader::OnFrameUpdate(){
	deResourceLoader &resourceLoader = *pDS->GetGameEngine()->GetResourceLoader();
	const float maxUpdateTime = 0.25f;
	float elapsedUpdateTime = 0.0f;
	deResourceLoaderInfo info;
	deFileResource *resource;
	decTimer timer;
	int task;
	
	timer.Reset();
	
	while( elapsedUpdateTime < maxUpdateTime && resourceLoader.NextFinishedRequest( info ) ){
		task = pIndexOfTaskWith( info.GetPath(), info.GetResourceType() );
		
		if( task != -1 ){
			resource = info.GetResource();
			
			try{
				if( resource ){
					pTasks[ task ]->NotifyLoadingFinished( resource );
					
				}else{
					pTasks[ task ]->NotifyLoadingFailed();
				}
				
			}catch( const duException &e ){
				pDS->SetErrorTraceDS( e );
				deErrorTracePoint * const tracePoint = pDS->AddErrorTracePoint(
					"dedsResourceLoader::Update", __LINE__ );
				tracePoint->AddValue( "resource", info.GetPath() );
				pDS->pAddExceptionTrace( tracePoint );
				pDS->GetScriptEngine()->GetMainRunTime()->PrintExceptionTrace(); // really?
				pDS->LogExceptionDS( e );
				pDS->GetScriptEngine()->GetMainRunTime()->ClearExceptionTrace();
				DETHROW( deeScriptError );
				
			}catch( const deException &e ){
				pDS->SetErrorTrace( e );
				deErrorTracePoint * const tracePoint = pDS->AddErrorTracePoint(
					"dedsResourceLoader::Update", __LINE__ );
				tracePoint->AddValue( "resource", info.GetPath() );
				pDS->pAddExceptionTrace( tracePoint );
				pDS->GetScriptEngine()->GetMainRunTime()->PrintExceptionTrace(); // really?
				pDS->GetScriptEngine()->GetMainRunTime()->ClearExceptionTrace();
				pDS->LogException( e );
				throw;
			}
			
			pRemoveTaskFrom( task );
		}
		
		elapsedUpdateTime += timer.GetElapsedTime();
	}
}

void dedsResourceLoader::AddRequest( const char* filename,
deResourceLoader::eResourceType resourceType, dsRealObject* listener ){
	if( ! filename || ! listener ) DSTHROW( dueInvalidParam );
	int index = pIndexOfTaskWith( filename, resourceType );
	
	if( index == -1 ){
		if( pTaskCount == pTaskSize ){
			int newSize = pTaskSize * 3 / 2 + 1;
			dedsResourceLoaderTask **newArray = new dedsResourceLoaderTask*[ newSize ];
			if( pTasks ){
				memcpy( newArray, pTasks, sizeof( dedsResourceLoaderTask* ) * pTaskSize );
				delete [] pTasks;
			}
			pTasks = newArray;
			pTaskSize = newSize;
		}
		
		pTasks[ pTaskCount ] = new dedsResourceLoaderTask( pDS, filename, resourceType );
		if( ! pTasks[ pTaskCount ] ) DSTHROW( dueOutOfMemory );
		index = pTaskCount++;
		
		deEngine &engine = *pDS->GetGameEngine();
		engine.GetResourceLoader()->AddLoadRequest( engine.GetVirtualFileSystem(),
			filename, resourceType );
	}
	
	pTasks[ index ]->AddListener( listener );
}

void dedsResourceLoader::CancelRequest( const char* filename,
deResourceLoader::eResourceType resourceType, dsRealObject* listener ){
	if( ! filename || ! listener ) DSTHROW( dueInvalidParam );
	int index = pIndexOfTaskWith( filename, resourceType );
	if( index != -1 ){
		pTasks[ index ]->RemoveListener( listener );
		if( pTasks[ index ]->GetListenerCount() == 0 ){
			pRemoveTaskFrom( index );
		}
	}
}

void dedsResourceLoader::CancelAllRequests(){
	while( pTaskCount > 0 ){
		pTaskCount--;
		delete pTasks[ pTaskCount ];
	}
}



// Private Functions
//////////////////////

int dedsResourceLoader::pIndexOfTaskWith( const char* filename,
deResourceLoader::eResourceType resourceType ) const{
	int i;
	
	for( i=0; i<pTaskCount; i++ ){
		if( pTasks[ i ]->Matches( filename, resourceType ) ){
			return i;
		}
	}
	
	return -1;
}

void dedsResourceLoader::pRemoveTaskFrom( int index ){
	int i;
	
	delete pTasks[ index ];
	for( i=index+1; i<pTaskCount; i++ ){
		pTasks[ i - 1 ] = pTasks[ i ];
	}
	pTaskCount--;
}
