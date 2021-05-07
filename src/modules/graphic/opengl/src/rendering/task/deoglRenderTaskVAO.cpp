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

#include "deoglRenderTaskVAO.h"
#include "deoglRenderTaskInstance.h"
#include "deoglRenderTaskInstanceGroup.h"
#include "../../vbo/deoglVBOLayout.h"
#include "../../utils/deoglQuickSorter.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskVAO
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskVAO::deoglRenderTaskVAO() :
pVAO( NULL ),
pRootInstance( NULL ),
pTailInstance( NULL ),
pInstanceCount( 0 ),
pNextVAO( NULL ),
pLLNext( NULL ),
pHasInstance( NULL ),
pHasInstanceCount( 0 ),
pHasInstanceSize( 0 ){
}

deoglRenderTaskVAO::~deoglRenderTaskVAO(){
	if( pHasInstance ){
		delete [] pHasInstance;
	}
}



// Management
///////////////

void deoglRenderTaskVAO::Reset(){
	pVAO = NULL;
	
	pRootInstance = NULL;
	pTailInstance = NULL;
	pInstanceCount = 0;
	
	pHasInstanceCount = 0;
	
	pNextVAO = NULL;
}



void deoglRenderTaskVAO::SetVAO( const deoglRenderTaskSharedVAO *vao ){
	pVAO = vao;
}



int deoglRenderTaskVAO::GetTotalPointCount() const{
	deoglRenderTaskInstance *instance = pRootInstance;
	int subInstanceCount;
	int pointCount = 0;
	
	while( instance ){
		subInstanceCount = instance->GetSubInstanceCount();
		if( subInstanceCount < 1 ){
			subInstanceCount = 1;
		}
		
		pointCount += ( instance->GetIndexCount() + instance->GetPointCount() ) * subInstanceCount;
		instance = instance->GetNextInstance();
	}
	
	return pointCount;
}

int deoglRenderTaskVAO::GetTotalSubInstanceCount() const{
	deoglRenderTaskInstance *instance = pRootInstance;
	int subInstanceCount = 0;
	
	while( instance ){
		subInstanceCount += instance->GetSubInstanceCount();
		instance = instance->GetNextInstance();
	}
	
	return subInstanceCount;
}



deoglRenderTaskInstance *deoglRenderTaskVAO::GetInstanceWith( deoglRenderTaskInstanceGroup *group ){
	deoglRenderTaskInstance *instance = pRootInstance;
	while( instance ){
		if( instance->GetGroup() == group ){
			return instance;
		}
		instance = instance->GetNextInstance();
	}
	return NULL;
}

void deoglRenderTaskVAO::AddInstance( deoglRenderTaskInstance *instance ){
	if( ! instance ){
		DETHROW( deeInvalidParam );
	}
	
	if( pTailInstance ){
		pTailInstance->SetNextInstance( instance );
	}
	instance->SetNextInstance( NULL );
	
	pTailInstance = instance;
	
	if( ! pRootInstance ){
		pRootInstance = instance;
	}
	
	pInstanceCount++;
	
	// mark as added if group is present
	if( ! instance->GetGroup() ){
		return;
	}
	
	const int index = instance->GetGroup()->GetIndex();
	
	if( index >= pHasInstanceCount ){
		if( index >= pHasInstanceSize ){
			deoglRenderTaskInstance ** const newArray = new deoglRenderTaskInstance*[ index + 1 ];
			
			if( pHasInstance ){
				if( pHasInstanceCount > 0 ){
					memcpy( newArray, pHasInstance, sizeof( deoglRenderTaskInstance* ) * pHasInstanceCount );
				}
				delete [] pHasInstance;
			}
			
			pHasInstance = newArray;
			pHasInstanceSize = index + 1;
		}
		
		while( pHasInstanceCount < index ){
			pHasInstance[ pHasInstanceCount++ ] = NULL;
		}
		pHasInstanceCount++;
	}
	
	pHasInstance[ index ] = instance;
}

deoglRenderTaskInstance * deoglRenderTaskVAO::GetInstanceForIndex( int index ){
	return index < pHasInstanceCount ? pHasInstance[ index ] : NULL;
}



void deoglRenderTaskVAO::SortInstancesByDistance( deoglQuickSorter &sorter,
const decDVector &position, const decDVector &direction, double posDotDir ){
	// instance has no more a matrix model view parameter. to get sorting working again it would be required
	// to stored in the instance the sorting score directly
#if 0
	if( pInstanceCount > 1 ){
		deoglRenderTaskInstance *instance = pRootInstance;
		deoglRenderTaskInstance *newRoot, *newTail;
		int i;
		
		sorter.RemoveAllElements();
		while( instance ){
			sorter.AddElement( instance, ( float )( instance->GetMatrixModelView().GetPosition() * direction - posDotDir ) );
			instance = instance->GetNextInstance();
		}
		
		sorter.SortAscending();
		
		void ** elements = sorter.GetElements();
		
		newRoot = ( deoglRenderTaskInstance* )elements[ 0 ];
		newTail = newRoot;
		
		for( i=1; i<pInstanceCount; i++ ){
			instance = ( deoglRenderTaskInstance* )elements[ i ];
			newTail->SetNextInstance( instance );
			newTail = instance;
		}
		
		newTail->SetNextInstance( NULL );
		
		pRootInstance = newRoot;
		pTailInstance = newTail;
	}
#endif
}



void deoglRenderTaskVAO::SetNextVAO( deoglRenderTaskVAO *vao ){
	pNextVAO = vao;
}



// Linked List
////////////////

void deoglRenderTaskVAO::SetLLNext( deoglRenderTaskVAO *vao ){
	pLLNext = vao;
}
