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

#include "deoglRImage.h"
#include "texture2d/deoglTexture.h"
#include "cubemap/deoglCubeMap.h"
#include "arraytexture/deoglArrayTexture.h"
#include "../deoglBasics.h"
#include "../configuration/deoglConfiguration.h"
#include "../delayedoperation/deoglDelayedOperations.h"
#include "../memory/deoglMemoryManager.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/image/deImage.h>



// Class deoglRImage
//////////////////////

// Constructor, destructor
////////////////////////////

deoglRImage::deoglRImage( deoglRenderThread &renderThread, const deImage &image ) :
pRenderThread( renderThread ),

pFilename( image.GetFilename() ),
pWidth( image.GetWidth() ),
pHeight( image.GetHeight() ),
pDepth( image.GetDepth() ),
pComponentCount( image.GetComponentCount() ),
pBitCount( image.GetBitCount() ),

pTexture( NULL ),
pCubeMap( NULL ),
pArrayTexture( NULL ),
pSkinUse( false ),
pScaleU( 1.0f ),
pScaleV( 1.0f ),

pSkinMemUse( renderThread.GetMemoryManager().GetConsumption().skin )
{
	LEAK_CHECK_CREATE( renderThread, Image );
}

deoglRImage::~deoglRImage(){
	LEAK_CHECK_FREE( pRenderThread, Image );
	pCleanUp();
}



// Management
///////////////

void deoglRImage::SetPixelBuffer( deoglPixelBuffer *pixelBuffer ){
	// WARNING Called during synchronization from main thread only
	
	pPixelBuffer = pixelBuffer;
}

void deoglRImage::SetTexture( deoglTexture *texture ){
	// called from render thread. does not use delayed operations to avoid dead-locking
	if( texture == pTexture ){
		return;
	}
	pDirectReleaseTextures();
	pTexture = texture;
	pSkinUse = true;
	pUpdateSkinMemoryUsage();
}

void deoglRImage::SetCubeMap( deoglCubeMap *cubemap ){
	// called from render thread. does not use delayed operations to avoid dead-locking
	if( cubemap == pCubeMap ){
		return;
	}
	pDirectReleaseTextures();
	pCubeMap = cubemap;
	pSkinUse = true;
	pUpdateSkinMemoryUsage();
}

void deoglRImage::SetArrayTexture( deoglArrayTexture *arrayTexture ){
	// called from render thread. does not use delayed operations to avoid dead-locking
	if( arrayTexture == pArrayTexture ){
		return;
	}
	pDirectReleaseTextures();
	pArrayTexture = arrayTexture;
	pSkinUse = true;
	pUpdateSkinMemoryUsage();
}



void deoglRImage::PrepareForRender(){
	if( ! pPixelBuffer ){
		return;
	}
	
	if( ! pTexture && ! pCubeMap && ! pArrayTexture ){
		// this should happen only for callers except skins. if the depth is 1 a 2D texture
		// is created. if the depth is 6 and width and height are equals a cube map is created.
		// otherwise an array texture is created. users of images have to cope with these
		// three types of textures to exist
		
		// if the image is too small disable compression. otherwise the result
		// can turn out smeared out. observed are problems at less than 5 pixel
		// width or height. with 5 or more pixel no such problem is observed
		const bool compressed = false; //pWidth >= 5 && pHeight >= 5;
		
		// NOTE since this is usually called for images used in UI using compression typically
		//      results in visible artifacts. since these places have no way to tell if using
		//      compression is a problem default to not use compression
		
		if( pDepth == 1 ){
			pTexture = new deoglTexture( pRenderThread );
			pTexture->SetSize( pWidth, pHeight );
			pTexture->SetMapingFormat( pComponentCount, pBitCount > 8, compressed );
			pTexture->SetMipMapped( false );
			
		}else if( pDepth == 6 && pWidth == pHeight ){
			pCubeMap = new deoglCubeMap( pRenderThread );
			pCubeMap->SetSize( pWidth );
			pCubeMap->SetMapingFormat( pComponentCount, pBitCount > 8, compressed );
			pCubeMap->SetMipMapped( false );
			
		}else{
			pArrayTexture = new deoglArrayTexture( pRenderThread );
			pArrayTexture->SetSize( pWidth, pHeight, pDepth );
			pArrayTexture->SetMapingFormat( pComponentCount, pBitCount > 8, compressed );
			pArrayTexture->SetMipMapped( false );
		}
	}
	
	if( pArrayTexture ){
		pArrayTexture->SetPixels( pPixelBuffer );
		
	}else if( pCubeMap ){
		pCubeMap->SetPixels( pPixelBuffer );
		
	}else{
		pTexture->SetPixels( pPixelBuffer );
	}
	
	pPixelBuffer = nullptr;
}



// Private Functions
//////////////////////

void deoglRImage::pCleanUp(){
	pReleaseTextures();
}

void deoglRImage::pReleaseTextures(){
	pRenderThread.GetDelayedOperations().RemoveInitImage( this );
	pDirectReleaseTextures();
}

void deoglRImage::pDirectReleaseTextures(){
	// called from render thread and from pReleaseTextures
	if( pTexture ){
		delete pTexture;
		pTexture = NULL;
	}
	if( pCubeMap ){
		delete pCubeMap;
		pCubeMap = NULL;
	}
	if( pArrayTexture ){
		delete pArrayTexture;
		pArrayTexture = NULL;
	}
	
	pUpdateSkinMemoryUsage();
}

void deoglRImage::pUpdateSkinMemoryUsage(){
	pSkinMemUse.Clear();
	
	if( ! pSkinUse ){
		return;
	}
	
	if( pTexture ){
		pSkinMemUse.compressed += pTexture->GetMemoryConsumption().TotalCompressed();
		pSkinMemUse.uncompressed += pTexture->GetMemoryConsumption().TotalUncompressed();
	}
	
	if( pCubeMap ){
		pSkinMemUse.compressed += pCubeMap->GetMemoryConsumption().TotalCompressed();
		pSkinMemUse.uncompressed += pCubeMap->GetMemoryConsumption().TotalUncompressed();
	}
	
	if( pArrayTexture ){
		pSkinMemUse.compressed += pArrayTexture->GetMemoryConsumption().TotalCompressed();
		pSkinMemUse.uncompressed += pArrayTexture->GetMemoryConsumption().TotalUncompressed();
	}
}
