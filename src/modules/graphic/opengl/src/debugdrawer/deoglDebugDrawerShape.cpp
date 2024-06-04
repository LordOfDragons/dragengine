/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglDebugDrawer.h"
#include "deoglDebugDrawerShape.h"
#include "../utils/convexhull/deoglConvexHull3D.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/shape/decShapeVisitor.h>
#include <dragengine/common/shape/decShapeHull.h>
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
pLinePointCount( 0 ),
pHullPoints( NULL ),
pHullPointCount( 0 ),
pDirtyHulls( false ){
}

deoglDebugDrawerShape::~deoglDebugDrawerShape(){
	if( pHullPoints ){
		delete [] pHullPoints;
	}
}



// Management
///////////////

void deoglDebugDrawerShape::SetMatrix( const decMatrix &matrix ){
	pMatrix = matrix;
	pDirtyHulls |= pHullIndices.GetCount() > 0;
}

void deoglDebugDrawerShape::SetEdgeColor( const decColor &color ){
	pEdgeColor = color;
}

void deoglDebugDrawerShape::SetFillColor( const decColor &color ){
	pFillColor = color;
}

void deoglDebugDrawerShape::SetShapeList( const decShapeList &shapes ){
	pShapeList = shapes;
	pDirtyHulls = shapes.GetCount() > 0;
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
	
	pPrepareHulls();
	
	const int faceCount = ddshape.GetFaceCount();
	int i;
	
	pFillFirstPoint = firstPoint;
	for( i=0; i<faceCount; i++ ){
		const int vertexCount = ddshape.GetFaceAt( i )->GetVertexCount();
		if( vertexCount > 2 ){
			pFillPointCount += ( vertexCount - 2 ) * 3;
		}
	}
	pFillPointCount += pHullIndices.GetCount();
	
	pLineFirstPoint = firstPoint + pFillPointCount;
	for( i=0; i<faceCount; i++ ){
		const int vertexCount = ddshape.GetFaceAt( i )->GetVertexCount();
		if( vertexCount > 2 ){
			pLinePointCount += vertexCount * 2;
		}
	}
	pLinePointCount += pHullIndices.GetCount() * 2;
	
	return pFillPointCount + pLinePointCount;
}

void deoglDebugDrawerShape::WriteVBOData( const deDebugDrawerShape &ddshape, oglVector3 *vboData ){
	const int hullIndexCount = pHullIndices.GetCount();
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
	
	for( i=0; i<hullIndexCount; i++ ){
		const decVector &point = pHullPoints[ pHullIndices.GetAt( i ) ];
		vboData[ vi ].x = point.x;
		vboData[ vi ].y = point.y;
		vboData[ vi ].z = point.z;
		vi++;
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
	
	for( i=0; i<hullIndexCount; i+=3 ){
		const decVector &point1 = pHullPoints[ pHullIndices.GetAt( i ) ];
		const decVector &point2 = pHullPoints[ pHullIndices.GetAt( i + 1 ) ];
		const decVector &point3 = pHullPoints[ pHullIndices.GetAt( i + 2 ) ];
		
		vboData[ vi ].x = point1.x;
		vboData[ vi ].y = point1.y;
		vboData[ vi ].z = point1.z;
		vi++;
		
		vboData[ vi ].x = point2.x;
		vboData[ vi ].y = point2.y;
		vboData[ vi ].z = point2.z;
		vi++;
		
		vboData[ vi ].x = point2.x;
		vboData[ vi ].y = point2.y;
		vboData[ vi ].z = point2.z;
		vi++;
		
		vboData[ vi ].x = point3.x;
		vboData[ vi ].y = point3.y;
		vboData[ vi ].z = point3.z;
		vi++;
		
		vboData[ vi ].x = point3.x;
		vboData[ vi ].y = point3.y;
		vboData[ vi ].z = point3.z;
		vi++;
		
		vboData[ vi ].x = point1.x;
		vboData[ vi ].y = point1.y;
		vboData[ vi ].z = point1.z;
		vi++;
	}
	
	if( vi != pLineFirstPoint + pLinePointCount ){
		DETHROW( deeInvalidParam );
	}
}



// Private Functions
//////////////////////

class deoglDebugDrawerShape_ProcessHull : public decShapeVisitor{
	decVector **pHullPoints;
	int *pHullPointCount;
	decIntList &pHullIndices;
	
public:
	deoglDebugDrawerShape_ProcessHull( decVector **hullPoints, int *hullPointCount, decIntList &hullIndices ) :
	pHullPoints( hullPoints ), pHullPointCount( hullPointCount ), pHullIndices( hullIndices ){
	}
	
	virtual void VisitShapeHull( decShapeHull &hull ){
		const int count = hull.GetPointCount();
		if( count < 3 ){
			return;
		}
		
		deoglConvexHull3D calculator;
		int i;
		for( i=0; i<count; i++ ){
			calculator.AddPoint( hull.GetPointAt( i ) );
		}
		calculator.CalculateHull();
		
		const decIntList &indices = calculator.GetHullIndices();
		if( indices.GetCount() < 3 ){
			return;
		}
		
		const int offset = *pHullPointCount;
		decVector * const newArray = new decVector[ offset + count ];
		if( *pHullPoints ){
			memcpy( newArray, *pHullPoints, sizeof( decVector ) * *pHullPointCount );
			delete [] *pHullPoints;
		}
		
		*pHullPoints = newArray;
		*pHullPointCount += count;
		
		for( i=0; i<count; i++ ){
			newArray[ offset + i ] = hull.GetPointAt( i );
		}
		
		for( i=0; i<indices.GetCount(); i++ ){
			pHullIndices.Add( offset + indices.GetAt( i ) );
		}
	}
};

void deoglDebugDrawerShape::pPrepareHulls(){
	if( ! pDirtyHulls ){
		return;
	}
	
	pHullIndices.RemoveAll();
	if( pHullPoints ){
		delete [] pHullPoints;
		pHullPoints = NULL;
		pHullPointCount = 0;
	}
	
	deoglDebugDrawerShape_ProcessHull visitor( &pHullPoints, &pHullPointCount, pHullIndices );
	const int count = pShapeList.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pShapeList.GetAt( i )->Visit( visitor );
	}
	
	pDirtyHulls = false;
}
