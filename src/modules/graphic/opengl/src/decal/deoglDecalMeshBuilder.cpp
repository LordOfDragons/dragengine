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

#include "deoglDecalMeshBuilder.h"
#include "deoglDecalMeshBuilderFace.h"
#include "deoglDMBConvexVolumeFace.h"
#include "deoglDMBConvexVolumeList.h"
#include "deoglRDecal.h"
#include "deoglMOVDecalCollect.h"
#include "../component/deoglRComponent.h"
#include "../component/deoglRComponentLOD.h"
#include "../model/deoglRModel.h"
#include "../model/deoglModelLOD.h"
#include "../model/face/deoglModelFace.h"
#include "../model/texture/deoglModelTexture.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTLogger.h"
#include "../triangles/deoglTriangleSorter.h"
#include "../utils/deoglConvexFaceClipper.h"
#include "../utils/collision/deoglDCollisionBox.h"
#include "../utils/collision/deoglDCollisionSphere.h"
#include "../utils/collision/deoglDCollisionTriangle.h"
#include "../utils/collision/deoglDCollisionDetection.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>



// Class deoglDecalMeshBuilder
////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglDecalMeshBuilder::deoglDecalMeshBuilder(deoglRenderThread &renderThread) :
pRenderThread(renderThread)
{
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

deoglDecalMeshBuilder::~deoglDecalMeshBuilder(){
	if(pFaces){
		delete [] pFaces;
	}
	if(pPoints){
		delete [] pPoints;
	}
	if(pDecalBox){
		delete pDecalBox;
	}
	if(pCVolList){
		delete pCVolList;
	}
}



// Management
///////////////

void deoglDecalMeshBuilder::Init(const deoglRDecal &decal, float distance){
	pSize = decal.GetSize();
	const decVector halfSize = decVector(pSize * 0.5f);
	decConvexVolume *volume = NULL;
	
	if(distance < 0.001f){
		distance = 0.001f;
	}
	
	// free the old stuff
	if(pCVolList){
		delete pCVolList;
		pCVolList = NULL;
	}
	
	// tuck away the infos
	pDecalMatrix.SetWorld(decal.GetPosition(), decal.GetOrientation());
	pDecalView = pDecalMatrix.TransformView();
	pOrigin = decal.GetPosition();
	pDistance = distance;
	
	pProjVector = -pDecalView * pDistance; // small offset just to make sure
	
	// create decal box
	if(pDecalBox){
		delete pDecalBox;
		pDecalBox = NULL;
	}
	pDecalBox = new deoglDCollisionBox(pOrigin - pDecalView * (pDistance * 0.5f),
		decVector(halfSize.x, halfSize.y, pDistance * 0.5f), decal.GetOrientation());
	
	// create convex volume list
	try{
		pCVolList = new deoglDMBConvexVolumeList;
		
		volume = new decConvexVolume;
		volume->AddVertex(pDecalMatrix.Transform(halfSize.x,  halfSize.y, 0.0f));
		volume->AddVertex(pDecalMatrix.Transform(-halfSize.x,  halfSize.y, 0.0f));
		volume->AddVertex(pDecalMatrix.Transform(-halfSize.x, -halfSize.y, 0.0f));
		volume->AddVertex(pDecalMatrix.Transform(halfSize.x, -halfSize.y, 0.0f));
		volume->AddVertex(pDecalMatrix.Transform(halfSize.x,  halfSize.y, -pDistance));
		volume->AddVertex(pDecalMatrix.Transform(-halfSize.x,  halfSize.y, -pDistance));
		volume->AddVertex(pDecalMatrix.Transform(-halfSize.x, -halfSize.y, -pDistance));
		volume->AddVertex(pDecalMatrix.Transform(halfSize.x, -halfSize.y, -pDistance));
		
		pVolumeAddFace(volume, 0, 1, 2, 3, pDecalView, false); // front
		pVolumeAddFace(volume, 4, 5, 1, 0, pDecalMatrix.TransformUp(), false); // top
		pVolumeAddFace(volume, 1, 5, 6, 2, -pDecalMatrix.TransformRight(), false); // right
		pVolumeAddFace(volume, 3, 2, 6, 7, -pDecalMatrix.TransformUp(), false); // bottom
		pVolumeAddFace(volume, 4, 0, 3, 7, pDecalMatrix.TransformRight(), false); // left
		pVolumeAddFace(volume, 5, 4, 7, 6, -pDecalView, false); // back
		
		pCVolList->AddVolume(volume);
		
	}catch(const deException &){
		if(volume){
			delete volume;
		}
		throw;
	}
}

void deoglDecalMeshBuilder::CutTriangle(const decVector &v1, const decVector &v2, const decVector &v3){
	const decVector normal((v2 - v1) % (v3 - v2));
	if(normal * pDecalView < 0.001f){
		return;
	}
	
	decConvexVolume splitVolume;
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

// #include <dragengine/common/utils/decTimer.h>
void deoglDecalMeshBuilder::BuildMeshForComponent(const deoglRComponent &component){
	BuildMeshForComponent(component.GetLODAt(0));
}

void deoglDecalMeshBuilder::BuildMeshForComponent (const deoglRComponentLOD& lod){
	deoglModelLOD &modelLod = lod.GetModelLODRef();
	
	RemoveAllFaces();
	RemoveAllPoints();
	
	// octree version
	modelLod.PrepareOctree();
	
// 		decTimer timer;
	deoglTriangleSorter &triangleSorter = pRenderThread.GetTriangleSorter();
	deoglMOVDecalCollect collect(modelLod, triangleSorter);
	collect.CollectFaces(pDecalMatrix, pSize);
// 		printf( "CollectFaces: %.2f\n", timer.GetElapsedTime() * 1e3f );
	
	const decVector projectVector(-pDecalView * 1e3f); // anything large enough should work
	const int faceCount = triangleSorter.GetTriangleCount();
	decConvexVolume splitVolume;
	int i;
	
	for(i=0; i<faceCount; i++){
		const decVector &v1 = triangleSorter.GetTriangleVertex1(i);
		const decVector &v2 = triangleSorter.GetTriangleVertex2(i);
		const decVector &v3 = triangleSorter.GetTriangleVertex3(i);
		
		// cut by the face
		splitVolume.SetEmpty();
		splitVolume.AddVertex(v1);
		splitVolume.AddVertex(v2);
		splitVolume.AddVertex(v3);
		splitVolume.AddVertex(v1 + projectVector);
		splitVolume.AddVertex(v2 + projectVector);
		splitVolume.AddVertex(v3 + projectVector);
		
		// front and back
		const decVector volumeFaceNormal((v2 - v1) % (v3 - v2));
		pVolumeAddFace(&splitVolume, 0, 1, 2, volumeFaceNormal, true);
		pVolumeAddFace(&splitVolume, 5, 4, 3, -volumeFaceNormal, false);
		
		// sides
		const decVector s1normal((v2 - v1) % pDecalView);
		if(s1normal.IsZero(0.001f)){
			break;
		}
		pVolumeAddFace(&splitVolume, 0, 3, 4, 1, s1normal, false);
		
		const decVector s2normal((v3 - v2) % pDecalView);
		if(s2normal.IsZero(0.001f)){
			break;
		}
		pVolumeAddFace(&splitVolume, 1, 4, 5, 2, s2normal, false);
		
		const decVector s3normal((v1 - v3) % pDecalView);
		if(s3normal.IsZero(0.001f)){
			break;
		}
		pVolumeAddFace(&splitVolume, 2, 5, 3, 0, s3normal, false);
		
		pCVolList->SplitByVolume(splitVolume);
	}
// 		printf( "Split: %.2f\n", timer.GetElapsedTime() * 1e3f );
	
	// add faces matching the volume
	const int volumeCount = pCVolList->GetVolumeCount();
	deoglConvexFaceClipper faceClipper;
	int j, k, l;
	
	for(i=0; i<volumeCount; i++){
		const decConvexVolume &volume = *pCVolList->GetVolumeAt(i);
		const int volumeFaceCount = volume.GetFaceCount();
		
		for(j=0; j<volumeFaceCount; j++){
			deoglDMBConvexVolumeFace &volumeFace = *((deoglDMBConvexVolumeFace*)volume.GetFaceAt(j));
			
			if(! volumeFace.GetDecalFace() || volumeFace.GetVertexCount() < 3){
				continue;
			}
			
			const decVector volumeFaceNormal(-volumeFace.GetNormal()); // since it faces backwards
			const float volumeFaceDot = volumeFaceNormal * volume.GetVertexAt(volumeFace.GetVertexAt(0));
			
			const int volumeVertexCount = volumeFace.GetVertexCount();
			
			// test all collected faces
			for(k=0; k<faceCount; k++){
				const decVector &v1 = triangleSorter.GetTriangleVertex1(k);
				const decVector &v2 = triangleSorter.GetTriangleVertex2(k);
				const decVector &v3 = triangleSorter.GetTriangleVertex3(k);
				
				// the face has to be coplanar with the volume face
				if(fabsf(volumeFaceNormal * v1 - volumeFaceDot) > 1e-5f
				|| fabsf(volumeFaceNormal * v2 - volumeFaceDot) > 1e-5f
				|| fabsf(volumeFaceNormal * v3 - volumeFaceDot) > 1e-5f){
					continue;
				}
				
				// clip the face
				faceClipper.RemoveAllVertices();
				for(l=volumeVertexCount-1; l>=0; l--){
					faceClipper.AddVertex(volume.GetVertexAt(volumeFace.GetVertexAt(l)));
				}
				
				faceClipper.ClipByPlane(volumeFaceNormal % (v2 - v1), v1);
				faceClipper.ClipByPlane(volumeFaceNormal % (v3 - v2), v2);
				faceClipper.ClipByPlane(volumeFaceNormal % (v1 - v3), v3);
				
				// if something is left it is a part of the decal mesh
				const int vertexCount = faceClipper.GetVertexCount();
				if(vertexCount < 3){
					continue;
				}
				
				const int firstPoint = AddPoint(faceClipper.GetVertexAt(0).ToVector());
				int lastPoint = AddPoint(faceClipper.GetVertexAt(1).ToVector());
				
				for(l=2; l<vertexCount; l++){
					const int nextPoint = AddPoint(faceClipper.GetVertexAt(l).ToVector());
					
					deoglDecalMeshBuilderFace &dmbFace = *AddFace();
					dmbFace.SetPoint1(firstPoint);
					dmbFace.SetPoint2(lastPoint);
					dmbFace.SetPoint3(nextPoint);
					dmbFace.SetFaceNormal(volumeFaceNormal);
					
					lastPoint = nextPoint;
				}
			}
		}
	}
// 		printf( "Convert: %.2f\n", timer.GetElapsedTime() * 1e3f );
}

#if 0
void deoglDecalMeshBuilder::BuildMeshForComponent (const deoglRComponentLOD& lod){
	RemoveAllFaces();
	RemoveAllPoints();
	
	// non-octree version
	const deoglRModel &oglModel = oglComponent.GetModel();
	const demodelLod &modelLod = oglModel.GetLODAt(0);
	const oglModelPosition * const modelPositions = modelLod.GetPositions();
	const oglModelVertex * const modelVertices = modelLod.GetVertices();
	const deoglModelFace * const modelFaces = modelLod.GetFaces();
	deoglConvexFaceClipper faceClipper;
	decConvexVolume splitVolume;
	int v, vf, f, x;
	
	// cut volume
	const int faceCount = modelLod.GetFaceCount();
	
	for(f=0; f<faceCount; f++){
		const deoglModelFace &face = modelFaces[f];
		if(modelLod.GetTextureAt(face.GetTexture()).GetDecal()){
			continue; // decal faces are not used to cut decals
		}
		
		const decVector &v1 = modelPositions[modelVertices[face.GetVertex1()].position].position;
		const decVector &v2 = modelPositions[modelVertices[face.GetVertex2()].position].position;
		const decVector &v3 = modelPositions[modelVertices[face.GetVertex3()].position].position;
		
		// face has to face into the direction of the decal
		if(face.GetFaceNormal() * pDecalView < FLOAT_SAFE_EPSILON){
			continue;
		}
		
		// clip the face to avoid artefacts
		faceClipper.RemoveAllVertices();
		faceClipper.AddVertex(v1);
		faceClipper.AddVertex(v2);
		faceClipper.AddVertex(v3);
		faceClipper.ClipByPlane(-pDecalView, pOrigin);
		
		const int vertexCount = faceClipper.GetVertexCount();
		if(vertexCount < 3){
			continue;
		}
		
		// cut by the face
		splitVolume.SetEmpty();
		for(x=0; x<vertexCount; x++){
			splitVolume.AddVertex(faceClipper.GetVertexAt(x).ToVector());
		}
		for(x=0; x<vertexCount; x++){
			splitVolume.AddVertex(faceClipper.GetVertexAt(x).ToVector() + pProjVector);
		}
		
		// front
		const decVector volumeFaceNormal(((v2 - v1) % (v3 - v2)).Normalized());
		
		deoglDMBConvexVolumeFace *volumeFace = NULL;
		try{
			volumeFace = new deoglDMBConvexVolumeFace;
			volumeFace->SetNormal(volumeFaceNormal);
			
			for(x=0; x<vertexCount; x++){
				volumeFace->AddVertex(x);
			}
			volumeFace->SetDecalFace(true);
			
			splitVolume.AddFace(volumeFace);
			
		}catch(const deException &){
			if(volumeFace){
				delete volumeFace;
			}
			throw;
		}
		
		// back
		volumeFace = NULL;
		try{
			volumeFace = new deoglDMBConvexVolumeFace;
			
			volumeFace->SetNormal(-volumeFaceNormal);
			
			for(x=vertexCount-1; x>=0; x--){
				volumeFace->AddVertex(vertexCount + x);
			}
			
			splitVolume.AddFace(volumeFace);
			
		}catch(const deException &){
			if(volumeFace){
				delete volumeFace;
			}
			throw;
		}
		
		// sides
		for(x=0; x<vertexCount; x++){
			const int x2 = (x + 1) % vertexCount;
			const decVector normal((splitVolume.GetVertexAt(x2) - splitVolume.GetVertexAt(x)) % pDecalView);
			
			if(normal.IsZero(0.001f)){
				// if the input face is not co-planar it is possible for a normal to end 0-length
				// due to the edge ending up co-linear with the decal view. this is an error case
				// in the model provided by the user and is silently skipped since no useful
				// projection of the decal is possible in this case
				splitVolume.SetEmpty();
				break;
			}
			
			pVolumeAddFace(&splitVolume, vertexCount + x, vertexCount + x2, x2, x,
				(splitVolume.GetVertexAt(x2) - splitVolume.GetVertexAt(x)) % pDecalView, false);
		}
		
		if(splitVolume.GetFaceCount() > 0){
			pCVolList->SplitByVolume(splitVolume);
		}
	}
	
	// add faces matching the volume
	const int volumeCount = pCVolList->GetVolumeCount();
	
	for(v=0; v<volumeCount; v++){
		const decConvexVolume &volume = *pCVolList->GetVolumeAt(v);
		const int volumeFaceCount = volume.GetFaceCount();
		
		for(vf=0; vf<volumeFaceCount; vf++){
			deoglDMBConvexVolumeFace &volumeFace = *((deoglDMBConvexVolumeFace*)volume.GetFaceAt(vf));
			
			if(! volumeFace.GetDecalFace()){
				continue;
			}
			if(volumeFace.GetNormal() * pDecalView > 0.001f){
				continue;
			}
			if(volumeFace.GetVertexCount() < 3){
				continue;
			}
			
			const decVector volumeFaceNormal(-volumeFace.GetNormal()); // since it faces backwards
			const float volumeFaceDot = volumeFaceNormal * volume.GetVertexAt(volumeFace.GetVertexAt(0));
			
			const int volumeVertexCount = volumeFace.GetVertexCount();
			
			// now test all faces of the component
			for(f=0; f<faceCount; f++){
				const deoglModelFace &oglCFace = modelFaces[f];
				if(modelLod.GetTextureAt(oglCFace.GetTexture()).GetDecal()){
					continue; // decal faces are not used to cut decals
				}
				
				// the face has to be coplanar with the volume face
				const decVector &v1 = modelPositions[modelVertices[oglCFace.GetVertex1()].position].position;
				
				if(fabs(volumeFaceNormal * oglCFace.GetFaceNormal()) < 0.99999f){
					continue;
				}
				if(fabs(volumeFaceNormal * v1 - volumeFaceDot) > FLOAT_SAFE_EPSILON){
					continue;
				}
				
				const decVector &v2 = modelPositions[modelVertices[oglCFace.GetVertex2()].position].position;
				const decVector &v3 = modelPositions[modelVertices[oglCFace.GetVertex3()].position].position;
				
				// clip the face
				faceClipper.RemoveAllVertices();
				for(x=volumeVertexCount-1; x>=0; x--){
					faceClipper.AddVertex(volume.GetVertexAt(volumeFace.GetVertexAt(x)));
				}
				
				faceClipper.ClipByPlane(oglCFace.GetFaceNormal() % (v2 - v1), v1);
				faceClipper.ClipByPlane(oglCFace.GetFaceNormal() % (v3 - v2), v2);
				faceClipper.ClipByPlane(oglCFace.GetFaceNormal() % (v1 - v3), v3);
				
				// if something is left it is a part of the decal mesh
				const int vertexCount = faceClipper.GetVertexCount();
				if(vertexCount < 3){
					continue;
				}
				
				const int firstPoint = AddPoint(faceClipper.GetVertexAt(0).ToVector());
				int lastPoint = AddPoint(faceClipper.GetVertexAt(1).ToVector());
				
				for(x=2; x<vertexCount; x++){
					const int nextPoint = AddPoint(faceClipper.GetVertexAt(x).ToVector());
					
					deoglDecalMeshBuilderFace &dmbFace = *AddFace();
					dmbFace.SetPoint1(firstPoint);
					dmbFace.SetPoint2(lastPoint);
					dmbFace.SetPoint3(nextPoint);
					dmbFace.SetFaceNormal(oglCFace.GetFaceNormal());
					
					lastPoint = nextPoint;
				}
			}
		}
	}
}
#endif


void deoglDecalMeshBuilder::Debug(){
	deoglRTLogger &logger = pRenderThread.GetLogger();
	const int volumeCount = pCVolList->GetVolumeCount();
	int v, f, x;
	
	logger.LogInfo("DecalMeshBuilder: Convex volume infos");
	for(v=0; v<volumeCount; v++){
		const decConvexVolume &volume = *pCVolList->GetVolumeAt(v);
		logger.LogInfoFormat("  volume %d: vertices(%d)", v + 1, volume.GetVertexCount());
		
		const int faceCount = volume.GetFaceCount();
		for(f=0; f<faceCount; f++){
			const deoglDMBConvexVolumeFace &volumeFace =
				*( ( deoglDMBConvexVolumeFace* )volume.GetFaceAt( f ) );
			logger.LogInfoFormat("    face %d: decalFace(%d)", f + 1, volumeFace.GetDecalFace());
			
			const int vertexCount = volumeFace.GetVertexCount();
			for(x=0; x<vertexCount; x++){
				const decVector &vertex = volume.GetVertexAt(volumeFace.GetVertexAt(x));
				logger.LogInfoFormat("      vertex %d: %g, %g, %g", x + 1, vertex.x, vertex.y, vertex.z);
			}
		}
	}
	
	logger.LogInfo("DecalMeshBuilder: Build mesh infos");
	for(f=0; f<pFaceCount; f++){
		logger.LogInfoFormat("  face %d: v1(%g,%g,%g) v2(%g,%g,%g) v3(%g,%g,%g) n(%g,%g,%g)", f + 1,
			pPoints[pFaces[f].GetPoint1()].x, pPoints[pFaces[f].GetPoint1()].y,
			pPoints[pFaces[f].GetPoint1()].z, pPoints[pFaces[f].GetPoint2()].x,
			pPoints[pFaces[f].GetPoint2()].y, pPoints[pFaces[f].GetPoint2()].z,
			pPoints[pFaces[f].GetPoint3()].x, pPoints[pFaces[f].GetPoint3()].y,
			pPoints[pFaces[f].GetPoint3()].z, pFaces[f].GetFaceNormal().x,
			pFaces[f].GetFaceNormal().y, pFaces[f].GetFaceNormal().z);
	}
}


const decVector &deoglDecalMeshBuilder::GetPointAt(int index) const{
	if(index < 0 || index >= pPointCount){
		DETHROW(deeInvalidParam);
	}
	
	return pPoints[index];
}

int deoglDecalMeshBuilder::AddPoint(const decVector &point){
	int index = pIndexOfPoint(point);
	
	if(index == -1){
		if(pPointCount == pPointSize){
			const int newSize = pPointSize * 3 / 2 + 1;
			decVector * const newArray = new decVector[newSize];
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

void deoglDecalMeshBuilder::RemoveAllPoints(){
	pPointCount = 0;
}

deoglDecalMeshBuilderFace *deoglDecalMeshBuilder::GetFaceAt(int index) const{
	if(index < 0 || index >= pFaceCount){
		DETHROW(deeInvalidParam);
	}
	
	return pFaces + index;
}

deoglDecalMeshBuilderFace *deoglDecalMeshBuilder::AddFace(){
	if(pFaceCount == pFaceSize){
		const int newSize = pFaceSize * 3 / 2 + 1;
		deoglDecalMeshBuilderFace * const newArray = new deoglDecalMeshBuilderFace[newSize];
		if(pFaces){
			memcpy(newArray, pFaces, sizeof(deoglDecalMeshBuilderFace) * pFaceSize);
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaceCount++;
	
	return pFaces + (pFaceCount - 1);
}

void deoglDecalMeshBuilder::RemoveAllFaces(){
	pFaceCount = 0;
}



// Private Functions
//////////////////////

void deoglDecalMeshBuilder::pVolumeAddFace(decConvexVolume *volume,
int p1, int p2, int p3, const decVector &normal, bool decalFace){
	deoglDMBConvexVolumeFace *face = NULL;
	
	try{
		face = new deoglDMBConvexVolumeFace;
		face->SetNormal(normal.Normalized());
		face->AddVertex(p1);
		face->AddVertex(p2);
		face->AddVertex(p3);
		face->SetDecalFace(decalFace);
		volume->AddFace(face);
		
	}catch(const deException &){
		if(face){
			delete face;
		}
		throw;
	}
}

void deoglDecalMeshBuilder::pVolumeAddFace(decConvexVolume *volume,
int p1, int p2, int p3, int p4, const decVector &normal, bool decalFace){
	deoglDMBConvexVolumeFace *face = NULL;
	
	try{
		face = new deoglDMBConvexVolumeFace;
		face->SetNormal(normal.Normalized());
		face->AddVertex(p1);
		face->AddVertex(p2);
		face->AddVertex(p3);
		face->AddVertex(p4);
		face->SetDecalFace(decalFace);
		volume->AddFace(face);
		
	}catch(const deException &){
		if(face){
			delete face;
		}
		throw;
	}
}

int deoglDecalMeshBuilder::pIndexOfPoint(const decVector &point) const{
	int i;
	
	for(i=0; i<pPointCount; i++){
		if(point.IsEqualTo(pPoints[i])){
			return i;
		}
	}
	
	return -1;
}
