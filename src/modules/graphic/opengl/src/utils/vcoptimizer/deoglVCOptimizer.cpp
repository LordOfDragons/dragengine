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
#include <string.h>
#include <stdlib.h>

#include "deoglVCOptimizer.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTLogger.h"

#include <dragengine/common/exceptions.h>



// Class deoglVCOptimizer
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglVCOptimizer::deoglVCOptimizer(){
	int i;
	
	pCacheDecayPower = 1.5f;
	pLastFaceScore = 0.75f;
	pUsageBoostScale = 2.0f;
	pUsageBoostPower = -0.5f;
	
	pCacheSize = 32;
	pCache = new int[pCacheSize + 3];
	for(i=0; i<pCacheSize+3; i++){
		pCache[i] = -1;
	}
	pInvCacheFactor = 1.0f / (float)pCacheSize;
	
	pGroups = NULL;
	pFaces = NULL;
	pVertices = NULL;
	pVertexFaces = NULL;
	pReorderedFaces = NULL;
	pReorderedVertices = NULL;
	pGroupCount = 0;
	pFaceCount = 0;
	pVertexCount = 0;
	pReorderedFaceCount = 0;
	pReorderedVertexCount = 0;
	pNextFaceCheck = 0;
}

deoglVCOptimizer::~deoglVCOptimizer(){
	if(pReorderedFaces){
		delete [] pReorderedFaces;
	}
	if(pReorderedVertices){
		delete [] pReorderedVertices;
	}
	if(pGroups){
		delete [] pGroups;
	}
	if(pFaces){
		delete [] pFaces;
	}
	if(pVertexFaces){
		delete [] pVertexFaces;
	}
	if(pVertices){
		delete [] pVertices;
	}
	if(pCache){
		delete [] pCache;
	}
}



// Management
///////////////

void deoglVCOptimizer::SetVertexCount(int vertexCount){
	if(pReorderedVertices){
		delete pReorderedVertices;
		pReorderedVertices = NULL;
	}
	if(pVertices){
		delete [] pVertices;
		pVertices = NULL;
	}
	pReorderedVertexCount = 0;
	pVertexCount = 0;
	
	if(vertexCount > 0){
		pVertices = new sVertex[vertexCount];
		pReorderedVertices = new int[vertexCount];
		pVertexCount = vertexCount;
	}
}

void deoglVCOptimizer::SetFaceCount(int faceCount){
	if(pReorderedFaces){
		delete [] pReorderedFaces;
		pReorderedFaces = NULL;
	}
	if(pVertexFaces){
		delete [] pVertexFaces;
		pVertexFaces = NULL;
	}
	if(pFaces){
		delete [] pFaces;
		pFaces = NULL;
	}
	pReorderedFaceCount = 0;
	pFaceCount = 0;
	pNextFaceCheck = 0;
	
	if(faceCount > 0){
		pFaces = new sFace[faceCount];
		pVertexFaces = new int[faceCount * 3];
		pReorderedFaces = new int[faceCount];
		pFaceCount = faceCount;
	}
}

void deoglVCOptimizer::SetFaceAt(int face, int vertex1, int vertex2, int vertex3){
	if(face < 0 || face >= pFaceCount){
		DETHROW(deeInvalidParam);
	}
	
	pFaces[face].vertices[0] = vertex1;
	pFaces[face].vertices[1] = vertex2;
	pFaces[face].vertices[2] = vertex3;
}

void deoglVCOptimizer::SetGroupCount(int groupCount){
	if(pGroups){
		delete [] pGroups;
		pGroups = NULL;
	}
	pGroupCount = 0;
	
	if(groupCount > 0){
		pGroups = new sGroup[groupCount];
		pGroupCount = groupCount;
	}
}

void deoglVCOptimizer::SetGroupAt(int group, int firstFace, int faceCount){
	if(group < 0 || group >= pGroupCount){
		DETHROW(deeInvalidParam);
	}
	
	pGroups[group].firstFace = firstFace;
	pGroups[group].faceCount = faceCount;
}



void deoglVCOptimizer::Optimize(){
	pReorderedFaceCount = 0;
	pReorderedVertexCount = 0;
	
	OptimizeFaceOrder();
	OptimizeVertexOrder();
}



