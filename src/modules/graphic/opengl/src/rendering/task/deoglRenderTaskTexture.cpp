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
#include "deoglRenderTaskTexture.h"
#include "deoglRenderTaskVAO.h"
#include "shared/deoglRenderTaskSharedVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskTexture
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskTexture::deoglRenderTaskTexture() :
pTexture( NULL ),

pVAOCount( 0 ),

pHasVAO( NULL ),
pHasVAOCount( 0 ),
pHasVAOSize( 0 ){
}

deoglRenderTaskTexture::~deoglRenderTaskTexture(){
	if( pHasVAO ){
		delete [] pHasVAO;
	}
	
	const int vaoCount = pVAOs.GetCount();
	int i;
	for( i=0; i<vaoCount; i++ ){
		delete ( deoglRenderTaskVAO* )pVAOs.GetAt( i );
	}
	pVAOs.RemoveAll();
}



// Management
///////////////

void deoglRenderTaskTexture::Reset(){
	pTexture = NULL;
	pHasVAOCount = 0;
	pVAOCount = 0;
}



int deoglRenderTaskTexture::GetTotalPointCount() const{
	int i, pointCount = 0;
	for( i=0; i<pVAOCount; i++ ){
		pointCount += ( ( deoglRenderTaskVAO* )pVAOs.GetAt( i ) )->GetTotalPointCount();
	}
	return pointCount;
}

int deoglRenderTaskTexture::GetTotalInstanceCount() const{
	int i, instanceCount = 0;
	for( i=0; i<pVAOCount; i++ ){
		instanceCount += ( ( deoglRenderTaskVAO* )pVAOs.GetAt( i ) )->GetInstanceCount();
	}
	return instanceCount;
}

int deoglRenderTaskTexture::GetTotalSubInstanceCount() const{
	int i, subInstanceCount = 0;
	for( i=0; i<pVAOCount; i++ ){
		subInstanceCount += ( ( deoglRenderTaskVAO* )pVAOs.GetAt( i ) )->GetTotalSubInstanceCount();
	}
	return subInstanceCount;
}



void deoglRenderTaskTexture::SetTexture( const deoglRenderTaskSharedTexture *texture ){
	pTexture = texture;
}



deoglRenderTaskVAO *deoglRenderTaskTexture::GetVAOAt( int index ) const{
	return ( deoglRenderTaskVAO* )pVAOs.GetAt( index );
}

deoglRenderTaskVAO *deoglRenderTaskTexture::AddVAO( deoglRenderTaskSharedVAO *vao ){
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	const int index = vao->GetIndex();
	
	if( index >= pHasVAOCount ){
		if( index >= pHasVAOSize ){
			deoglRenderTaskVAO ** const newArray = new deoglRenderTaskVAO*[ index + 1 ];
			
			if( pHasVAO ){
				if( pHasVAOCount > 0 ){
					memcpy( newArray, pHasVAO, sizeof( deoglRenderTaskVAO* ) * pHasVAOCount );
				}
				delete [] pHasVAO;
			}
			
			pHasVAO = newArray;
			pHasVAOSize = index + 1;
		}
		
		if( pHasVAOCount <= index ){
			memset( pHasVAO + pHasVAOCount, 0, sizeof( deoglRenderTaskVAO* ) * ( index - pHasVAOCount + 1 ) );
			pHasVAOCount = index + 1;
		}
	}
	
	deoglRenderTaskVAO *rtvao = pHasVAO[ index ];
	if( rtvao ){
		return rtvao;
	}
	
	if( pVAOCount == pVAOs.GetCount() ){
		rtvao = new deoglRenderTaskVAO;
		pVAOs.Add( rtvao );
		
	}else{
		rtvao = ( deoglRenderTaskVAO* )pVAOs.GetAt( pVAOCount );
		rtvao->Reset();
	}
	pVAOCount++;
	
	rtvao->SetVAO( vao );
	pHasVAO[ index ] = rtvao;
	return rtvao;
}
