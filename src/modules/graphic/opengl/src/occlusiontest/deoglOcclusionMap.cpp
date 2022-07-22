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

#include "deoglOcclusionMap.h"
#include "../capabilities/deoglCapabilities.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
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
