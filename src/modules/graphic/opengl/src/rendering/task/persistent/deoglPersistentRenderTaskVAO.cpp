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
#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskTexture.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "../../../vbo/deoglVBOLayout.h"
#include "../../../utils/deoglQuickSorter.h"
#include "../../../shaders/paramblock/shared/deoglSharedSPBRTIGroup.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskVAO
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskVAO::deoglPersistentRenderTaskVAO( deoglPersistentRenderTaskPool &pool ) :
pPool( pool ),
pLLTexture( this ),

pParentTexture( NULL ),
pVAO( NULL ){
}

deoglPersistentRenderTaskVAO::~deoglPersistentRenderTaskVAO(){
	Clear();
}



// Management
///////////////

void deoglPersistentRenderTaskVAO::SetParentTexture( deoglPersistentRenderTaskTexture *texture ){
	pParentTexture = texture;
}

void deoglPersistentRenderTaskVAO::SetVAO( deoglVAO *vao ){
	pVAO = vao;
}

int deoglPersistentRenderTaskVAO::GetTotalPointCount() const{
	decPointerLinkedList::cListEntry *iter = pInstances.GetRoot();
	int pointCount = 0;
	
	while( iter ){
		const deoglPersistentRenderTaskInstance &instance = *( ( deoglPersistentRenderTaskInstance* )iter->GetOwner() );
		pointCount += ( instance.GetIndexCount() + instance.GetPointCount() ) * decMath::max( instance.GetSubInstanceCount(), 1 );
		iter = iter->GetNext();
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskVAO::GetTotalSubInstanceCount() const{
	decPointerLinkedList::cListEntry *iter = pInstances.GetRoot();
	int subInstanceCount = 0;
	
	while( iter ){
		subInstanceCount += ( ( deoglPersistentRenderTaskInstance* )iter->GetOwner() )->GetSubInstanceCount();
		iter = iter->GetNext();
	}
	
	return subInstanceCount;
}



int deoglPersistentRenderTaskVAO::GetInstanceCount() const{
	return pInstances.GetCount();
}

decPointerLinkedList::cListEntry *deoglPersistentRenderTaskVAO::GetRootInstance() const{
	return pInstances.GetRoot();
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskVAO::GetInstanceWith( deoglSharedSPBRTIGroup *group ) const{
	if( ! group ){
		DETHROW( deeInvalidParam );
	}
	
	deoglPersistentRenderTaskInstance *instance;
	return pInstancesMap.GetAt( group, group->GetUniqueKey(), ( void** )&instance ) ? instance : NULL;
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskVAO::AddInstance(
deoglSharedSPB *spb, deoglSharedSPBRTIGroup *group ){
	deoglPersistentRenderTaskInstance * const instance = pPool.GetInstance();
	pInstances.Add( &instance->GetLLVAO() );
	instance->SetParentVAO( this );
	instance->SetSubInstanceSPB( spb, group );
	if( group ){
		pInstancesMap.SetAt( group, group->GetUniqueKey(), instance );
	}
	return instance;
}

void deoglPersistentRenderTaskVAO::RemoveInstance( deoglPersistentRenderTaskInstance *instance ){
	if( ! instance ){
		DETHROW( deeInvalidParam );
	}
	
	if( instance->GetSubInstanceSPBGroup() ){
		pInstancesMap.Remove( instance->GetSubInstanceSPBGroup(), instance->GetSubInstanceSPBGroup()->GetUniqueKey() );
	}
	pInstances.Remove( &instance->GetLLVAO() );
	pPool.ReturnInstance( instance );
}

void deoglPersistentRenderTaskVAO::RemoveAllInstances(){
	decPointerLinkedList::cListEntry *iter = pInstances.GetRoot();
	while( iter ){
		pPool.ReturnInstance( ( deoglPersistentRenderTaskInstance* )iter->GetOwner() );
		iter = iter->GetNext();
	}
	pInstances.RemoveAll();
	pInstancesMap.RemoveAll();
}



void deoglPersistentRenderTaskVAO::Clear(){
	RemoveAllInstances();
	
	pParentTexture = NULL;
	pVAO = NULL;
}

void deoglPersistentRenderTaskVAO::RemoveFromParentIfEmpty(){
	if( pInstances.GetCount() > 0 ){
		return;
	}
	
	deoglPersistentRenderTaskTexture * const texture = pParentTexture;
	texture->RemoveVAO( this ); // clears pParentTexture
	texture->RemoveFromParentIfEmpty();
}
