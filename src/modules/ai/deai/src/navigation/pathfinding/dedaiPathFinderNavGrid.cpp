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
#include <string.h>

#include "dedaiPathFinderNavGrid.h"
#include "../../deDEAIModule.h"
#include "../../navigation/dedaiNavigator.h"
#include "../../navigation/costs/dedaiCostTable.h"
#include "../../navigation/spaces/dedaiSpace.h"
#include "../../navigation/spaces/grid/dedaiSpaceGrid.h"
#include "../../navigation/spaces/grid/dedaiSpaceGridEdge.h"
#include "../../navigation/spaces/grid/dedaiSpaceGridVertex.h"
#include "../layer/dedaiLayer.h"
#include "../../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/world/deWorld.h>



// #define DEBUG 1

// Class dedaiPathFinderNavGrid
/////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiPathFinderNavGrid::dedaiPathFinderNavGrid(){
	pWorld = NULL;
	pNavigator = NULL;
	pPathPoints = NULL;
	pPathPointCount = 0;
	pPathPointSize = 0;
}

dedaiPathFinderNavGrid::~dedaiPathFinderNavGrid(){
	if(pPathPoints){
		delete [] pPathPoints;
	}
}



// Management
///////////////

void dedaiPathFinderNavGrid::SetWorld(dedaiWorld *world){
	pWorld = world;
}

void dedaiPathFinderNavGrid::SetNavigator(dedaiNavigator *navigator){
	pNavigator = navigator;
}

void dedaiPathFinderNavGrid::SetStartPoint(const decDVector &point){
	pStartPoint = point;
}

void dedaiPathFinderNavGrid::SetEndPoint(const decDVector &point){
	pEndPoint = point;
}



void dedaiPathFinderNavGrid::FindPath(){
	if(! pNavigator || ! pWorld){
		return;
	}
	
	try{
		pFindVertexPath();
		pClearLists();
		
	}catch(const deException &){
		pClearLists();
		throw;
	}
}



void dedaiPathFinderNavGrid::AddPathPoint(const decDVector &point){
	if(pPathPointCount == pPathPointSize){
		int newSize = pPathPointCount + 10;
		decDVector *newArray = new decDVector[newSize];
		if(pPathPoints){
			memcpy(newArray, pPathPoints, sizeof(decDVector) * pPathPointCount);
			delete [] pPathPoints;
		}
		pPathPoints = newArray;
		pPathPointSize = newSize;
	}
	
	pPathPoints[pPathPointCount++] = point;
}

void dedaiPathFinderNavGrid::RemoveAllPathPoints(){
	pPathPointCount = 0;
}



// Private Functions
//////////////////////

void dedaiPathFinderNavGrid::pClearLists(){
	int i, count;
	
	count = pListClosed.GetCount();
	for(i=0; i<count; i++){
		((dedaiSpaceGridVertex*)pListClosed.GetAt(i))->ClearPath();
	}
	pListClosed.RemoveAll();
	
	count = pListOpen.GetCount();
	for(i=0; i<count; i++){
		((dedaiSpaceGridVertex*)pListOpen.GetAt(i))->ClearPath();
	}
	pListOpen.RemoveAll();
}

