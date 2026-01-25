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

#include "deoglVCOptimizer.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>


// Class deoglVCOptimizer
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglVCOptimizer::deoglVCOptimizer(){
	pCacheDecayPower = 1.5f;
	pLastFaceScore = 0.75f;
	pUsageBoostScale = 2.0f;
	pUsageBoostPower = -0.5f;
	
	pCacheSize = 32;
	pInvCacheFactor = 1.0f / (float)pCacheSize;
	
	pCache.SetAll(pCacheSize + 3, -1);
	pNextFaceCheck = 0;
}

deoglVCOptimizer::~deoglVCOptimizer() = default;


// Management
///////////////

void deoglVCOptimizer::SetVertexCount(int vertexCount){
	pVertices.SetCountDiscard(vertexCount);
	pReorderedVertices.SetCountDiscard(0);
	pReorderedVertices.EnlargeCapacity(vertexCount);
}

void deoglVCOptimizer::SetFaceCount(int faceCount){
	pFaces.SetCountDiscard(faceCount);
	pVertexFaces.SetCountDiscard(faceCount * 3);
	pReorderedFaces.SetCountDiscard(0);
	pReorderedFaces.EnlargeCapacity(faceCount);
	pNextFaceCheck = 0;
}

void deoglVCOptimizer::SetFaceAt(int face, int vertex1, int vertex2, int vertex3){
	sFace &f = pFaces[face];
	f.vertices[0] = vertex1;
	f.vertices[1] = vertex2;
	f.vertices[2] = vertex3;
}

void deoglVCOptimizer::SetGroupCount(int groupCount){
	pGroups.SetCountDiscard(groupCount);
}

void deoglVCOptimizer::SetGroupAt(int group, int firstFace, int faceCount){
	pGroups[group] = {firstFace, faceCount};
}



void deoglVCOptimizer::Optimize(){
	pReorderedFaces.SetCountDiscard(0);
	pReorderedVertices.SetCountDiscard(0);
	
	OptimizeFaceOrder();
	OptimizeVertexOrder();
}



void deoglVCOptimizer::OptimizeFaceOrder(){
	DEASSERT_TRUE(pReorderedFaces.IsEmpty())
	DEASSERT_TRUE(pReorderedVertices.IsEmpty())
	DEASSERT_TRUE(pGroups.IsNotEmpty())
	
	int j, k, l, nextFace;
	
	// set faces to not consumed
	pFaces.Visit([&](sFace &face){
		face.consumed = false;
	});
	
	// init the vertex faces
	pVertices.Visit([&](sVertex &vertex){
		vertex.faceCount = 0;
	});
	sVertex * const vertices = pVertices.GetArrayPointer();
	
	pFaces.Visit([&](const sFace &face){
		vertices[face.vertices[0]].faceCount++;
		vertices[face.vertices[1]].faceCount++;
		vertices[face.vertices[2]].faceCount++;
	});
	sFace * const faces = pFaces.GetArrayPointer();
	int * const vertexFaces = pVertexFaces.GetArrayPointer();
	
	if(pVertices.IsNotEmpty()){
		vertices[0].firstFace = 0;
		
		int i;
		for(i=1; i<pVertices.GetCount(); i++){
			vertices[i].firstFace = vertices[i - 1].firstFace + vertices[i - 1].faceCount;
		}
	}
	
	// process each group individually
	pGroups.Visit([&](const sGroup &group){
		const int faceCount = group.faceCount;
		if(faceCount == 0){
			return;
		}
		
		const int firstFace = group.firstFace;
		const int faceLimit = firstFace + faceCount;
		
		// initialize
		pCache.SetRangeAt(0, pCacheSize + 3, -1);
		
		pVertices.Visit([&](sVertex &vertex){
			vertex.consumed = false;
			vertex.cachePosition = -1;
			vertex.score = 0.0f;
			vertex.faceCount = 0;
		});
		
		for(j=firstFace; j<faceLimit; j++){
			for(k=0; k<3; k++){
				sVertex &vertex = vertices[faces[j].vertices[k]];
				
				vertexFaces[vertex.firstFace + vertex.faceCount] = j;
				vertex.faceCount++;
			}
		}
		
		pVertices.Visit([&](sVertex &vertex){
			CalculateVertexScore(vertex);
		});
		
		pFaces.Visit(firstFace, faceLimit, [&](sFace &face){
			CalculateFaceScore(face);
		});
		
		// sort faces in this group adding them to the face list
		pNextFaceCheck = pReorderedFaces.GetCount();
		bool fullSearch = true;
		
		while(pReorderedFaces.GetCount() < faceLimit){
			// find next face to add
			if(fullSearch){
				nextFace = FindBestScoreFace(firstFace, faceLimit);
				fullSearch = false;
				
			}else{
				nextFace = FindBestScoreFaceInCache();
				
				if(nextFace == -1){
					nextFace = FindNextFace(faceLimit);
					DEASSERT_TRUE(nextFace != -1)
				}
			}
			
			// add face
			pReorderedFaces.Add(nextFace);
			faces[nextFace].consumed = true;
			
			// remove the face from the vertices
			for(k=0; k<3; k++){
				sVertex &vertex = vertices[faces[nextFace].vertices[k]];
				const int firstVertexFace = vertex.firstFace;
				const int limitVertexFace = vertex.firstFace + vertex.faceCount;
				
				for(l=firstVertexFace; l<limitVertexFace; l++){
					if(vertexFaces[l] == nextFace){
						break;
					}
				}
				
				// move the last face index in the list to the the face index to remove. if l is already
				// the last face index in the list nothing bad happens. this way we can avoid using an
				// if statement which does not do anything useful
				vertexFaces[l] = vertexFaces[limitVertexFace - 1];
				vertex.faceCount--;
			}
			
			// add vertices to the cache and update the scores
			AddVertexToCache(faces[nextFace].vertices[0]);
			AddVertexToCache(faces[nextFace].vertices[1]);
			AddVertexToCache(faces[nextFace].vertices[2]);
			
			UpdateCacheScores();
		}
		
		DEASSERT_TRUE(pReorderedFaces.GetCount() == faceLimit)
	});
}

