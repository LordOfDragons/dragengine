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
#include <string.h>
#include <stdlib.h>

#include "deoglArrayTexture.h"
#include "deoglRenderableArrayTexture.h"
#include "../pixelbuffer/deoglPixelBuffer.h"
#include "../../deoglBasics.h"
#include "../../memory/deoglMemoryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableArrayTexture
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableArrayTexture::deoglRenderableArrayTexture( deoglRenderThread &renderThread,
int width, int height, int layerCount, int format ) :
pWidth( width ),
pHeight( height ),
pLayerCount( layerCount ),
pFormat( format ),
pInUse( false ),
pTexture( NULL ),
pMemoryUsageGPU( 0 )
{
	if( width < 1 || height < 1 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pTexture = new deoglArrayTexture( renderThread );
		pTexture->SetSize( width, height, layerCount );
		
		if( format == deoglPixelBuffer::epfByte1 ){
			pTexture->SetFBOFormat( 1, false );
			
		}else if( format == deoglPixelBuffer::epfByte2 ){
			pTexture->SetFBOFormat( 2, false );
			
		}else if( format == deoglPixelBuffer::epfByte3 ){
			pTexture->SetFBOFormat( 3, false );
			
		}else if( format == deoglPixelBuffer::epfByte4 ){
			pTexture->SetFBOFormat( 4, false );
			
		}else if( format == deoglPixelBuffer::epfFloat1 ){
			pTexture->SetFBOFormat( 1, true );
			
		}else if( format == deoglPixelBuffer::epfFloat2 ){
			pTexture->SetFBOFormat( 2, true );
			
		}else if( format == deoglPixelBuffer::epfFloat3 ){
			pTexture->SetFBOFormat( 3, true );
			
		}else if( format == deoglPixelBuffer::epfFloat4 ){
			pTexture->SetFBOFormat( 4, true );
			
		}else if( format == deoglPixelBuffer::epfDepth ){
			pTexture->SetDepthFormat( false );
			
		}else if( format == deoglPixelBuffer::epfDepthStencil ){
			pTexture->SetDepthFormat( true );
			
		}else{
			DETHROW( deeInvalidParam );
		}
		
		pTexture->CreateTexture();
		UpdateMemoryUsage();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderableArrayTexture::~deoglRenderableArrayTexture(){
	pCleanUp();
	
	//pRenderThread.GetMemoryManager().IncRenderableArrayGPUTotal( -pMemoryUsageGPU );
}



// Management
///////////////

bool deoglRenderableArrayTexture::Matches( int width, int height, int layerCount, int format ) const{
	return ( width == pWidth ) && ( height == pHeight ) && ( layerCount == pLayerCount ) && ( format == pFormat );
}

void deoglRenderableArrayTexture::SetInUse( bool inUse ){
	pInUse = inUse;
}

void deoglRenderableArrayTexture::UpdateMemoryUsage(){
	//const int oldMemoryGPU = pMemoryUsageGPU;
	
	if( pTexture ){
		pMemoryUsageGPU = pTexture->GetMemoryUsageGPU();
		
	}else{
		pMemoryUsageGPU = 0;
	}
	
	//pRenderThread.GetMemoryManager().IncRenderableArrayGPUTotal( pMemoryUsageGPU - oldMemoryGPU );
}



// private functions
//////////////////////

void deoglRenderableArrayTexture::pCleanUp(){
	if( pTexture ){
		delete pTexture;
	}
}
