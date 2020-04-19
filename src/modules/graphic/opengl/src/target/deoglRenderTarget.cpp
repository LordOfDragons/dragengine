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

#include "deoglRenderTarget.h"
#include "../framebuffer/deoglFramebuffer.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTFramebuffer.h"
#include "../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTarget
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTarget::deoglRenderTarget( deoglRenderThread &renderThread, int width, int height, int componentCount, int bitCount ) :
pRenderThread( renderThread ),

pWidth( decMath::max( width, 1 ) ),
pHeight( decMath::max( height, 1 ) ),
pTextureWidth( pWidth ),
pTextureHeight( pHeight ),
pAspectRatio( ( float )pWidth / ( float )pHeight ),
pBitCount( bitCount ),
pComponentCount( componentCount ),
pFloatTexture( bitCount != 8 ),

pDirtyTexture( true ),

pTexture( NULL ),
pFBO( NULL )
{
	/*if( ogl->GetConfiguration()->GetUsePOTTextures() ){
		for( pTextureWidth=1; pTextureWidth<pWidth; pTextureWidth<<=1 );
		for( pTextureHeight=1; pTextureHeight<pHeight; pTextureHeight<<=1 );
	}*/
}

deoglRenderTarget::~deoglRenderTarget(){
	ReleaseFramebuffer();
	
	if( pTexture ){
		delete pTexture;
	}
}



// Management
///////////////

void deoglRenderTarget::SetSize( int width, int height ){
	if( width < 1 || height < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	if( width == pWidth && height == pHeight ){
		return;
	}
	
	ReleaseFramebuffer();
	if( pTexture ){
		delete pTexture;
		pTexture = NULL;
	}
	
	pWidth = width;
	pHeight = height;
	pAspectRatio = ( float )pWidth / ( float )pHeight;
	
	pTextureWidth = pWidth;
	pTextureHeight = pHeight;
	/*if( ogl->GetConfiguration()->GetUsePOTTextures() ){
		for( pTextureWidth=1; pTextureWidth<pWidth; pTextureWidth<<=1 );
		for( pTextureHeight=1; pTextureHeight<pHeight; pTextureHeight<<=1 );
	}*/
	
	pDirtyTexture = true;
}



void deoglRenderTarget::PrepareFramebuffer(){
	if( ! pTexture ){
		pTexture = new deoglTexture( pRenderThread );
		pTexture->SetSize( pTextureWidth, pTextureHeight );
		pTexture->SetFBOFormat( pComponentCount, pFloatTexture );
		pTexture->SetMipMapped( false );
		pTexture->CreateTexture(); // require or framebuffer attaching fails
	}
	
	if( ! pFBO ){
		pFBO = new deoglFramebuffer( pRenderThread, false );
		
		pRenderThread.GetFramebuffer().Activate( pFBO );
		
		pFBO->AttachColorTexture( 0, pTexture );
		
		const GLenum buffers[ 1 ] = { GL_COLOR_ATTACHMENT0 };
		OGL_CHECK( pRenderThread, pglDrawBuffers( 1, buffers ) );
		OGL_CHECK( pRenderThread, glReadBuffer( GL_COLOR_ATTACHMENT0 ) );
		
		pFBO->Verify();
	}
	
	pRenderThread.GetFramebuffer().Activate( pFBO );
}

void deoglRenderTarget::ReleaseFramebuffer(){
	pRenderThread.GetFramebuffer().Activate( NULL );
	
	if( pFBO ){
		delete pFBO;
		pFBO = NULL;
	}
}



void deoglRenderTarget::SetTextureDirty( bool dirty ){
	pDirtyTexture = dirty;
}

#if 0
void deoglRenderTarget::ClearTexture(){
	deoglPixelBuffer::ePixelFormats pbtype = deoglPixelBuffer::epfByte3;
	bool useFloat = false;
	
	if( pBitCount == 8 ){
		if( pComponentCount == 1 ){
			pbtype = deoglPixelBuffer::epfByte1;
			
		}else if( pComponentCount == 2 ){
			pbtype = deoglPixelBuffer::epfByte2;
			
		}else if( pComponentCount == 3 ){
			pbtype = deoglPixelBuffer::epfByte3;
			
		}else if( pComponentCount == 4 ){
			pbtype = deoglPixelBuffer::epfByte4;
		}
		
	}else{
		useFloat = true;
		
		if( pComponentCount == 1 ){
			pbtype = deoglPixelBuffer::epfFloat1;
			
		}else if( pComponentCount == 2 ){
			pbtype = deoglPixelBuffer::epfFloat2;
			
		}else if( pComponentCount == 3 ){
			pbtype = deoglPixelBuffer::epfFloat3;
			
		}else if( pComponentCount == 4 ){
			pbtype = deoglPixelBuffer::epfFloat4;
		}
	}
	
	deoglPixelBuffer pixelBuffer( pbtype, pTextureWidth, pTextureHeight, 1 );
	const int pixelCount = pTextureWidth * pTextureHeight;
	
	if( useFloat ){
		if( pComponentCount == 1 ){
			deoglPixelBuffer::sFloat1 * const destData = pixelBuffer.GetPointerFloat1();
			
			for( i=0; i< pixelCount; i++ ){
				destData[ i ].r = 0.0f;
			}
			
		}else if( pComponentCount == 2 ){
			deoglPixelBuffer::sFloat2 * const destData = pixelBuffer.GetPointerFloat2();
			
			for( i=0; i< pixelCount; i++ ){
				destData[ i ].r = 0.0f;
				destData[ i ].g = 1.0f;
			}
			
		}else if( pComponentCount == 3 ){
			deoglPixelBuffer::sFloat3 * const destData = pixelBuffer.GetPointerFloat3();
			
			for( i=0; i< pixelCount; i++ ){
				destData[ i ].r = 0.0f;
				destData[ i ].g = 0.0f;
				destData[ i ].b = 0.0f;
			}
			
		}else if( pComponentCount == 4 ){
			deoglPixelBuffer::sFloat4 * const destData = pixelBuffer.GetPointerFloat4();
			
			for( i=0; i< pixelCount; i++ ){
				destData[ i ].r = 0.0f;
				destData[ i ].g = 0.0f;
				destData[ i ].b = 0.0f;
				destData[ i ].a = 1.0f;
			}
		}
		
	}else{
		if( pComponentCount == 1 ){
			deoglPixelBuffer::sByte1 * const destData = pixelBuffer.GetPointerByte1();
			
			for( i=0; i< pixelCount; i++ ){
				destData[ i ].r = 0;
			}
			
		}else if( pComponentCount == 2 ){
			deoglPixelBuffer::sByte2 * const destData = pixelBuffer.GetPointerByte2();
			
			for( i=0; i< pixelCount; i++ ){
				destData[ i ].r = 0;
				destData[ i ].g = 255;
			}
			
		}else if( pComponentCount == 3 ){
			deoglPixelBuffer::sByte3 * const destData = pixelBuffer.GetPointerByte3();
			
			for( i=0; i< pixelCount; i++ ){
				destData[ i ].r = 0;
				destData[ i ].g = 0;
				destData[ i ].b = 0;
			}
			
		}else if( pComponentCount == 4 ){
			deoglPixelBuffer::sByte4 * const destData = pixelBuffer.GetPointerByte4();
			
			for( i=0; i< pixelCount; i++ ){
				destData[ i ].r = 0;
				destData[ i ].g = 0;
				destData[ i ].b = 0;
				destData[ i ].a = 255;
			}
		}
	}
	
	pTexture->SetPixels( pixelBuffer );
}
#endif
