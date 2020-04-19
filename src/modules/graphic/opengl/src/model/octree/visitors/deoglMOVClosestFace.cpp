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

#include "deoglMOVClosestFace.h"
#include "../deoglModelOctree.h"
#include "../../deoglModelLOD.h"
#include "../../face/deoglModelFace.h"
#include "../../../utils/collision/deoglCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// #define DEBUG_MOV_CLOSEST_FACE

#ifdef DEBUG_MOV_CLOSEST_FACE
int movClosestFace_NodesTested = 0;
int movClosestFace_FacesTested = 0;
int movClosestFace_TotalFaces = 0;
#endif



// Class deoglMOVClosestFace
//////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglMOVClosestFace::deoglMOVClosestFace( const deoglModelLOD &modelLOD ) :
pModelLOD( modelLOD ),
pMaxRadius( 1.0f ),
pFaceIndex( -1 ),
pFaceDistanceSquared( 0.0f )
{ }

deoglMOVClosestFace::~deoglMOVClosestFace(){
}



// Visiting
/////////////

void deoglMOVClosestFace::SetTestPoint( const decVector &testPoint, float maxRadius ){
	if( maxRadius < 0.0f ){
		maxRadius = 0.0f;
	}
	
	pTestPoint = testPoint;
	pMaxRadius = maxRadius;
	
	pTestBoxMin.x = testPoint.x - maxRadius;
	pTestBoxMin.y = testPoint.y - maxRadius;
	pTestBoxMin.z = testPoint.z - maxRadius;
	
	pTestBoxMax.x = testPoint.x + maxRadius;
	pTestBoxMax.y = testPoint.y + maxRadius;
	pTestBoxMax.z = testPoint.z + maxRadius;
}

void deoglMOVClosestFace::Reset(){
	pFaceDistanceSquared = pMaxRadius * pMaxRadius;
	pFaceIndex = -1;
	
	#ifdef DEBUG_MOV_CLOSEST_FACE
	movClosestFace_NodesTested = 0;
	movClosestFace_FacesTested = 0;
	movClosestFace_TotalFaces = 0;
	#endif
}

int deoglMOVClosestFace::FindClosestFace( const decVector &testPoint, float maxRadius ){
	if( ! pModelLOD.GetOctree() ){
		DETHROW( deeInvalidParam );
	}
	
	SetTestPoint( testPoint, maxRadius );
	Reset();
	pModelLOD.GetOctree()->VisitNodesColliding( this, pTestBoxMin, pTestBoxMax );
	return pFaceIndex;
}



void deoglMOVClosestFace::VisitNode( deoglOctree *node, int intersection ){
	const deoglModelOctree &psonode = *( ( deoglModelOctree* )node );
	const oglModelPosition * const positions = pModelLOD.GetPositions();
	const oglModelVertex * const vertices = pModelLOD.GetVertices();
	const deoglModelFaceList &list = psonode.GetFaceList();
	const int faceCount = list.GetCount();
	int i;
	
	#ifdef DEBUG_MOV_CLOSEST_FACE
	movClosestFace_NodesTested++;
	movClosestFace_TotalFaces += faceCount;
	#endif
	
	for( i=0; i<faceCount; i++ ){
		const deoglModelFace &face = *list.GetAt( i );
		
		if( pTestBoxMax >= face.GetMinExtend() && pTestBoxMin <= face.GetMaxExtend() ){
			#ifdef DEBUG_MOV_CLOSEST_FACE
			movClosestFace_FacesTested++;
			#endif
			
			// TODO this can be optimized by moving the constant edge parameters (normal, distance)
			//      into deoglModelFace. this reduces the ClosestPointOnTriangle calculation to
			//      a few cheap operations removing the need for multiple normalizations
			const oglModelVertex &v1 = vertices[ face.GetVertex1() ];
			const oglModelVertex &v2 = vertices[ face.GetVertex2() ];
			const oglModelVertex &v3 = vertices[ face.GetVertex3() ];
			const decVector &p1 = positions[ v1.position ].position;
			const decVector &p2 = positions[ v2.position ].position;
			const decVector &p3 = positions[ v3.position ].position;
			const float distanceSquared = ( deoglCollisionDetection::ClosestPointOnTriangle( p1, p2, p3, pTestPoint ) - pTestPoint ).LengthSquared();
			
			if( distanceSquared < pFaceDistanceSquared ){
				pFaceDistanceSquared = distanceSquared;
				pFaceIndex = face.GetIndex();
			}
		}
	}
}
