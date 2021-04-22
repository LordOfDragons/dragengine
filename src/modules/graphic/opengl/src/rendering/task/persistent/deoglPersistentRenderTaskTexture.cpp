/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskTexture
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskTexture::deoglPersistentRenderTaskTexture( deoglTexUnitsConfig *tuc ) :
pTUC( NULL ),
pParamBlock( NULL )
{
	if( ! tuc ){
		DETHROW( deeInvalidParam );
	}
	
	pTUC = tuc;
	tuc->AddUsage();
}

deoglPersistentRenderTaskTexture::~deoglPersistentRenderTaskTexture(){
	RemoveAllVAOs();
	
	if( pTUC ){
		pTUC->RemoveUsage();
	}
}



// Management
///////////////

int deoglPersistentRenderTaskTexture::GetTotalPointCount() const{
	const int count = pVAOs.GetCount();
	int pointCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		pointCount += ( ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( i ) )->GetTotalPointCount();
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskTexture::GetTotalInstanceCount() const{
	const int count = pVAOs.GetCount();
	int instanceCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		instanceCount += ( ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( i ) )->GetInstanceCount();
	}
	
	return instanceCount;
}

int deoglPersistentRenderTaskTexture::GetTotalSubInstanceCount() const{
	const int count = pVAOs.GetCount();
	int subInstanceCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		subInstanceCount += ( ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( i ) )->GetTotalSubInstanceCount();
	}
	
	return subInstanceCount;
}



void deoglPersistentRenderTaskTexture::SetParameterBlock( deoglSPBlockUBO *block ){
	pParamBlock = block;
}



int deoglPersistentRenderTaskTexture::GetVAOCount() const{
	return pVAOs.GetCount();
}

deoglPersistentRenderTaskVAO * deoglPersistentRenderTaskTexture::GetVAOAt( int index ) const{
	return ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( index );
}

deoglPersistentRenderTaskVAO *deoglPersistentRenderTaskTexture::GetVAOWith( deoglVAO *vao ) const{
	const int count = pVAOs.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskVAO * const rtvao = ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( i );
		if( rtvao->GetVAO() == vao ){
			return rtvao;
		}
	}
	
	return NULL;
}

deoglPersistentRenderTaskVAO *deoglPersistentRenderTaskTexture::AddVAO( deoglVAO *vao ){
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	deoglPersistentRenderTaskVAO * const rtvao = new deoglPersistentRenderTaskVAO( vao );
	pVAOs.Add( rtvao );
	return rtvao;
}

void deoglPersistentRenderTaskTexture::RemoveAllVAOs(){
	const int count = pVAOs.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( i );
	}
	pVAOs.RemoveAll();
}



void deoglPersistentRenderTaskTexture::RemoveOwnedBy( void *owner ){
	int i, count = pVAOs.GetCount();
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskVAO * const vao = ( deoglPersistentRenderTaskVAO* )pVAOs.GetAt( i );
		
		if( ! ( vao->GetOwner() && vao->GetOwner() == owner ) ){
			vao->RemoveOwnedBy( owner );
			
			if( vao->GetInstanceCount() > 0 ){
				continue;
			}
		}
		
		if( i < count - 1 ){
			pVAOs.SetAt( i, pVAOs.GetAt( count - 1 ) );
		}
		pVAOs.RemoveFrom( count - 1 );
		count--;
		i--;
		
		delete vao;
	}
}
