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

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "deoglConvexHull3D.h"

#define CONVHULL_3D_ENABLE
#define CONVHULL_3D_USE_FLOAT_PRECISION
#include "../../../convexhull3d/convhull_3d.h"

#include <dragengine/common/exceptions.h>



// Class deoglConvexHull3D
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConvexHull3D::deoglConvexHull3D(){
	pPoints = NULL;
	pPointCount = 0;
	pPointSize = 0;
}

deoglConvexHull3D::~deoglConvexHull3D(){
	if( pPoints ){
		delete [] pPoints;
	}
}



// Management
///////////////

const decVector &deoglConvexHull3D::GetPointAt( int index ) const{
	if( index < 0 || index >= pPointCount ){
		DETHROW( deeInvalidParam );
	}
	return pPoints[ index ];
}

void deoglConvexHull3D::AddPoint( const decVector &point ){
	if( pPointCount == pPointSize ){
		const int newSize = pPointSize + 10;
		decVector * const newArray = new decVector[ newSize ];
		
		if( pPoints ){
			memcpy( newArray, pPoints, sizeof( decVector ) * pPointCount );
			delete [] pPoints;
		}
		pPoints = newArray;
		
		pPointSize = newSize;
	}
	
	pPoints[ pPointCount++ ] = point;
}

void deoglConvexHull3D::RemoveAllPoints(){
	pPointCount = 0;
}



void deoglConvexHull3D::CalculateHull(){
	pHullIndices.RemoveAll();
	
	if( pPointCount < 3 ){
		return;
	}
	
	ch_vertex * const vertices = new ch_vertex[ pPointCount ];
	int i;
	for( i=0; i<pPointCount; i++ ){
		vertices[ i ].x = pPoints[ i ].x;
		vertices[ i ].y = pPoints[ i ].y;
		vertices[ i ].z = pPoints[ i ].z;
	}
	
	int *faceIndices = NULL;
	int nFaces;
	convhull_3d_build( vertices, pPointCount, &faceIndices, &nFaces );
	
	for( i=0; i<nFaces*3; i++ ){
		pHullIndices.Add( faceIndices[ i ] );
	}
	
	free( faceIndices );
	delete [] vertices;
}
