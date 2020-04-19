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

#include "deoglTexture.h"
#include "deoglRenderableColorTexture.h"
#include "../../deoglBasics.h"
#include "../../memory/deoglMemoryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableColorTexture
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableColorTexture::deoglRenderableColorTexture( deoglRenderThread &renderThread,
int width, int height, int componentCount, bool isFloat ) :
pWidth( width ),
pHeight( height ),
pComponentCount( componentCount ),
pIsFloat( isFloat ),
pInUse( false ),
pTexture( NULL ),
pMemoryUsageGPU( 0 )
{
	if( width < 1 || height < 1 ){
		DETHROW( deeInvalidParam );
	}
	if( componentCount < 1 || componentCount > 4 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pTexture = new deoglTexture( renderThread );
		pTexture->SetSize( width, height );
		pTexture->SetFBOFormat( componentCount, isFloat );
		pTexture->CreateTexture();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
	
	UpdateMemoryUsage();
}

deoglRenderableColorTexture::~deoglRenderableColorTexture(){
	pCleanUp();
	
//	pRenderThread.GetMemoryManager().IncRenderable2DColorGPUTotal( -pMemoryUsageGPU );
}



// Management
///////////////

bool deoglRenderableColorTexture::Matches( int width, int height, int componentCount, bool isFloat ) const{
	return ( width == pWidth ) && ( height == pHeight ) && ( componentCount == pComponentCount ) && ( isFloat == pIsFloat );
}

void deoglRenderableColorTexture::SetInUse( bool inUse ){
	pInUse = inUse;
}



void deoglRenderableColorTexture::UpdateMemoryUsage(){
	//const int oldMemoryGPU = pMemoryUsageGPU;
	
	if( pTexture ){
		pMemoryUsageGPU = pTexture->GetMemoryUsageGPU();
		
	}else{
		pMemoryUsageGPU = 0;
	}
	
//	pRenderThread.GetMemoryManager().IncRenderable2DColorGPUTotal( pMemoryUsageGPU - oldMemoryGPU );
}



// Private Functions
//////////////////////

void deoglRenderableColorTexture::pCleanUp(){
	if( pTexture ){
		delete pTexture;
	}
}
