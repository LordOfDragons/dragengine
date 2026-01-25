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

#include "dedaiSpaceGrid.h"
#include "dedaiSpaceGridEdge.h"
#include "dedaiSpaceGridVertex.h"
#include "../dedaiSpace.h"
#include "../../dedaiNavSpace.h"
#include "../../blocker/dedaiNavBlocker.h"
#include "../../heightterrain/dedaiHeightTerrain.h"
#include "../../heightterrain/dedaiHeightTerrainSector.h"
#include "../../heightterrain/dedaiHeightTerrainNavSpace.h"
#include "../../../deDEAIModule.h"
#include "../../../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceEdge.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpace.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpaceEdge.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>



// Structures
///////////////

struct sLink{
	int vertexIndex;
	dedaiSpaceGridVertex *linkedVertex;
};



// Class dedaiSpaceGrid
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceGrid::dedaiSpaceGrid(dedaiSpace &space) :
pSpace(space){
}

dedaiSpaceGrid::~dedaiSpaceGrid(){
	Clear();
}



// Management
///////////////

void dedaiSpaceGrid::AddVertex(const decVector &position){
	pVertices.Add({});
	dedaiSpaceGridVertex &vertex = pVertices.Last();
	vertex.SetIndex(pVertices.GetCount() - 1);
	vertex.SetGrid(this);
	vertex.SetPosition(position);
	vertex.SetFirstEdge(0);
	vertex.SetEdgeCount(0);
}

dedaiSpaceGridVertex *dedaiSpaceGrid::GetVertexClosestTo(const decVector &position, float &distance) const{
	// for the time being brute force
	dedaiSpaceGridVertex *bestVertex = nullptr;
	float bestDistSquared = 0.0f;
	decVector testDirection;
	float testDistSquared;
	
	pVertices.Visit([&](dedaiSpaceGridVertex &v){
		if(!v.GetEnabled()){
			return;
		}
		
		testDirection = v.GetPosition() - position;
		testDistSquared = testDirection * testDirection;
		
		if(!bestVertex || testDistSquared < bestDistSquared){
			bestVertex = &v;
			bestDistSquared = testDistSquared;
		}
	});
	
	if(bestVertex){
		distance = sqrtf(bestDistSquared);
	}
	return bestVertex;
}



void dedaiSpaceGrid::AddEdge(unsigned short vertex1, unsigned short vertex2){
	pEdges.Add({});
	dedaiSpaceGridEdge &edge = pEdges.Last();
	edge.SetGrid(this);
	edge.SetIndex(pEdges.GetCount() - 1);
	edge.SetVertex1(vertex1);
	edge.SetVertex2(vertex2);
	edge.SetTypeNumber1(0);
	edge.SetTypeNumber2(0);
}

int dedaiSpaceGrid::IndexOfEdgeMatching(unsigned short vertex1, unsigned short vertex2) const{
	return pEdges.IndexOfMatching([&](const dedaiSpaceGridEdge &edge){
		return (edge.GetVertex1() == vertex1 && edge.GetVertex2() == vertex2)
			|| (edge.GetVertex1() == vertex2 && edge.GetVertex2() == vertex1);
	});
}



void dedaiSpaceGrid::RemoveAllLinks(){
	if(pLinks.IsEmpty()){
		return;
	}
	
	pVertices.Visit([&](dedaiSpaceGridVertex &v){
		v.SetFirstLink(0);
		v.SetLinkCount(0);
	});
	
	pLinks.SetCountDiscard(0);
	
	// tell the owner links have to be updated the next time
	pSpace.LinksRemoves();
}



