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

deoglRVideo::deoglRVideo( deoglRenderThread &renderThread, int width, int height,
	int componentCount, int frameCount ) :
pRenderThread( renderThread ),

pWidth( width ),
pHeight( height ),
pComponentCount( componentCount ),

pFrames( nullptr ),
pFrameCount( 0 ),
pFrameCountToCache( -1 ),

pUpdateFrame( -1 )
{
	if( frameCount > 0 ){
		pFrames = new deoglTexture*[ frameCount ];
		for( pFrameCount=0; pFrameCount<frameCount; pFrameCount++ ){
			pFrames[ pFrameCount ] = nullptr;
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
	DEASSERT_TRUE( frame >= 0 )
	DEASSERT_TRUE( frame < pFrameCount )
	DEASSERT_TRUE( pUpdateFrame == -1 )
	
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
		pFrames[ pUpdateFrame ]->SetMapingFormat( pComponentCount, false, false );
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