void deoglVCOptimizer::OptimizeFaceOrder(){
	if(pReorderedFaceCount != 0){
		DETHROW(deeInvalidParam);
	}
	if(pReorderedVertexCount != 0){
		DETHROW(deeInvalidParam);
	}
	if(pGroupCount == 0){
		DETHROW(deeInvalidParam);
	}
	
	int i, j, k, l, nextFace;
	
	// set faces to not consumed
	for(i=0; i<pFaceCount; i++){
		pFaces[i].consumed = false;
	}
	
	// init the vertex faces
	for(i=0; i<pVertexCount; i++){
		pVertices[i].faceCount = 0;
	}
	
	for(i=0; i<pFaceCount; i++){
		pVertices[pFaces[i].vertices[0]].faceCount++;
		pVertices[pFaces[i].vertices[1]].faceCount++;
		pVertices[pFaces[i].vertices[2]].faceCount++;
	}
	
	if(pVertexCount > 0){
		pVertices[0].firstFace = 0;
		
		for(i=1; i<pVertexCount; i++){
			pVertices[i].firstFace = pVertices[i - 1].firstFace + pVertices[i - 1].faceCount;
		}
	}
	
	// process each group individually
	for(i=0; i<pGroupCount; i++){
		const int faceCount = pGroups[i].faceCount;
		
		if(faceCount > 0){
			const int firstFace = pGroups[i].firstFace;
			const int faceLimit = firstFace + faceCount;
			
			// initialize
			for(j=0; j<pCacheSize+3; j++){
				pCache[j] = -1;
			}
			
			for(j=0; j<pVertexCount; j++){
				pVertices[j].consumed = false;
				pVertices[j].cachePosition = -1;
				pVertices[j].score = 0.0f;
				pVertices[j].faceCount = 0;
			}
			
			for(j=firstFace; j<faceLimit; j++){
				for(k=0; k<3; k++){
					sVertex &vertex = pVertices[pFaces[j].vertices[k]];
					
					pVertexFaces[vertex.firstFace + vertex.faceCount] = j;
					vertex.faceCount++;
				}
			}
			
			for(j=0; j<pVertexCount; j++){
				CalculateVertexScore(j);
			}
			
			for(j=firstFace; j<faceLimit; j++){
				CalculateFaceScore(j);
			}
			
			// sort faces in this group adding them to the face list
			pNextFaceCheck = pReorderedFaceCount;
			bool fullSearch = true;
			
			while(pReorderedFaceCount < faceLimit){
				// find next face to add
				if(fullSearch){
					nextFace = FindBestScoreFace(firstFace, faceLimit);
					fullSearch = false;
					
				}else{
					nextFace = FindBestScoreFaceInCache();
					
					if(nextFace == -1){
						nextFace = FindNextFace(faceLimit);
						
						if(nextFace == -1){
							DETHROW(deeInvalidParam);
						}
					}
				}
				
				// add face
				pReorderedFaces[pReorderedFaceCount++] = nextFace;
				pFaces[nextFace].consumed = true;
				
				// remove the face from the vertices
				for(k=0; k<3; k++){
					sVertex &vertex = pVertices[pFaces[nextFace].vertices[k]];
					const int firstVertexFace = vertex.firstFace;
					const int limitVertexFace = vertex.firstFace + vertex.faceCount;
					
					for(l=firstVertexFace; l<limitVertexFace; l++){
						if(pVertexFaces[l] == nextFace){
							break;
						}
					}
					
					// move the last face index in the list to the the face index to remove. if l is already
					// the last face index in the list nothing bad happens. this way we can avoid using an
					// if statement which does not do anything useful
					pVertexFaces[l] = pVertexFaces[limitVertexFace - 1];
					vertex.faceCount--;
				}
				
				// add vertices to the cache and update the scores
				AddVertexToCache(pFaces[nextFace].vertices[0]);
				AddVertexToCache(pFaces[nextFace].vertices[1]);
				AddVertexToCache(pFaces[nextFace].vertices[2]);
				
				UpdateCacheScores();
			}
			
			if(pReorderedFaceCount != faceLimit){
				DETHROW(deeInvalidParam);
			}
		}
	}
}