dedaiSpaceGridEdge *dedaiSpaceGrid::NearestPoint(const decVector &point, float radius,
decVector &nearestPosition, float &nearestDistSquared, float &nearestLambda) const{
	const float radiusSquared = radius * radius;
	dedaiSpaceGridEdge *nearestEdge = nullptr;
	nearestDistSquared = radiusSquared;
	
	pEdges.Visit([&](dedaiSpaceGridEdge &edge){
		const dedaiSpaceGridVertex &v1 = pVertices[edge.GetVertex1()];
		const dedaiSpaceGridVertex &v2 = pVertices[edge.GetVertex2()];
		
// 		if( ! edge.GetEnabled() ){
// 			return;
// 		}
		if(!v1.GetEnabled() || !v2.GetEnabled()){
			return;
		}
		
		const decVector edgeDirection(v2.GetPosition() - v1.GetPosition());
		const decVector testDirection(point - v1.GetPosition());
		const float edgeLambda = decMath::clamp(
			edgeDirection * testDirection / edgeDirection.LengthSquared(), 0.0f, 1.0f);
		const decVector edgeClosest(v1.GetPosition() + edgeDirection * edgeLambda);
		const float edgeDistanceSquared = (edgeClosest - point).LengthSquared();
		
		if(edgeDistanceSquared > nearestDistSquared){
			return;
		}
		
		nearestEdge = &edge;
		nearestDistSquared = edgeDistanceSquared;
		nearestPosition = edgeClosest;
		nearestLambda = edgeLambda;
	});
	
	return nearestEdge;
}



void dedaiSpaceGrid::InitFromSpace(){
	if(pSpace.GetOwnerNavSpace()){
		pInitFromNavSpace();
		
	}else if(pSpace.GetOwnerHTNavSpace()){
		pInitFromHTNavSpace();
	}
}

void dedaiSpaceGrid::LinkToOtherGrids(){
	const deWorld &engWorld = pSpace.GetParentWorld()->GetWorld();
	const decDMatrix &spaceMatrix = pSpace.GetMatrix();
	const float threshold = pSpace.GetSnapDistance();
	decTList<sLink> links;
	int i, j, k, l;
	
// 	pSpace.GetDEAI().LogInfoFormat( "Linking %p...", this );
	RemoveAllLinks();
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pSpace.GetParentWorld()->GetHeightTerrain();
	if(heightTerrain){
		const int sectorCount = heightTerrain->GetSectors().GetCount();
		
		for(i=0; i<sectorCount; i++){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectors().GetAt(i);
			const int navSpaceCount = sector.GetNavSpaces().GetCount();
			for(j=0; j<navSpaceCount; j++){
				dedaiHeightTerrainNavSpace * const navspace = sector.GetNavSpaces().GetAt(j);
				if(navspace == pSpace.GetOwnerHTNavSpace()){
					continue;
				}
				
				dedaiSpace &space = *navspace->GetSpace();
				dedaiSpaceGrid * const grid = space.GetGrid();
				if(!grid){
					continue;
				}
				
				const decMatrix matrix(spaceMatrix * space.GetInverseMatrix());
				dedaiSpaceGridVertex * const vertices = grid->GetVertices().GetArrayPointer();
				const int vertexCount = grid->GetVertices().GetCount();
				
				// this is now a slow test. each vertex of this navigation grid is tested against all vertices
				// of the other grid. this is an N^2 test which is slow but this could be later on improved by
				// using an octree or some other kind of data structure
				for(k=0; k<pVertices.GetCount(); k++){
					const decVector v1 = matrix * pVertices[k].GetPosition();
					
					for(l=0; l<vertexCount; l++){
						if(v1.IsEqualTo(vertices[l].GetPosition(), threshold)){
							//pSpace.GetDEAI().LogInfoFormat( "Link Found: (ng=%p v=%d) => (ng=%p v=%d)", pSpace, k, navspace, l );
							links.Add({k, vertices + l});
							break;
						}
					}
				}
			}
		}
	}
	
	// navigation spaces
	engWorld.GetNavigationSpaces().Visit([&](deNavigationSpace *engNavSpace){
		dedaiNavSpace * const navspace = (dedaiNavSpace*)engNavSpace->GetPeerAI();
		
		if(navspace && navspace != pSpace.GetOwnerNavSpace()){
			dedaiSpace * const space = navspace->GetSpace();
			dedaiSpaceGrid *navgrid = space->GetGrid();
			
			if(navgrid){
				const decMatrix matrix = (spaceMatrix * space->GetInverseMatrix()).ToMatrix();
				dedaiSpaceGridVertex * const vertices = navgrid->GetVertices().GetArrayPointer();
				const int vertexCount = navgrid->GetVertices().GetCount();
				
				// this is now a slow test. each vertex of this navigation grid is tested against all vertices
				// of the other grid. this is an N^2 test which is slow but this could be later on improved by
				// using an octree or some other kind of data structure
				for(i=0; i<pVertices.GetCount(); i++){
					const decVector v1 = matrix * pVertices[i].GetPosition();
					
					for(j=0; j<vertexCount; j++){
						if(v1.IsEqualTo(vertices[j].GetPosition(), threshold)){
							//pSpace.GetDEAI().LogInfoFormat( "Link Found: (ng=%p v=%i) => (ng=%p v=%i)", pSpace, i, navspace, j );
							links.Add({i, vertices + j});
							break;
						}
					}
				}
			}
		}
	});
	
	pVertices.VisitIndexed([&](int vi, dedaiSpaceGridVertex &vertex){
		vertex.SetFirstLink(pLinks.GetCount());
		
		links.Visit([&](const sLink &link){
			if(link.vertexIndex == vi){
				pLinks.Add(link.linkedVertex);
			}
		});
		
		vertex.SetLinkCount(pLinks.GetCount() - vertex.GetFirstLink());
	});
	
#if 0
	// debug
	for(i=0; i<pCornerCount; i++){
		if(pCorners[i].GetLink() != -1){
			const dedaiNavMeshLink &link = pLinks[pCorners[i].GetLink()];
			
			pSpace.GetDEAI().LogInfoFormat("Debug Link: %i => ns=%p f=%i C=%i", i, link.GetNavMesh()->GetNavSpace(), link.GetFace(), link.GetCorner());
		}
	}
#endif
}

