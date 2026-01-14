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

// includes
#include <stdio.h>
#include <stdlib.h>
#include "decConvexVolume.h"
#include "decConvexVolumeFace.h"
#include "decConvexVolumeList.h"
#include "../exceptions.h"



// Definitions
////////////////

#define TEST_NONE			0
#define TEST_SPLIT			1
#define TEST_REMOVE			2

#define SPLIT_FRONT_BACK	0
#define SPLIT_FRONT			1
#define SPLIT_BACK			2
#define SPLIT_NONE			3

#define EQUALITY_THRESHOLD	1e-3f



// Class decConvexVolumeList
//////////////////////////////

// Volume Management
//////////////////////

const decConvexVolume::Ref &decConvexVolumeList::GetVolumeAt(int index) const{
	return pVolumes.GetAt(index);
}

bool decConvexVolumeList::HasVolume(decConvexVolume *volume) const{
	return pVolumes.Has(volume);
}

int decConvexVolumeList::IndexOfVolume(decConvexVolume *volume) const{
	return pVolumes.IndexOf(volume);
}

void decConvexVolumeList::AddVolume(decConvexVolume::Ref &&volume){
	DEASSERT_NOTNULL(volume)
	DEASSERT_FALSE(HasVolume(volume))
	
	pVolumes.Add(std::move(volume));
}

void decConvexVolumeList::RemoveVolume(decConvexVolume *volume){
	RemoveVolumeAt(pVolumes.IndexOf(volume));
}

void decConvexVolumeList::RemoveVolumeAt(int index){
	pVolumes.RemoveFrom(index);
}

void decConvexVolumeList::RemoveAllVolumes(){
	pVolumes.RemoveAll();
}



// Management
///////////////

void decConvexVolumeList::SetToCube(const decVector &halfSize){
	if(halfSize < decVector(0.0f, 0.0f, 0.0f)) DETHROW(deeInvalidParam);
	
	// remove all volumes
	RemoveAllVolumes();
	
	// add new volume set to cube
	decConvexVolume::Ref volume = CreateVolume(nullptr, true);
	volume->SetToCube(halfSize);
	AddVolume(std::move(volume));
}

void decConvexVolumeList::Move(const decVector &direction){
	pVolumes.Visit([&](decConvexVolume &v){
		v.Move(direction);
	});
}

void decConvexVolumeList::SplitByPlane(const decVector &splitNormal,
const decVector &splitPosition, bool deleteBackVolume, const decConvexVolumeFace *cutFaceInit){
	//printf( "split by plane: normal=(%g,%g,%g) position=(%g,%g,%g)\n", splitNormal.x, splitNormal.y, splitNormal.z, splitPosition.x, splitPosition.y, splitPosition.z );
	// determine the split plane parameters
	const float splitDot = splitNormal * splitPosition;
	int i;
	
	for(i=0; i<pVolumes.GetCount(); i++){
		const int result = pTestByPlane(i, splitNormal, splitDot);
		
		if(deleteBackVolume){
			if(result == TEST_SPLIT){
				if(pSplitByPlane(i, splitNormal, splitDot, deleteBackVolume, cutFaceInit) == SPLIT_NONE){
					i--;
				}
				
			}else if(result == TEST_REMOVE){
				RemoveVolumeAt(i);
				i--;
			}
			
		}else{
			if(result == TEST_SPLIT){
				if(pSplitByPlane(i, splitNormal, splitDot, deleteBackVolume, cutFaceInit) == SPLIT_NONE){
					i--;
				}
			}
		}
	}
}

void decConvexVolumeList::SplitByFace(const decConvexVolume &volume, int face){
	const decConvexVolumeFace &splitFace = volume.GetFaceAt(face);
	const decVector &splitNormal = splitFace.GetNormal();
	const decVector &splitVertex = volume.GetVertexAt(splitFace.GetVertexAt(0));
	const float splitDot = splitNormal * splitVertex;
	int i;
	
	for(i=0; i<pVolumes.GetCount(); i++){
		if(pTestByFace(i, volume, splitFace)){
			if(pSplitByPlane(i, splitNormal, splitDot, false, &splitFace) == SPLIT_NONE){
				i--;
			}
		}
	}
}

