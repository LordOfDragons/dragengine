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

#include "debpDecal.h"
#include "debpDecalFace.h"
#include "debpDecalMeshBuilder.h"
#include "debpDecalMeshBuilderFace.h"
#include "../coldet/collision/debpDCollisionDetection.h"
#include "../coldet/collision/debpDCollisionTriangle.h"
#include "../coldet/collision/debpDCollisionVolume.h"
#include "../component/debpModel.h"
#include "../component/debpComponent.h"

#include <dragengine/resources/decal/deDecal.h>
#include <dragengine/resources/model/deModel.h>
#include <dragengine/resources/component/deComponent.h>
#include <dragengine/common/exceptions.h>



// Class debpDecal
////////////////////

// Constructor, destructor
////////////////////////////

debpDecal::debpDecal(deDecal *decal){
	if(! decal) DETHROW(deeInvalidParam);
	
	pDecal = decal;
	
	pFaces = NULL;
	pFaceCount = 0;
	pDirtyGeometry = false;
	
	pParentComponent = NULL;
}

debpDecal::~debpDecal(){
	pCleanUp();
}



// Management
///////////////

void debpDecal::MarkDirty(){
	if(pFaces){
		delete [] pFaces;
		pFaces = NULL;
		pFaceCount = 0;
	}
	
	pDirtyGeometry = true;
}

void debpDecal::Update(){
	if(pDirtyGeometry){
		if(pParentComponent){
			pCreateMeshComponent();
		}
		
		pDirtyGeometry = false;
	}
}



void debpDecal::SetParentComponent(debpComponent *component){
	if(component) DETHROW(deeInvalidParam);
	
	pParentComponent = component;
	MarkDirty();
}



bool debpDecal::TouchesVolume(debpDCollisionVolume *volume) const{
	if(! volume) DETHROW(deeOutOfMemory);
	debpDCollisionTriangle triangle;
	int i;
	
	// TODO: early exit check with a bounding box or sphere
	
	// test each face for a hit
	for(i=0; i<pFaceCount; i++){
		triangle.SetCorners(pFaces[i].GetPoint1(), pFaces[i].GetPoint2(), pFaces[i].GetPoint3());
		if(volume->TriangleHitsVolume(&triangle)) return true;
	}
	
	// no collision
	return false;
}

bool debpDecal::TouchesPoint(const decVector &point) const{
	int i;
	
	for(i=0; i<pFaceCount; i++){
		if(debpDCollisionDetection::PointInTriangle(pFaces[i].GetPoint1(),
		pFaces[i].GetPoint2(), pFaces[i].GetPoint3(), point)){
			return true;
		}
	}
	
	return false;
}



// Notifications
//////////////////

void debpDecal::GeometryChanged(){
	MarkDirty();
}

void debpDecal::SkinChanged(){
}

void debpDecal::VisibleChanged(){
}



// Private Functions
//////////////////////

void debpDecal::pCleanUp(){
	if(pFaces) delete [] pFaces;
}

