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
#include <stdlib.h>

#include "decCurveBezier3DPoint.h"

#include "../exceptions.h"



// Class decCurveBezier3DPoint
////////////////////////////////

// Constructor, destructor
////////////////////////////

decCurveBezier3DPoint::decCurveBezier3DPoint(){
}

decCurveBezier3DPoint::decCurveBezier3DPoint( const decVector &point ) :
pPoint( point ),
pHandle1( point ),
pHandle2( point ){
}

decCurveBezier3DPoint::decCurveBezier3DPoint( const decVector &point, const decVector &handle1, const decVector &handle2 ) :
pPoint( point ),
pHandle1( handle1 ),
pHandle2( handle2 ){
}

decCurveBezier3DPoint::decCurveBezier3DPoint( const decCurveBezier3DPoint &point ) :
pPoint( point.pPoint ),
pHandle1( point.pHandle1 ),
pHandle2( point.pHandle2 ){
}



// Management
///////////////

void decCurveBezier3DPoint::SetPoint( const decVector &point ){
	pPoint = point;
	pHandle1 = point;
	pHandle2 = point;
}

void decCurveBezier3DPoint::SetPoint( const decVector &point, const decVector &handle1, const decVector &handle2 ){
	pPoint = point;
	pHandle1 = handle1;
	pHandle2 = handle2;
}



// Operators
//////////////

bool decCurveBezier3DPoint::operator==( const decCurveBezier3DPoint &point ) const{
	return pPoint.IsEqualTo( point.pPoint )
		&& pHandle1.IsEqualTo( point.pHandle1 )
		&& pHandle2.IsEqualTo( point.pHandle2 );
}

bool decCurveBezier3DPoint::operator!=( const decCurveBezier3DPoint &point ) const{
	return ! pPoint.IsEqualTo( point.pPoint )
		|| ! pHandle1.IsEqualTo( point.pHandle1 )
		|| ! pHandle2.IsEqualTo( point.pHandle2 );
}
