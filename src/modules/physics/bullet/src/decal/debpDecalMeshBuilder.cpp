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

#include "debpDecalMeshBuilder.h"
#include "debpDecalMeshBuilderFace.h"
#include "debpDMBConvexVolumeFace.h"
#include "debpDMBConvexVolumeList.h"
#include "debpConvexFaceClipper.h"
//#include "../component/debpModel.h"
//#include "../component/debpComponent.h"
#include "../coldet/collision/debpDCollisionBox.h"
#include "../coldet/collision/debpDCollisionSphere.h"
#include "../coldet/collision/debpDCollisionTriangle.h"
#include "../coldet/collision/debpDCollisionDetection.h"

#include <dragengine/resources/decal/deDecal.h>
//#include <dragengine/resources/model/deModel.h>
//#include <dragengine/resources/model/deModelVertex.h>
//#include <dragengine/resources/component/deComponent.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/exceptions.h>



// Class debpDecalMeshBuilder
///////////////////////////////

// Constructor, destructor
////////////////////////////
#include "../dePhysicsBullet.h"

debpDecalMeshBuilder::debpDecalMeshBuilder(){
	pCVolList = NULL;
	pDistance = 0.0f;
	pDecalBox = NULL;
	
	pPoints = NULL;
	pPointCount = 0;
	pPointSize = 0;
	pFaces = NULL;
	pFaceCount = 0;
	pFaceSize = 0;
}

debpDecalMeshBuilder::~debpDecalMeshBuilder(){
	if(pFaces) delete [] pFaces;
	if(pPoints) delete [] pPoints;
	if(pDecalBox) delete pDecalBox;
	if(pCVolList) delete pCVolList;
}



// Management
///////////////

void debpDecalMeshBuilder::Init(deDecal *decal, float distance){
	if(! decal || distance < 1e-3f) DETHROW(deeInvalidParam);
	decVector halfSize(decal->GetSize() * 0.5f);
	decConvexVolume *volume = NULL;
	
	// free the old stuff
	if(pCVolList){
		delete pCVolList;
		pCVolList = NULL;
	}
	
	// tuck away the infos
	pDecalMatrix = decMatrix::CreateFromQuaternion(decal->GetOrientation())
		* decMatrix::CreateTranslation( decal->GetPosition() );
	pDecalView = pDecalMatrix.TransformView();
	pOrigin = decal->GetPosition();
	pDistance = distance;
	
	pProjVector = -pDecalView * pDistance; // small offset just to make sure
	
	// create decal box
	if(pDecalBox){
		delete pDecalBox;
		pDecalBox = NULL;
	}
	pDecalBox = new debpDCollisionBox(pOrigin - pDecalView * (pDistance * 0.5f),
		decVector(halfSize.x, halfSize.y, pDistance * 0.5f), decal->GetOrientation());
	if(! pDecalBox) DETHROW(deeOutOfMemory);
	
	// create convex volume list
	try{
		pCVolList = new debpDMBConvexVolumeList;
		if(! pCVolList) DETHROW(deeOutOfMemory);
		
		volume = new decConvexVolume;
		if(! volume) DETHROW(deeOutOfMemory);
		
		volume->AddVertex(pDecalMatrix * decVector(halfSize.x,  halfSize.y, 0.0f));
		volume->AddVertex(pDecalMatrix * decVector(-halfSize.x,  halfSize.y, 0.0f));
		volume->AddVertex(pDecalMatrix * decVector(-halfSize.x, -halfSize.y, 0.0f));
		volume->AddVertex(pDecalMatrix * decVector(halfSize.x, -halfSize.y, 0.0f));
		volume->AddVertex(pDecalMatrix * decVector(halfSize.x,  halfSize.y, -pDistance));
		volume->AddVertex(pDecalMatrix * decVector(-halfSize.x,  halfSize.y, -pDistance));
		volume->AddVertex(pDecalMatrix * decVector(-halfSize.x, -halfSize.y, -pDistance));
		volume->AddVertex(pDecalMatrix * decVector(halfSize.x, -halfSize.y, -pDistance));
		
		pVolumeAddFace(volume, 0, 1, 2, 3, pDecalView, false); // front
		pVolumeAddFace(volume, 4, 5, 1, 0, pDecalMatrix.TransformUp(), false); // top
		pVolumeAddFace(volume, 1, 5, 6, 2, -pDecalMatrix.TransformRight(), false); // right
		pVolumeAddFace(volume, 3, 2, 6, 7, -pDecalMatrix.TransformUp(), false); // bottom
		pVolumeAddFace(volume, 4, 0, 3, 7, pDecalMatrix.TransformRight(), false); // left
		pVolumeAddFace(volume, 5, 4, 7, 6, -pDecalView, false); // back
		
		pCVolList->AddVolume(volume);
		
	}catch(const deException &){
		if(volume) delete volume;
		throw;
	}
}

