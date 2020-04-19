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

#include "igdeResourceLoaderTask.h"
#include "igdeResourceLoaderListener.h"

#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>



// Class igdeResourceLoaderTask
/////////////////////////////////

// Constructor, destructor
////////////////////////////

igdeResourceLoaderTask::igdeResourceLoaderTask( const char *filename,
	deResourceLoader::eResourceType resourceType ) :
pFilename( filename ),
pResourceType( resourceType ){
}

igdeResourceLoaderTask::~igdeResourceLoaderTask(){
}



// Listeners
//////////////

void igdeResourceLoaderTask::AddListener( igdeResourceLoaderListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.AddIfAbsent( listener );
}

void igdeResourceLoaderTask::RemoveListener( igdeResourceLoaderListener *listener ){
	pListeners.RemoveIfPresent( listener );
}

void igdeResourceLoaderTask::NotifyLoadingFinished( deLogger &logger, deFileResource *resource ){
	if( ! resource ){
		DETHROW( deeInvalidParam );
	}
	
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		igdeResourceLoaderListener &listener = *( ( igdeResourceLoaderListener* )pListeners.GetAt( i ) );
		
		try{
			listener.LoadingFinished( *this, resource );
			
		}catch( const deException &e ){
			logger.LogException( "IGDE", e );
		}
	}
	
	pListeners.RemoveAll();
}

void igdeResourceLoaderTask::NotifyLoadingFailed( deLogger &logger ){
	const int count = pListeners.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		igdeResourceLoaderListener &listener = *( ( igdeResourceLoaderListener* )pListeners.GetAt( i ) );
		
		try{
			listener.LoadingFailed( *this );
			
		}catch( const deException &e ){
			logger.LogException( "IGDE", e );
		}
	}
	
	pListeners.RemoveAll();
}
