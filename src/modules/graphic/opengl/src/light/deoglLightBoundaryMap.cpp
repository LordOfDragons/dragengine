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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglLightBoundaryMap.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../framebuffer/deoglFramebufferManager.h"
#include "../framebuffer/deoglRestoreFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../renderthread/deoglRTTexture.h"
#include "../renderthread/deoglRTLogger.h"
#include "../texture/deoglTextureStageManager.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightBoundaryMap::deoglLightBoundaryMap( deoglRenderThread &renderThread, int size ) :
pRenderThread( renderThread  ),

pTextureMin( NULL ),
pTextureMax( NULL ),
pFBOs( NULL ),

pPixBufBoundaryMin( deoglPixelBuffer::Ref::New( new deoglPixelBuffer( deoglPixelBuffer::epfFloat3, 1, 1, 1 ) ) ),
pPixBufBoundaryMax( deoglPixelBuffer::Ref::New( new deoglPixelBuffer( deoglPixelBuffer::epfFloat3, 1, 1, 1 ) ) ),

pSize( size ),
pLevelCount( 1 )
{
	if( pSize < 2 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pCreateTextures();
		pCreateFBOs();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglLightBoundaryMap::~deoglLightBoundaryMap(){
	pCleanUp();
}



// Management
///////////////

int deoglLightBoundaryMap::GetBaseLevel( int baseSize ){
	int tempSize = pSize;
	int baseLevel = 0;
	
	while( tempSize > baseSize ){
		tempSize >>= 1;
		baseLevel++;
	}
	
	return baseLevel;
}



deoglFramebuffer *deoglLightBoundaryMap::GetFBOAt( int level ){
	if( level < 0 || level >= pLevelCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pFBOs[ level ];
}



void deoglLightBoundaryMap::GetResult( decVector &boundaryMin, decVector &boundaryMax ){
	pTextureMin->GetPixelsLevel( pLevelCount - 1, pPixBufBoundaryMin );
	pTextureMax->GetPixelsLevel( pLevelCount - 1, pPixBufBoundaryMax );
	
	const deoglPixelBuffer::sFloat3 &resultMin = *pPixBufBoundaryMin->GetPointerFloat3();
	const deoglPixelBuffer::sFloat3 &resultMax = *pPixBufBoundaryMax->GetPointerFloat3();
	
	boundaryMin.x = ( float )resultMin.r;
	boundaryMin.y = ( float )resultMin.g;
	boundaryMin.z = ( float )resultMin.b;
	
	boundaryMax.x = ( float )resultMax.r;
	boundaryMax.y = ( float )resultMax.g;
	boundaryMax.z = ( float )resultMax.b;
}



// Private Functions
//////////////////////

void deoglLightBoundaryMap::pCleanUp(){
	if( pFBOs ){
		int i;
		
		for( i=0; i<pLevelCount; i++ ){
			if( pFBOs[ i ] ){
				delete pFBOs[ i ];
			}
		}
		
		delete [] pFBOs;
	}
	
	if( pTextureMax ){
		delete pTextureMax;
	}
	if( pTextureMin ){
		delete pTextureMin;
	}
}

void deoglLightBoundaryMap::pCreateTextures(){
	pLevelCount = ( int )( ceilf( log2f( ( float )pSize ) ) ) + 1;
	
	pTextureMin = new deoglTexture( pRenderThread );
	pTextureMin->SetSize( pSize, pSize );
	pTextureMin->SetFBOFormat( 3, true );
	pTextureMin->SetMipMapped( true );
	pTextureMin->SetMipMapLevelCount( pLevelCount - 1 );
	pTextureMin->CreateTexture();
	
	pTextureMax = new deoglTexture( pRenderThread );
	pTextureMax->SetSize( pSize, pSize );
	pTextureMax->SetFBOFormat( 3, true );
	pTextureMax->SetMipMapped( true );
	pTextureMax->SetMipMapLevelCount( pLevelCount - 1 );
	pTextureMax->CreateTexture();
}

void deoglLightBoundaryMap::pCreateFBOs(){
	const deoglRestoreFramebuffer restoreFbo( pRenderThread );
	const GLenum buffers[ 2 ] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	int i;
	
	pFBOs = new deoglFramebuffer*[ pLevelCount ];
	
	for( i=0; i<pLevelCount; i++ ){
		pFBOs[ i ] = NULL;
	}
	
	for( i=0; i<pLevelCount; i++ ){
		pFBOs[ i ] = new deoglFramebuffer( pRenderThread, false );
		
		pRenderThread.GetFramebuffer().Activate( pFBOs[ i ] );
		
		pFBOs[ i ]->AttachColorTextureLevel( 0, pTextureMin, i );
		pFBOs[ i ]->AttachColorTextureLevel( 1, pTextureMax, i );
		
		OGL_CHECK( pRenderThread, pglDrawBuffers( 2, &buffers[ 0 ] ) );
		
		pFBOs[ i ]->Verify();
	}
}