void decConvexVolumeList::SplitByVolume(const decConvexVolume &volume){
	int i;
	for(i=0; i<pVolumes.GetCount(); i++){
		// test what to do with the volume
		const int result = pTestByVolume(i, volume);
		if(result == TEST_SPLIT){
			// assign the current volume as the test volume
			int testVolume = i;
			
			// loop over all split faces
			const int faceCount = volume.GetFaceCount();
			int j;
			
			for(j=0; j<faceCount; j++){
				const decConvexVolumeFace &splitFace = volume.GetFaceAt(j);
				
				// determine the split plane parameters
				const decVector &splitNormal = splitFace.GetNormal();
				const decVector &splitVertex = volume.GetVertexAt(splitFace.GetVertexAt(0));
				const float splitDot = splitNormal * splitVertex;
				
				// split the volume using the split plane
				const int result2 = pSplitByPlane(testVolume, splitNormal, splitDot, false, &splitFace);
				if(result2 == SPLIT_FRONT_BACK){
					testVolume = pVolumes.GetCount() - 1;
					
				}else if(result2 == SPLIT_FRONT){
					testVolume = -1;
					break;
					
				}else if(result2 == SPLIT_NONE){
					if(testVolume == i){
						i--;
					}
					testVolume = -1;
					break;
				}
			}
			
			// if a test volume exists it is now completly inside the
			// split volume and has to be removed
			if(testVolume != -1){
				RemoveVolumeAt(testVolume);
				if(testVolume == i){
					i--;
				}
			}
			
		}else if(result == TEST_REMOVE){
			RemoveVolumeAt(i);
			i--;
		}
	}
}



// Protected Functions
////////////////////////

decConvexVolume::Ref decConvexVolumeList::CreateVolume(const decConvexVolume*, bool){
	return decConvexVolume::Ref::New();
}

decConvexVolumeFace::Ref decConvexVolumeList::CreateVolumeFace(const decConvexVolumeFace *face){
	return decConvexVolumeFace::Ref::New(face ? face->GetMarker() : 0);
}


// Private Functions
//////////////////////

int decConvexVolumeList::pTestByVolume(int volume, const decConvexVolume &splitVolume) const{
	int vertexCount = pVolumes.GetAt(volume)->GetVertexCount();
	int sv, svCount = splitVolume.GetVertexCount();
	int sf, sfCount = splitVolume.GetFaceCount();
	int v, f, faceCount;
	
	// loop over all split faces
	int resultSide = TEST_REMOVE;
	for(sf=0; sf<sfCount; sf++){
		const decConvexVolumeFace &splitFace = *splitVolume.GetFaceAt(sf);
		
		// determine the split plane parameters
		const decVector &splitNormal = splitFace.GetNormal();
		const decVector &splitVertex = splitVolume.GetVertexAt(splitFace.GetVertexAt(0));
		float splitDot = splitNormal * splitVertex;
		
		// if the volume is completly on either side there is no split
		int curSide = TEST_SPLIT;
		for(v=0; v<vertexCount; v++){
			const float dot = splitNormal * pVolumes.GetAt(volume)->GetVertexAt(v) - splitDot;
			int testSide;
			
			if(dot > EQUALITY_THRESHOLD){
				testSide = TEST_NONE;
			}else if(dot < -EQUALITY_THRESHOLD){
				testSide = TEST_REMOVE;
			}else{
				testSide = TEST_SPLIT;
			}
			if(curSide == TEST_SPLIT){
				curSide = testSide;
			}else if(testSide != TEST_SPLIT && testSide != curSide){
				curSide = TEST_SPLIT;
				break;
			}
		}
		if(curSide == TEST_NONE) return TEST_NONE;
		if(curSide == TEST_SPLIT) resultSide = TEST_SPLIT;
	}
	
	// if any of the volume face normals separates there is no split
	faceCount = pVolumes.GetAt(volume)->GetFaceCount();
	for(f=0; f<faceCount; f++){
		const decConvexVolumeFace &testFace = *pVolumes.GetAt(volume)->GetFaceAt(f);
		const decVector &normal = testFace.GetNormal();
		const float planeDot = normal * pVolumes.GetAt(volume)->GetVertexAt(testFace.GetVertexAt(0));
		
		for(sv=0; sv<svCount; sv++){
			if(normal * splitVolume.GetVertexAt(sv) - planeDot < -EQUALITY_THRESHOLD) break;
		}
		if(sv == svCount) return TEST_NONE;
	}
	
	// return the result
	return resultSide;
}

