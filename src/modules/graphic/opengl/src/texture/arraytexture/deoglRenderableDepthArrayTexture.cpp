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
#include "deoglRenderableDepthArrayTexture.h"
#include "../../deoglBasics.h"
#include "../../memory/deoglMemoryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableDepthArrayTexture
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableDepthArrayTexture::deoglRenderableDepthArrayTexture( deoglRenderThread &renderThread,
	int width, int height, int layerCount, bool withStencil, bool useFloat ) :
pWidth( width ),
pHeight( height ),
pLayerCount( layerCount ),
pWithStencil( withStencil ),
pUseFloat( useFloat ),
pInUse( false ),
pArrayTexture( NULL ),
pMemoryUsageGPU( 0 )
{
	if( width < 1 || height < 1 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pArrayTexture = new deoglArrayTexture( renderThread );
		pArrayTexture->SetSize( width, height, layerCount );
		pArrayTexture->SetDepthFormat( withStencil, useFloat );
		pArrayTexture->CreateTexture();
		UpdateMemoryUsage();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderableDepthArrayTexture::~deoglRenderableDepthArrayTexture(){
	pCleanUp();
//	pRenderThread.GetMemoryManager().IncRenderable2DDepthGPUTotal( -pMemoryUsageGPU );
}



// Management
///////////////

bool deoglRenderableDepthArrayTexture::Matches( int width, int height, int layerCount,
bool withStencil, bool useFloat ) const{
	return width == pWidth && height == pHeight && pLayerCount == layerCount
		&& withStencil == pWithStencil && useFloat == pUseFloat;
}

void deoglRenderableDepthArrayTexture::SetInUse( bool inUse ){
	pInUse = inUse;
}

void deoglRenderableDepthArrayTexture::UpdateMemoryUsage(){
	if( pArrayTexture ){
		pMemoryUsageGPU = ( int )pArrayTexture->GetMemoryConsumption().Total();
		
	}else{
		pMemoryUsageGPU = 0;
	}
}



// private functions
//////////////////////

void deoglRenderableDepthArrayTexture::pCleanUp(){
	if( pArrayTexture ){
		delete pArrayTexture;
	}
}
