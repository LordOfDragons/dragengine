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

#include "deoglVisCollectOccMeshes.h"

#include "../../world/deoglWorldOctree.h"
#include "../../component/deoglRComponent.h"
#include "../../triangles/deoglTriangleSorter.h"
#include "../../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../../utils/collision/deoglDCollisionDetection.h"

#include <dragengine/common/exceptions.h>



// Class deoglVisCollectOccMeshes
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglVisCollectOccMeshes::deoglVisCollectOccMeshes(){
	pTriangleSorter = new deoglTriangleSorter;
}

deoglVisCollectOccMeshes::~deoglVisCollectOccMeshes(){
	if( pTriangleSorter ){
		delete pTriangleSorter;
	}
}



// Visiting
/////////////

void deoglVisCollectOccMeshes::SetVisitExtends( const decDVector &minExtend, const decDVector &maxExtend ){
	pVisitMinExtend = minExtend;
	pVisitMaxExtend = maxExtend;
}

void deoglVisCollectOccMeshes::SetMatrixInvHull( const decDMatrix &matrixInvHull ){
	pMatrixInvHull = matrixInvHull;
}

void deoglVisCollectOccMeshes::Reset(){
	pTriangleSorter->RemoveAllTriangles();
}



void deoglVisCollectOccMeshes::VisitNode( deoglDOctree *node, int intersection ){
	const deoglWorldOctree &soNode = *( ( deoglWorldOctree* )node );
	const int componentCount = soNode.GetComponentCount();
	int i;
	
	for( i=0; i<componentCount; i++ ){
		deoglRComponent &component = *soNode.GetComponentAt( i );
		const deoglROcclusionMesh * const occlusionMesh = component.GetOcclusionMesh();
		
		if( occlusionMesh ){
			const decDVector &minExtend = component.GetMinimumExtend();
			const decDVector &maxExtend = component.GetMaximumExtend();
			
			if( deoglDCollisionDetection::AABoxHitsAABox( pVisitMinExtend, pVisitMaxExtend, minExtend, maxExtend ) ){
				AddOcclusionMesh( component );
			}
		}
	}
}

void deoglVisCollectOccMeshes::AddOcclusionMesh( deoglRComponent &component ){
	const decMatrix matrix = ( component.GetMatrix() * pMatrixInvHull ).ToMatrix();
	const deoglROcclusionMesh &occlusionMesh = *component.GetOcclusionMesh();
	const int singleSidedFaceCount = occlusionMesh.GetSingleSidedFaceCount();
	const int doubleSidedFaceCount = occlusionMesh.GetDoubleSidedFaceCount();
	const deoglROcclusionMesh::sVertex * const vertices = occlusionMesh.GetVertices();
	const int faceCount = singleSidedFaceCount + doubleSidedFaceCount;
	unsigned short * const corners = occlusionMesh.GetCorners();
	decVector tv1, tv2, tv3;
	const decVector origin;
	int i, pointIndex = 0;
	
	for( i=0; i<faceCount; i++ ){
		const deoglROcclusionMesh::sVertex &v3 = vertices[ corners[ pointIndex++ ] ];
		const deoglROcclusionMesh::sVertex &v2 = vertices[ corners[ pointIndex++ ] ];
		const deoglROcclusionMesh::sVertex &v1 = vertices[ corners[ pointIndex++ ] ];
		
		tv1 = matrix * v1.position;
		tv2 = matrix * v2.position;
		tv3 = matrix * v3.position;
		
		if( ( origin - tv1 ) * ( ( tv2 - tv1 ) % ( tv3 - tv2 ) ) > 0.0f ){ // front facing
			pTriangleSorter->AddTriangle( tv1, tv2, tv3 );
			
		}else{ // back facing
			if( i < singleSidedFaceCount ){
				continue;
			}
			pTriangleSorter->AddTriangle( tv3, tv2, tv1 );
		}
	}
}
