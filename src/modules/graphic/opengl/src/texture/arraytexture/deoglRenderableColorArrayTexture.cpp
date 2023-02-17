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
#include "deoglRenderableColorArrayTexture.h"
#include "../../deoglBasics.h"
#include "../../memory/deoglMemoryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorArrayTexture
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorArrayTexture::deoglRenderableColorArrayTexture( deoglRenderThread &renderThread,
	int width, int height, int layerCount, int componentCount, bool isFloat ) :
pWidth( width ),
pHeight( height ),
pLayerCount( layerCount ),
pComponentCount( componentCount ),
pIsFloat( isFloat ),
pInUse( false ),
pArrayTexture( NULL ),
pMemoryUsageGPU( 0 )
{
	if( width < 1 || height < 1 || layerCount < 1 ){
		DETHROW( deeInvalidParam );
	}
	if( componentCount < 1 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pArrayTexture = new deoglArrayTexture( renderThread );
		pArrayTexture->SetSize( width, height, layerCount );
		pArrayTexture->SetFBOFormat( componentCount, isFloat );
		pArrayTexture->CreateTexture();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	UpdateMemoryUsage();
}

deoglRenderableColorArrayTexture::~deoglRenderableColorArrayTexture(){
	pCleanUp();
	
//	pRenderThread.GetMemoryManager().IncRenderable2DColorGPUTotal( -pMemoryUsageGPU );
}



// Management
///////////////

bool deoglRenderableColorArrayTexture::Matches( int width, int height, int layers,
int componentCount, bool isFloat ) const{
	return width == pWidth && height == pHeight && layers == pLayerCount
		&& componentCount == pComponentCount && isFloat == pIsFloat;
}

void deoglRenderableColorArrayTexture::SetInUse( bool inUse ){
	pInUse = inUse;
}



void deoglRenderableColorArrayTexture::UpdateMemoryUsage(){
	if( pArrayTexture ){
		pMemoryUsageGPU = ( int )pArrayTexture->GetMemoryConsumption().Total();
		
	}else{
		pMemoryUsageGPU = 0;
	}
}



// Private Functions
//////////////////////

void deoglRenderableColorArrayTexture::pCleanUp(){
	if( pArrayTexture ){
		delete pArrayTexture;
	}
}