void dedaiSpaceGrid::UpdateBlocking(){
	// enable all vertices
	pVertices.Visit([&](dedaiSpaceGridVertex &v){
		v.SetEnabled(true);
	});
	
	// process overlapping blockers
	if(pSpace.GetParentWorld()){
		// create list of splitter volumes
		decConvexVolumeList splitterList;
		pSpace.AddBlockerSplitters(splitterList);
		pSpace.AddSpaceBlockerSplitters(splitterList);
		
		// disable vertices inside any of the splitter volumes
		if(splitterList.GetVolumeCount() > 0){
			pDisableVertices(splitterList);
		}
	}
}

void dedaiSpaceGrid::Clear(){
	pLinks.SetCountDiscard(0);
	pEdges.SetCountDiscard(0);
	pVertices.SetCountDiscard(0);
	pVertexEdges.SetCountDiscard(0);
}



void dedaiSpaceGrid::UpdateDDSSpaceShape(){
	deDebugDrawerShape &ddsShape = *pSpace.GetDDSSpace();
	deDebugDrawerShape &ddsNormals = *pSpace.GetDDSNormals();
	deDebugDrawerShape &ddsMismatching = *pSpace.GetDDSMismatching();
	
	ddsShape.RemoveAllFaces();
	ddsShape.GetShapeList().RemoveAll();
	
	ddsNormals.RemoveAllFaces();
	ddsNormals.GetShapeList().RemoveAll();
	
	ddsMismatching.RemoveAllFaces();
	ddsMismatching.GetShapeList().RemoveAll();
	
	pEdges.Visit([&](dedaiSpaceGridEdge &edge){
		const dedaiSpaceGridVertex &vertex1 = pVertices[edge.GetVertex1()];
		const dedaiSpaceGridVertex &vertex2 = pVertices[edge.GetVertex2()];
		if(!vertex1.GetEnabled() || !vertex2.GetEnabled()){
			return;
		}
		
		const decVector &position1 = vertex1.GetPosition();
		const decVector &position2 = vertex2.GetPosition();
		
		auto ddsFace = deDebugDrawerShapeFace::Ref::New();
		ddsFace->AddVertex(position1);
		ddsFace->AddVertex(position2);
		ddsFace->AddVertex(position2);
		ddsShape.AddFace(std::move(ddsFace));
	});
}