bool decConvexVolumeList::pTestByFace(int volume, const decConvexVolume &splitVolume,
const decConvexVolumeFace &splitFace) const{
	int vertexCount = pVolumes.GetAt(volume)->GetVertexCount();
	int curSide, testSide;
	int v, f, faceCount;
	
	// determine the split plane parameters
	const decVector &splitNormal = splitFace.GetNormal();
	const decVector &splitVertex = splitVolume.GetVertexAt(splitFace.GetVertexAt(0));
	float splitDot = splitNormal * splitVertex;
	
	// if the volume is completly on either side there is no split
	curSide = TEST_SPLIT;
	for(v=0; v<vertexCount; v++){
		const float dot = splitNormal * pVolumes.GetAt(volume)->GetVertexAt(v) - splitDot;
		if(dot > EQUALITY_THRESHOLD){
			testSide = TEST_NONE;
		}else if(dot < -EQUALITY_THRESHOLD){
			testSide = TEST_REMOVE;
		}else{
			testSide = TEST_SPLIT;
		}
		if(curSide == TEST_SPLIT){
			curSide = testSide;
		}else if(testSide != TEST_SPLIT && testSide != curSide){
			curSide = TEST_SPLIT;
			break;
		}
	}
	if(curSide != TEST_SPLIT) return false;
	
	// if any of the volume face normals separates there is no split
	faceCount = pVolumes.GetAt(volume)->GetFaceCount();
	vertexCount = splitFace.GetVertexCount();
	
	for(f=0; f<faceCount; f++){
		const decConvexVolumeFace &testFace = pVolumes.GetAt(volume)->GetFaceAt(f);
		const decVector &normal = testFace.GetNormal();
		const float planeDot = normal * pVolumes.GetAt(volume)->GetVertexAt(testFace.GetVertexAt(0));
		
		for(v=0; v<vertexCount; v++){
			if(normal * splitVolume.GetVertexAt(v) - planeDot < -EQUALITY_THRESHOLD) break;
		}
		if(v == vertexCount) return false;
	}
	
	// the volume has to be split
	return true;
}

int decConvexVolumeList::pTestByPlane(int volume, const decVector &splitNormal, float splitDot) const{
	int vertexCount = pVolumes.GetAt(volume)->GetVertexCount();
	int curSide, testSide, v;
	
	// if the volume is completly on either side there is no split
	curSide = TEST_SPLIT;
	for(v=0; v<vertexCount; v++){
		const float dot = splitNormal * pVolumes.GetAt(volume)->GetVertexAt(v) - splitDot;
		if(dot > EQUALITY_THRESHOLD){
			testSide = TEST_NONE;
		}else if(dot < -EQUALITY_THRESHOLD){
			testSide = TEST_REMOVE;
		}else{
			testSide = TEST_SPLIT;
		}
		if(curSide == TEST_SPLIT){
			curSide = testSide;
		}else if(testSide != TEST_SPLIT && testSide != curSide){
			curSide = TEST_SPLIT;
			break;
		}
	}
	if(curSide != TEST_SPLIT) return curSide;
	
	// the volume has to be split
	return TEST_SPLIT;
}

