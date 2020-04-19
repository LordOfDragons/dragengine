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

#include "deSkinPropertyConstructed.h"
#include "deSkinPropertyImage.h"
#include "deSkinPropertyVideo.h"
#include "deSkinPropertyVisitorLoad.h"
#include "node/deSkinPropertyNodeGroup.h"
#include "../../image/deImage.h"
#include "../../image/deImageReference.h"
#include "../../image/deImageManager.h"
#include "../../video/deVideo.h"
#include "../../video/deVideoReference.h"
#include "../../video/deVideoManager.h"
#include "../../../deEngine.h"
#include "../../../common/exceptions.h"



// Class deSkinPropertyVisitorLoad
////////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyVisitorLoad::deSkinPropertyVisitorLoad( deEngine &engine,
deVirtualFileSystem *vfs, const char *basePath ) :
pEngine( engine ),
pVirtualFileSystem( vfs ),
pBasePath( basePath ),
pLoadNode( engine, vfs, basePath ){
}

deSkinPropertyVisitorLoad::~deSkinPropertyVisitorLoad(){
}



// Visiting
/////////////

void deSkinPropertyVisitorLoad::VisitImage( deSkinPropertyImage &property ){
	if( property.GetImage() ){
		return;
	}
	
	deImageReference image;
	
	if( ! property.GetPath().IsEmpty() ){
		try{
			image.TakeOver( pEngine.GetImageManager()->LoadImage(
				pVirtualFileSystem, property.GetPath(), pBasePath ) );
			property.SetImage( image );
			
		}catch( const deException & ){
		}
	}
	
	// TODO missing image has to be handled by graphic module
	if( ! property.GetImage() ){
		try{
			image.TakeOver( pEngine.GetImageManager()->LoadDefault() );
			property.SetImage( image );
			
		}catch( const deException & ){
		}
	}
}

void deSkinPropertyVisitorLoad::VisitVideo( deSkinPropertyVideo &property ){
	if( property.GetVideo() ){
		return;
	}
	
	deVideoReference video;
	
	if( ! property.GetPath().IsEmpty() ){
		try{
			video.TakeOver( pEngine.GetVideoManager()->LoadVideo(
				pVirtualFileSystem, property.GetPath(), pBasePath, false ) );
			property.SetVideo( video );
			
		}catch( const deException & ){
		}
	}
}

void deSkinPropertyVisitorLoad::VisitConstructed( deSkinPropertyConstructed &property ){
	property.GetContent().Visit( pLoadNode );
}
