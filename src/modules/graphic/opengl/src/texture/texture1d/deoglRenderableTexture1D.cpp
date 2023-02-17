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

#include "deoglTexture1D.h"
#include "deoglRenderableTexture1D.h"
#include "../pixelbuffer/deoglPixelBuffer.h"
#include "../../deoglBasics.h"
#include "../../memory/deoglMemoryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableTexture1D
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableTexture1D::deoglRenderableTexture1D( deoglRenderThread &renderThread, int size, int format ) :
pSize( size ),
pFormat( format ),
pInUse( false ),
pTexture( NULL ),
pMemoryUsageGPU( 0 )
{
	if( size < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pTexture = new deoglTexture1D( renderThread );
		pTexture->SetSize( size );
		
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

deoglRenderableTexture1D::~deoglRenderableTexture1D(){
	pCleanUp();
	
	//pRenderThread.GetMemoryManager().IncRenderable1DGPUTotal( -pMemoryUsageGPU );
}



// Management
///////////////

bool deoglRenderableTexture1D::Matches( int size, int format ) const{
	return ( size == pSize ) && ( format == pFormat );
}

void deoglRenderableTexture1D::SetInUse( bool inUse ){
	pInUse = inUse;
}

void deoglRenderableTexture1D::UpdateMemoryUsage(){
	if( pTexture ){
		pMemoryUsageGPU = ( int )pTexture->GetMemoryConsumption().Total();
		
	}else{
		pMemoryUsageGPU = 0;
	}
}



// private functions
//////////////////////

void deoglRenderableTexture1D::pCleanUp(){
	if( pTexture ){
		delete pTexture;
	}
}
