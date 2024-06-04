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

#include "deoglRDSRenderableColor.h"
#include "../../deoglRDynamicSkin.h"
#include "../../../../renderthread/deoglRenderThread.h"
#include "../../../../renderthread/deoglRTLogger.h"
#include "../../../../texture/texture2d/deoglTexture.h"
#include "../../../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../../../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDSRenderableColor
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDSRenderableColor::deoglRDSRenderableColor( deoglRDynamicSkin &dynamicSkin ) :
deoglRDSRenderable( etColor, dynamicSkin ),
pTexture( NULL ),
pDirty( true )
{
	LEAK_CHECK_CREATE( dynamicSkin.GetRenderThread(), DSRenderableColor );
}

deoglRDSRenderableColor::~deoglRDSRenderableColor(){
	LEAK_CHECK_FREE( GetDynamicSkin().GetRenderThread(), DSRenderableColor );
	if( pTexture ){
		delete pTexture;
	}
}



// Management
///////////////

void deoglRDSRenderableColor::SetColor( const decColor &color ){
	if( color.IsEqualTo( pColor ) ){
		return;
	}
	
	pColor = color;
	pDirty = true;
}

void deoglRDSRenderableColor::PrepareForRender( const deoglRenderPlanMasked * ){
}

decColor deoglRDSRenderableColor::GetRenderColor( const decColor & ){
	return pColor;
}

deoglTexture *deoglRDSRenderableColor::GetRenderTexture(){
	if( ! pTexture ){
		pTexture = new deoglTexture( GetDynamicSkin().GetRenderThread() );
		pTexture->SetSize( 1, 1 );
		pTexture->SetMipMapped( false );
		pTexture->SetMapingFormat( 4, false, false );
		
		pDirty = true;
	}
	
	if( pDirty ){
		const deoglPixelBuffer::Ref pixelBuffer( deoglPixelBuffer::Ref::New(
			new deoglPixelBuffer( deoglPixelBuffer::epfByte4, 1, 1, 1 ) ) );
		pixelBuffer->SetToFloatColor( pColor.r, pColor.g, pColor.b, pColor.a );
		pTexture->SetPixels( pixelBuffer );
		
		pDirty = false;
	}
	
	return pTexture;
}
