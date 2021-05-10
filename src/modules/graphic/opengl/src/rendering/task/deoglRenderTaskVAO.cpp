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
#include "deoglRenderTaskVAO.h"
#include "deoglRenderTaskInstance.h"
#include "shared/deoglRenderTaskSharedInstance.h"
#include "../../vbo/deoglVBOLayout.h"
#include "../../utils/deoglQuickSorter.h"

#include <dragengine/common/exceptions.h>



// Class deoglRenderTaskVAO
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglRenderTaskVAO::deoglRenderTaskVAO() :
pVAO( NULL ),
pInstanceCount( 0 ),
pHasInstance( NULL ),
pHasInstanceCount( 0 ),
pHasInstanceSize( 0 ){
}

deoglRenderTaskVAO::~deoglRenderTaskVAO(){
	if( pHasInstance ){
		delete [] pHasInstance;
	}
	
	const int instanceCount = pInstances.GetCount();
	int i;
	for( i=0; i<instanceCount; i++ ){
		delete ( deoglRenderTaskInstance* )pInstances.GetAt( i );
	}
	pInstances.RemoveAll();
}



// Management
///////////////

void deoglRenderTaskVAO::Reset(){
	pVAO = NULL;
	pInstanceCount = 0;
	pHasInstanceCount = 0;
}



void deoglRenderTaskVAO::SetVAO( const deoglRenderTaskSharedVAO *vao ){
	pVAO = vao;
}



int deoglRenderTaskVAO::GetTotalPointCount() const{
	int i, pointCount = 0;
	for( i=0; i<pInstanceCount; i++ ){
		const deoglRenderTaskSharedInstance &instance =
			*( ( deoglRenderTaskInstance* )pInstances.GetAt( i ) )->GetInstance();
		pointCount += ( instance.GetIndexCount() + instance.GetPointCount() )
			* decMath::max( instance.GetSubInstanceCount(), 1 );
	}
	return pointCount;
}

int deoglRenderTaskVAO::GetTotalSubInstanceCount() const{
	int i, subInstanceCount = 0;
	for( i=0; i<pInstanceCount; i++ ){
		subInstanceCount += ( ( deoglRenderTaskInstance* )pInstances.GetAt( i ) )->GetSubInstanceCount();
	}
	return subInstanceCount;
}



deoglRenderTaskInstance *deoglRenderTaskVAO::GetInstanceAt( int index ) const{
	return ( deoglRenderTaskInstance* )pInstances.GetAt( index );
}

deoglRenderTaskInstance *deoglRenderTaskVAO::AddInstance( deoglRenderTaskSharedInstance *instance ){
	if( ! instance ){
		DETHROW( deeInvalidParam );
	}
	
	const int index = instance->GetIndex();
	
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
		
		if( pHasInstanceCount <= index ){
			memset( pHasInstance + pHasInstanceCount, 0,
				sizeof( deoglRenderTaskInstance* ) * ( index - pHasInstanceCount + 1 ) );
			pHasInstanceCount = index + 1;
		}
	}
	
	deoglRenderTaskInstance *rtinstance = pHasInstance[ index ];
	if( rtinstance ){
		return rtinstance;
	}
	
	if( pInstanceCount == pInstances.GetCount() ){
		rtinstance = new deoglRenderTaskInstance;
		pInstances.Add( rtinstance );
		
	}else{
		rtinstance = ( deoglRenderTaskInstance* )pInstances.GetAt( pInstanceCount );
		rtinstance->Reset();
	}
	pInstanceCount++;
	
	rtinstance->SetInstance( instance );
	pHasInstance[ index ] = rtinstance;
	return rtinstance;
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