void debpDecalMeshBuilder::CutTriangle(const decVector &v1, const decVector &v2, const decVector &v3){
	decVector normal = (v2 - v1) % (v3 - v2);
	decConvexVolume splitVolume;
	
	if(normal * pDecalView < 1e-3) return;
	
	splitVolume.AddVertex(v1);
	splitVolume.AddVertex(v2);
	splitVolume.AddVertex(v3);
	splitVolume.AddVertex(v1 + pProjVector);
	splitVolume.AddVertex(v2 + pProjVector);
	splitVolume.AddVertex(v3 + pProjVector);
	
	pVolumeAddFace(&splitVolume, 0, 1, 2, normal, true); // front
	pVolumeAddFace(&splitVolume, 5, 4, 3, -normal, false); // back
	pVolumeAddFace(&splitVolume, 0, 3, 4, 1, (v2 - v1) % pDecalView, false); // side
	pVolumeAddFace(&splitVolume, 1, 4, 5, 2, (v3 - v2) % pDecalView, false); // side
	pVolumeAddFace(&splitVolume, 2, 5, 3, 0, (v1 - v3) % pDecalView, false); // side
	
	pCVolList->SplitByVolume(splitVolume);
}

void debpDecalMeshBuilder::BuildMeshForComponent(debpComponent *oglComponent){
/*
	if(! oglComponent){
		DETHROW(deeInvalidParam);
	}
	
	deComponent *component = oglComponent->GetComponent();
	deModel *model = component->GetModel();
	if(! model){
		return;
	}
	
	debpModel *oglModel = (debpModel*)model->GetPeerGraphic();
	const oglModelVertex *modelVertices = oglModel->GetVertices();
	const oglModelFace *modelFaces = oglModel->GetFaces();
	debpDMBConvexVolumeFace *volumeFace;
	int firstPoint, lastPoint, nextPoint;
	debpDecalMeshBuilderFace *dmbFace;
	debpConvexFaceClipper faceClipper;
	decConvexVolume splitVolume;
	debpDCollisionTriangle colTri;
	decVector volumeFaceNormal;
	decConvexVolume *volume;
	decVector v1, v2, v3;
	float volumeFaceDot;
	int v, volumeCount;
	int vf, volumeFaceCount;
	int volumeVertexCount;
	int f, faceCount;
	int x, vertexCount;
	int ci;
	
	// cut volume
	faceCount = oglModel->GetFaceCount();
	for(f=0; f<faceCount; f++){
		const oglModelFace &face = modelFaces[f];
		
		const oglModelVertex &bp1 = modelVertices[face.vertices[0]];
		v1.x = bp1.x;
		v1.y = bp1.y;
		v1.z = bp1.z;
		const oglModelVertex &bp2 = modelVertices[face.vertices[1]];
		v2.x = bp2.x;
		v2.y = bp2.y;
		v2.z = bp2.z;
		const oglModelVertex &bp3 = modelVertices[face.vertices[2]];
		v3.x = bp3.x;
		v3.y = bp3.y;
		v3.z = bp3.z;
		
		// face has to face into the direction of the decal
		if(face.faceNormal * pDecalView < 1e-5f) continue;
		
		// clip the face to avoid artefacts
		faceClipper.RemoveAllVertices();
		faceClipper.AddVertex(v1);
		faceClipper.AddVertex(v2);
		faceClipper.AddVertex(v3);
		faceClipper.ClipByPlane(-pDecalView, pOrigin);
		
		vertexCount = faceClipper.GetVertexCount();
		if(vertexCount < 3) continue;
		
		// cut by the face
		splitVolume.SetEmpty();
		for(x=0; x<vertexCount; x++){
			splitVolume.AddVertex(faceClipper.GetVertexAt(x));
		}
		for(x=0; x<vertexCount; x++){
			splitVolume.AddVertex(faceClipper.GetVertexAt(x) + pProjVector);
		}
		
		// front
		volumeFace = NULL;
		try{
			volumeFace = new debpDMBConvexVolumeFace;
			if(! volumeFace) DETHROW(deeOutOfMemory);
			
			volumeFaceNormal = (v2 - v1) % (v3 - v2);
			volumeFaceNormal.Normalize();
			volumeFace->SetNormal(volumeFaceNormal);
			
			for(x=0; x<vertexCount; x++){
				volumeFace->AddVertex(x);
			}
			volumeFace->SetDecalFace(true);
			
			splitVolume.AddFace(volumeFace);
			
		}catch(const deException &){
			if(volumeFace) delete volumeFace;
			throw;
		}
		
		// back
		volumeFace = NULL;
		try{
			volumeFace = new debpDMBConvexVolumeFace;
			if(! volumeFace) DETHROW(deeOutOfMemory);
			
			volumeFace->SetNormal(-volumeFaceNormal);
			
			for(x=vertexCount-1; x>=0; x--){
				volumeFace->AddVertex(vertexCount + x);
			}
			
			splitVolume.AddFace(volumeFace);
			
		}catch(const deException &){
			if(volumeFace) delete volumeFace;
			throw;
		}
		
		// sides
		for(x=0; x<vertexCount; x++){
			ci = (x + 1) % vertexCount;
			pVolumeAddFace(&splitVolume, x, ci, vertexCount + ci, vertexCount + x,
				(splitVolume.GetVertexAt(ci) - splitVolume.GetVertexAt(x)) % pDecalView, false);
		}
		
		pCVolList->SplitByVolume(splitVolume);
	}
	
	// prepare
	RemoveAllFaces();
	RemoveAllPoints();
	
	// add faces matching the volume
	volumeCount = pCVolList->GetVolumeCount();
	for(v=0; v<volumeCount; v++){
		volume = pCVolList->GetVolumeAt(v);
		volumeFaceCount = volume->GetFaceCount();
		for(vf=0; vf<volumeFaceCount; vf++){
			volumeFace = (debpDMBConvexVolumeFace*)volume->GetFaceAt(vf);
			
			if(! volumeFace->GetDecalFace()) continue;
			if(volumeFace->GetNormal() * pDecalView > 1e-3) continue;
			if(volumeFace->GetVertexCount() < 3) continue;
			
			volumeFaceNormal = -volumeFace->GetNormal(); // since it faces backwards
			volumeFaceDot = volumeFaceNormal * volume->GetVertexAt(volumeFace->GetVertexAt(0));
			
			volumeVertexCount = volumeFace->GetVertexCount();
			
			// now test all faces of the component
			for(f=0; f<faceCount; f++){
				const oglModelFace &oglCFace = modelFaces[f];
				
				// the face has to be coplanar with the volume face
				const oglModelVertex &bp1 = modelVertices[oglCFace.vertices[0]];
				v1.x = bp1.x;
				v1.y = bp1.y;
				v1.z = bp1.z;
				
				if(fabs(volumeFaceNormal * oglCFace.faceNormal) < 0.99999) continue;
				if(fabs(volumeFaceNormal * v1 - volumeFaceDot) > 1e-5) continue;
				
				const oglModelVertex &bp2 = modelVertices[oglCFace.vertices[1]];
				v2.x = bp2.x;
				v2.y = bp2.y;
				v2.z = bp2.z;
				const oglModelVertex &bp3 = modelVertices[oglCFace.vertices[2]];
				v3.x = bp3.x;
				v3.y = bp3.y;
				v3.z = bp3.z;
				
				// clip the face
				faceClipper.RemoveAllVertices();
				for(x=volumeVertexCount-1; x>=0; x--){
					faceClipper.AddVertex(volume->GetVertexAt(volumeFace->GetVertexAt(x)));
				}
				
				faceClipper.ClipByPlane(oglCFace.faceNormal % (v2 - v1), v1);
				faceClipper.ClipByPlane(oglCFace.faceNormal % (v3 - v2), v2);
				faceClipper.ClipByPlane(oglCFace.faceNormal % (v1 - v3), v3);
				
				// if something is left it is a part of the decal mesh
				vertexCount = faceClipper.GetVertexCount();
				if(vertexCount > 2){
					firstPoint = AddPoint(faceClipper.GetVertexAt(0));
					lastPoint = AddPoint(faceClipper.GetVertexAt(1));
					
					for(x=2; x<vertexCount; x++){
						nextPoint = AddPoint(faceClipper.GetVertexAt(x));
						
						dmbFace = AddFace();
						dmbFace->SetPoint1(firstPoint);
						dmbFace->SetPoint2(lastPoint);
						dmbFace->SetPoint3(nextPoint);
						dmbFace->SetFaceIndex(f);
						
						lastPoint = nextPoint;
					}
				}
			}
		}
	}
*/
}