// Private Functions
//////////////////////

void dedaiSpaceGrid::pInitFromNavSpace(){
	const deNavigationSpace &engNavSpace = pSpace.GetOwnerNavSpace()->GetNavigationSpace();
	const int edgeCount = engNavSpace.GetEdges().GetCount();
	
	// make sure all is cleared first
	Clear();
	
	// adjust arrays
	pVertexEdges.EnlargeCapacity(edgeCount * 2);
	pVertices.SetAll(engNavSpace.GetVertices().GetCount(), {});
	pEdges.SetAll(edgeCount, {});
	
	// init vertices and edges
	pVertices.VisitIndexed([&](int i, dedaiSpaceGridVertex &vertex){
		vertex.SetGrid(this);
		vertex.SetIndex((unsigned short)i);
		vertex.SetPosition(engNavSpace.GetVertices()[i]);
		vertex.SetFirstEdge(0);
		vertex.SetEdgeCount(0);
	});
	
	engNavSpace.GetEdges().VisitIndexed([&](int i, const deNavigationSpaceEdge &edge){
		if(edge.GetVertex1() == edge.GetVertex2()){
			return;
		}
		
		pEdges[i].SetGrid(this);
		pEdges[i].SetIndex((unsigned short)i);
		pEdges[i].SetVertex1(edge.GetVertex1());
		pEdges[i].SetVertex2(edge.GetVertex2());
		pEdges[i].SetTypeNumber1(pSpace.AddTypeMapping(edge.GetType1()));
		pEdges[i].SetTypeNumber2(pSpace.AddTypeMapping(edge.GetType2()));
		pEdges[i].SetLength((engNavSpace.GetVertices()[edge.GetVertex2()]
			- engNavSpace.GetVertices()[edge.GetVertex1()]).Length());
	});
	
	pVertices.VisitIndexed([&](int i, dedaiSpaceGridVertex &vertex){
		vertex.SetFirstEdge(pVertexEdges.GetCount());
		int vertexEdgeCount = 0;
		pEdges.VisitIndexed([&](int j, dedaiSpaceGridEdge &edge){
			if(edge.GetVertex1() == i || edge.GetVertex2() == i){
				pVertexEdges.Add((unsigned short)j);
				vertexEdgeCount++;
			}
		});
		vertex.SetEdgeCount(vertexEdgeCount);
	});
	
	// debug
#if 0
	printf("vertex count %i\n", pVertexCount);
	for(i=0; i<pVertexCount; i++){
		printf("%i) p=(%f,%f,%f) f=%i ec=%i\n", i, pVertices[i].GetPosition().x, pVertices[i].GetPosition().y,
			pVertices[i].GetPosition().z, pVertices[i].GetFirstEdge(), pVertices[i].GetEdgeCount());
	}
	printf("edge count %i\n", pEdgeCount);
	for(i=0; i<pEdgeCount; i++){
		printf("%i) v1=%i v2=%i t1=%i t2=%i l=%f\n", i, pEdges[i].GetVertex1(), pEdges[i].GetVertex2(),
			pEdges[i].GetTypeNumber1(), pEdges[i].GetTypeNumber2(), pEdges[i].GetLength());
	}
	printf("vertex edge count %i\n", pVertexEdgeCount);
	for(i=0; i<pVertexEdgeCount; i++){
		printf("%i) e=%i\n", i, pVertexEdges[i]);
	}
#endif
}

