/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglRVideo.h"
#include "../deoglBasics.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/video/deVideo.h>



// Class deoglRVideo
//////////////////////

// Constructor, destructor
////////////////////////////

deoglRVideo::deoglRVideo( deoglRenderThread &renderThread, int width, int height, int frameCount ) :
pRenderThread( renderThread ),

pWidth( width ),
pHeight( height ),

pFrames( NULL ),
pFrameCount( 0 ),
pFrameCountToCache( -1 ),

pUpdateFrame( -1 )
{
	if( frameCount > 0 ){
		pFrames = new deoglTexture*[ frameCount ];
		for( pFrameCount=0; pFrameCount<frameCount; pFrameCount++ ){
			pFrames[ pFrameCount ] = NULL;
		}
		pFrameCountToCache = pFrameCount;
	}
}

deoglRVideo::~deoglRVideo(){
	if( pFrames ){
		int i;
		for( i=0; i<pFrameCount; i++ ){
			if( pFrames[ i ] ){
				delete pFrames[ i ];
			}
		}
		delete [] pFrames;
	}
}



// Management
///////////////

deoglTexture *deoglRVideo::GetTexture( int frame ) const{
	if( frame < 0 || frame >= pFrameCount ){
		try{
			DETHROW( deeInvalidParam );
		}catch( const deException &e ){
			e.PrintError();
		}
		DETHROW( deeInvalidParam );
	}
	return pFrames[ frame ];
}

deoglPixelBuffer::Ref deoglRVideo::SetPixelBuffer( int frame, deoglPixelBuffer *pixelBuffer ){
	if( frame < 0 || frame >= pFrameCount ){
		DETHROW( deeInvalidParam );
	}
	if( pUpdateFrame != -1 ){
		DETHROW( deeInvalidParam );
	}
	
	const deoglPixelBuffer::Ref prevPixelBuffer( pPixelBuffer );
	
	pPixelBuffer = pixelBuffer;
	pUpdateFrame = frame;
	
	return prevPixelBuffer;
}

void deoglRVideo::UpdateTexture(){
	if( pUpdateFrame == -1 ){
		return;
	}
	
	if( ! pFrames[ pUpdateFrame ] ){
		pFrames[ pUpdateFrame ] = new deoglTexture( pRenderThread );
		pFrames[ pUpdateFrame ]->SetSize( pWidth, pHeight );
		pFrames[ pUpdateFrame ]->SetMapingFormat( 3, false, false );
		pFrames[ pUpdateFrame ]->SetMipMapped( false ); // true would be nicer but doing it every frame is a waste
		pFrames[ pUpdateFrame ]->CreateTexture();
	}
	
	if( pPixelBuffer ){
		pFrames[ pUpdateFrame ]->SetPixels( pPixelBuffer );
	}
	
	//pRenderThread.GetLogger().LogInfoFormat( "Video: update texture frame=%i remaining=%i", pUpdateFrame, pFrameCountToCache );
	pUpdateFrame = -1;
	pFrameCountToCache--;
	
	//if( pFrameCountToCache == 0 ){
	//	pRenderThread.GetLogger().LogInfo( "Video: all frame textures ready" );
	//}
}



// Private Functions
//////////////////////
