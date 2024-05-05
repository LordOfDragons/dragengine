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
