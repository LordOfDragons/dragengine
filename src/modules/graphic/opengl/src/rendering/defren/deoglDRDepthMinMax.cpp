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
