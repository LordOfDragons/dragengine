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

#include "deoglRenderTask.h"
#include "deoglRenderTaskPipeline.h"
#include "deoglRenderTaskTexture.h"
#include "shared/deoglRenderTaskSharedTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskPipeline
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskPipeline::deoglRenderTaskPipeline() :
pPipeline( nullptr ),

pTextureCount( 0 ),

pHasTexture( nullptr ),
pHasTextureCount( 0 ),
pHasTextureSize( 0 ){
}

deoglRenderTaskPipeline::~deoglRenderTaskPipeline(){
	if( pHasTexture ){
		delete [] pHasTexture;
	}
	
	const int textureCount = pTextures.GetCount();
	int i;
	for( i=0; i<textureCount; i++ ){
		delete ( deoglRenderTaskTexture* )pTextures.GetAt( i );
	}
	pTextures.RemoveAll();
}



// Management
///////////////

void deoglRenderTaskPipeline::Reset(){
	pPipeline = nullptr;
	pHasTextureCount = 0;
	pTextureCount = 0;
}



int deoglRenderTaskPipeline::GetTotalPointCount() const{
	int i, pointCount = 0;
	for( i=0; i<pTextureCount; i++ ){
		pointCount += ( ( deoglRenderTaskTexture* )pTextures.GetAt( i ) )->GetTotalPointCount();
	}
	return pointCount;
}

int deoglRenderTaskPipeline::GetTotalVAOCount() const{
	int i, vaoCount = 0;
	for( i=0; i<pTextureCount; i++ ){
		vaoCount += ( ( deoglRenderTaskTexture* )pTextures.GetAt( i ) )->GetVAOCount();
	}
	return vaoCount;
}

int deoglRenderTaskPipeline::GetTotalInstanceCount() const{
	int i, instanceCount = 0;
	for( i=0; i<pTextureCount; i++ ){
		instanceCount += ( ( deoglRenderTaskTexture* )pTextures.GetAt( i ) )->GetTotalInstanceCount();
	}
	return instanceCount;
}

int deoglRenderTaskPipeline::GetTotalSubInstanceCount() const{
	int i, subInstanceCount = 0;
	for( i=0; i<pTextureCount; i++ ){
		subInstanceCount += ( ( deoglRenderTaskTexture* )pTextures.GetAt( i ) )->GetTotalSubInstanceCount();
	}
	return subInstanceCount;
}



void deoglRenderTaskPipeline::SetPipeline( const deoglPipeline *pipeline ){
	pPipeline = pipeline;
}



deoglRenderTaskTexture *deoglRenderTaskPipeline::GetTextureAt( int index ) const{
	return ( deoglRenderTaskTexture* )pTextures.GetAt( index );
}

deoglRenderTaskTexture *deoglRenderTaskPipeline::AddTexture( const deoglRenderTaskSharedTexture *texture ){
	DEASSERT_NOTNULL( texture )
	
	const int index = texture->GetIndex();
	
	if( index >= pHasTextureCount ){
		if( index >= pHasTextureSize ){
			deoglRenderTaskTexture ** const newArray = new deoglRenderTaskTexture*[ index + 1 ];
			
			if( pHasTexture ){
				if( pHasTextureCount > 0 ){
					memcpy( newArray, pHasTexture, sizeof( deoglRenderTaskTexture* ) * pHasTextureCount );
				}
				delete [] pHasTexture;
			}
			
			pHasTexture = newArray;
			pHasTextureSize = index + 1;
		}
		
		if( pHasTextureCount <= index ){
			memset( pHasTexture + pHasTextureCount, 0, sizeof( deoglRenderTaskTexture* ) * ( index - pHasTextureCount + 1 ) );
			pHasTextureCount = index + 1;
		}
	}
	
	deoglRenderTaskTexture *rttexture = pHasTexture[ index ];
	if( rttexture ){
		return rttexture;
	}
	
	if( pTextureCount == pTextures.GetCount() ){
		rttexture = new deoglRenderTaskTexture;
		pTextures.Add( rttexture );
		
	}else{
		rttexture = ( deoglRenderTaskTexture* )pTextures.GetAt( pTextureCount );
		rttexture->Reset();
	}
	pTextureCount++;
	
	rttexture->SetTexture( texture );
	pHasTexture[ index ] = rttexture;
	return rttexture;
}