void debpDecalMeshBuilder::Debug(dePhysicsBullet &module){
	int v, volumeCount = pCVolList->GetVolumeCount();
	debpDMBConvexVolumeFace *volumeFace;
	decConvexVolume *volume;
	int x, vertexCount;
	int f, faceCount;
	
	module.LogInfo("DecalMeshBuilder: Convex volume infos");
	for(v=0; v<volumeCount; v++){
		volume = pCVolList->GetVolumeAt(v);
		module.LogInfoFormat("  volume %i: vertices(%i)", v + 1, volume->GetVertexCount());
		faceCount = volume->GetFaceCount();
		for(f=0; f<faceCount; f++){
			volumeFace = (debpDMBConvexVolumeFace*)volume->GetFaceAt(f);
			vertexCount = volumeFace->GetVertexCount();
			module.LogInfoFormat("    face %i: decal(%i)", f + 1, volumeFace->GetDecalFace() ? 1 : 0);
			for(x=0; x<vertexCount; x++){
				const decVector &vertex = volume->GetVertexAt(volumeFace->GetVertexAt(x));
				module.LogInfoFormat("      vertex %i: %g, %g, %g", x + 1, vertex.x, vertex.y, vertex.z);
			}
		}
	}
	
	module.LogInfo("DecalMeshBuilder: Build mesh infos");
	for(f=0; f<pFaceCount; f++){
		module.LogInfoFormat("  face %i: index(%i) v1(%g,%g,%g) v2(%g,%g,%g) v3(%g,%g,%g)", f + 1,
			pFaces[f].GetFaceIndex(), pPoints[pFaces[f].GetPoint1()].x,
			pPoints[pFaces[f].GetPoint1()].y, pPoints[pFaces[f].GetPoint1()].z,
			pPoints[pFaces[f].GetPoint2()].x, pPoints[pFaces[f].GetPoint2()].y,
			pPoints[pFaces[f].GetPoint2()].z, pPoints[pFaces[f].GetPoint3()].x,
			pPoints[pFaces[f].GetPoint3()].y, pPoints[pFaces[f].GetPoint3()].z);
	}
}


