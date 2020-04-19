/* 
 * Drag[en]gine IGDE
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

#include "igdeResourceLoader.h"
#include "igdeResourceLoaderTask.h"
#include "igdeResourceLoaderListener.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/file/decPath.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/deFileResource.h>
#include <dragengine/resources/loader/deResourceLoader.h>
#include <dragengine/resources/loader/deResourceLoaderInfo.h>



// Class igdeResourceLoader
/////////////////////////////

// Constructor, destructor
////////////////////////////

igdeResourceLoader::igdeResourceLoader( igdeEnvironment &environment ) :
pEnvironment( environment ){
}

igdeResourceLoader::~igdeResourceLoader(){
}



// Management
///////////////

void igdeResourceLoader::Update(){
	deResourceLoader &loader = *pEnvironment.GetEngineController()->GetEngine()->GetResourceLoader();
	deLogger &logger = *pEnvironment.GetLogger();
	deResourceLoaderInfo info;
	
	while( loader.NextFinishedRequest( info ) ){
		const int index = pIndexOfTaskWith( info.GetPath(), info.GetResourceType() );
		if( index == -1 ){
			continue;
		}
		
		const deObjectReference guardTask( pTasks.GetAt( index ) );
		pTasks.RemoveFrom( index );
		igdeResourceLoaderTask &task = ( igdeResourceLoaderTask& )( deObject& )guardTask;
		
		if( info.GetResource() ){
			task.NotifyLoadingFinished( logger, info.GetResource() );
			
		}else{
			task.NotifyLoadingFailed( logger );
		}
	}
}

void igdeResourceLoader::RequestResource( const char *filename,
deResourceLoader::eResourceType resourceType, igdeResourceLoaderListener *listener ){
	if( ! filename || ! listener ){
		DETHROW( deeInvalidParam );
	}
	
	int task = pIndexOfTaskWith( filename, resourceType );
	if( task == -1 ){
		pAddTask( filename, resourceType );
		task = pTasks.GetCount() - 1;
	}
	
	( ( igdeResourceLoaderTask* )pTasks.GetAt( task ) )->AddListener( listener );
}



// Private Functions
//////////////////////

int igdeResourceLoader::pIndexOfTaskWith( const char *filename,
deResourceLoader::eResourceType resourceType ){
	int i;
	for( i=0; i<pTasks.GetCount(); i++ ){
		const igdeResourceLoaderTask &task = *( ( igdeResourceLoaderTask* )pTasks.GetAt( i ) );
		if( task.GetFilename() == filename && task.GetResourceType() == resourceType ){
			return i;
		}
	}
	
	return -1;
}

void igdeResourceLoader::pAddTask( const char *filename,
deResourceLoader::eResourceType resourceType ){
	deObjectReference task;
	task.TakeOver( new igdeResourceLoaderTask( filename, resourceType ) );
	pTasks.Add( task );
	
	deEngine &engine = *pEnvironment.GetEngineController()->GetEngine();
	engine.GetResourceLoader()->AddLoadRequest( engine.GetVirtualFileSystem(), filename, resourceType );
}
