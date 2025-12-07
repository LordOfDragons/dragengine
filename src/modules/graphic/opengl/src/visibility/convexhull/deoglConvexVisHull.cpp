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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglConvexVisHull.h"
#include "../../delayedoperation/deoglDelayedOperations.h"
#include "../../vbo/deoglVBOLayout.h"
#include "../../vbo/deoglVBOAttribute.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../utils/collision/deoglDCollisionBox.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>



// Class deoglConvexVisHull
/////////////////////////////

// Constructor, destructor
////////////////////////////

deoglConvexVisHull::deoglConvexVisHull(deoglRenderThread &renderThread) :
pRenderThread(renderThread),
pVAO(0),
pPoints(NULL),
pPointCount(0),
pVBO(0),
pVBOPointCount(0){
}

deoglConvexVisHull::~deoglConvexVisHull(){
	deoglDelayedOperations &dops = pRenderThread.GetDelayedOperations();
	dops.DeleteOpenGLBuffer(pVBO);
	dops.DeleteOpenGLVertexArray(pVAO);
	
	if(pPoints){
		delete [] pPoints;
	}
}



// Management
///////////////

void deoglConvexVisHull::SetExtends(const decVector &minExtend, const decVector &maxExtend){
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
}

void deoglConvexVisHull::CalcBoundingBox(decDVector &boxMinExtend, decDVector &boxMaxExtend, const decDMatrix &matrix) const{
	if(pPointCount == 0){
		boxMinExtend = matrix.GetPosition();
		boxMaxExtend = boxMinExtend;
		
	}else{
		decDVector point;
		int i;
		
		for(i=0; i<pPointCount; i++){
			point = matrix.Transform((double)pPoints[i].x, (double)pPoints[i].y, (double)pPoints[i].z);
			
			if(i == 0){
				boxMinExtend = point;
				boxMaxExtend = point;
				
			}else{
				if(point.x < boxMinExtend.x){
					boxMinExtend.x = point.x;
					
				}else if(point.x > boxMaxExtend.x){
					boxMaxExtend.x = point.x;
				}
				
				if(point.y < boxMinExtend.y){
					boxMinExtend.y = point.y;
					
				}else if(point.y > boxMaxExtend.y){
					boxMaxExtend.y = point.y;
				}
				
				if(point.z < boxMinExtend.z){
					boxMinExtend.z = point.z;
					
				}else if(point.z > boxMaxExtend.z){
					boxMaxExtend.z = point.z;
				}
			}
		}
	}
}

void deoglConvexVisHull::SetPointCount(int count){
	if(count < 0){
		DETHROW(deeInvalidParam);
	}
	
	if(count != pPointCount){
		if(pPoints){
			delete [] pPoints;
			pPoints = NULL;
		}
		
		if(count > 0){
			pPoints = new oglVector3[count];
		}
		
		pPointCount = count;
	}
}



void deoglConvexVisHull::UpdateVBO(){
	if(pPointCount > 0){
		// update vbo
		if(! pVBO){
			OGL_CHECK(pRenderThread, pglGenBuffers(1, &pVBO));
			if(! pVBO){
				DETHROW(deeOutOfMemory);
			}
		}
		
		OGL_CHECK(pRenderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBO));
		OGL_CHECK(pRenderThread, pglBufferData(GL_ARRAY_BUFFER, sizeof(oglVector3) * pPointCount, pPoints, GL_STATIC_DRAW));
		pVBOPointCount = pPointCount;
		
		OGL_CHECK(pRenderThread, pglBindBuffer(GL_ARRAY_BUFFER, 0));
		
		// update vao
		if(! pVAO){
			deoglVBOLayout layout;
			
			layout.SetAttributeCount(1);
			layout.SetStride(12);
			layout.SetIndexType(deoglVBOLayout::eitNone); // use indices later on
			
			layout.GetAttributeAt(0).SetComponentCount(3);
			layout.GetAttributeAt(0).SetDataType(deoglVBOAttribute::edtFloat);
			layout.GetAttributeAt(0).SetOffset(0);
			
			OGL_CHECK(pRenderThread, pglGenVertexArrays(1, &pVAO));
			if(! pVAO){
				DETHROW(deeOutOfMemory);
			}
			
			OGL_CHECK(pRenderThread, pglBindVertexArray(pVAO));
			
			OGL_CHECK(pRenderThread, pglBindBuffer(GL_ARRAY_BUFFER, pVBO));
			layout.SetVAOAttributeAt(pRenderThread, 0, 0); // position(0) => vao(0)
			
			OGL_CHECK(pRenderThread, pglBindBuffer(GL_ARRAY_BUFFER, 0));
			OGL_CHECK(pRenderThread, pglBindVertexArray(0));
		}
		//pRenderThread.GetLogger().LogInfoFormat( "convex visibility hull %p: points=%i size=%i vbo=%u", this, pPointCount, pPointCount*sizeof(oglVector3), pVBO );
	}
}



