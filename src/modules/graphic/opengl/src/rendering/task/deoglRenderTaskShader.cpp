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

#include "deoglRenderTaskShader.h"
#include "deoglRenderTaskTexture.h"
#include "../../texture/texunitsconfig/deoglTexUnitsConfig.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskShader
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskShader::deoglRenderTaskShader() :
pShader( NULL ),
pParamBlock( NULL ),
pSPBInstanceIndexBase( -1 ),

pRootTexture( NULL ),
pTailTexture( NULL ),
pTextureCount( 0 ),

pHasTexture( NULL ),
pHasTextureCount( 0 ),
pHasTextureSize( 0 ){
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
	pParamBlock = NULL;
	pSPBInstanceIndexBase = -1;
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



void deoglRenderTaskShader::SetShader( deoglShaderProgram *shader ){
	pShader = shader;
}

void deoglRenderTaskShader::SetParameterBlock( deoglSPBlockUBO *block ){
	pParamBlock = block;
}

void deoglRenderTaskShader::SetSPBInstanceIndexBase( int parameter ){
	pSPBInstanceIndexBase = parameter;
}



void deoglRenderTaskShader::AddTexture( deoglRenderTaskTexture *texture ){
	if( ! texture ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTailTexture ){
		pTailTexture->SetNextTexture( texture );
	}
	texture->SetNextTexture( NULL );
	
	pTailTexture = texture;
	
	if( ! pRootTexture ){
		pRootTexture = texture;
	}
	
	pTextureCount++;
	
	// mark as added
	const int tucIndex = texture->GetTUC()->GetRenderTaskTUCIndex();
	
	if( tucIndex >= pHasTextureCount ){
		if( tucIndex >= pHasTextureSize ){
			deoglRenderTaskTexture ** const newArray = new deoglRenderTaskTexture*[ tucIndex + 1 ];
			
			if( pHasTexture ){
				if( pHasTextureCount > 0 ){
					memcpy( newArray, pHasTexture, sizeof( deoglRenderTaskTexture* ) * pHasTextureCount );
				}
				delete [] pHasTexture;
			}
			
			pHasTexture = newArray;
			pHasTextureSize = tucIndex + 1;
		}
		
		while( pHasTextureCount < tucIndex ){
			pHasTexture[ pHasTextureCount++ ] = NULL;
		}
		pHasTextureCount++;
	}
	
	pHasTexture[ tucIndex ] = texture;
}

deoglRenderTaskTexture *deoglRenderTaskShader::GetTextureForIndex( int tucIndex ){
	if( tucIndex < pHasTextureCount ){
		return pHasTexture[ tucIndex ];
		
	}else{
		return NULL;
	}
}