void dedaiPathFinderNavGrid::pFindVertexPath(){
	const float blockingCost = pNavigator->GetNavigator().GetBlockingCost();
	dedaiSpaceGridVertex *testVertex, *nextVertex;
	float fixCost, costPerMeter;
	float distance = 0.0f;
	float gcost = 0.0f;
	int v, vertexCount;
	int l, linkCount;
	int e, endEdge;
	int typeNumber;
	
	pPathVertices.RemoveAll();
	RemoveAllPathPoints();
	
	pStartVertex = pNavigator->GetLayer()->GetGridVertexClosestTo(pStartPoint, distance);
	pEndVertex = pNavigator->GetLayer()->GetGridVertexClosestTo(pEndPoint, distance);
	
#ifdef DEBUG
	pWorld->GetDEAI().LogInfo("Find Path:");
	pWorld->GetDEAI().LogInfoFormat("   Start: ng=%p v=%i", pStartVertex?pStartVertex->GetGrid():NULL, pStartVertex?(int)pStartVertex->GetIndex():-1);
	pWorld->GetDEAI().LogInfoFormat("   End: ng=%p v=%i", pEndVertex?pEndVertex->GetGrid():NULL, pEndVertex?(int)pEndVertex->GetIndex():-1);
#endif
	
	if(pEndVertex != pStartVertex){
		pListOpen.Add(pStartVertex);
		pStartVertex->SetPathParent(NULL);
		pStartVertex->SetPathCostG(0.0f);
		pStartVertex->SetPathCostH((pEndVertex->GetPosition() - pStartVertex->GetPosition()).Length());
		pStartVertex->SetPathCostF(pStartVertex->GetPathCostH());
		pStartVertex->SetPathType(dedaiSpaceGridVertex::epftOpen);
#ifdef DEBUG
		pWorld->GetDEAI().LogInfoFormat("   Open Add Vertex: ng=%p v=%i", pStartVertex->GetGrid(), pStartVertex->GetIndex());
#endif
		testVertex = pStartVertex;
		
		while(testVertex){
#ifdef DEBUG
			pWorld->GetDEAI().LogInfoFormat("   Testing Vertex: ng=%p v=%i", testVertex->GetGrid(), testVertex->GetIndex());
#endif
			const dedaiSpaceGrid &navgrid = *testVertex->GetGrid();
			const unsigned short * const vertexEdges = navgrid.GetVertexEdges();
			dedaiSpaceGridVertex * const vertices = navgrid.GetVertices();
			const dedaiSpaceGridEdge * const edges = navgrid.GetEdges();
			dedaiSpaceGridVertex ** const links = navgrid.GetLinks();
			
			// add linked vertices to the open list if free. the idea is rather simple here. since the linked
			// vertices are at the exact same location in space as the test vertex is we can add them to the
			// open list with the same values (including the parent) as the test vertex has. this way they
			// will be tested later on too
			linkCount = testVertex->GetLinkCount();
			
			for(l=0; l<linkCount; l++){
				nextVertex = links[testVertex->GetFirstLink() + l];
				if(! nextVertex->GetEnabled()){
					continue;
				}
				
				if(nextVertex->GetPathType() == dedaiSpaceGridVertex::epftFree){
					nextVertex->SetPathParent(testVertex->GetPathParent());
					nextVertex->SetPathCostG(testVertex->GetPathCostG());
					nextVertex->SetPathCostH(testVertex->GetPathCostH());
					nextVertex->SetPathCostF(testVertex->GetPathCostF());
					
					pListOpen.Add(nextVertex);
					nextVertex->SetPathType(dedaiSpaceGridVertex::epftOpen);
#ifdef DEBUG
					pWorld->GetDEAI().LogInfoFormat("   Open Add Vertex (Linked): ng=%p v=%i (p=%i c=(%g,%g,%g))",
						nextVertex->GetGrid(), nextVertex->GetIndex(), testVertex->GetIndex(), nextVertex->GetPathCostF(),
						nextVertex->GetPathCostG(), nextVertex->GetPathCostH());
#endif
					
				}else if(nextVertex->GetPathType() == dedaiSpaceGridVertex::epftOpen){
					if(testVertex->GetPathCostG() < nextVertex->GetPathCostG()){
						nextVertex->SetPathParent(testVertex->GetPathParent());
						nextVertex->SetPathCostG(testVertex->GetPathCostG());
						nextVertex->SetPathCostF(testVertex->GetPathCostG() + nextVertex->GetPathCostH());
#ifdef DEBUG
						pWorld->GetDEAI().LogInfoFormat("   Improve Parent Path (Linked): ng=%p v=%i (p=%i c=(%g,%g,%g))",
							nextVertex->GetGrid(), nextVertex->GetIndex(), testVertex->GetIndex(), nextVertex->GetPathCostF(),
							nextVertex->GetPathCostG(), nextVertex->GetPathCostH());
#endif
					}
				}
			}
			
			// test edges inside the grid
			endEdge = testVertex->GetFirstEdge() + testVertex->GetEdgeCount();
			
			for(e=testVertex->GetFirstEdge(); e<endEdge; e++){
				const dedaiSpaceGridEdge &edge = edges[vertexEdges[e]];
				
				// determine the next vertex to check
				if(edge.GetVertex1() == testVertex->GetIndex()){
					nextVertex = vertices + edge.GetVertex2();
					typeNumber = edge.GetTypeNumber1();
					
				}else{
					nextVertex = vertices + edge.GetVertex1();
					typeNumber = edge.GetTypeNumber2();
				}
				
				// if disabled ignore it
				if(! nextVertex->GetEnabled()){
					continue;
				}
				
				// calculate costs only if the vertex is not on the closed list
				if(nextVertex->GetPathType() != dedaiSpaceGridVertex::epftClosed){
					pNavigator->GetCostParametersFor(typeNumber, fixCost, costPerMeter);
					gcost = testVertex->GetPathCostG();
					gcost += fixCost + costPerMeter * edge.GetLength();
				}
				
				// add it to the list if not on one already
				if(nextVertex->GetPathType() == dedaiSpaceGridVertex::epftFree){
					nextVertex->SetPathParent(testVertex);
					nextVertex->SetPathCostG(gcost);
					nextVertex->SetPathCostH((pEndVertex->GetPosition() - testVertex->GetPosition()).Length());
					nextVertex->SetPathCostF(gcost + nextVertex->GetPathCostH());
					// add to open list if the cost is not larger than the blocking cost
					if(nextVertex->GetPathCostF() < blockingCost){
						pListOpen.Add(nextVertex);
						nextVertex->SetPathType(dedaiSpaceGridVertex::epftOpen);
#ifdef DEBUG
						pWorld->GetDEAI().LogInfoFormat("   Open Add Vertex: ng=%p v=%i (p=%i c=(%g,%g,%g) t=%i)",
							nextVertex->GetGrid(), nextVertex->GetIndex(), testVertex->GetIndex(), nextVertex->GetPathCostF(),
							nextVertex->GetPathCostG(), nextVertex->GetPathCostH(), typeNumber);
#endif
					// if the cost is larger than the blocking cost this edge can not be crossed. in this case
					// add the vertex to the closed list so it is not tested anymore in the future
					}else{
						pListClosed.Add(nextVertex);
						nextVertex->SetPathType(dedaiSpaceGridVertex::epftClosed);
#ifdef DEBUG
						pWorld->GetDEAI().LogInfoFormat("   Blocking: Closed Add Vertex: ng=%p v=%i (p=%i c=(%g,%g,%g) t=%i)",
							nextVertex->GetGrid(), nextVertex->GetIndex(), testVertex->GetIndex(), nextVertex->GetPathCostF(),
							nextVertex->GetPathCostG(), nextVertex->GetPathCostH(), typeNumber);
#endif
					}
					
				}else if(nextVertex->GetPathType() == dedaiSpaceGridVertex::epftOpen){
					if(gcost < nextVertex->GetPathCostG()){
						nextVertex->SetPathParent(testVertex);
						nextVertex->SetPathCostG(gcost);
						nextVertex->SetPathCostF(gcost + nextVertex->GetPathCostH());
#ifdef DEBUG
						pWorld->GetDEAI().LogInfoFormat("   Improve Parent Path: ng=%p v=%i (p=%i c=(%g,%g,%g) t=%i)",
							nextVertex->GetGrid(), nextVertex->GetIndex(), testVertex->GetIndex(), nextVertex->GetPathCostF(),
							nextVertex->GetPathCostG(), nextVertex->GetPathCostH(), typeNumber);
#endif
					}
				}
			}
			
			pListOpen.RemoveFrom(pListOpen.IndexOf(testVertex));
#ifdef DEBUG
			pWorld->GetDEAI().LogInfoFormat("   Open Remove Vertex: ng=%p v=%i (p=%i c=(%g,%g,%g))", testVertex->GetGrid(),
				testVertex->GetIndex(), testVertex->GetPathParent()?testVertex->GetPathParent()->GetIndex():-1,
				testVertex->GetPathCostF(), testVertex->GetPathCostG(), testVertex->GetPathCostH());
#endif
			pListClosed.Add(testVertex);
#ifdef DEBUG
			pWorld->GetDEAI().LogInfoFormat("   Closed Add Vertex: ng=%p v=%i (p=%i c=(%g,%g,%g))", testVertex->GetGrid(),
				testVertex->GetIndex(), testVertex->GetPathParent()?testVertex->GetPathParent()->GetIndex():-1,
				testVertex->GetPathCostF(), testVertex->GetPathCostG(), testVertex->GetPathCostH());
#endif
			
			if(testVertex == pEndVertex){
				break;
			}
			
			vertexCount = pListOpen.GetCount();
			if(vertexCount == 0){
				testVertex = NULL;
				
			}else{
				testVertex = (dedaiSpaceGridVertex*)pListOpen.GetAt(vertexCount - 1);
				for(v=vertexCount-2; v>=0; v--){
					nextVertex = (dedaiSpaceGridVertex*)pListOpen.GetAt(v);
					
					if(nextVertex->GetPathCostF() < testVertex->GetPathCostF()){
						testVertex = nextVertex;
					}
				}
			}
			
		}
		
#ifdef DEBUG
		pWorld->GetDEAI().LogInfo("   Debug:");
		pWorld->GetDEAI().LogInfo("      Open List:");
		for(e=0; e<pListOpen.GetCount(); e++){
			testVertex = (dedaiSpaceGridVertex*)pListOpen.GetAt(e);
			pWorld->GetDEAI().LogInfoFormat("         Vertex: ng=%p v=%i p=%i c=(%g,%g,%g)", testVertex->GetGrid(),
				testVertex->GetIndex(), testVertex->GetPathParent()?testVertex->GetPathParent()->GetIndex():-1,
				testVertex->GetPathCostF(), testVertex->GetPathCostG(), testVertex->GetPathCostH());
		}
		pWorld->GetDEAI().LogInfo("      Closed List:");
		for(e=0; e<pListClosed.GetCount(); e++){
			testVertex = (dedaiSpaceGridVertex*)pListClosed.GetAt(e);
			pWorld->GetDEAI().LogInfoFormat("         Vertex: ng=%p v=%i p=%i c=(%g,%g,%g)", testVertex->GetGrid(),
				testVertex->GetIndex(), testVertex->GetPathParent()?testVertex->GetPathParent()->GetIndex():-1,
				testVertex->GetPathCostF(), testVertex->GetPathCostG(), testVertex->GetPathCostH());
		}
#endif
	}
	
	if(pListClosed.GetCount() > 0){
		testVertex = (dedaiSpaceGridVertex*)pListClosed.GetAt(pListClosed.GetCount() - 1);
		
		// if the last vertex in the closed list is not the end vertex then there is no path to get
		// to the end vertex using the current configuration. in this case look for the vertex with
		// the smallest f-cost value and use this as the end vertex. this at last yields a path
		// which gets us the closest to the vertex
		//if( testVertex != pEndVertex ){
		//}
		
		if(testVertex == pEndVertex){
			vertexCount = 0;
			nextVertex = testVertex;
			while(nextVertex){
				vertexCount++;
				nextVertex = nextVertex->GetPathParent();
				pPathVertices.Add(NULL);
			}
			
			for(vertexCount--; vertexCount>=0; vertexCount--){
				pPathVertices.SetAt(vertexCount, testVertex);
				testVertex = testVertex->GetPathParent();
			}
		}
	}
#ifdef DEBUG
	pWorld->GetDEAI().LogInfo("      Path Vertices:");
	for(v=0; v<pPathVertices.GetCount(); v++){
		testVertex = (dedaiSpaceGridVertex*)pPathVertices.GetAt(v);
		pWorld->GetDEAI().LogInfoFormat("         Vertex: ng=%p v=%i p=%i c=(%g,%g,%g)", testVertex->GetGrid(),
			testVertex->GetIndex(), testVertex->GetPathParent()?testVertex->GetPathParent()->GetIndex():-1,
			testVertex->GetPathCostF(), testVertex->GetPathCostG(), testVertex->GetPathCostH());
	}
#endif
	
	const int count = pPathVertices.GetCount();
	if(count == 0){
		return;
	}
	
	for(v=0; v<count; v++){
		testVertex = (dedaiSpaceGridVertex*)pPathVertices.GetAt(v);
		AddPathPoint(testVertex->GetGrid()->GetSpace().GetMatrix() * testVertex->GetPosition());
	}
	
	if(! pPathPoints[count - 1].IsEqualTo(pEndPoint)){
		AddPathPoint(pEndPoint);
	}
}
