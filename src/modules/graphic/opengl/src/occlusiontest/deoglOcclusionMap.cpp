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

#include "deoglOcclusionMap.h"
#include "../capabilities/deoglCapabilities.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../texture/arraytexture/deoglArrayTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglOcclusionMap::deoglOcclusionMap( deoglRenderThread &renderThread, int width, int height, int layerCount ) :
pRenderThread( renderThread  ){
	if( width < 4 || height < 4 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = NULL;
	pFBOs = NULL;
	
	pWidth = width;
	pHeight = height;
	pLayerCount = layerCount;
	pLevelCount = 1;
	
	try{
		pCreateTextures();
		pCreateFBOs();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglOcclusionMap::~deoglOcclusionMap(){
	pCleanUp();
}



// Management
///////////////

deoglFramebuffer *deoglOcclusionMap::GetFBOAt( int level ){
	if( level < 0 || level >= pLevelCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pFBOs[ level ];
}



// Private Functions
//////////////////////

void deoglOcclusionMap::pCleanUp(){
	if( pFBOs ){
		int i;
		
		for( i=0; i<pLevelCount; i++ ){
			if( pFBOs[ i ] ){
				delete pFBOs[ i ];
			}
		}
		
		delete [] pFBOs;
	}
	
	if( pTexture ){
		delete pTexture;
	}
}

void deoglOcclusionMap::pCreateTextures(){
	int levelSize = pWidth;
	
	if( pHeight > pWidth ){
		levelSize = pHeight;
	}
	
	pLevelCount = 1;
	for( ; levelSize>2; levelSize>>=1 ){
		pLevelCount++;
	}
	
	pTexture = new deoglArrayTexture( pRenderThread );
	pTexture->SetSize( pWidth, pHeight, pLayerCount );
	pTexture->SetDepthFormat( false, false );
	pTexture->SetMipMapped( true );
	pTexture->SetMipMapLevelCount( pLevelCount - 1 );
	pTexture->CreateTexture();
}

void deoglOcclusionMap::pCreateFBOs(){
	const deoglRestoreFramebuffer restoreFbo( pRenderThread );
	int i;
	
	pFBOs = new deoglFramebuffer*[ pLevelCount ];
	
	for( i=0; i<pLevelCount; i++ ){
		pFBOs[ i ] = NULL;
	}
	
	for( i=0; i<pLevelCount; i++ ){
		pFBOs[ i ] = new deoglFramebuffer( pRenderThread, false );
		
		pRenderThread.GetFramebuffer().Activate( pFBOs[ i ] );
		
		pFBOs[ i ]->AttachDepthArrayTextureLevel( pTexture, i );
		
		const GLenum buffers[ 1 ] = { GL_NONE };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
		
		pFBOs[ i ]->Verify();
	}
}
