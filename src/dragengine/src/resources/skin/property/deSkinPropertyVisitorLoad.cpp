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
