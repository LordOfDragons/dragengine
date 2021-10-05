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
deoglRDSRenderable( dynamicSkin ),
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
		deoglPixelBuffer pixelBuffer( deoglPixelBuffer::epfByte4, 1, 1, 1 );
		pixelBuffer.SetToFloatColor( pColor.r, pColor.g, pColor.b, pColor.a );
		pTexture->SetPixels( pixelBuffer );
		
		pDirty = false;
	}
	
	return pTexture;
}
