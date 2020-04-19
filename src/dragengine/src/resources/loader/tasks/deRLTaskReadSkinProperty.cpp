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

#include "deRLTaskReadSkinProperty.h"
#include "deRLTaskReadSkinInternal.h"
#include "../deResourceLoader.h"
#include "../../image/deImage.h"
#include "../../image/deImageManager.h"
#include "../../image/deImageReference.h"
#include "../../skin/property/deSkinPropertyConstructed.h"
#include "../../skin/property/deSkinPropertyImage.h"
#include "../../skin/property/deSkinPropertyVideo.h"
#include "../../skin/property/node/deSkinPropertyNodeGroup.h"
#include "../../video/deVideo.h"
#include "../../video/deVideoManager.h"
#include "../../video/deVideoReference.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"
#include "../../../common/file/decPath.h"



// Class deRLTaskReadSkinProperty
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deRLTaskReadSkinProperty::deRLTaskReadSkinProperty( deResourceLoader &resourceLoader,
deRLTaskReadSkinInternal &task, deEngine &engine, deVirtualFileSystem *vfs, const char *basePath ) :
pResourceLoader( resourceLoader ),
pTask( task ),
pEngine( engine ),
pVirtualFileSystem( vfs ),
pBasePath( basePath ),
pLoadNode( resourceLoader, task, engine, vfs, basePath ){
}

deRLTaskReadSkinProperty::~deRLTaskReadSkinProperty(){
}



// Visiting
/////////////

void deRLTaskReadSkinProperty::VisitImage( deSkinPropertyImage &property ){
	if( property.GetImage() ){
		return;
	}
	
	decString path( property.GetPath() );
	
	if( path.IsEmpty() ){
		// TODO has to be done by the graphic module not here
		deImageReference image;
		
		try{
			image.TakeOver( pEngine.GetImageManager()->LoadDefault() );
			property.SetImage( image );
			
		}catch( const deException & ){
		}
		return;
	}
	
	if( ! decPath::IsUnixPathAbsolute( path ) ){
		decPath resourcePath;
		resourcePath.SetFromUnix( pBasePath );
		resourcePath.AddUnixPath( path );
		path = resourcePath.GetPathUnix();
	}
	
	deResourceLoaderTask * const task = pResourceLoader.AddLoadRequest(
		pVirtualFileSystem, path, deResourceLoader::ertImage );
	deRLTaskReadSkinInternal::cInternalTask *internalTask = NULL;
	
	try{
		internalTask = new deRLTaskReadSkinInternal::cInternalTask( &property, task );
		pTask.AddInternalTask( internalTask );
		internalTask->FreeReference();
		
	}catch( const deException & ){
		if( internalTask ){
			internalTask->FreeReference();
		}
		throw;
	}
}

void deRLTaskReadSkinProperty::VisitVideo( deSkinPropertyVideo &property ){
	if( property.GetVideo() ){
		return;
	}
	
	decString path( property.GetPath() );
	if( path.IsEmpty() ){
		return;
	}
	
	if( ! decPath::IsUnixPathAbsolute( path ) ){
		decPath resourcePath;
		resourcePath.SetFromUnix( pBasePath );
		resourcePath.AddUnixPath( path );
		path = resourcePath.GetPathUnix();
	}
	
	// loaded directly since video uses decoders the responsible module create later on
	deVideoReference video;
	try{
		video.TakeOver( pEngine.GetVideoManager()->LoadVideo( pVirtualFileSystem, path, pBasePath, false ) );
		property.SetVideo( video );
		
	}catch( const deException & ){
	}
}

void deRLTaskReadSkinProperty::VisitConstructed( deSkinPropertyConstructed &property ){
	property.GetContent().Visit( pLoadNode );
}
