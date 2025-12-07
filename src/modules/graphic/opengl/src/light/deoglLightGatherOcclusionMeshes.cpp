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

deoglLightGatherOcclusionMeshes::deoglLightGatherOcclusionMeshes(deoglRLight &light) :
pLight(light){
}

deoglLightGatherOcclusionMeshes::~deoglLightGatherOcclusionMeshes(){
}



// Visiting
/////////////

void deoglLightGatherOcclusionMeshes::Reset(){
	pTriangleSorter.RemoveAllTriangles();
}



void deoglLightGatherOcclusionMeshes::VisitNode(deoglDOctree *node, int intersection){
	const deoglWorldOctree &soNode = *((deoglWorldOctree*)node);
	const int componentCount = soNode.GetComponentCount();
	const decDVector &lightMinExtend = pLight.GetFullMinExtend();
	const decDVector &lightMaxExtend = pLight.GetFullMaxExtend();
	int i;
	
	for(i=0; i<componentCount; i++){
		deoglRComponent &component = *soNode.GetComponentAt(i);
		
		if(component.GetRenderStatic()){
			const deoglROcclusionMesh * const occlusionMesh = component.GetOcclusionMesh();
			
			if(occlusionMesh){
				const decDVector &componentMinExtend = component.GetMinimumExtend();
				const decDVector &componentMaxExtend = component.GetMaximumExtend();
				
				if(deoglDCollisionDetection::AABoxHitsAABox(lightMinExtend, lightMaxExtend, componentMinExtend, componentMaxExtend)){
					AddOcclusionMesh(component);
				}
			}
		}
	}
}

void deoglLightGatherOcclusionMeshes::AddOcclusionMesh(deoglRComponent &component){
	const decMatrix matrix = (component.GetMatrix() * pLight.GetInverseMatrix()).ToMatrix();
	const deoglROcclusionMesh &occlusionMesh = *component.GetOcclusionMesh();
	const int singleSidedFaceCount = occlusionMesh.GetSingleSidedFaceCount();
	const int doubleSidedFaceCount = occlusionMesh.GetDoubleSidedFaceCount();
	const deoglROcclusionMesh::sVertex * const vertices = occlusionMesh.GetVertices();
	const int faceCount = singleSidedFaceCount + doubleSidedFaceCount;
	unsigned short * const corners = occlusionMesh.GetCorners();
	const decVector origin; // decVector()
	decVector tv1, tv2, tv3;
	int i, pointIndex = 0;
	
	for(i=0; i<faceCount; i++){
		const deoglROcclusionMesh::sVertex &v3 = vertices[corners[pointIndex++]];
		const deoglROcclusionMesh::sVertex &v2 = vertices[corners[pointIndex++]];
		const deoglROcclusionMesh::sVertex &v1 = vertices[corners[pointIndex++]];
		
		tv1 = matrix * v1.position;
		tv2 = matrix * v2.position;
		tv3 = matrix * v3.position;
		
		if((origin - tv1) * ((tv2 - tv1) % (tv3 - tv2)) > 0.0f){ // front facing
			pTriangleSorter.AddTriangle(tv1, tv2, tv3);
			
		}else{ // back facing
			if(i < singleSidedFaceCount){
				continue;
			}
			pTriangleSorter.AddTriangle(tv3, tv2, tv1);
		}
	}
}
