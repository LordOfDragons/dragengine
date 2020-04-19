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

#include "deoglRLight.h"
#include "deoglLightGatherOcclusionMeshes.h"

#include "../component/deoglRComponent.h"
#include "../occlusiontest/mesh/deoglROcclusionMesh.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../world/deoglWorldOctree.h"

#include <dragengine/common/exceptions.h>
#include "../utils/collision/deoglDCollisionDetection.h"



// Class deoglLightGatherOcclusionMeshes
//////////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglLightGatherOcclusionMeshes::deoglLightGatherOcclusionMeshes( deoglRLight &light ) :
pLight( light ){
}

deoglLightGatherOcclusionMeshes::~deoglLightGatherOcclusionMeshes(){
}



// Visiting
/////////////

void deoglLightGatherOcclusionMeshes::Reset(){
	pTriangleSorter.RemoveAllTriangles();
}



void deoglLightGatherOcclusionMeshes::VisitNode( deoglDOctree *node, int intersection ){
	const deoglWorldOctree &soNode = *( ( deoglWorldOctree* )node );
	const int componentCount = soNode.GetComponentCount();
	const decDVector &lightMinExtend = pLight.GetFullMinExtend();
	const decDVector &lightMaxExtend = pLight.GetFullMaxExtend();
	int i;
	
	for( i=0; i<componentCount; i++ ){
		deoglRComponent &component = *soNode.GetComponentAt( i );
		
		if( component.GetRenderStatic() ){
			const deoglROcclusionMesh * const occlusionMesh = component.GetOcclusionMesh();
			
			if( occlusionMesh ){
				const decDVector &componentMinExtend = component.GetMinimumExtend();
				const decDVector &componentMaxExtend = component.GetMaximumExtend();
				
				if( deoglDCollisionDetection::AABoxHitsAABox( lightMinExtend, lightMaxExtend, componentMinExtend, componentMaxExtend ) ){
					AddOcclusionMesh( component );
				}
			}
		}
	}
}

void deoglLightGatherOcclusionMeshes::AddOcclusionMesh( deoglRComponent &component ){
	const decMatrix matrix = ( component.GetMatrix() * pLight.GetInverseMatrix() ).ToMatrix();
	const deoglROcclusionMesh &occlusionMesh = *component.GetOcclusionMesh();
	const int singleSidedFaceCount = occlusionMesh.GetSingleSidedFaceCount();
	const int doubleSidedFaceCount = occlusionMesh.GetDoubleSidedFaceCount();
	const deoglROcclusionMesh::sVertex * const vertices = occlusionMesh.GetVertices();
	const int faceCount = singleSidedFaceCount + doubleSidedFaceCount;
	unsigned short * const corners = occlusionMesh.GetCorners();
	const decVector origin; // decVector()
	decVector tv1, tv2, tv3;
	int i, pointIndex = 0;
	
	for( i=0; i<faceCount; i++ ){
		const deoglROcclusionMesh::sVertex &v3 = vertices[ corners[ pointIndex++ ] ];
		const deoglROcclusionMesh::sVertex &v2 = vertices[ corners[ pointIndex++ ] ];
		const deoglROcclusionMesh::sVertex &v1 = vertices[ corners[ pointIndex++ ] ];
		
		tv1 = matrix * v1.position;
		tv2 = matrix * v2.position;
		tv3 = matrix * v3.position;
		
		if( ( origin - tv1 ) * ( ( tv2 - tv1 ) % ( tv3 - tv2 ) ) > 0.0f ){ // front facing
			pTriangleSorter.AddTriangle( tv1, tv2, tv3 );
			
		}else{ // back facing
			if( i < singleSidedFaceCount ){
				continue;
			}
			pTriangleSorter.AddTriangle( tv3, tv2, tv1 );
		}
	}
}
