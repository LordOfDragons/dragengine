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

#include "decCurveBezierPoint.h"

#include "../exceptions.h"



// Class decCurveBezierPoint
//////////////////////////////

// Constructor, destructor
////////////////////////////

decCurveBezierPoint::decCurveBezierPoint(){
}

decCurveBezierPoint::decCurveBezierPoint( const decVector2 &point ) :
pPoint( point ),
pHandle1( point ),
pHandle2( point ){
}

decCurveBezierPoint::decCurveBezierPoint( const decVector2 &point, const decVector2 &handle1, const decVector2 &handle2 ) :
pPoint( point ),
pHandle1( handle1 ),
pHandle2( handle2 ){
}

decCurveBezierPoint::decCurveBezierPoint( const decCurveBezierPoint &point ) :
pPoint( point.pPoint ),
pHandle1( point.pHandle1 ),
pHandle2( point.pHandle2 ){
}



// Management
///////////////

void decCurveBezierPoint::SetPoint( const decVector2 &point ){
	pPoint = point;
	pHandle1 = point;
	pHandle2 = point;
}

void decCurveBezierPoint::SetPoint( const decVector2 &point, const decVector2 &handle1, const decVector2 &handle2 ){
	pPoint = point;
	pHandle1 = handle1;
	pHandle2 = handle2;
}

bool decCurveBezierPoint::IsEqualTo( const decCurveBezierPoint &point, float threshold ) const{
	return pPoint.IsEqualTo( point.pPoint, threshold )
		&& pHandle1.IsEqualTo( point.pHandle1, threshold )
		&& pHandle2.IsEqualTo( point.pHandle2, threshold );
}