int decConvexVolumeList::pSplitByPlane(int volume, const decVector &splitNormal,
float splitDot, bool deleteBackVolume, const decConvexVolumeFace *face){
	int f, faceCount = pVolumes.GetAt(volume)->GetFaceCount();
	decConvexVolume::Ref volumeFront, volumeBack;
	decConvexVolumeFace::Ref faceFront, faceBack, faceCutFront, faceCutBack;
	int result = SPLIT_FRONT;
	
	// loop over all faces
	for(f=0; f<faceCount; f++){
		const decConvexVolumeFace &testFace = pVolumes.GetAt(volume)->GetFaceAt(f);
		const int vertexCount = testFace.GetVertexCount();
		int v;
		
		// for every face loop over all vertices
		for(v=0; v<vertexCount; v++){
			// get the first and second vertex of this edge
			const int vertexFirst = testFace.GetVertexAt(v);
			const int vertexSecond = testFace.GetVertexAt((v + 1) % vertexCount);
			
			const decVector &vertexFirstPosition = pVolumes.GetAt(volume)->GetVertexAt(vertexFirst);
			const decVector &vertexSecondPosition = pVolumes.GetAt(volume)->GetVertexAt(vertexSecond);
			
			// determine if the vertices are in front of the split plane
			const float dotFirst = splitNormal * vertexFirstPosition - splitDot;
			const float dotSecond = splitNormal * vertexSecondPosition - splitDot;
			const bool isFrontFirst = dotFirst > EQUALITY_THRESHOLD;
			const bool isFrontSecond = dotSecond > EQUALITY_THRESHOLD;
			
			// add vertex to the front volume if front side
			if(isFrontFirst){
				// if the front volume does not exist we create it
				if(!volumeFront){
					volumeFront = CreateVolume(pVolumes.GetAt(volume), true);
				}
				
				// if the front face does not exist we create it
				if(!faceFront){
					faceFront = CreateVolumeFace(&testFace);
					faceFront->SetNormal(testFace.GetNormal());
				}
				
				// add the vertex if not existing already
				int vertexIndex = volumeFront->IndexOfVertex(vertexFirstPosition);
				if(vertexIndex == -1){
					vertexIndex = volumeFront->GetVertexCount();
					volumeFront->AddVertex(vertexFirstPosition);
				}
				
				// add to front face
				faceFront->AddVertex(vertexIndex);
				
			// add vertex to the back volume if on back side and not marked for deletion
			}else if(!deleteBackVolume){
				// if the back volume does not exist we create it
				if(!volumeBack){
					volumeBack = CreateVolume(pVolumes.GetAt(volume), false);
				}
				
				// if the back face does not exist we create it
				if(!faceBack){
					faceBack = CreateVolumeFace(&testFace);
					faceBack->SetNormal(testFace.GetNormal());
				}
				
				// add the vertex if not existing already
				int vertexIndex = volumeBack->IndexOfVertex(vertexFirstPosition);
				if(vertexIndex == -1){
					vertexIndex = volumeBack->GetVertexCount();
					volumeBack->AddVertex(vertexFirstPosition);
				}
				
				// add to back face
				faceBack->AddVertex(vertexIndex);
			}
			
			// if both are different add cut vertex
			if(isFrontFirst != isFrontSecond){
				// calculate the cut vertex
				const decVector edgeDirection(vertexSecondPosition - vertexFirstPosition);
				const float lambda = (splitDot - splitNormal * vertexFirstPosition)
					/ (splitNormal * edgeDirection);
				const decVector cutVertexPosition(vertexFirstPosition + edgeDirection * lambda);
				
				// if the front volume does not exist we create it
				if(!volumeFront){
					volumeFront = CreateVolume(pVolumes.GetAt(volume), true);
				}
				
				// if the front face does not exist we create it
				if(!faceFront){
					faceFront = CreateVolumeFace(&testFace);
					faceFront->SetNormal(testFace.GetNormal());
				}
				
				// if the front cut face does not exist we create it
				if(!faceCutFront){
					faceCutFront = CreateVolumeFace(face);
					faceCutFront->SetNormal(-splitNormal);
				}
				
				// add the vertex to front volume if not existing already
				int vertexIndex = volumeFront->IndexOfVertex(cutVertexPosition);
				if(vertexIndex == -1){
					vertexIndex = volumeFront->GetVertexCount();
					volumeFront->AddVertex(cutVertexPosition);
				}
				
				// add vertex to front face and front cut face
				faceFront->AddVertex(vertexIndex);
				if(!faceCutFront->HasVertex(vertexIndex)){
					faceCutFront->AddVertex(vertexIndex);
				}
				
				// back volume stuff only if not marked for deletion
				if(!deleteBackVolume){
					// if the back volume does not exist we create it
					if(!volumeBack){
						volumeBack = CreateVolume(pVolumes.GetAt(volume), false);
					}
					
					// if the back face does not exist we create it
					if(!faceBack){
						faceBack = CreateVolumeFace(&testFace);
						faceBack->SetNormal(testFace.GetNormal());
					}
					
					// if the back cut face does not exist we create it
					if(!faceCutBack){
						faceCutBack = CreateVolumeFace(face);
						faceCutBack->SetNormal(splitNormal);
					}
					
					// add the vertex to back volume if not existing already
					vertexIndex = volumeBack->IndexOfVertex(cutVertexPosition);
					if(vertexIndex == -1){
						vertexIndex = volumeBack->GetVertexCount();
						volumeBack->AddVertex(cutVertexPosition);
					}
					
					// add vertex to back face and back cut face
					faceBack->AddVertex(vertexIndex);
					if(!faceCutBack->HasVertex(vertexIndex)){
						faceCutBack->AddVertex(vertexIndex);
					}
				}
			}
		}
		
		// add faces to volumes if exising
		if(faceFront){
			volumeFront->AddFace(std::move(faceFront));
		}
		if(faceBack){
			volumeBack->AddFace(std::move(faceBack));
		}
	}
	
	// add cut faces if existing
	if(faceCutFront && !faceCutFront->IsTooSmall(volumeFront)){
		faceCutFront->SortVertices(volumeFront);
		volumeFront->AddFace(std::move(faceCutFront));
	}
	
	if(faceCutBack && !faceCutBack->IsTooSmall(volumeBack)){
		faceCutBack->SortVertices(volumeBack);
		volumeBack->AddFace(std::move(faceCutBack));
	}
	
	// add volumes if existing
	if(volumeFront){
		pVolumes.SetAt(volume, std::move(volumeFront));
		
		if(volumeBack){
			AddVolume(std::move(volumeBack));
			result = SPLIT_FRONT_BACK;
			
		}else{
			result = SPLIT_FRONT;
		}
		
	}else if(volumeBack){
		pVolumes.SetAt(volume, std::move(volumeBack));
		result = SPLIT_BACK;
		
	}else{
		RemoveVolumeAt(volume);
		result = SPLIT_NONE;
	}
	
	return result;
}

