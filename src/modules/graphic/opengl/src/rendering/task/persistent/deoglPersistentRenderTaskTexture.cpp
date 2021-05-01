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

#include "deoglPersistentRenderTaskPool.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskShader.h"
#include "../../../texture/texunitsconfig/deoglTexUnitsConfig.h"
#include "../../../vao/deoglVAO.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskTexture
///////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskTexture::deoglPersistentRenderTaskTexture( deoglPersistentRenderTaskPool &pool ) :
pPool( pool ),
pLLShader( this ),

pParentShader( NULL ),
pTUC( NULL ),
pParamBlock( NULL ){
}

deoglPersistentRenderTaskTexture::~deoglPersistentRenderTaskTexture(){
	Clear();
}



// Management
///////////////

void deoglPersistentRenderTaskTexture::SetParentShader( deoglPersistentRenderTaskShader *shader ){
	pParentShader = shader;
}

void deoglPersistentRenderTaskTexture::SetTUC( const deoglTexUnitsConfig *tuc ){
	pTUC = tuc;
}

int deoglPersistentRenderTaskTexture::GetTotalPointCount() const{
	decPointerLinkedList::cListEntry *iter = pVAOs.GetRoot();
	int pointCount = 0;
	
	while( iter ){
		pointCount += ( ( deoglPersistentRenderTaskVAO* )iter->GetOwner() )->GetTotalPointCount();
		iter = iter->GetNext();
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskTexture::GetTotalInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pVAOs.GetRoot();
	int instanceCount = 0;
	
	while( iter ){
		instanceCount += ( ( deoglPersistentRenderTaskVAO* )iter->GetOwner() )->GetInstanceCount();
		iter = iter->GetNext();
	}
	
	return instanceCount;
}

int deoglPersistentRenderTaskTexture::GetTotalSubInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pVAOs.GetRoot();
	int subInstanceCount = 0;
	
	while( iter ){
		subInstanceCount += ( ( deoglPersistentRenderTaskVAO* )iter->GetOwner() )->GetTotalSubInstanceCount();
		iter = iter->GetNext();
	}
	
	return subInstanceCount;
}



void deoglPersistentRenderTaskTexture::SetParameterBlock( const deoglSPBlockUBO *block ){
	pParamBlock = block;
}



int deoglPersistentRenderTaskTexture::GetVAOCount() const{
	return pVAOs.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTaskTexture::GetRootVAO() const{
	return pVAOs.GetRoot();
}

deoglPersistentRenderTaskVAO *deoglPersistentRenderTaskTexture::GetVAOWith( const deoglVAO *vao ) const{
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	deoglPersistentRenderTaskVAO *rtvao;
	return pVAOsMap.GetAt( vao, vao->GetUniqueKey(), ( void** )&rtvao ) ? rtvao : NULL;
}

deoglPersistentRenderTaskVAO *deoglPersistentRenderTaskTexture::AddVAO( const deoglVAO *vao ){
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	// commented out in the name of performance
// 	if( pVAOsMap.Has( vao, vao->GetUniqueKey() ) ){
// 		DETHROW( deeInvalidParam );
// 	}
	
	deoglPersistentRenderTaskVAO * const rtvao = pPool.GetVAO();
	pVAOs.Add( &rtvao->GetLLTexture() );
	rtvao->SetParentTexture( this );
	rtvao->SetVAO( vao );
	pVAOsMap.SetAt( vao, vao->GetUniqueKey(), rtvao );
	return rtvao;
}

void deoglPersistentRenderTaskTexture::RemoveVAO( deoglPersistentRenderTaskVAO *vao ){
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	pVAOsMap.Remove( vao->GetVAO(), vao->GetVAO()->GetUniqueKey() );
	pVAOs.Remove( &vao->GetLLTexture() );
	pPool.ReturnVAO( vao );
}

void deoglPersistentRenderTaskTexture::RemoveAllVAOs(){
	decPointerLinkedList::cListEntry *iter = pVAOs.GetRoot();
	while( iter ){
		pPool.ReturnVAO( ( deoglPersistentRenderTaskVAO* )iter->GetOwner() );
		iter = iter->GetNext();
	}
	pVAOs.RemoveAll();
	pVAOsMap.RemoveAll();
}

void deoglPersistentRenderTaskTexture::Clear(){
	RemoveAllVAOs();
	
	pParentShader = NULL;
	pTUC = NULL;
	pParamBlock = NULL;
}

void deoglPersistentRenderTaskTexture::RemoveFromParentIfEmpty(){
	if( pVAOs.GetCount() > 0 ){
		return;
	}
	
	deoglPersistentRenderTaskShader * const shader = pParentShader;
	shader->RemoveTexture( this ); // clears pParentShader
	shader->RemoveFromParentIfEmpty();
}
