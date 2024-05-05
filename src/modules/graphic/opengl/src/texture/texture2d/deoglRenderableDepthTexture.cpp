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

#include "deoglTexture.h"
#include "deoglRenderableDepthTexture.h"
#include "../../deoglBasics.h"
#include "../../memory/deoglMemoryManager.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderableDepthTexture
//////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderableDepthTexture::deoglRenderableDepthTexture( deoglRenderThread &renderThread,
int width, int height, bool withStencil, bool useFloat ) :
pWidth( width ),
pHeight( height ),
pWithStencil( withStencil ),
pUseFloat( useFloat ),
pInUse( false ),
pTexture( NULL ),
pMemoryUsageGPU( 0 )
{
	if( width < 1 || height < 1 ){
		DETHROW( deeInvalidParam );
	}
	
	try{
		pTexture = new deoglTexture( renderThread );
		pTexture->SetSize( width, height );
		pTexture->SetDepthFormat( withStencil, useFloat );
		pTexture->CreateTexture();
		UpdateMemoryUsage();
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglRenderableDepthTexture::~deoglRenderableDepthTexture(){
	pCleanUp();
//	pRenderThread.GetMemoryManager().IncRenderable2DDepthGPUTotal( -pMemoryUsageGPU );
}



// Management
///////////////

bool deoglRenderableDepthTexture::Matches( int width, int height, bool withStencil, bool useFloat ) const{
	return width == pWidth
		&& height == pHeight
		&& withStencil == pWithStencil
		&& useFloat == pUseFloat;
}

void deoglRenderableDepthTexture::SetInUse( bool inUse ){
	pInUse = inUse;
}

void deoglRenderableDepthTexture::UpdateMemoryUsage(){
	if( pTexture ){
		pMemoryUsageGPU = ( int )pTexture->GetMemoryConsumption().Total();
		
	}else{
		pMemoryUsageGPU = 0;
	}
}



// private functions
//////////////////////

void deoglRenderableDepthTexture::pCleanUp(){
	if( pTexture ){
		delete pTexture;
	}
}