void deoglVCOptimizer::OptimizeVertexOrder(){
	if(pReorderedFaceCount != pFaceCount){
		DETHROW(deeInvalidParam);
	}
	if(pReorderedVertexCount != 0){
		DETHROW(deeInvalidParam);
	}
	
	int i, j;
	
	for(i=0; i<pVertexCount; i++){
		pVertices[i].consumed = false;
	}
	
	for(i=0; i<pReorderedFaceCount; i++){
		const sFace &face = pFaces[pReorderedFaces[i]];
		
		for(j=0; j<3; j++){
			const int vertex = face.vertices[j];
			
			if(!pVertices[vertex].consumed){
				if(pReorderedVertexCount == pVertexCount){
					DETHROW(deeInvalidParam);
				}
				
				pVertices[vertex].consumed = true;
				pVertices[vertex].reorderedIndex = pReorderedVertexCount;
				pReorderedVertices[pReorderedVertexCount++] = vertex;
			}
		}
	}
	
	if(pReorderedVertexCount != pVertexCount){
		DETHROW(deeInvalidParam);
	}
}



void deoglVCOptimizer::CalculateVertexScore(int vertex){
	const int faceCount = pVertices[vertex].faceCount;
	float score = -1.0f;
	
	if(faceCount > 0){
		const int cachePosition = pVertices[vertex].cachePosition;
		
		score = 0.0f;
		
		if(cachePosition >= 0){
			if(cachePosition < 3){
				score = pLastFaceScore;
				
			}else{
				if(cachePosition >= pCacheSize + 3){
					DETHROW(deeInvalidParam);
				}
				
				// points for being high in the cache
				score = powf(1.0f - (float)(cachePosition - 3) * pInvCacheFactor, pCacheDecayPower);
			}
		}
		
		// bonus points for having a low number of faces still to use the vertex, so we get rid of lone verts quickly
		score += powf((float)faceCount, pUsageBoostPower) * pUsageBoostScale;
	}
	
	pVertices[vertex].score = score;
}

void deoglVCOptimizer::CalculateFaceScore(int face){
	pFaces[face].score = pVertices[pFaces[face].vertices[0]].score;
	pFaces[face].score += pVertices[pFaces[face].vertices[1]].score;
	pFaces[face].score += pVertices[pFaces[face].vertices[2]].score;
}

void deoglVCOptimizer::AddVertexToCache(int vertex){
	int i, index;
	
	for(index=0; index<pCacheSize+2; index++){
		if(pCache[index] == vertex){
			break;
		}
	}
	
	for(i=index; i>0; i--){
		pCache[i] = pCache[i - 1];
	}
	
	pCache[0] = vertex;
}

void deoglVCOptimizer::UpdateCacheScores(){
	float score, scoreChange;
	int i, j;
	
	for(i=0; i<pCacheSize+3; i++){
		if(pCache[i] != -1){
			sVertex &vertex = pVertices[pCache[i]];
			const int firstFace = vertex.firstFace;
			const int limitFace = vertex.firstFace + vertex.faceCount;
			
			score = vertex.score;
			
			if(i < pCacheSize){
				vertex.cachePosition = i;
				
			}else{
				vertex.cachePosition = -1;
			}
			
			CalculateVertexScore(pCache[i]);
			scoreChange = vertex.score - score;
			
			for(j=firstFace; j<limitFace; j++){
				pFaces[pVertexFaces[j]].score += scoreChange;
			}
		}
	}
	
	pCache[pCacheSize] = -1;
	pCache[pCacheSize + 1] = -1;
	pCache[pCacheSize + 2] = -1;
}

int deoglVCOptimizer::FindBestScoreFaceInCache(){
	int i, j, bestFace = -1;
	float bestScore = -1.0f;
	
	for(i=0; i<pCacheSize; i++){
		if(pCache[i] != -1){
			const sVertex &vertex = pVertices[pCache[i]];
			const int firstFace = vertex.firstFace;
			const int limitFace = vertex.firstFace + vertex.faceCount;
			
			for(j=firstFace; j<limitFace; j++){
				const sFace &face = pFaces[pVertexFaces[j]];
				
				if(!face.consumed && face.score > bestScore){
					bestFace = pVertexFaces[j];
					bestScore = face.score;
				}
			}
		}
	}
	
	return bestFace;
}

