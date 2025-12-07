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

#include <string.h>
#include <stdio.h>

#include "dedaiShapeToConvexVolume.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/shape/decShapeBox.h>
#include <dragengine/common/shape/decShapeSphere.h>
#include <dragengine/common/shape/decShapeCapsule.h>
#include <dragengine/common/shape/decShapeCylinder.h>
#include <dragengine/common/shape/decShapeHull.h>
#include <dragengine/common/shape/decShapeVisitor.h>



// Class dedaiShapeToConvexVolume
/////////////////////////////////

// Constructors, destructors
//////////////////////////////

dedaiShapeToConvexVolume::dedaiShapeToConvexVolume() :
pList(NULL),
pSphereRingCount(9),
pSphereSegmentCount(20){
}

dedaiShapeToConvexVolume::~dedaiShapeToConvexVolume(){
}



// Management
///////////////

void dedaiShapeToConvexVolume::SetList(decConvexVolumeList *list){
	pList = list;
}



void dedaiShapeToConvexVolume::SetSphereRingCount(int ringCount){
	pSphereRingCount = decMath::max(ringCount, 2);
}

void dedaiShapeToConvexVolume::SetSphereSegmentsCount(int segmentCount) {
	pSphereSegmentCount = decMath::max(segmentCount, 8);
}



void dedaiShapeToConvexVolume::AddTriangle(decConvexVolume &volume, int p1, int p2, int p3){
	decConvexVolumeFace *face = NULL;
	
	try{
		face = CreateFace();
		face->AddVertex(p1);
		face->AddVertex(p2);
		face->AddVertex(p3);
		
		const decVector &pos1 = volume.GetVertexAt(p1);
		const decVector &pos2 = volume.GetVertexAt(p2);
		const decVector &pos3 = volume.GetVertexAt(p3);
		face->SetNormal(((pos2 - pos1) % (pos3 - pos2)).Normalized());
		
		volume.AddFace(face);
		
	}catch(const deException &){
		if(face){
			delete face;
		}
		throw;
	}
}

void dedaiShapeToConvexVolume::AddQuad(decConvexVolume &volume, int p1, int p2, int p3, int p4){
	decConvexVolumeFace *face = NULL;
	
	try{
		face = CreateFace();
		face->AddVertex(p1);
		face->AddVertex(p2);
		face->AddVertex(p3);
		face->AddVertex(p4);
		
		const decVector &pos1 = volume.GetVertexAt(p1);
		const decVector &pos2 = volume.GetVertexAt(p2);
		const decVector &pos3 = volume.GetVertexAt(p3);
		face->SetNormal(((pos2 - pos1) % (pos3 - pos2)).Normalized());
		
		volume.AddFace(face);
		
	}catch(const deException &){
		if(face){
			delete face;
		}
		throw;
	}
}



decConvexVolume *dedaiShapeToConvexVolume::CreateVolume(){
	return new decConvexVolume;
}

decConvexVolumeFace *dedaiShapeToConvexVolume::CreateFace(){
	return new decConvexVolumeFace;
}



// Visiting
/////////////

void dedaiShapeToConvexVolume::VisitShape(decShape &shape){
}