void dedaiSpaceGrid::pInitFromHTNavSpace(){
	const dedaiHeightTerrainNavSpace &navspace = *pSpace.GetOwnerHTNavSpace();
	const dedaiHeightTerrainSector &sector = navspace.GetSector();
	const decTList<float> &heights = sector.GetHeights();
	const deHeightTerrainNavSpace &engNavSpace = navspace.GetNavigationSpace();
	
	const deHeightTerrain &engHT = sector.GetHeightTerrain().GetHeightTerrain();
	const int imageDim = engHT.GetSectorResolution();
	const float posScale = (float)engHT.GetSectorSize() / (float)(imageDim - 1);
	const float posOffset = (float)engHT.GetSectorSize() * 0.5f;
	
	const decTList<deHeightTerrainNavSpaceEdge> &engEdges = engNavSpace.GetEdges();
	const int edgeCount = engEdges.GetCount();
	const decTList<unsigned int> &vertices = navspace.GetVertices();
	
	// check for obvious bugs
	if(navspace.GetEdges().IsEmpty() || vertices.IsEmpty()){
		DETHROW(deeInvalidParam);
	}
	
	// make sure all is cleared first
	Clear();
	
	// adjust arrays
	pVertexEdges.EnlargeCapacity(edgeCount * 2);
	
	pVertices.SetAll(vertices.GetCount(), {});
	pVertices.VisitIndexed([&](int i, dedaiSpaceGridVertex &vertex){
		vertex.SetGrid(this);
		vertex.SetIndex((unsigned short)i);
		vertex.SetPosition(decVector(
			posScale * (float)(vertices[i] % imageDim) - posOffset,
			heights[vertices[i]],
			posOffset - posScale * (float)(vertices[i] / imageDim)));
		vertex.SetFirstEdge(0);
		vertex.SetEdgeCount(0);
	});
	
	pEdges.SetAll(edgeCount, {});
	
	// init vertices and edges
	navspace.GetEdges().VisitIndexed([&](int i, const dedaiHeightTerrainNavSpace::sEdge &edge){
		if(edge.vertex1 == edge.vertex2){
			return;
		}
		
		const deHeightTerrainNavSpaceEdge &engEdge = engEdges[i];
		pEdges[i].SetGrid(this);
		pEdges[i].SetIndex((unsigned short)i);
		pEdges[i].SetVertex1(edge.vertex1);
		pEdges[i].SetVertex2(edge.vertex2);
		pEdges[i].SetTypeNumber1(pSpace.AddTypeMapping(engEdge.GetType1()));
		pEdges[i].SetTypeNumber2(pSpace.AddTypeMapping(engEdge.GetType2()));
		pEdges[i].SetLength((pVertices[edge.vertex2].GetPosition()
			- pVertices[edge.vertex1].GetPosition()).Length());
	});
	
	pVertices.VisitIndexed([&](int i, dedaiSpaceGridVertex &vertex){
		vertex.SetFirstEdge(pVertexEdges.GetCount());
		int vertexEdgeCount = 0;
		pEdges.VisitIndexed([&](int j, dedaiSpaceGridEdge &edge){
			if(edge.GetVertex1() == i || edge.GetVertex2() == i){
				pVertexEdges.Add((unsigned short)j);
				vertexEdgeCount++;
			}
		});
		vertex.SetEdgeCount(vertexEdgeCount);
	});
}



void dedaiSpaceGrid::pDisableVertices(const decConvexVolumeList &list){
	pVertices.Visit([&](dedaiSpaceGridVertex &vertex){
		const decVector &position = vertex.GetPosition();
		
		if(!list.GetVolumes().AllMatching([&](const decConvexVolume &volume){
			return volume.GetFaces().HasMatching([&](const decConvexVolumeFace &volumeFace){
				const decVector &planePosition = volume.GetVertexAt(volumeFace.GetVertices().First());
				const decVector &planeNormal = volumeFace.GetNormal();
				const float planeDist = planeNormal * planePosition;
				return planeNormal * position > planeDist;
			});
		})){
			vertex.SetEnabled(false);
		}
	});
}
