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

#include "deoglRDSRenderableValue.h"
#include "../../deoglRDynamicSkin.h"
#include "../../../../renderthread/deoglRenderThread.h"
#include "../../../../renderthread/deoglRTLogger.h"
#include "../../../../texture/texture2d/deoglTexture.h"
#include "../../../../texture/pixelbuffer/deoglPixelBuffer.h"
#include "../../../../delayedoperation/deoglDelayedOperations.h"

#include <dragengine/common/exceptions.h>



// Class deoglRDSRenderableValue
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRDSRenderableValue::deoglRDSRenderableValue( deoglRDynamicSkin &dynamicSkin ) :
deoglRDSRenderable( etValue, dynamicSkin ),
pTexture( NULL ),
pValue( 0.0f ),
pDirty( true )
{
	LEAK_CHECK_CREATE( dynamicSkin.GetRenderThread(), DSRenderableValue );
}

deoglRDSRenderableValue::~deoglRDSRenderableValue(){
	LEAK_CHECK_FREE( GetDynamicSkin().GetRenderThread(), DSRenderableValue );
	if( pTexture ){
		delete pTexture;
	}
}



// Management
///////////////

void deoglRDSRenderableValue::SetValue( float value ){
	if( fabsf( value - pValue ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pValue = value;
	pDirty = true;
}

void deoglRDSRenderableValue::PrepareForRender( const deoglRenderPlanMasked * ){
}

float deoglRDSRenderableValue::GetRenderValue( float ){
	return pValue;
}

decColor deoglRDSRenderableValue::GetRenderColor( const decColor & ){
	return decColor( pValue, pValue, pValue, 1.0f );
}

deoglTexture *deoglRDSRenderableValue::GetRenderTexture(){
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
		pixelBuffer->SetToFloatColor( pValue, pValue, pValue, 1.0f );
		pTexture->SetPixels( pixelBuffer );
		
		pDirty = false;
	}
	
	return pTexture;
}