void dedaiShapeToConvexVolume::VisitShapeSphere(decShapeSphere &sphere){
	if(! pList){
		DETHROW(deeInvalidParam);
	}
	
	const float &radius = sphere.GetRadius();
	const decVector2 &axisScaling = sphere.GetAxisScaling();
	const float stepAngleSegment = PI * 2.0f / (float)(pSphereSegmentCount);
	const float stepAngleRing = PI / (float)(pSphereRingCount + 1);
	const decMatrix matrix = decMatrix::CreateScale(radius * axisScaling.x, radius, radius * axisScaling.y)
		* decMatrix::CreateWorld( sphere.GetPosition(), sphere.GetOrientation() );
	float angle, radiusRing, heightRing;
	int i, j, base;
	
	decConvexVolume *volume = NULL;
	
	try{
		volume = CreateVolume();
		
		// add vertices
		volume->AddVertex(matrix.Transform(0.0f, 1.0f, 0.0f)); // top pole
		
		for(i=0; i<pSphereRingCount; i++){
			angle = stepAngleRing * (float)(i + 1); // first ring is actually at i=1
			radiusRing = sinf(angle);
			heightRing = cosf(angle);
			
			for(j=0; j<pSphereSegmentCount; j++){
				angle = stepAngleSegment * (float)j;
				volume->AddVertex(matrix.Transform(sinf(angle) * radiusRing, heightRing, cosf(angle) * radiusRing));
			}
		}
		
		volume->AddVertex(matrix.Transform(0.0f, -1.0f, 0.0f)); // bottom pole
		
		// add faces
		for(i=0; i<pSphereSegmentCount; i++){
			AddTriangle(*volume, 0, 1 + i, 1 + (i + 1) % pSphereSegmentCount);
		}
		
		for(i=0; i<pSphereRingCount-1; i++){
			base = 1 + pSphereSegmentCount * i;
			for(j=0; j<pSphereSegmentCount; j++){
				AddQuad(*volume, base + j, base + pSphereSegmentCount + j,
					base + pSphereSegmentCount + (j + 1) % pSphereSegmentCount,
					base + (j + 1) % pSphereSegmentCount);
			}
		}
		
		base = 1 + pSphereSegmentCount * (pSphereRingCount - 1);
		const int last = volume->GetVertexCount() - 1;
		for(i=0; i<pSphereSegmentCount; i++){
			AddTriangle(*volume, last, base + (i + 1) % pSphereSegmentCount, base + i);
		}
		
		pList->AddVolume(volume);
		
	}catch(const deException &){
		if(volume){
			delete volume;
		}
		throw;
	}
}

void dedaiShapeToConvexVolume::VisitShapeBox(decShapeBox &box){
	if(! pList){
		DETHROW(deeInvalidParam);
	}
	
	const decMatrix matrix(decMatrix::CreateWorld(box.GetPosition(), box.GetOrientation(), box.GetHalfExtends()));
	const decVector2 &tapering = box.GetTapering();
	
	decConvexVolume *volume = NULL;
	
	try{
		volume = CreateVolume();
		
		volume->AddVertex(matrix.Transform(-tapering.x, 1.0f,  tapering.y));
		volume->AddVertex(matrix.Transform(tapering.x, 1.0f,  tapering.y));
		volume->AddVertex(matrix.Transform(tapering.x, 1.0f, -tapering.y));
		volume->AddVertex(matrix.Transform(-tapering.x, 1.0f, -tapering.y));
		volume->AddVertex(matrix.Transform(-1.0f, -1.0f,  1.0f));
		volume->AddVertex(matrix.Transform(1.0f, -1.0f,  1.0f));
		volume->AddVertex(matrix.Transform(1.0f, -1.0f, -1.0f));
		volume->AddVertex(matrix.Transform(-1.0f, -1.0f, -1.0f));
		
		AddQuad(*volume, 0, 1, 2, 3); // top (y+)
		AddQuad(*volume, 7, 6, 5, 4); // bottom (y-)
		AddQuad(*volume, 6, 7, 3, 2); // front (z+)
		AddQuad(*volume, 4, 5, 1, 0); // back (z-)
		AddQuad(*volume, 5, 6, 2, 1); // left (x+)
		AddQuad(*volume, 7, 4, 0, 3); // right (x-)
		
		pList->AddVolume(volume);
		
	}catch(const deException &){
		if(volume){
			delete volume;
		}
		throw;
	}
}

void dedaiShapeToConvexVolume::VisitShapeCylinder(decShapeCylinder &cylinder){
}

void dedaiShapeToConvexVolume::VisitShapeCapsule(decShapeCapsule &capsule){
}

void dedaiShapeToConvexVolume::VisitShapeHull(decShapeHull &hull){
}