const decVector &debpDecalMeshBuilder::GetPointAt(int index) const{
	if(index < 0 || index >= pPointCount) DETHROW(deeInvalidParam);
	
	return pPoints[index];
}

int debpDecalMeshBuilder::AddPoint(const decVector &point){
	int index = pIndexOfPoint(point);
	
	if(index == -1){
		if(pPointCount == pPointSize){
			int newSize = pPointSize * 3 / 2 + 1;
			decVector *newArray = new decVector[newSize];
			if(! newArray) DETHROW(deeOutOfMemory);
			if(pPoints){
				memcpy(newArray, pPoints, sizeof(decVector) * pPointSize);
				delete [] pPoints;
			}
			pPoints = newArray;
			pPointSize = newSize;
		}
		
		pPoints[pPointCount] = point;
		index = pPointCount;
		pPointCount++;
	}
	
	return index;
}

void debpDecalMeshBuilder::RemoveAllPoints(){
	pPointCount = 0;
}

debpDecalMeshBuilderFace *debpDecalMeshBuilder::GetFaceAt(int index) const{
	if(index < 0 || index >= pFaceCount) DETHROW(deeInvalidParam);
	
	return pFaces + index;
}

debpDecalMeshBuilderFace *debpDecalMeshBuilder::AddFace(){
	if(pFaceCount == pFaceSize){
		int newSize = pFaceSize * 3 / 2 + 1;
		debpDecalMeshBuilderFace *newArray = new debpDecalMeshBuilderFace[newSize];
		if(! newArray) DETHROW(deeOutOfMemory);
		if(pFaces){
			memcpy(newArray, pFaces, sizeof(debpDecalMeshBuilderFace) * pFaceSize);
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaceCount++;
	
	return pFaces + (pFaceCount - 1);
}

void debpDecalMeshBuilder::RemoveAllFaces(){
	pFaceCount = 0;
}



// Private Functions
//////////////////////

void debpDecalMeshBuilder::pVolumeAddFace(decConvexVolume *volume, int p1, int p2, int p3, const decVector &normal, bool decal){
	debpDMBConvexVolumeFace *face = NULL;
	decVector faceNormal;
	
	try{
		face = new debpDMBConvexVolumeFace;
		if(! face) DETHROW(deeOutOfMemory);
		
		faceNormal = normal;
		faceNormal.Normalize();
		face->SetNormal(faceNormal);
		
		face->AddVertex(p1);
		face->AddVertex(p2);
		face->AddVertex(p3);
		
		face->SetDecalFace(decal);
		
		volume->AddFace(face);
		
	}catch(const deException &){
		if(face) delete face;
		throw;
	}
}

void debpDecalMeshBuilder::pVolumeAddFace(decConvexVolume *volume, int p1, int p2, int p3, int p4, const decVector &normal, bool decal){
	debpDMBConvexVolumeFace *face = NULL;
	decVector faceNormal;
	
	try{
		face = new debpDMBConvexVolumeFace;
		if(! face) DETHROW(deeOutOfMemory);
		
		faceNormal = normal;
		faceNormal.Normalize();
		face->SetNormal(faceNormal);
		
		face->AddVertex(p1);
		face->AddVertex(p2);
		face->AddVertex(p3);
		face->AddVertex(p4);
		
		face->SetDecalFace(decal);
		
		volume->AddFace(face);
		
	}catch(const deException &){
		if(face) delete face;
		throw;
	}
}

int debpDecalMeshBuilder::pIndexOfPoint(const decVector &point) const{
	int i;
	
	for(i=0; i<pPointCount; i++){
		if(point.IsEqualTo(pPoints[i])) return i;
	}
	
	return -1;
}
