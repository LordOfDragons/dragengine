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

#include "deoglRVideoPlayer.h"
#include "deoglRVideo.h"
#include "../deoglBasics.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"
#include "../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRVideoPlayer
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRVideoPlayer::deoglRVideoPlayer( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),

pVideo( NULL ),
pCachedFrameTexture( NULL ),
pUpdateCachedFrameTexture( -1 ),

pWidth( 1 ),
pHeight( 1 ),
pComponentCount( 3 ),

pPixelBuffer( NULL ),
pTexture( NULL ),
pDirtyTexture( false )
{
}

deoglRVideoPlayer::~deoglRVideoPlayer(){
	SetVideo( NULL );
	
	if( pPixelBuffer ){
		delete pPixelBuffer;
	}
	if( pTexture ){
		delete pTexture;
	}
}



// Management
///////////////

void deoglRVideoPlayer::SetVideo( deoglRVideo *video ){
	if( video == pVideo ){
		return;
	}
	
	if( pVideo ){
		pVideo->FreeReference();
	}
	
	pVideo = video;
	
	if( video ){
		video->AddReference();
	}
}

void deoglRVideoPlayer::SetCachedFrameTexture( deoglTexture *texture ){
	pCachedFrameTexture = texture;
}

void deoglRVideoPlayer::SetUpdateCachedFrameTexture( int updateCachedFrameTexture ){
	pUpdateCachedFrameTexture = updateCachedFrameTexture;
	if( updateCachedFrameTexture != -1 ){
		pDirtyTexture = true;
	}
}

bool deoglRVideoPlayer::HasCachedFrameTexture() const{
	return pCachedFrameTexture || pUpdateCachedFrameTexture != -1;
}



void deoglRVideoPlayer::SetVideoSize( int width, int height, int componentCount ){
	if( width < 1 || height < 1 || componentCount < 3 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
	
	if( width == pWidth && height == pHeight && componentCount == pComponentCount ){
		return;
	}
	
	pWidth = width;
	pHeight = height;
	pComponentCount = componentCount;
	
	if( pPixelBuffer ){
		delete pPixelBuffer;
		pPixelBuffer = NULL;
	}
	
	if( pTexture ){
		delete pTexture;
		pTexture = NULL;
	}
	
	pUpdateCachedFrameTexture = -1;
	pCachedFrameTexture = NULL;
	
	pDirtyTexture = true;
}

deoglPixelBuffer *deoglRVideoPlayer::SetPixelBuffer( deoglPixelBuffer *pixelBuffer ){
	deoglPixelBuffer * const prevPixelBuffer = pPixelBuffer;
	
	pPixelBuffer = pixelBuffer;
	pDirtyTexture  = true;
	
	return prevPixelBuffer;
}

deoglTexture *deoglRVideoPlayer::GetTexture() const{
	return pCachedFrameTexture ? pCachedFrameTexture : pTexture;
}



void deoglRVideoPlayer::UpdateTexture(){
	if( ! pDirtyTexture ){
		return;
	}
	
	if( pUpdateCachedFrameTexture != -1 ){
		pVideo->UpdateTexture();
		pCachedFrameTexture = pVideo->GetTexture( pUpdateCachedFrameTexture );
		pUpdateCachedFrameTexture = -1;
	}
	
	if( ! pCachedFrameTexture ){
		//pRenderThread.GetLogger().LogInfo( "VideoPlayer: update not cached" );
		if( ! pTexture ){
			pTexture = new deoglTexture( pRenderThread );
			pTexture->SetSize( pWidth, pHeight );
			pTexture->SetMapingFormat( pComponentCount, false, false );
			pTexture->SetMipMapped( false ); // true would be nicer but doing it every frame is a waste
			pTexture->CreateTexture();
		}
		
		if( pPixelBuffer ){
			pTexture->SetPixels( *pPixelBuffer );
		}
	}
	
	pDirtyTexture = false;
}
