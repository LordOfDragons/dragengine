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