void deoglVCOptimizer::OptimizeVertexOrder(){
	DEASSERT_TRUE(pReorderedFaces.GetCount() == pFaces.GetCount())
	DEASSERT_TRUE(pReorderedVertices.IsEmpty())
	
	int j;
	
	pVertices.Visit([&](sVertex &vertex){
		vertex.consumed = false;
	});
	
	const sFace * const faces = pFaces.GetArrayPointer();
	sVertex * const vertices = pVertices.GetArrayPointer();
	
	pReorderedFaces.Visit([&](int &reorderedFaceIndex){
		const sFace &face = faces[reorderedFaceIndex];
		
		for(j=0; j<3; j++){
			const int vertex = face.vertices[j];
			
			if(!vertices[vertex].consumed){
				DEASSERT_FALSE(pReorderedVertices.GetCount() == pVertices.GetCount())
				
				vertices[vertex].consumed = true;
				vertices[vertex].reorderedIndex = pReorderedVertices.GetCount();
				pReorderedVertices.Add(vertex);
			}
		}
	});
	
	DEASSERT_TRUE(pReorderedVertices.GetCount() == pVertices.GetCount())
}



void deoglVCOptimizer::CalculateVertexScore(sVertex &vertex){
	const int faceCount = vertex.faceCount;
	float score = -1.0f;
	
	if(faceCount > 0){
		const int cachePosition = vertex.cachePosition;
		
		score = 0.0f;
		
		if(cachePosition >= 0){
			if(cachePosition < 3){
				score = pLastFaceScore;
				
			}else{
				DEASSERT_TRUE(cachePosition < pCacheSize + 3)
				
				// points for being high in the cache
				score = powf(1.0f - (float)(cachePosition - 3) * pInvCacheFactor, pCacheDecayPower);
			}
		}
		
		// bonus points for having a low number of faces still to use the vertex, so we get rid of lone verts quickly
		score += powf((float)faceCount, pUsageBoostPower) * pUsageBoostScale;
	}
	
	vertex.score = score;
}

void deoglVCOptimizer::CalculateFaceScore(sFace &face){
	const sVertex * const vertices = pVertices.GetArrayPointer();
	face.score = vertices[face.vertices[0]].score;
	face.score += vertices[face.vertices[1]].score;
	face.score += vertices[face.vertices[2]].score;
}

void deoglVCOptimizer::AddVertexToCache(int vertex){
	int * const cache = pCache.GetArrayPointer();
	int i, index;
	
	for(index=0; index<pCacheSize+2; index++){
		if(cache[index] == vertex){
			break;
		}
	}
	
	for(i=index; i>0; i--){
		cache[i] = cache[i - 1];
	}
	
	cache[0] = vertex;
}

void deoglVCOptimizer::UpdateCacheScores(){
	sVertex * const vertices = pVertices.GetArrayPointer();
	sFace * const faces = pFaces.GetArrayPointer();
	int * const cache = pCache.GetArrayPointer();
	float score, scoreChange;
	int i, j;
	
	for(i=0; i<pCacheSize+3; i++){
		if(cache[i] == -1){
			continue;
		}
		
		sVertex &vertex = vertices[cache[i]];
		const int firstFace = vertex.firstFace;
		const int limitFace = vertex.firstFace + vertex.faceCount;
		
		score = vertex.score;
		
		if(i < pCacheSize){
			vertex.cachePosition = i;
			
		}else{
			vertex.cachePosition = -1;
		}
		
		CalculateVertexScore(vertices[cache[i]]);
		scoreChange = vertex.score - score;
		
		for(j=firstFace; j<limitFace; j++){
			faces[pVertexFaces[j]].score += scoreChange;
		}
	}
	
	cache[pCacheSize] = -1;
	cache[pCacheSize + 1] = -1;
	cache[pCacheSize + 2] = -1;
}

