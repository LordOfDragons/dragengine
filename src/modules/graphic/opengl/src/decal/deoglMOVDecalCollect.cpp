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

#include "deoglMOVDecalCollect.h"
#include "../model/deoglModelLOD.h"
#include "../model/face/deoglModelFace.h"
#include "../model/octree/deoglModelOctree.h"
#include "../model/texture/deoglModelTexture.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../utils/collision/deoglCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoglMOVDecalCollect
///////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglMOVDecalCollect::deoglMOVDecalCollect( const deoglModelLOD &modelLOD,
deoglTriangleSorter &triangleSorter ) :
pModelLOD( modelLOD ),
pTriangleSorter( triangleSorter ){
}

deoglMOVDecalCollect::~deoglMOVDecalCollect(){
}



// Visiting
/////////////

void deoglMOVDecalCollect::CollectFaces( const decMatrix &matrix, const decVector &size ){
	if( ! pModelLOD.GetOctree() ){
		DETHROW( deeInvalidParam );
	}
	
	pTriangleSorter.RemoveAllTriangles();
	
	// calculate planes. first plane is required to be the near plane. the decal matrix
	// faces away from the surface so the planes might look funny because they form the
	// box starting at the decal plane going backwards (relative to decal matrix)
	enum ePlanes{ epNear, epFar, epLeft, epRight, epBottom, epTop };
	
	pPlaneNormal[ epFar ] = matrix.TransformView();
	pPlaneNormal[ epNear ] = -pPlaneNormal[ epFar ];
	pPlaneNormal[ epRight ] = matrix.TransformRight();
	pPlaneNormal[ epLeft ] = -pPlaneNormal[ epRight ];
	pPlaneNormal[ epBottom ] = matrix.TransformUp();
	pPlaneNormal[ epTop ] = -pPlaneNormal[ epBottom ];
	
	const decVector view( pPlaneNormal[ epNear ] * size.z );
	const decVector right( pPlaneNormal[ epLeft ] * ( size.x * 0.5f ) );
	const decVector up( pPlaneNormal[ epBottom ] * ( size.y * 0.5f ) );
	const decVector origin( matrix.GetPosition() );
	const decVector center( origin + view * 0.5f );
	
	const decVector back( origin + view );
	pPlaneDistance[ epNear ] = pPlaneNormal[ epNear ] * origin;
	pPlaneDistance[ epFar ] = pPlaneNormal[ epFar ] * back;
	pPlaneDistance[ epLeft ] = pPlaneNormal[ epLeft ] * ( center - right );
	pPlaneDistance[ epRight ] = pPlaneNormal[ epRight ] * ( center + right );
	pPlaneDistance[ epBottom ] = pPlaneNormal[ epBottom ] * ( center - up );
	pPlaneDistance[ epTop ] = pPlaneNormal[ epTop ] * ( center + up );
	
	// calculate box to limit visiting
	const decVector temp[ 4 ] = {
		origin + right, origin - right, back + right, back - right };
	
	const decVector box[ 8 ] = {
		temp[ 0 ] + up, temp[ 0 ] - up, temp[ 1 ] + up, temp[ 1 ] - up,
		temp[ 2 ] + up, temp[ 2 ] - up, temp[ 3 ] + up, temp[ 3 ] - up };
	
	const decVector boxMin( box[ 0 ].Smallest( box[ 1 ] )
		.Smallest( box[ 2 ] ).Smallest( box[ 3 ] ).Smallest( box[ 4 ] )
		.Smallest( box[ 5 ] ).Smallest( box[ 6 ] ).Smallest( box[ 7 ] ) );
	
	const decVector boxMax( box[ 0 ].Largest( box[ 1 ] )
		.Largest( box[ 2 ] ).Largest( box[ 3 ] ).Largest( box[ 4 ] )
		.Largest( box[ 5 ] ).Largest( box[ 6 ] ).Largest( box[ 7 ] ) );
	
	pModelLOD.GetOctree()->VisitNodesColliding( this, boxMin, boxMax );
	
	// sort triangles front to back
	pTriangleSorter.SortLinear( origin, pPlaneNormal[ epNear ] );
}



void deoglMOVDecalCollect::VisitNode( deoglOctree *node, int intersection ){
	const deoglModelOctree &psonode = *( ( deoglModelOctree* )node );
	const oglModelPosition * const positions = pModelLOD.GetPositions();
	const oglModelVertex * const vertices = pModelLOD.GetVertices();
	const deoglModelFaceList &list = psonode.GetFaceList();
	const int faceCount = list.GetCount();
	int i, j;
	
	for( i=0; i<faceCount; i++ ){
		const deoglModelFace &face = *list.GetAt( i );
		if( pModelLOD.GetTextureAt( face.GetTexture() ).GetDecal() ){
			continue; // decal faces are not used to cut decals
		}
		
		// back facing triangles are ignored. the first plane is the near plane.
		// the normal of this plane is the view normal required for this test
		if( face.GetFaceNormal() * pPlaneNormal[ 0 ] > -0.001f ){
			continue;
		}
		
		// test against all planes. if at least one plane has all triangle points
		// on the back side the triangle is ignored
		const decVector &p1 = positions[ vertices[ face.GetVertex1() ].position ].position;
		const decVector &p2 = positions[ vertices[ face.GetVertex2() ].position ].position;
		const decVector &p3 = positions[ vertices[ face.GetVertex3() ].position ].position;
		
		for( j=0; j<6; j++ ){
			if( p1 * pPlaneNormal[ j ] <= pPlaneDistance[ j ]
			&&  p2 * pPlaneNormal[ j ] <= pPlaneDistance[ j ]
			&&  p3 * pPlaneNormal[ j ] <= pPlaneDistance[ j ] ){
				break;
			}
		}
		if( j < 6 ){
			continue;
		}
		
		// triangle is a potential candidate. add it to the triangle sorter
		pTriangleSorter.AddTriangle( p1, p2, p3 );
	}
}
