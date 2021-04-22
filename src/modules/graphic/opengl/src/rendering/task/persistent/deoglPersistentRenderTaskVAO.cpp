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

#include "deoglPersistentRenderTaskVAO.h"
#include "deoglPersistentRenderTaskInstance.h"
#include "../../../vbo/deoglVBOLayout.h"
#include "../../../utils/deoglQuickSorter.h"

#include <dragengine/common/exceptions.h>



// Class deoglPersistentRenderTaskVAO
///////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglPersistentRenderTaskVAO::deoglPersistentRenderTaskVAO( deoglVAO *vao ) :
pVAO( NULL ),
pOwner( NULL )
{
	if( ! vao ){
		DETHROW( deeInvalidParam );
	}
	
	pVAO = vao;
}

deoglPersistentRenderTaskVAO::~deoglPersistentRenderTaskVAO(){
	RemoveAllInstances();
}



// Management
///////////////

void deoglPersistentRenderTaskVAO::SetOwner( void *owner ){
	pOwner = owner;
}

int deoglPersistentRenderTaskVAO::GetTotalPointCount() const{
	const int count = pInstances.GetCount();
	int subInstanceCount;
	int pointCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		const deoglPersistentRenderTaskInstance &instance = *( ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( i ) );
		subInstanceCount = instance.GetSubInstanceCount();
		if( subInstanceCount < 1 ){
			subInstanceCount = 1;
		}
		
		pointCount += ( instance.GetIndexCount() + instance.GetPointCount() ) * subInstanceCount;
	}
	
	return pointCount;
}

int deoglPersistentRenderTaskVAO::GetTotalSubInstanceCount() const{
	const int count = pInstances.GetCount();
	int subInstanceCount = 0;
	int i;
	
	for( i=0; i<count; i++ ){
		subInstanceCount += ( ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( i ) )->GetSubInstanceCount();
	}
	
	return subInstanceCount;
}



int deoglPersistentRenderTaskVAO::GetInstanceCount() const{
	return pInstances.GetCount();
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskVAO::GetInstanceAt( int index ) const{
	return ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( index );
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskVAO::GetInstanceWith( deoglSharedSPBRTIGroup *group ) const{
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskInstance * const instance = ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( i );
		if( instance->GetSubInstanceSPBGroup() == group ){
			return instance;
		}
	}
	
	return NULL;
}

deoglPersistentRenderTaskInstance *deoglPersistentRenderTaskVAO::AddInstance(){
	deoglPersistentRenderTaskInstance * const instance = new deoglPersistentRenderTaskInstance;
	pInstances.Add( instance );
	return instance;
}

void deoglPersistentRenderTaskVAO::RemoveAllInstances(){
	const int count = pInstances.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		delete ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( i );
	}
	pInstances.RemoveAll();
}



void deoglPersistentRenderTaskVAO::RemoveOwnedBy( void *owner ){
	int i, count = pInstances.GetCount();
	for( i=0; i<count; i++ ){
		deoglPersistentRenderTaskInstance * const instance = ( deoglPersistentRenderTaskInstance* )pInstances.GetAt( i );
		
		if( ! ( instance->GetOwner() && instance->GetOwner() == owner ) ){
			instance->RemoveOwnedBy( owner );
			
			if( instance->GetSubInstanceCount() > 0 ){
				continue;
			}
		}
		
		if( i < count - 1 ){
			pInstances.SetAt( i, pInstances.GetAt( count - 1 ) );
		}
		pInstances.RemoveFrom( count - 1 );
		count--;
		i--;
		
		delete instance;
	}
}