int deoglVCOptimizer::FindBestScoreFaceInCache(){
	int * const cache = pCache.GetArrayPointer();
	const sVertex * const vertices = pVertices.GetArrayPointer();
	const sFace * const faces = pFaces.GetArrayPointer();
	int * const vertexFaces = pVertexFaces.GetArrayPointer();
	
	int i, j, bestFace = -1;
	float bestScore = -1.0f;
	
	for(i=0; i<pCacheSize; i++){
		if(cache[i] == -1){
			continue;
		}
		
		const sVertex &vertex = vertices[cache[i]];
		const int firstFace = vertex.firstFace;
		const int limitFace = vertex.firstFace + vertex.faceCount;
		
		for(j=firstFace; j<limitFace; j++){
			const sFace &face = faces[vertexFaces[j]];
			
			if(!face.consumed && face.score > bestScore){
				bestFace = vertexFaces[j];
				bestScore = face.score;
			}
		}
	}
	
	return bestFace;
}

int deoglVCOptimizer::FindBestScoreFace(int firstFace, int faceLimit){
	const sFace * const faces = pFaces.GetArrayPointer();
	
	float bestScore = -1.0f;
	int i, bestFace = -1;
	
	for(i=firstFace; i<faceLimit; i++){
		if(!faces[i].consumed && faces[i].score > bestScore){
			bestFace = i;
			bestScore = faces[i].score;
		}
	}
	
	return bestFace;
}

int deoglVCOptimizer::FindNextFace(int faceLimit){
	const sFace * const faces = pFaces.GetArrayPointer();
	
	for(; pNextFaceCheck<faceLimit; pNextFaceCheck++){
		if(!faces[pNextFaceCheck].consumed){
			return pNextFaceCheck++;
		}
	}
	
	return -1;
}



void deoglVCOptimizer::DebugPrint(deoglRenderThread& renderThread){
	int i, j;
	
	renderThread.GetLogger().LogInfo("Optimize Vertex Cache:");
	
	renderThread.GetLogger().LogInfoFormat("Groups (%i):", pGroups.GetCount());
	for(i=0; i<pGroups.GetCount(); i++){
		renderThread.GetLogger().LogInfoFormat("- Group %.4i = First Face %i, Face Count %i", i, pGroups[i].firstFace, pGroups[i].faceCount);
	}
	
	renderThread.GetLogger().LogInfoFormat("Faces (%i):", pFaces.GetCount());
	for(i=0; i<pGroups.GetCount(); i++){
		const int firstFace = pGroups[i].firstFace;
		const int faceLimit = firstFace + pGroups[i].faceCount;
		for(j=firstFace; j<faceLimit; j++){
			renderThread.GetLogger().LogInfoFormat("- Face %.4i = Vertices(%.4i | %.4i | %.4i) Group %i", j, pFaces[j].vertices[0],
				pFaces[j].vertices[1], pFaces[j].vertices[2], i);
		}
	}
	
	renderThread.GetLogger().LogInfoFormat("Reordered Faces (%i):", pReorderedFaces.GetCount());
	for(i=0; i<pGroups.GetCount(); i++){
		const int firstFace = pGroups[i].firstFace;
		const int faceLimit = firstFace + pGroups[i].faceCount;
		for(j=firstFace; j<faceLimit; j++){
			renderThread.GetLogger().LogInfoFormat("- Face %.4i = %.4i (Group %i)", i, pReorderedFaces[j], i);
		}
	}
	
	renderThread.GetLogger().LogInfoFormat("Reordered Vertices (%i %i):", pReorderedVertices.GetCount(), pVertices.GetCount());
	for(i=0; i<pReorderedVertices.GetCount(); i++){
		renderThread.GetLogger().LogInfoFormat("- Vertex %.4i = %.4i", i, pReorderedVertices[i]);
	}
	
	// python debug
	renderThread.GetLogger().LogInfo("Python:");
	
	renderThread.GetLogger().LogInfoFormat("g = []");
	for(i=0; i<pGroups.GetCount(); i++){
		renderThread.GetLogger().LogInfoFormat("g.append([%i,%i])", pGroups[i].firstFace, pGroups[i].faceCount);
	}
	
	renderThread.GetLogger().LogInfoFormat("f = []");
	for(i=0; i<pGroups.GetCount(); i++){
		const int firstFace = pGroups[i].firstFace;
		const int faceLimit = firstFace + pGroups[i].faceCount;
		for(j=firstFace; j<faceLimit; j++){
			renderThread.GetLogger().LogInfoFormat("f.append([%i,%i,%i,%i])",
				pFaces[j].vertices[0], pFaces[j].vertices[1], pFaces[j].vertices[2], i);
		}
	}
	
	renderThread.GetLogger().LogInfoFormat("rf = []");
	for(i=0; i<pReorderedFaces.GetCount(); i++){
		renderThread.GetLogger().LogInfoFormat("rf.append(%i)", pReorderedFaces[i]);
	}
	
	renderThread.GetLogger().LogInfoFormat("rv = []");
	for(i=0; i<pReorderedVertices.GetCount(); i++){
		renderThread.GetLogger().LogInfoFormat("rv.append(%i)", pReorderedVertices[i]);
	}
}