int deoglVCOptimizer::FindBestScoreFace(int firstFace, int faceLimit){
	float bestScore = -1.0f;
	int i, bestFace = -1;
	
	for(i=firstFace; i<faceLimit; i++){
		if(!pFaces[i].consumed && pFaces[i].score > bestScore){
			bestFace = i;
			bestScore = pFaces[i].score;
		}
	}
	
	return bestFace;
}

int deoglVCOptimizer::FindNextFace(int faceLimit){
	for(; pNextFaceCheck<faceLimit; pNextFaceCheck++){
		if(!pFaces[pNextFaceCheck].consumed){
			return pNextFaceCheck++;
		}
	}
	
	return -1;
}



void deoglVCOptimizer::DebugPrint(deoglRenderThread& renderThread){
	int i, j;
	
	renderThread.GetLogger().LogInfo("Optimize Vertex Cache:");
	
	renderThread.GetLogger().LogInfoFormat("Groups (%i):", pGroupCount);
	for(i=0; i<pGroupCount; i++){
		renderThread.GetLogger().LogInfoFormat("- Group %.4i = First Face %i, Face Count %i", i, pGroups[i].firstFace, pGroups[i].faceCount);
	}
	
	renderThread.GetLogger().LogInfoFormat("Faces (%i):", pFaceCount);
	for(i=0; i<pGroupCount; i++){
		const int firstFace = pGroups[i].firstFace;
		const int faceLimit = firstFace + pGroups[i].faceCount;
		for(j=firstFace; j<faceLimit; j++){
			renderThread.GetLogger().LogInfoFormat("- Face %.4i = Vertices(%.4i | %.4i | %.4i) Group %i", j, pFaces[j].vertices[0],
				pFaces[j].vertices[1], pFaces[j].vertices[2], i);
		}
	}
	
	renderThread.GetLogger().LogInfoFormat("Reordered Faces (%i):", pReorderedFaceCount);
	for(i=0; i<pGroupCount; i++){
		const int firstFace = pGroups[i].firstFace;
		const int faceLimit = firstFace + pGroups[i].faceCount;
		for(j=firstFace; j<faceLimit; j++){
			renderThread.GetLogger().LogInfoFormat("- Face %.4i = %.4i (Group %i)", i, pReorderedFaces[j], i);
		}
	}
	
	renderThread.GetLogger().LogInfoFormat("Reordered Vertices (%i %i):", pReorderedVertexCount, pVertexCount);
	for(i=0; i<pReorderedVertexCount; i++){
		renderThread.GetLogger().LogInfoFormat("- Vertex %.4i = %.4i", i, pReorderedVertices[i]);
	}
	
	// python debug
	renderThread.GetLogger().LogInfo("Python:");
	
	renderThread.GetLogger().LogInfoFormat("g = []");
	for(i=0; i<pGroupCount; i++){
		renderThread.GetLogger().LogInfoFormat("g.append([%i,%i])", pGroups[i].firstFace, pGroups[i].faceCount);
	}
	
	renderThread.GetLogger().LogInfoFormat("f = []");
	for(i=0; i<pGroupCount; i++){
		const int firstFace = pGroups[i].firstFace;
		const int faceLimit = firstFace + pGroups[i].faceCount;
		for(j=firstFace; j<faceLimit; j++){
			renderThread.GetLogger().LogInfoFormat("f.append([%i,%i,%i,%i])",
				pFaces[j].vertices[0], pFaces[j].vertices[1], pFaces[j].vertices[2], i);
		}
	}
	
	renderThread.GetLogger().LogInfoFormat("rf = []");
	for(i=0; i<pReorderedFaceCount; i++){
		renderThread.GetLogger().LogInfoFormat("rf.append(%i)", pReorderedFaces[i]);
	}
	
	renderThread.GetLogger().LogInfoFormat("rv = []");
	for(i=0; i<pReorderedVertexCount; i++){
		renderThread.GetLogger().LogInfoFormat("rv.append(%i)", pReorderedVertices[i]);
	}
}
