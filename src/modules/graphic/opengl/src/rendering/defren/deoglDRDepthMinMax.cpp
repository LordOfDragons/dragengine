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

#include "deoglDRDepthMinMax.h"
#include "../../capabilities/deoglCapabilities.h"
#include "../../framebuffer/deoglFramebuffer.h"
#include "../../framebuffer/deoglRestoreFramebuffer.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTFramebuffer.h"
#include "../../texture/arraytexture/deoglArrayTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglOcclusionTest
/////////////////////////////

// Constructor, destructor
////////////////////////////

// WARNING DO NOT ENABLE FOR THE TIME BEING!!!
int deoglDRDepthMinMax::USAGE_VERSION = -1; // 0=2pixel, 1=2texture, 2=splitTexture, -1=disabled

deoglDRDepthMinMax::deoglDRDepthMinMax( deoglRenderThread &renderThread,
int width, int height, int layerCount, int maxLevelCount ) :
pRenderThread( renderThread){
	if( width < 1 || height < 1 || layerCount < 1 || maxLevelCount < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	pTexture = NULL;
	pFBOs = NULL;
	
	pTextureMin = NULL;
	pFBOMin = NULL;
	pTextureMax = NULL;
	pFBOMax = NULL;
	
	pWidth = width;
	pHeight = height;
	pLayerCount = layerCount;
	pLevelCount = 1;
	pMaxLevelCount = maxLevelCount;
	
	try{
		pCreateTextures();
		pCreateFBOs();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglDRDepthMinMax::~deoglDRDepthMinMax(){
	pCleanUp();
}



// Management
///////////////

deoglFramebuffer *deoglDRDepthMinMax::GetFBOAt( int level ){
	if( level < 0 || level >= pLevelCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pFBOs[ level ];
}

deoglFramebuffer *deoglDRDepthMinMax::GetFBOMinAt( int level ){
	if( level < 0 || level >= pLevelCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pFBOMin[ level ];
}

deoglFramebuffer *deoglDRDepthMinMax::GetFBOMaxAt( int level ){
	if( level < 0 || level >= pLevelCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pFBOMax[ level ];
}



// Private Functions
//////////////////////

void deoglDRDepthMinMax::pCleanUp(){
	int i;
	
	if( pFBOMax ){
		for( i=0; i<pLevelCount; i++ ){
			if( pFBOMax[ i ] ){
				delete pFBOMax[ i ];
			}
		}
		delete [] pFBOMax;
	}
	if( pTextureMax ){
		delete pTextureMax;
	}
	
	if( pFBOMin ){
		for( i=0; i<pLevelCount; i++ ){
			if( pFBOMin[ i ] ){
				delete pFBOMin[ i ];
			}
		}
		delete [] pFBOMin;
	}
	if( pTextureMin ){
		delete pTextureMin;
	}
	
	
	
	if( pFBOs ){
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

void deoglDRDepthMinMax::pCreateTextures(){
	int largestSize;
	pLevelCount = 1;
	for( largestSize=decMath::max( pWidth, pHeight ); largestSize>2; largestSize>>=1 ){
		pLevelCount++;
	}
	
	if( pLevelCount > pMaxLevelCount ){
		//pLevelCount = pMaxLevelCount;
	}
	
	if( USAGE_VERSION == 0 ){
		pTexture = new deoglArrayTexture( pRenderThread );
		pTexture->SetSize( pWidth, pHeight, pLayerCount );
		//pTexture->SetFBOFormat( 2, true );
		pTexture->SetFormatMappingByNumber( deoglCapsFmtSupport::eutfRG32F ); // on the laptop eutfRG16 is faster
		pTexture->SetMipMapped( true );
		pTexture->SetMipMapLevelCount( pLevelCount );
		pTexture->CreateTexture();
		
	}else if( USAGE_VERSION == 1 ){
		pTextureMin = new deoglArrayTexture( pRenderThread );
		pTextureMin->SetSize( pWidth, pHeight, pLayerCount );
		pTextureMin->SetDepthFormat( false, false );
		pTextureMin->SetMipMapped( true );
		pTextureMin->SetMipMapLevelCount( pLevelCount - 1 );
		pTextureMin->CreateTexture();
		
		pTextureMax = new deoglArrayTexture( pRenderThread );
		pTextureMax->SetSize( pWidth, pHeight, pLayerCount );
		pTextureMax->SetDepthFormat( false, false );
		pTextureMax->SetMipMapped( true );
		pTextureMax->SetMipMapLevelCount( pLevelCount - 1 );
		pTextureMax->CreateTexture();
		
	}else if( USAGE_VERSION == 2 ){
		pTexture = new deoglArrayTexture( pRenderThread );
		pTexture->SetSize( pWidth << 1, pHeight, pLayerCount );
		pTexture->SetDepthFormat( false, false );
		pTexture->SetMipMapped( true );
		pTexture->SetMipMapLevelCount( pLevelCount - 1 );
		pTexture->CreateTexture();
		
	}else{
		DETHROW_INFO( deeInvalidParam, "Disabled" );
	}
}

void deoglDRDepthMinMax::pCreateFBOs(){
	const deoglRestoreFramebuffer restoreFbo( pRenderThread );
	int i;
	
	if( pLevelCount > 0 ){
		if( USAGE_VERSION == 0 ){
			pFBOs = new deoglFramebuffer*[ pLevelCount ];
			
			for( i=0; i<pLevelCount; i++ ){
				pFBOs[ i ] = NULL;
			}
			
			for( i=0; i<pLevelCount; i++ ){
				pFBOs[ i ] = new deoglFramebuffer( pRenderThread, false );
				
				pRenderThread.GetFramebuffer().Activate( pFBOs[ i ] );
				
				pFBOs[ i ]->AttachColorArrayTextureLevel( 0, pTexture, i );
				
				const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
				
				pFBOs[ i ]->Verify();
			}
			
		}else if( USAGE_VERSION == 1 ){
			// min
			pFBOMin = new deoglFramebuffer*[ pLevelCount ];
			
			for( i=0; i<pLevelCount; i++ ){
				pFBOMin[ i ] = NULL;
			}
			
			for( i=0; i<pLevelCount; i++ ){
				pFBOMin[ i ] = new deoglFramebuffer( pRenderThread, false );
				
				pRenderThread.GetFramebuffer().Activate( pFBOMin[ i ] );
				
				pFBOMin[ i ]->AttachDepthArrayTextureLevel( pTextureMin, i );
				
				const GLenum buffers[ 1 ] = { GL_NONE };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
				
				pFBOMin[ i ]->Verify();
			}
			
			// max
			pFBOMax = new deoglFramebuffer*[ pLevelCount ];
			
			for( i=0; i<pLevelCount; i++ ){
				pFBOMax[ i ] = NULL;
			}
			
			for( i=0; i<pLevelCount; i++ ){
				pFBOMax[ i ] = new deoglFramebuffer( pRenderThread, false );
				
				pRenderThread.GetFramebuffer().Activate( pFBOMax[ i ] );
				
				pFBOMax[ i ]->AttachDepthArrayTextureLevel( pTextureMax, i );
				
				const GLenum buffers[ 1 ] = { GL_NONE };
				OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
				OGL_CHECK( pRenderThread, glReadBuffer( GL_NONE ) );
				
				pFBOMax[ i ]->Verify();
			}
			
		}else if( USAGE_VERSION == 2 ){
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
			
		}else{
			DETHROW_INFO( deeInvalidParam, "Disabled" );
		}
	}
}