int decConvexVolumeList::pSplitByVolume(int volume, const decConvexVolume &splitVolume){
#ifdef NEVER_SET_ME
	int f, faceCount = pVolumes.GetAt(volume)->GetFaceCount();
	decConvexVolume::Ref volumeFront, volumeBack;
	decConvexVolumeFace *faceFront = nullptr;
	decConvexVolumeFace *faceBack = nullptr;
	decConvexVolumeFace *faceCutFront = nullptr;
	decConvexVolumeFace *faceCutBack = nullptr;
	decConvexVolumeFace *testFace;
	decVector edgeDirection, cutVertexPosition;
	bool isFrontFirst, isFrontSecond, hasCutVertex;
	float dotFirst, dotSecond;
	int vertexFirst, vertexSecond;
	int vertexIndex, v, vertexCount;
	bool volumeRemoved = false;
	float lambda;
	
	// things can go wrong so we protect this place
	try{
		// loop over all faces
		for(f=0; f<faceCount; f++){
			testFace = pVolumes.GetAt(volume)->GetFaceAt(f);
			vertexCount = testFace.GetVertexCount();
			
			// for every face loop over all vertices
			for(v=0; v<vertexCount; v++){
				// get the first and second vertex of this edge
				vertexFirst = testFace.GetVertexAt(v);
				vertexSecond = testFace.GetVertexAt((v + 1) % vertexCount);
				
				const decVector &vertexFirstPosition = pVolumes.GetAt(volume)->GetVertexAt(vertexFirst);
				const decVector &vertexSecondPosition = pVolumes.GetAt(volume)->GetVertexAt(vertexSecond);
				
				// determine if the vertices are in front of the split plane
				dotFirst = splitNormal * vertexFirstPosition - splitDot;
				dotSecond = splitNormal * vertexSecondPosition - splitDot;
				isFrontFirst = (dotFirst > EQUALITY_THRESHOLD);
				isFrontSecond = (dotSecond > EQUALITY_THRESHOLD);
				
				// add vertex to the front volume if front side
				if(isFrontFirst){
					// if the front volume does not exist we create it
					if(!volumeFront){
						volumeFront = CreateVolume(pVolumes.GetAt(volume), true);
					}
					
					// if the front face does not exist we create it
					if(!faceFront){
						faceFront = CreateVolumeFace(testFace);
						faceFront->SetNormal(testFace.GetNormal());
					}
					
					// add the vertex if not existing already
					vertexIndex = volumeFront->IndexOfVertex(vertexFirstPosition);
					if(vertexIndex == -1){
						vertexIndex = volumeFront->GetVertexCount();
						volumeFront->AddVertex(vertexFirstPosition);
					}
					
					// add to front face
					faceFront->AddVertex(vertexIndex);
					
				// add vertex to the back volume if on back side and not marked for deletion
				}else if(!deleteBackVolume){
					// if the back volume does not exist we create it
					if(!volumeBack){
						volumeBack = CreateVolume(pVolumes.GetAt(volume), false);
					}
					
					// if the back face does not exist we create it
					if(!faceBack){
						faceBack = CreateVolumeFace(testFace);
						faceBack->SetNormal(testFace.GetNormal());
					}
					
					// add the vertex if not existing already
					vertexIndex = volumeBack->IndexOfVertex(vertexFirstPosition);
					if(vertexIndex == -1){
						vertexIndex = volumeBack->GetVertexCount();
						volumeBack->AddVertex(vertexFirstPosition);
					}
					
					// add to back face
					faceBack->AddVertex(vertexIndex);
				}
				
				// if both are different add cut vertex
				if(isFrontFirst != isFrontSecond){
					// calculate the cut vertex
					edgeDirection = vertexSecondPosition - vertexFirstPosition;
					lambda = (splitDot - splitNormal * vertexFirstPosition)
						/ (splitNormal * edgeDirection);
					cutVertexPosition = vertexFirstPosition + edgeDirection * lambda;
					
					// if the front volume does not exist we create it
					if(!volumeFront){
						volumeFront = CreateVolume(pVolumes.GetAt(volume), true);
					}
					
					// if the front face does not exist we create it
					if(!faceFront){
						faceFront = CreateVolumeFace(testFace);
						faceFront->SetNormal(testFace.GetNormal());
					}
					
					// if the front cut face does not exist we create it
					if(!faceCutFront){
						faceCutFront = CreateVolumeFace();
						faceCutFront->SetNormal(-splitNormal);
					}
					
					// add the vertex to front volume if not existing already
					vertexIndex = volumeFront->IndexOfVertex(cutVertexPosition);
					if(vertexIndex == -1){
						vertexIndex = volumeFront->GetVertexCount();
						volumeFront->AddVertex(cutVertexPosition);
					}
					
					// add vertex to front face and front cut face
					faceFront->AddVertex(vertexIndex);
					if(!faceCutFront->HasVertex(vertexIndex)){
						faceCutFront->AddVertex(vertexIndex);
					}
					
					// back volume stuff only if not marked for deletion
					if(!deleteBackVolume){
						// if the back volume does not exist we create it
						if(!volumeBack){
							volumeBack = CreateVolume(pVolumes.GetAt(volume), false);
						}
						
						// if the back face does not exist we create it
						if(!faceBack){
							faceBack = CreateVolumeFace(testFace);
							faceBack->SetNormal(testFace.GetNormal());
						}
						
						// if the back cut face does not exist we create it
						if(!faceCutBack){
							faceCutBack = CreateVolumeFace();
							faceCutBack->SetNormal(splitNormal);
						}
						
						// add the vertex to back volume if not existing already
						vertexIndex = volumeBack->IndexOfVertex(cutVertexPosition);
						if(vertexIndex == -1){
							vertexIndex = volumeBack->GetVertexCount();
							volumeBack->AddVertex(cutVertexPosition);
						}
						
						// add vertex to back face and back cut face
						faceBack->AddVertex(vertexIndex);
						if(!faceCutBack->HasVertex(vertexIndex)){
							faceCutBack->AddVertex(vertexIndex);
						}
					}
				}
			}
			
			// add faces to volumes if exising
			if(faceFront){
				volumeFront->AddFace(faceFront);
				faceFront = NULL;
			}
			if(faceBack){
				volumeBack->AddFace(faceBack);
				faceBack = NULL;
			}
		}
		
		// add cut faces if existing
		if(faceCutFront){
			faceCutFront->SortVertices(volumeFront);
			volumeFront->AddFace(faceCutFront);
			faceCutFront = NULL;
		}
		if(faceCutBack){
			faceCutBack->SortVertices(volumeBack);
			volumeBack->AddFace(faceCutBack);
			faceCutBack = NULL;
		}
		
		// add volumes if existing
		if(volumeFront){
			pVolumes.SetAt(volume, std::move(volumeFront));
			
			if(volumeBack){
				AddVolume(std::move(volumeBack));
			}
			
		}else if(volumeBack){
			pVolumes.SetAt(volume, std::move(volumeBack));
			
		}else{
			RemoveVolumeAt(volume);
			volumeRemoved = true;
		}
		
	}catch(const deException &){
		if(faceCutFront) delete faceCutFront;
		if(faceCutBack) delete faceCutBack;
		if(faceFront) delete faceFront;
		if(faceBack) delete faceBack;
		throw;
	}
	
	return volumeRemoved;
#endif
	return SPLIT_FRONT;
}
