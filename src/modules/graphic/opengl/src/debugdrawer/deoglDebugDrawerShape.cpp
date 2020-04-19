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

#include "deoglDebugDrawer.h"
#include "deoglDebugDrawerShape.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/debug/deDebugDrawer.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>



// Class deoglDebugDrawerShape
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDebugDrawerShape::deoglDebugDrawerShape() :
pFillFirstPoint( 0 ),
pFillPointCount( 0 ),
pLineFirstPoint( 0 ),
pLinePointCount( 0 ){
}

deoglDebugDrawerShape::~deoglDebugDrawerShape(){
}



// Management
///////////////

void deoglDebugDrawerShape::SetMatrix( const decMatrix &matrix ){
	pMatrix = matrix;
}

void deoglDebugDrawerShape::SetEdgeColor( const decColor &color ){
	pEdgeColor = color;
}

void deoglDebugDrawerShape::SetFillColor( const decColor &color ){
	pFillColor = color;
}

void deoglDebugDrawerShape::SetShapeList( const decShapeList &shapes ){
	pShapeList = shapes;
}

void deoglDebugDrawerShape::SetFillFirstPoint( int firstPoint ){
	pFillFirstPoint = firstPoint;
}

void deoglDebugDrawerShape::SetFillPointCount( int pointCount ){
	pFillPointCount = pointCount;
}

void deoglDebugDrawerShape::SetLineFirstPoint( int firstPoint ){
	pLineFirstPoint = firstPoint;
}

void deoglDebugDrawerShape::SetLinePointCount( int pointCount ){
	pLinePointCount = pointCount;
}



int deoglDebugDrawerShape::CalcRequiredPoints( const deDebugDrawerShape &ddshape, int firstPoint ){
	pFillFirstPoint = 0;
	pFillPointCount = 0;
	pLineFirstPoint = 0;
	pLinePointCount = 0;
	
	const int faceCount = ddshape.GetFaceCount();
	int i;
	
	pFillFirstPoint = firstPoint;
	for( i=0; i<faceCount; i++ ){
		const int vertexCount = ddshape.GetFaceAt( i )->GetVertexCount();
		if( vertexCount > 2 ){
			pFillPointCount += ( vertexCount - 2 ) * 3;
		}
	}
	
	pLineFirstPoint = firstPoint + pFillPointCount;
	for( i=0; i<faceCount; i++ ){
		const int vertexCount = ddshape.GetFaceAt( i )->GetVertexCount();
		if( vertexCount > 2 ){
			pLinePointCount += vertexCount * 2;
		}
	}
	
	return pFillPointCount + pLinePointCount;
}

void deoglDebugDrawerShape::WriteVBOData( const deDebugDrawerShape &ddshape, oglVector3 *vboData ){
	const int faceCount = ddshape.GetFaceCount();
	int i, j, vi;
	
	// fill faces
	vi = pFillFirstPoint;
	for( i=0; i<faceCount; i++ ){
		const deDebugDrawerShapeFace &ddsface = *ddshape.GetFaceAt( i );
		const int vertexCount = ddsface.GetVertexCount();
		
		if( vertexCount > 2 ){
			const decVector &v1 = ddsface.GetVertexAt( 0 );
			
			for( j=2; j<vertexCount; j++ ){
				const decVector &v2 = ddsface.GetVertexAt( j - 1 );
				const decVector &v3 = ddsface.GetVertexAt( j );
				
				vboData[ vi ].x = v1.x;
				vboData[ vi ].y = v1.y;
				vboData[ vi ].z = v1.z;
				vi++;
				
				vboData[ vi ].x = v3.x;
				vboData[ vi ].y = v3.y;
				vboData[ vi ].z = v3.z;
				vi++;
				
				vboData[ vi ].x = v2.x;
				vboData[ vi ].y = v2.y;
				vboData[ vi ].z = v2.z;
				vi++;
			}
		}
	}
	if( vi != pFillFirstPoint + pFillPointCount ){
		DETHROW( deeInvalidParam );
	}
	
	// lines
	vi = pLineFirstPoint;
	for( i=0; i<faceCount; i++ ){
		const deDebugDrawerShapeFace &ddsface = *ddshape.GetFaceAt( i );
		const int vertexCount = ddsface.GetVertexCount();
		
		if( vertexCount > 2 ){
			for( j=0; j<vertexCount; j++ ){
				const decVector &v1 = ddsface.GetVertexAt( j );
				const decVector &v2 = ddsface.GetVertexAt( ( j + 1 ) % vertexCount );
				
				vboData[ vi ].x = v1.x;
				vboData[ vi ].y = v1.y;
				vboData[ vi ].z = v1.z;
				vi++;
				
				vboData[ vi ].x = v2.x;
				vboData[ vi ].y = v2.y;
				vboData[ vi ].z = v2.z;
				vi++;
			}
		}
	}
	if( vi != pLineFirstPoint + pLinePointCount ){
		DETHROW( deeInvalidParam );
	}
}
