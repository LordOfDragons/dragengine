/* 
 * Drag[en]gine IGDE Rig Editor
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "reRigShapeHull.h"
#include "../reRig.h"
#include "../bone/reRigBone.h"

#include <dragengine/deEngine.h>
#include <dragengine/deObjectReference.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/resources/rig/deRig.h>
#include <dragengine/resources/rig/deRigBone.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/collider/deColliderVolume.h>



// Class reRigShapeHull
/////////////////////////

// Constructor, destructor
////////////////////////////

reRigShapeHull::reRigShapeHull( deEngine *engine ) :
reRigShape( engine, estHull ),
pPoints( NULL ),
pPointCount( 0 ){
}

reRigShapeHull::~reRigShapeHull(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

const decVector &reRigShapeHull::GetPointAt( int index ) const{
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeOutOfBoundary );
	}
	
	return pPoints[ index ];
}

void reRigShapeHull::AddPoint( const decVector &point ){
	InsertPoint( point, pPointCount );
}

void reRigShapeHull::InsertPoint( const decVector &point, int index ){
	if( index < 0 || index > pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	decVector * const newArray = new decVector[ pPointCount + 1 ];
	if( pPoints ){
		int i;
		for( i=0; i<index; i++ ){
			newArray[ i ] = pPoints[ i ];
		}
		for( i=index; i<pPointCount; i++ ){
			newArray[ i + 1 ] = pPoints[ i ];
		}
		delete [] pPoints;
	}
	pPoints = newArray;
	pPointCount++;
	
	pPoints[ index ] = point;
	
	NotifyShapeChanged();
}

void reRigShapeHull::SetPointAt( int index, const decVector &point ){
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeOutOfBoundary );
	}
	
	if( pPoints[ index ].IsEqualTo( point ) ){
		return;
	}
	
	pPoints[ index ] = point;
	
	NotifyShapeChanged();
}

void reRigShapeHull::RemovePoint( int index ){
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	int i;
	for( i=index+1; i<pPointCount; i++ ){
		pPoints[ i - 1 ] = pPoints[ i ];
	}
	pPointCount--;
	
	NotifyShapeChanged();
}



reRigShape *reRigShapeHull::Duplicate() const{
	deObjectReference ref;
	ref.TakeOver( new reRigShapeHull( GetEngine() ) );
	
	reRigShapeHull * const shape = ( reRigShapeHull* )( deObject* )ref;
	shape->SetPosition( GetPosition() );
	shape->SetOrientation( GetOrientation() );
	if( pPointCount > 0 ){
		shape->pPoints = new decVector[ pPointCount ];
		shape->pPointCount = pPointCount;
		memcpy( shape->pPoints, pPoints, sizeof( decVector ) * pPointCount );
	}
	
	shape->AddReference(); // caller takes over reference
	return shape;
}

void reRigShapeHull::Scale( float scale ){
	SetPosition( GetPosition() * scale );
	int i;
	for( i=0; i<pPointCount; i++ ){
		pPoints[ i ] *= scale;
	}
}

decShape *reRigShapeHull::CreateShape(){
	decShapeHull * const shape = new decShapeHull( GetPosition(),
		decQuaternion::CreateFromEuler( GetOrientation() * DEG2RAD ) );
	if( pPointCount > 0 ){
		try{
			shape->SetPointCount( pPointCount );
			int i;
			for( i=0; i<pPointCount; i++ ){
				shape->SetPointAt( i, pPoints[ i ] );
			}
			
		}catch( const deException & ){
			delete shape;
			throw;
		}
	}
	
	return shape;
}
