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

#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskVAO.h"
#include "../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskTexture::deoglRenderTaskTexture() :
pTexture( NULL ),

pRootVAO( NULL ),
pTailVAO( NULL ),
pVAOCount( 0 ),

pNextTexture( NULL ),

pLLNext( NULL ),

pHasVAO( NULL ),
pHasVAOCount( 0 ),
pHasVAOSize( 0 ){
}

deoglRenderTaskTexture::~deoglRenderTaskTexture(){
	if( pHasVAO ){
		delete [] pHasVAO;
	}
}



// Management
///////////////

void deoglRenderTaskTexture::Reset(){
	pTexture = NULL;
	pHasVAOCount = 0;
	
	pRootVAO = NULL;
	pTailVAO = NULL;
	pVAOCount = 0;
	
	pNextTexture = NULL;
}



int deoglRenderTaskTexture::GetTotalPointCount() const{
	deoglRenderTaskVAO *vao = pRootVAO;
	int pointCount = 0;
	
	while( vao ){
		pointCount += vao->GetTotalPointCount();
		vao = vao->GetNextVAO();
	}
	
	return pointCount;
}

int deoglRenderTaskTexture::GetTotalInstanceCount() const{
	deoglRenderTaskVAO *vao = pRootVAO;
	int instanceCount = 0;
	
	while( vao ){
		instanceCount += vao->GetInstanceCount();
		vao = vao->GetNextVAO();
	}
	
	return instanceCount;
}

int deoglRenderTaskTexture::GetTotalSubInstanceCount() const{
	deoglRenderTaskVAO *vao = pRootVAO;
	int subInstanceCount = 0;
	
	while( vao ){
		subInstanceCount += vao->GetTotalSubInstanceCount();
		vao = vao->GetNextVAO();
	}
	
	return subInstanceCount;
}



void deoglRenderTaskTexture::SetTexture( deoglRenderTaskSharedTexture *texture ){
	pTexture = texture;
}



deoglRenderTaskVAO *deoglRenderTaskTexture::GetVAOWith( deoglVAO *vao ){
	deoglRenderTaskVAO *renderTaskVAO = pRootVAO;
	
	while( renderTaskVAO && renderTaskVAO->GetVAO() != vao ){
		renderTaskVAO = renderTaskVAO->GetNextVAO();
	}
	
	return renderTaskVAO;
}

void deoglRenderTaskTexture::AddVAO( deoglRenderTaskVAO *vao ){
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTailVAO ){
		pTailVAO->SetNextVAO( vao );
	}
	vao->SetNextVAO( NULL );
	
	pTailVAO = vao;
	
	if( ! pRootVAO ){
		pRootVAO = vao;
	}
	
	pVAOCount++;
	
	// mark as added
	const int vaoIndex = vao->GetVAO()->GetRenderTaskVAOIndex();
	
	if( vaoIndex >= pHasVAOCount ){
		if( vaoIndex >= pHasVAOSize ){
			deoglRenderTaskVAO ** const newArray = new deoglRenderTaskVAO*[ vaoIndex + 1 ];
			
			if( pHasVAO ){
				if( pHasVAOCount > 0 ){
					memcpy( newArray, pHasVAO, sizeof( deoglRenderTaskVAO* ) * pHasVAOCount );
				}
				delete [] pHasVAO;
			}
			
			pHasVAO = newArray;
			pHasVAOSize = vaoIndex + 1;
		}
		
		while( pHasVAOCount < vaoIndex ){
			pHasVAO[ pHasVAOCount++ ] = NULL;
		}
		pHasVAOCount++;
	}
	
	pHasVAO[ vaoIndex ] = vao;
}



deoglRenderTaskVAO *deoglRenderTaskTexture::GetVAOForIndex( int vaoIndex ){
	return vaoIndex < pHasVAOCount ? pHasVAO[ vaoIndex ] : NULL;
}



void deoglRenderTaskTexture::SetNextTexture( deoglRenderTaskTexture *texture ){
	pNextTexture = texture;
}



// Linked List
////////////////

void deoglRenderTaskTexture::SetLLNext( deoglRenderTaskTexture *texture ){
	pLLNext = texture;
}
