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
#include "deoglRenderTaskShader.h"
#include "deoglRenderTaskTexture.h"
#include "shared/deoglRenderTaskSharedTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskShader
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskShader::deoglRenderTaskShader() :
pShader( NULL ),

pRootTexture( NULL ),
pTailTexture( NULL ),
pTextureCount( 0 ),

pHasTexture( NULL ),
pHasTextureCount( 0 ),
pHasTextureSize( 0 ),

pNextShader( NULL ){
}

deoglRenderTaskShader::~deoglRenderTaskShader(){
	if( pHasTexture ){
		delete [] pHasTexture;
	}
}



// Management
///////////////

void deoglRenderTaskShader::Reset(){
	pShader = NULL;
	pHasTextureCount = 0;
	
	pRootTexture = NULL;
	pTailTexture = NULL;
	pTextureCount = 0;
}



int deoglRenderTaskShader::GetTotalPointCount() const{
	deoglRenderTaskTexture *texture = pRootTexture;
	int pointCount = 0;
	
	while( texture ){
		pointCount += texture->GetTotalPointCount();
		texture = texture->GetNextTexture();
	}
	
	return pointCount;
}

int deoglRenderTaskShader::GetTotalVAOCount() const{
	deoglRenderTaskTexture *texture = pRootTexture;
	int vaoCount = 0;
	
	while( texture ){
		vaoCount += texture->GetVAOCount();
		texture = texture->GetNextTexture();
	}
	
	return vaoCount;
}

int deoglRenderTaskShader::GetTotalInstanceCount() const{
	deoglRenderTaskTexture *texture = pRootTexture;
	int instanceCount = 0;
	
	while( texture ){
		instanceCount += texture->GetTotalInstanceCount();
		texture = texture->GetNextTexture();
	}
	
	return instanceCount;
}

int deoglRenderTaskShader::GetTotalSubInstanceCount() const{
	deoglRenderTaskTexture *texture = pRootTexture;
	int subInstanceCount = 0;
	
	while( texture ){
		subInstanceCount += texture->GetTotalSubInstanceCount();
		texture = texture->GetNextTexture();
	}
	
	return subInstanceCount;
}



void deoglRenderTaskShader::SetShader( const deoglRenderTaskSharedShader *shader ){
	pShader = shader;
}



deoglRenderTaskTexture *deoglRenderTaskShader::AddTexture(
deoglRenderTask &task, deoglRenderTaskSharedTexture *texture ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
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
	
	rttexture = task.TextureFromPool();
	rttexture->SetTexture( texture );
	
	if( pTailTexture ){
		pTailTexture->SetNextTexture( rttexture );
	}
	rttexture->SetNextTexture( NULL );
	
	pTailTexture = rttexture;
	
	if( ! pRootTexture ){
		pRootTexture = rttexture;
	}
	
	pTextureCount++;
	pHasTexture[ index ] = rttexture;
	return rttexture;
}



// Linked List
////////////////

void deoglRenderTaskShader::SetNextShader( deoglRenderTaskShader *shader ){
	pNextShader = shader;
}
