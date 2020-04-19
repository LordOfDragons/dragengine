/* 
 * Drag[en]gine Game Engine
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

#include "decShapeHull.h"
#include "decShapeVisitor.h"
#include "../exceptions.h"



// Class decShapeHull
///////////////////////

// Constructors, destructors
//////////////////////////////

decShapeHull::decShapeHull() :
pPoints( NULL ),
pPointCount( 0 ){
}

decShapeHull::decShapeHull( const decVector &position ) :
decShape( position ),
pPoints( NULL ),
pPointCount( 0 ){
}

decShapeHull::decShapeHull( const decVector &position, const decQuaternion &orientation ) :
decShape( position, orientation ),
pPoints( NULL ),
pPointCount( 0 ){
}

decShapeHull::~decShapeHull(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

void decShapeHull::SetPointCount( int count ){
	if( count < 0 ){
		DETHROW( deeInvalidParam );
	}
	
	if( count == pPointCount ){
		return;
	}
	
	if( pPoints ){
		delete [] pPoints;
		pPoints = NULL;
		pPointCount = 0;
	}
	
	if( count > 0 ){
		pPoints = new decVector[ count ];
		pPointCount = count;
	}
}

const decVector &decShapeHull::GetPointAt( int index ) const{
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeOutOfBoundary );
	}
	
	return pPoints[ index ];
}

void decShapeHull::SetPointAt( int index, const decVector &point ){
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeOutOfBoundary );
	}
	
	pPoints[ index ] = point;
}



decShape *decShapeHull::Copy() const{
	decShapeHull * const shape = new decShapeHull( GetPosition(), GetOrientation() );
	int i;
	
	try{
		shape->SetPointCount( pPointCount );
		for( i=0; i<pPointCount; i++ ){
			shape->SetPointAt( i, pPoints[ i ] );
		}
		
	}catch( const deException & ){
		if( shape ){
			delete shape;
		}
		throw;
	}
	
	return shape;
}



// Visiting
/////////////

void decShapeHull::Visit( decShapeVisitor &visitor ){
	visitor.VisitShapeHull( *this );
}