void deoglConvexVisHull::CreateFrom(const decConvexVolumeList &list){
	const int volumeCount = list.GetVolumeCount();
	int faceCount, vertexCount, i, j, k;
	bool firstVertex = true;
	int vboFaceCount = 0;
	
	// determine the number of faces required and the extends at the same time
	pMinExtend.SetZero();
	pMaxExtend.SetZero();
	
	for(i=0; i<volumeCount; i++){
		const decConvexVolume &volume = *list.GetVolumeAt(i);
		
		// count triangles required to render this face
		faceCount = volume.GetFaceCount();
		
		for(j=0; j<faceCount; j++){
			const decConvexVolumeFace &volumeFace = *volume.GetFaceAt(j);
			
			vertexCount = volumeFace.GetVertexCount();
			
			if(vertexCount > 2){
				vboFaceCount += vertexCount - 2;
			}
		}
		
		// update the extends
		vertexCount = volume.GetVertexCount();
		
		for(j=0; j<vertexCount; j++){
			const decVector &vertex = volume.GetVertexAt(j);
			
			if(firstVertex){
				pMinExtend = vertex;
				pMaxExtend = vertex;
				firstVertex = false;
				
			}else{
				if(vertex.x < pMinExtend.x){
					pMinExtend.x = vertex.x;
					
				}else if(vertex.x > pMaxExtend.x){
					pMaxExtend.x = vertex.x;
				}
				
				if(vertex.y < pMinExtend.y){
					pMinExtend.y = vertex.y;
					
				}else if(vertex.y > pMaxExtend.y){
					pMaxExtend.y = vertex.y;
				}
				
				if(vertex.z < pMinExtend.z){
					pMinExtend.z = vertex.z;
					
				}else if(vertex.z > pMaxExtend.z){
					pMaxExtend.z = vertex.z;
				}
			}
		}
	}
	
	// build the vbo
	SetPointCount(vboFaceCount * 3);
	
	if(vboFaceCount > 0){
		oglVector3 *vboDataPointer = pPoints;
		
		for(i=0; i<volumeCount; i++){
			const decConvexVolume &volume = *list.GetVolumeAt(i);
			
			faceCount = volume.GetFaceCount();
			
			for(j=0; j<faceCount; j++){
				const decConvexVolumeFace &volumeFace = *volume.GetFaceAt(j);
				
				vertexCount = volumeFace.GetVertexCount();
				
				if(vertexCount > 2){
					const decVector &vertex0 = volume.GetVertexAt(volumeFace.GetVertexAt(0));
					
					for(k=2; k<vertexCount; k++){
						const decVector &vertex2 = volume.GetVertexAt(volumeFace.GetVertexAt(k));
						const decVector &vertex1 = volume.GetVertexAt(volumeFace.GetVertexAt(k - 1));
						
						vboDataPointer->x = vertex2.x;
						vboDataPointer->y = vertex2.y;
						vboDataPointer->z = vertex2.z;
						vboDataPointer++;
						
						vboDataPointer->x = vertex1.x;
						vboDataPointer->y = vertex1.y;
						vboDataPointer->z = vertex1.z;
						vboDataPointer++;
						
						vboDataPointer->x = vertex0.x;
						vboDataPointer->y = vertex0.y;
						vboDataPointer->z = vertex0.z;
						vboDataPointer++;
					}
				}
			}
		}
		
		UpdateVBO();
	}
}
