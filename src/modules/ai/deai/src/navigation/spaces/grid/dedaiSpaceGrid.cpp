/* 
 * Drag[en]gine AI Module
 *
 * Copyright (C) 2020, Roland Plüss (roland@rptd.ch)
 * 
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License 
 * as published by the Free Software Foundation; either 
 * version 2 of the License, or (at your option) any later 
 * version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <string.h>

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

dedaiSpaceGrid::dedaiSpaceGrid( dedaiSpace &space ) :
pSpace( space ),

pVertexEdges( NULL ),
pVertexEdgeCount( 0 ),
pVertexEdgeSize( 0 ),

pVertices( NULL ),
pVertexCount( 0 ),
pVertexSize( 0 ),

pEdges( NULL ),
pEdgeCount( 0 ),
pEdgeSize( 0 ),

pLinks( NULL ),
pLinkCount( 0 ),
pLinkSize( 0 ){
}

dedaiSpaceGrid::~dedaiSpaceGrid(){
	Clear();
	
	if( pLinks ){
		delete [] pLinks;
	}
	if( pEdges ){
		delete [] pEdges;
	}
	if( pVertices ){
		delete [] pVertices;
	}
	if( pVertexEdges ){
		delete [] pVertexEdges;
	}
}



// Management
///////////////

unsigned short dedaiSpaceGrid::GetVertexEdgeAt( int index ) const{
	if( index < 0 || index >= pVertexEdgeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pVertexEdges[ index ];
}

void dedaiSpaceGrid::AddVertexEdge( unsigned short edge ){
	if( pVertexEdgeCount == pVertexEdgeSize ){
		int newSize = pVertexEdgeSize + 10;
		unsigned short *newArray = new unsigned short[ newSize ];
		if( pVertexEdges ){
			if( pVertexEdgeSize > 0 ){
				memcpy( newArray, pVertexEdges, sizeof( unsigned short ) * pVertexEdgeSize );
			}
			delete [] pVertexEdges;
		}
		pVertexEdges = newArray;
		pVertexEdgeSize = newSize;
	}
	
	pVertexEdges[ pVertexEdgeCount++ ] = edge;
}



dedaiSpaceGridVertex &dedaiSpaceGrid::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pVertices[ index ];
}

void dedaiSpaceGrid::AddVertex( const decVector &position ){
	if( pVertexCount == pVertexSize ){
		int newSize = pVertexSize + 10;
		dedaiSpaceGridVertex *newArray = new dedaiSpaceGridVertex[ newSize ];
		if( pVertices ){
			if( pVertexSize > 0 ){
				memcpy( newArray, pVertices, sizeof( dedaiSpaceGridVertex ) * pVertexSize );
			}
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = newSize;
	}
	
	pVertices[ pVertexCount ].SetIndex( pVertexCount );
	pVertices[ pVertexCount ].SetGrid( this );
	pVertices[ pVertexCount ].SetPosition( position );
	pVertices[ pVertexCount ].SetFirstEdge( 0 );
	pVertices[ pVertexCount ].SetEdgeCount( 0 );
	pVertexCount++;
}

dedaiSpaceGridVertex *dedaiSpaceGrid::GetVertexClosestTo( const decVector &position, float &distance ) const{
	// for the time being brute force
	dedaiSpaceGridVertex *bestVertex = NULL;
	float bestDistSquared = 0.0f;
	decVector testDirection;
	float testDistSquared;
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( ! pVertices[ i ].GetEnabled() ){
			continue;
		}
		
		testDirection = pVertices[ i ].GetPosition() - position;
		testDistSquared = testDirection * testDirection;
		
		if( ! bestVertex || testDistSquared < bestDistSquared ){
			bestVertex = pVertices + i;
			bestDistSquared = testDistSquared;
		}
	}
	
	if( bestVertex ){
		distance = sqrtf( bestDistSquared );
	}
	return bestVertex;
}



dedaiSpaceGridEdge &dedaiSpaceGrid::GetEdgeAt( int index ) const{
	if( index < 0 || index >= pEdgeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEdges[ index ];
}

void dedaiSpaceGrid::AddEdge( unsigned short vertex1, unsigned short vertex2 ){
	if( pEdgeCount == pEdgeSize ){
		int newSize = pEdgeSize + 10;
		dedaiSpaceGridEdge *newArray = new dedaiSpaceGridEdge[ newSize ];
		if( pEdges ){
			if( pEdgeSize > 0 ){
				memcpy( newArray, pEdges, sizeof( dedaiSpaceGridEdge ) * pEdgeSize );
			}
			delete [] pEdges;
		}
		pEdges = newArray;
		pEdgeSize = newSize;
	}
	
	pEdges[ pEdgeCount ].SetGrid( this );
	pEdges[ pEdgeCount ].SetIndex( pEdgeCount );
	pEdges[ pEdgeCount ].SetVertex1( vertex1 );
	pEdges[ pEdgeCount ].SetVertex2( vertex2 );
	pEdges[ pEdgeCount ].SetTypeNumber1( 0 );
	pEdges[ pEdgeCount ].SetTypeNumber2( 0 );
	pEdgeCount++;
}

int dedaiSpaceGrid::IndexOfEdgeMatching( unsigned short vertex1, unsigned short vertex2 ) const{
	int i;
	
	for( i=0; i<pEdgeCount; i++ ){
		if( ( ( pEdges[ i ].GetVertex1() == vertex1 ) && ( pEdges[ i ].GetVertex2() == vertex2 ) )
		||  ( ( pEdges[ i ].GetVertex1() == vertex2 ) && ( pEdges[ i ].GetVertex2() == vertex1 ) ) ){
			return i;
		}
	}
	
	return -1;
}



dedaiSpaceGridVertex *dedaiSpaceGrid::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pLinks[ index ];
}

void dedaiSpaceGrid::AddLink( dedaiSpaceGridVertex *vertex ){
	if( pLinkCount == pLinkSize ){
		int newSize = pLinkSize + 10;
		dedaiSpaceGridVertex **newArray = new dedaiSpaceGridVertex*[ newSize ];
		if( pLinks ){
			if( pLinkSize > 0 ){
				memcpy( newArray, pLinks, sizeof( dedaiSpaceGridVertex* ) * pLinkSize );
			}
			delete [] pLinks;
		}
		pLinks = newArray;
		pLinkSize = newSize;
	}
	
	pLinks[ pLinkCount++ ] = vertex;
}

void dedaiSpaceGrid::RemoveAllLinks(){
	if( pLinkCount == 0 ){
		return;
	}
	
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		pVertices[ i ].SetFirstLink( 0 );
		pVertices[ i ].SetLinkCount( 0 );
	}
	
	pLinkCount = 0;
	
	// tell the owner links have to be updated the next time
	pSpace.LinksRemoves();
}



dedaiSpaceGridEdge *dedaiSpaceGrid::NearestPoint( const decVector &point, float radius,
decVector &nearestPosition, float &nearestDistSquared, float &nearestLambda ) const{
	const float radiusSquared = radius * radius;
	dedaiSpaceGridEdge *nearestEdge = NULL;
	nearestDistSquared = radiusSquared;
	int i;
	
	for( i=0; i<pEdgeCount; i++ ){
		const dedaiSpaceGridEdge &edge = pEdges[ i ];
		const dedaiSpaceGridVertex &v1 = pVertices[ edge.GetVertex1() ];
		const dedaiSpaceGridVertex &v2 = pVertices[ edge.GetVertex2() ];
		
// 		if( ! edge.GetEnabled() ){
// 			return;
// 		}
		if( ! v1.GetEnabled() || ! v2.GetEnabled() ){
			continue;
		}
		
		const decVector edgeDirection( v2.GetPosition() - v1.GetPosition() );
		const decVector testDirection( point - v1.GetPosition() );
		const float edgeLambda = decMath::clamp(
			edgeDirection * testDirection / edgeDirection.LengthSquared(), 0.0f, 1.0f );
		const decVector edgeClosest( v1.GetPosition() + edgeDirection * edgeLambda );
		const float edgeDistanceSquared = ( edgeClosest - point ).LengthSquared();
		
		if( edgeDistanceSquared > nearestDistSquared ){
			continue;
		}
		
		nearestEdge = pEdges + i;
		nearestDistSquared = edgeDistanceSquared;
		nearestPosition = edgeClosest;
		nearestLambda = edgeLambda;
	}
	
	return nearestEdge;
}



void dedaiSpaceGrid::InitFromSpace(){
	if( pSpace.GetOwnerNavSpace() ){
		pInitFromNavSpace();
		
	}else if( pSpace.GetOwnerHTNavSpace() ){
		pInitFromHTNavSpace();
	}
}

void dedaiSpaceGrid::LinkToOtherGrids(){
	const deWorld &engWorld = pSpace.GetParentWorld()->GetWorld();
	const decDMatrix &spaceMatrix = pSpace.GetMatrix();
	const float threshold = pSpace.GetSnapDistance();
	sLink *links = NULL, *links2;
	int linkCount = 0;
	int i, j, k, l;
	
// 	pSpace.GetDEAI().LogInfoFormat( "Linking %p...", this );
	RemoveAllLinks();
	
	try{
		// height terrain navspaces
		const dedaiHeightTerrain * const heightTerrain = pSpace.GetParentWorld()->GetHeightTerrain();
		if( heightTerrain ){
			const int sectorCount = heightTerrain->GetSectorCount();
			
			for( i=0; i<sectorCount; i++ ){
				dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
				const int navSpaceCount = sector.GetNavSpaceCount();
				for( j=0; j<navSpaceCount; j++ ){
					dedaiHeightTerrainNavSpace * const navspace = sector.GetNavSpaceAt( j );
					if( navspace == pSpace.GetOwnerHTNavSpace() ){
						continue;
					}
					
					dedaiSpace &space = *navspace->GetSpace();
					dedaiSpaceGrid * const grid = space.GetGrid();
					if( ! grid ){
						continue;
					}
					
					const decMatrix matrix( spaceMatrix * space.GetInverseMatrix() );
					dedaiSpaceGridVertex * const vertices = grid->GetVertices();
					const int vertexCount = grid->GetVertexCount();
					
					// this is now a slow test. each vertex of this navigation grid is tested against all vertices
					// of the other grid. this is an N^2 test which is slow but this could be later on improved by
					// using an octree or some other kind of data structure
					for( k=0; k<pVertexCount; k++ ){
						const decVector v1 = matrix * pVertices[ k ].GetPosition();
						
						for( l=0; l<vertexCount; l++ ){
							if( v1.IsEqualTo( vertices[ l ].GetPosition(), threshold ) ){
								//pSpace.GetDEAI().LogInfoFormat( "Link Found: (ng=%p v=%d) => (ng=%p v=%d)", pSpace, k, navspace, l );
								
								links2 = new sLink[ linkCount + 1 ];
								if( links ){
									memcpy( links2, links, sizeof( sLink ) * linkCount );
									delete [] links;
								}
								links = links2;
								links[ linkCount ].vertexIndex = k;
								links[ linkCount ].linkedVertex = vertices + l;
								linkCount++;
								
								break;
							}
						}
					}
				}
			}
		}
		
		// navigation spaces
		deNavigationSpace *engNavSpace = engWorld.GetRootNavigationSpace();
		
		while( engNavSpace ){
			dedaiNavSpace * const navspace = ( dedaiNavSpace* )engNavSpace->GetPeerAI();
			
			if( navspace && navspace != pSpace.GetOwnerNavSpace() ){
				dedaiSpace * const space = navspace->GetSpace();
				dedaiSpaceGrid *navgrid = space->GetGrid();
				
				if( navgrid ){
					const decMatrix matrix = ( spaceMatrix * space->GetInverseMatrix() ).ToMatrix();
					dedaiSpaceGridVertex * const vertices = navgrid->GetVertices();
					const int vertexCount = navgrid->GetVertexCount();
					
					// this is now a slow test. each vertex of this navigation grid is tested against all vertices
					// of the other grid. this is an N^2 test which is slow but this could be later on improved by
					// using an octree or some other kind of data structure
					for( i=0; i<pVertexCount; i++ ){
						const decVector v1 = matrix * pVertices[ i ].GetPosition();
						
						for( j=0; j<vertexCount; j++ ){
							if( v1.IsEqualTo( vertices[ j ].GetPosition(), threshold ) ){
								//pSpace.GetDEAI().LogInfoFormat( "Link Found: (ng=%p v=%i) => (ng=%p v=%i)", pSpace, i, navspace, j );
								
								links2 = new sLink[ linkCount + 1 ];
								if( links ){
									memcpy( links2, links, sizeof( sLink ) * linkCount );
									delete [] links;
								}
								links = links2;
								links[ linkCount ].vertexIndex = i;
								links[ linkCount ].linkedVertex = vertices + j;
								linkCount++;
								
								break;
							}
						}
					}
				}
			}
			
			engNavSpace = engNavSpace->GetLLWorldNext();
		}
		
		for( i=0; i<pVertexCount; i++ ){
			pVertices[ i ].SetFirstLink( pLinkCount );
			
			for( j=0; j<linkCount; j++ ){
				if( links[ j ].vertexIndex == i ){
					AddLink( links[ j ].linkedVertex );
				}
			}
			
			pVertices[ i ].SetLinkCount( pLinkCount - pVertices[ i ].GetFirstLink() );
		}
		
		if( links ){
			delete [] links;
		}
		
	}catch( const deException & ){
		if( links ){
			delete [] links;
		}
		throw;
	}
	
#if 0
	// debug
	for( i=0; i<pCornerCount; i++ ){
		if( pCorners[ i ].GetLink() != -1 ){
			const dedaiNavMeshLink &link = pLinks[ pCorners[ i ].GetLink() ];
			
			pSpace.GetDEAI().LogInfoFormat( "Debug Link: %i => ns=%p f=%i C=%i", i, link.GetNavMesh()->GetNavSpace(), link.GetFace(), link.GetCorner() );
		}
	}
#endif
}

void dedaiSpaceGrid::UpdateBlocking(){
	// enable all vertices
	int i;
	for( i=0; i<pVertexCount; i++ ){
		pVertices[ i ].SetEnabled( true );
	}
	
	// process overlapping blockers
	if( pSpace.GetParentWorld() ){
		// create list of splitter volumes
		decConvexVolumeList splitterList;
		pSpace.AddBlockerSplitters( splitterList );
		pSpace.AddSpaceBlockerSplitters( splitterList );
		
		// disable vertices inside any of the splitter volumes
		if( splitterList.GetVolumeCount() > 0 ){
			pDisableVertices( splitterList );
		}
	}
}

void dedaiSpaceGrid::Clear(){
	pLinkCount = 0;
	pEdgeCount = 0;
	pVertexCount = 0;
	pVertexEdgeCount = 0;
}



void dedaiSpaceGrid::UpdateDDSSpaceShape(){
	deDebugDrawerShape &ddsShape = *pSpace.GetDDSSpace();
	deDebugDrawerShape &ddsNormals = *pSpace.GetDDSNormals();
	deDebugDrawerShape &ddsMismatching = *pSpace.GetDDSMismatching();
	deDebugDrawerShapeFace *ddsFace = NULL;
	int i;
	
	ddsShape.RemoveAllFaces();
	ddsShape.GetShapeList().RemoveAll();
	
	ddsNormals.RemoveAllFaces();
	ddsNormals.GetShapeList().RemoveAll();
	
	ddsMismatching.RemoveAllFaces();
	ddsMismatching.GetShapeList().RemoveAll();
	
	try{
		for( i=0; i<pEdgeCount; i++ ){
			const dedaiSpaceGridVertex &vertex1 = pVertices[ pEdges[ i ].GetVertex1() ];
			const dedaiSpaceGridVertex &vertex2 = pVertices[ pEdges[ i ].GetVertex2() ];
			if( ! vertex1.GetEnabled() || ! vertex2.GetEnabled() ){
				continue;
			}
			
			const decVector &position1 = vertex1.GetPosition();
			const decVector &position2 = vertex2.GetPosition();
			
			ddsFace = new deDebugDrawerShapeFace;
			ddsFace->AddVertex( position1 );
			ddsFace->AddVertex( position2 );
			ddsFace->AddVertex( position2 );
			ddsShape.AddFace( ddsFace );
			ddsFace = NULL;
		}
		
	}catch( const deException & ){
		if( ddsFace ){
			delete ddsFace;
		}
		throw;
	}
}



// Private Functions
//////////////////////

void dedaiSpaceGrid::pInitFromNavSpace(){
	const deNavigationSpace &engNavSpace = pSpace.GetOwnerNavSpace()->GetNavigationSpace();
	const int edgeCount = engNavSpace.GetEdgeCount();
	int vertexEdgeCount;
	int firstEdge = 0;
	int i, j, count;
	
	// make sure all is cleared first
	Clear();
	
	// adjust arrays
	count = edgeCount * 2;
	if( count > pVertexEdgeSize ){
		unsigned short *newArray = new unsigned short[ count ];
		if( pVertexEdges ){
			delete [] pVertexEdges;
		}
		pVertexEdges = newArray;
		pVertexEdgeSize = count;
	}
	pVertexEdgeCount = count;
	
	count = engNavSpace.GetVertexCount();
	if( count > pVertexSize ){
		dedaiSpaceGridVertex *newArray = new dedaiSpaceGridVertex[ count ];
		if( pVertices ){
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = count;
	}
	pVertexCount = count;
	
	count = edgeCount;
	if( count > pEdgeSize ){
		dedaiSpaceGridEdge *newArray = new dedaiSpaceGridEdge[ count ];
		if( pEdges ){
			delete [] pEdges;
		}
		pEdges = newArray;
		pEdgeSize = count;
	}
	pEdgeCount = count;
	
	// init vertices and edges
	for( i=0; i<pVertexCount; i++ ){
		pVertices[ i ].SetGrid( this );
		pVertices[ i ].SetIndex( ( unsigned short )i );
		pVertices[ i ].SetPosition( engNavSpace.GetVertexAt( i ) );
		pVertices[ i ].SetFirstEdge( 0 );
		pVertices[ i ].SetEdgeCount( 0 );
	}
	
	pEdgeCount = 0;
	for( i=0; i<edgeCount; i++ ){
		const deNavigationSpaceEdge &edge = engNavSpace.GetEdgeAt( i );
		if( edge.GetVertex1() == edge.GetVertex2() ){
			continue;
		}
		
		pEdges[ pEdgeCount ].SetGrid( this );
		pEdges[ pEdgeCount ].SetIndex( ( unsigned short )pEdgeCount );
		pEdges[ pEdgeCount ].SetVertex1( edge.GetVertex1() );
		pEdges[ pEdgeCount ].SetVertex2( edge.GetVertex2() );
		pEdges[ pEdgeCount ].SetTypeNumber1( pSpace.AddTypeMapping( edge.GetType1() ) );
		pEdges[ pEdgeCount ].SetTypeNumber2( pSpace.AddTypeMapping( edge.GetType2() ) );
		pEdges[ pEdgeCount ].SetLength( ( engNavSpace.GetVertexAt( edge.GetVertex2() )
			- engNavSpace.GetVertexAt( edge.GetVertex1() ) ).Length() );
		pEdgeCount++;
	}
	
	for( i=0; i<pVertexCount; i++ ){
		pVertices[ i ].SetFirstEdge( firstEdge );
		vertexEdgeCount = 0;
		for( j=0; j<pEdgeCount; j++ ){
			if( pEdges[ j ].GetVertex1() == i || pEdges[ j ].GetVertex2() == i ){
				pVertexEdges[ firstEdge++ ] = j;
				vertexEdgeCount++;
			}
		}
		pVertices[ i ].SetEdgeCount( vertexEdgeCount );
	}
	pVertexEdgeCount = firstEdge;
	
	// debug
#if 0
	printf( "vertex count %i\n", pVertexCount );
	for( i=0; i<pVertexCount; i++ ){
		printf( "%i) p=(%f,%f,%f) f=%i ec=%i\n", i, pVertices[i].GetPosition().x, pVertices[i].GetPosition().y,
			pVertices[i].GetPosition().z, pVertices[i].GetFirstEdge(), pVertices[i].GetEdgeCount() );
	}
	printf( "edge count %i\n", pEdgeCount );
	for( i=0; i<pEdgeCount; i++ ){
		printf( "%i) v1=%i v2=%i t1=%i t2=%i l=%f\n", i, pEdges[i].GetVertex1(), pEdges[i].GetVertex2(),
			pEdges[i].GetTypeNumber1(), pEdges[i].GetTypeNumber2(), pEdges[i].GetLength() );
	}
	printf( "vertex edge count %i\n", pVertexEdgeCount );
	for( i=0; i<pVertexEdgeCount; i++ ){
		printf( "%i) e=%i\n", i, pVertexEdges[i] );
	}
#endif
}

void dedaiSpaceGrid::pInitFromHTNavSpace(){
	const dedaiHeightTerrainNavSpace &navspace = *pSpace.GetOwnerHTNavSpace();
	const dedaiHeightTerrainSector &sector = navspace.GetSector();
	const float * const heights = sector.GetHeights();
	const deHeightTerrainNavSpace &engNavSpace = navspace.GetNavigationSpace();
	
	const deHeightTerrain &engHT = sector.GetHeightTerrain().GetHeightTerrain();
	const int imageDim = engHT.GetSectorResolution();
	const float posScale = ( float )engHT.GetSectorSize() / ( float )( imageDim - 1 );
	const float posOffset = ( float )engHT.GetSectorSize() * 0.5f;
	
	const deHeightTerrainNavSpaceEdge * const engEdges = engNavSpace.GetEdges();
	const dedaiHeightTerrainNavSpace::sEdge * const edges = navspace.GetEdges();
	const int edgeCount = engNavSpace.GetEdgeCount();
	const unsigned int * const vertices = navspace.GetVertices();
	const int vertexCount = navspace.GetVertexCount();
	int i, j;
	
	// check for obvious bugs
	if( ! edges || ! vertices ){
		DETHROW( deeInvalidParam );
	}
	
	// make sure all is cleared first
	Clear();
	
	// adjust arrays
	const int edgeVertexCount = edgeCount * 2;
	
	if( edgeVertexCount > pVertexEdgeSize ){
		unsigned short * const newArray = new unsigned short[ edgeVertexCount ];
		if( pVertexEdges ){
			delete [] pVertexEdges;
		}
		pVertexEdges = newArray;
		pVertexEdgeSize = edgeVertexCount;
	}
	pVertexEdgeCount = edgeVertexCount;
	
	if( vertexCount > pVertexSize ){
		dedaiSpaceGridVertex * const newArray = new dedaiSpaceGridVertex[ vertexCount ];
		if( pVertices ){
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = vertexCount;
	}
	pVertexCount = vertexCount;
	
	for( i=0; i<vertexCount; i++ ){
		pVertices[ i ].SetGrid( this );
		pVertices[ i ].SetIndex( ( unsigned short )i );
		pVertices[ i ].SetPosition( decVector(
			posScale * ( float )( vertices[ i ] % imageDim ) - posOffset,
			heights[ vertices[ i ] ],
			posOffset - posScale * ( float )( vertices[ i ] / imageDim ) ) );
		pVertices[ i ].SetFirstEdge( 0 );
		pVertices[ i ].SetEdgeCount( 0 );
	}
	
	if( edgeCount > pEdgeSize ){
		dedaiSpaceGridEdge * const newArray = new dedaiSpaceGridEdge[ edgeCount ];
		if( pEdges ){
			delete [] pEdges;
		}
		pEdges = newArray;
		pEdgeSize = edgeCount;
	}
	pEdgeCount = 0;
	
	// init vertices and edges
	for( i=0; i<edgeCount; i++ ){
		if( edges[ i ].vertex1 == edges[ i ].vertex2 ){
			continue;
		}
		
		const deHeightTerrainNavSpaceEdge &engEdge = engEdges[ i ];
		pEdges[ pEdgeCount ].SetGrid( this );
		pEdges[ pEdgeCount ].SetIndex( ( unsigned short )pEdgeCount );
		pEdges[ pEdgeCount ].SetVertex1( edges[ i ].vertex1 );
		pEdges[ pEdgeCount ].SetVertex2( edges[ i ].vertex2 );
		pEdges[ pEdgeCount ].SetTypeNumber1( pSpace.AddTypeMapping( engEdge.GetType1() ) );
		pEdges[ pEdgeCount ].SetTypeNumber2( pSpace.AddTypeMapping( engEdge.GetType2() ) );
		pEdges[ pEdgeCount ].SetLength( ( pVertices[ edges[ i ].vertex2 ].GetPosition()
			- pVertices[ edges[ i ].vertex1 ].GetPosition() ).Length() );
		pEdgeCount++;
	}
	
	int firstEdge = 0;
	
	for( i=0; i<pVertexCount; i++ ){
		pVertices[ i ].SetFirstEdge( firstEdge );
		int vertexEdgeCount = 0;
		for( j=0; j<pEdgeCount; j++ ){
			if( pEdges[ j ].GetVertex1() == i || pEdges[ j ].GetVertex2() == i ){
				pVertexEdges[ firstEdge++ ] = j;
				vertexEdgeCount++;
			}
		}
		pVertices[ i ].SetEdgeCount( vertexEdgeCount );
	}
	pVertexEdgeCount = firstEdge;
}



void dedaiSpaceGrid::pDisableVertices( const decConvexVolumeList &list ){
	const int splitterCount = list.GetVolumeCount();
	int i, j, k;
	
	for( i=0; i<pVertexCount; i++ ){
		const decVector &position = pVertices[ i ].GetPosition();
		
		for( j=0; j<splitterCount; j++ ){
			const decConvexVolume &volume = *list.GetVolumeAt( j );
			const int faceCount = volume.GetFaceCount();
			
			for( k=0; k<faceCount; k++ ){
				const decConvexVolumeFace &volumeFace = *volume.GetFaceAt( k );
				const decVector &planePosition = volume.GetVertexAt( volumeFace.GetVertexAt( 0 ) );
				const decVector &planeNormal = volumeFace.GetNormal();
				const float planeDist = planeNormal * planePosition;
				
				if( planeNormal * position > planeDist ){
					break;
				}
			}
			
			if( k == faceCount ){
				pVertices[ i ].SetEnabled( false );
				break;
			}
		}
	}
}