void debpDecal::pCreateMeshComponent(){
	/*
	if(! pParentComponent->GetComponent()){
		return;
	}
	oglVector *compVerts = pParentComponent->GetVertices();
	deComponent *component = pParentComponent->GetComponent();
	debpModel *oglModel = (debpModel*)component->GetModel()->GetPeerGraphic();
	debpModel::sFace *mdlFaces = oglModel->GetFaces();
	decVector2 halfSize = pDecal->GetSize() * 0.5f;
	int f, faceCount = oglModel->GetFaceCount();
	decVector hitPoint, testNormal, edges[3];
	int vertMap[6] = {2, 1, 0, 0, 3, 2};
	decVector decalNormal, decalTangent;
	decVector vertices[4], projAxis;
	decMatrix decalMatrix;
	float testDot, lambda;
	decVector v1, v2, v3;
	decVector2 tcs[4];
	bool isInsideTri;
	float d1v, d2v;
	int v, index;
	
	// get texture coordinates
	tcs[0] = pDecal->GetTextureCoordinatesAt(0);
	tcs[1] = pDecal->GetTextureCoordinatesAt(1);
	tcs[2] = pDecal->GetTextureCoordinatesAt(2);
	tcs[3] = pDecal->GetTextureCoordinatesAt(3);
	
	// get decal matrix and projection axis
	decalMatrix = decMatrix::CreateFromQuaternion(pDecal->GetOrientation())
		* decMatrix::CreateTranslation( pDecal->GetPosition() );
	
	projAxis = decalMatrix.TransformView();
	
	// create points array
	pPoints = new debpVBOPoint[6];
	if(! pPoints) DETHROW(deeOutOfMemory);
	pPointCount = 6;
	
	// calculate vertices
	vertices[0] = decalMatrix * decVector(halfSize.x,  halfSize.y, 0.0f);
	vertices[1] = decalMatrix * decVector(-halfSize.x,  halfSize.y, 0.0f);
	vertices[2] = decalMatrix * decVector(-halfSize.x, -halfSize.y, 0.0f);
	vertices[3] = decalMatrix * decVector(halfSize.x, -halfSize.y, 0.0f);
	
//printf( "decal %p: before projection (%g,%g,%g) (%g,%g,%g) (%g,%g,%g) (%g,%g,%g)\n", pDecal, vertices[0].x, vertices[0].y, vertices[0].z, vertices[1].x, vertices[1].y, vertices[1].z, vertices[2].x, vertices[2].y, vertices[2].z, vertices[3].x, vertices[3].y, vertices[3].z );
	// project vertices down to the parent mesh
	for(v=0; v<4; v++){
		for(f=0; f<faceCount; f++){
			index = mdlFaces[f].vertex1;
			v1.x = compVerts[index].x;
			v1.y = compVerts[index].y;
			v1.z = compVerts[index].z;
			index = mdlFaces[f].vertex2;
			v2.x = compVerts[index].x;
			v2.y = compVerts[index].y;
			v2.z = compVerts[index].z;
			index = mdlFaces[f].vertex3;
			v3.x = compVerts[index].x;
			v3.y = compVerts[index].y;
			v3.z = compVerts[index].z;
			
			edges[0] = v2 - v1;
			edges[1] = v3 - v2;
			edges[2] = v1 - v3;
			
			testNormal = edges[0] % edges[1];
			testNormal.Normalize();
			
			testDot = testNormal * projAxis;
			if(fabs(testDot) > 1e-5){
				lambda = testNormal * (vertices[v] - v2) / testDot;
				hitPoint = vertices[v] - projAxis * lambda;
				
				isInsideTri = ((testNormal % edges[0]) * (hitPoint - v1) >= 0.0f)
					&& ((testNormal % edges[1]) * (hitPoint - v2) >= 0.0f)
					&& ((testNormal % edges[2]) * (hitPoint - v3) >= 0.0f);
				
				if(isInsideTri){
					vertices[v] = hitPoint;
					
				}else{
					// BIG TIME TODO
				}
			}
		}
	}
//printf( "decal %p: after projection (%g,%g,%g) (%g,%g,%g) (%g,%g,%g) (%g,%g,%g)\n", pDecal, vertices[0].x, vertices[0].y, vertices[0].z, vertices[1].x, vertices[1].y, vertices[1].z, vertices[2].x, vertices[2].y, vertices[2].z, vertices[3].x, vertices[3].y, vertices[3].z );
		
	// calculate decal normal and tangent
	edges[0] = vertices[1] - vertices[0];
	edges[1] = vertices[2] - vertices[1];
	decalNormal = edges[0] % edges[1];
	decalNormal.Normalize();
	
	d1v = tcs[1].y - tcs[0].y;
	d2v = tcs[2].y - tcs[0].y;
	
	decalTangent.x = edges[0].x * d2v - edges[1].x * d1v;
	decalTangent.y = edges[0].y * d2v - edges[1].y * d1v;
	decalTangent.z = edges[0].z * d2v - edges[1].z * d1v;
	decalTangent.Normalize();
	
	// copy to points array
	for(v=0; v<6; v++){
		index = vertMap[v];
		pPoints[v].x = vertices[index].x;
		pPoints[v].y = vertices[index].y;
		pPoints[v].z = vertices[index].z;
		pPoints[v].u = tcs[index].x;
		pPoints[v].v = 1.0f - tcs[index].y;
		pPoints[v].nx = decalNormal.x;
		pPoints[v].ny = decalNormal.y;
		pPoints[v].nz = decalNormal.z;
		pPoints[v].tx = decalTangent.x;
		pPoints[v].ty = decalTangent.y;
		pPoints[v].tz = decalTangent.z;
	}*/
}
