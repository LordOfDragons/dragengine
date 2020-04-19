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

#include "dedaiSpaceMesh.h"
#include "dedaiSpaceMeshEdge.h"
#include "dedaiSpaceMeshCorner.h"
#include "dedaiSpaceMeshFace.h"
#include "dedaiSpaceMeshLink.h"
#include "../dedaiSpace.h"
#include "../../dedaiNavSpace.h"
#include "../../blocker/dedaiNavBlocker.h"
#include "../../costs/dedaiCostTable.h"
#include "../../heightterrain/dedaiHeightTerrain.h"
#include "../../heightterrain/dedaiHeightTerrainSector.h"
#include "../../heightterrain/dedaiHeightTerrainNavSpace.h"
#include "../../layer/dedaiLayer.h"
#include "../../../deDEAIModule.h"
#include "../../../world/dedaiWorld.h"
#include "../../../devmode/dedaiDeveloperMode.h"
#include "../../../utils/dedaiConvexFace.h"
#include "../../../utils/dedaiConvexFaceList.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/math/decConvexVolume.h>
#include <dragengine/common/math/decConvexVolumeList.h>
#include <dragengine/common/math/decConvexVolumeFace.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/navigation/blocker/deNavigationBlocker.h>
#include <dragengine/resources/navigation/space/deNavigationSpace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceCorner.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>



// Definitions
////////////////

#define THRESHOLD_EQUAL 0.0001



// Class dedaiSpaceMesh
/////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiSpaceMesh::dedaiSpaceMesh( dedaiSpace &space ) :
pSpace( space ),

pVertices( NULL ),
pVertexCount( 0 ),
pVertexSize( 0 ),
pStaticVertexCount( 0 ),
pBlockerBaseVertex( 0 ),

pEdges( NULL ),
pEdgeCount( 0 ),
pEdgeSize( 0 ),
pStaticEdgeCount( 0 ),
pBlockerBaseEdge( 0 ),

pCorners( NULL ),
pCornerCount( 0 ),
pCornerSize( 0 ),
pStaticCornerCount( 0 ),
pBlockerBaseCorner( 0 ),

pFaces( NULL ),
pFaceCount( 0 ),
pFaceSize( 0 ),
pStaticFaceCount( 0 ),
pBlockerBaseFace( 0 ),

pLinks( NULL ),
pLinkCount( 0 ),
pLinkSize( 0 ){
}

dedaiSpaceMesh::~dedaiSpaceMesh(){
	Clear();
	
	if( pLinks ){
		delete [] pLinks;
	}
	if( pFaces ){
		delete [] pFaces;
	}
	if( pCorners ){
		delete [] pCorners;
	}
	if( pEdges ){
		delete [] pEdges;
	}
	if( pVertices ){
		delete [] pVertices;
	}
}



// Management
///////////////

const decVector &dedaiSpaceMesh::GetVertexAt( int index ) const{
	if( index < 0 || index >= pVertexCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pVertices[ index ];
}

void dedaiSpaceMesh::AddVertex( const decVector &position ){
	if( pVertexCount == pVertexSize ){
		int newSize = pVertexSize + 10;
		decVector *newArray = new decVector[ newSize ];
		if( pVertices ){
			if( pVertexSize > 0 ){
				memcpy( newArray, pVertices, sizeof( decVector ) * pVertexSize );
			}
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = newSize;
	}
	
	pVertices[ pVertexCount++ ] = position;
}

int dedaiSpaceMesh::IndexOfVertex( const decVector &vertex ) const{
	int i;
	
	for( i=0; i<pVertexCount; i++ ){
		if( vertex.IsEqualTo( pVertices[ i ], THRESHOLD_EQUAL ) ){
			return i;
		}
	}
	
	return -1;
}



dedaiSpaceMeshEdge &dedaiSpaceMesh::GetEdgeAt( int index ) const{
	if( index < 0 || index >= pEdgeCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pEdges[ index ];
}

void dedaiSpaceMesh::AddEdge( unsigned short vertex1, unsigned short vertex2 ){
	if( pEdgeCount == pEdgeSize ){
		int newSize = pEdgeSize + 10;
		dedaiSpaceMeshEdge *newArray = new dedaiSpaceMeshEdge[ newSize ];
		if( pEdges ){
			if( pEdgeSize > 0 ){
				memcpy( newArray, pEdges, sizeof( dedaiSpaceMeshEdge ) * pEdgeSize );
			}
			delete [] pEdges;
		}
		pEdges = newArray;
		pEdgeSize = newSize;
	}
	
	pEdges[ pEdgeCount ].SetVertex1( vertex1 );
	pEdges[ pEdgeCount ].SetVertex2( vertex2 );
	pEdges[ pEdgeCount ].SetFace1( -1 );
	pEdges[ pEdgeCount ].SetFace2( -1 );
	pEdgeCount++;
}

int dedaiSpaceMesh::IndexOfEdgeMatching( unsigned short vertex1, unsigned short vertex2 ) const{
	int i;
	
	for( i=0; i<pEdgeCount; i++ ){
		if( ( ( pEdges[ i ].GetVertex1() == vertex1 ) && ( pEdges[ i ].GetVertex2() == vertex2 ) )
		||  ( ( pEdges[ i ].GetVertex1() == vertex2 ) && ( pEdges[ i ].GetVertex2() == vertex1 ) ) ){
			return i;
		}
	}
	
	return -1;
}



dedaiSpaceMeshCorner &dedaiSpaceMesh::GetCornerAt( int index ) const{
	if( index < 0 || index >= pCornerCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pCorners[ index ];
}

void dedaiSpaceMesh::AddCorner( unsigned short vertex, unsigned short edge, unsigned short type ){
	if( pCornerCount == pCornerSize ){
		int newSize = pCornerSize + 10;
		dedaiSpaceMeshCorner *newArray = new dedaiSpaceMeshCorner[ newSize ];
		if( pCorners ){
			if( pCornerSize > 0 ){
				memcpy( newArray, pCorners, sizeof( dedaiSpaceMeshCorner ) * pCornerSize );
			}
			delete [] pCorners;
		}
		pCorners = newArray;
		pCornerSize = newSize;
	}
	
	pCorners[ pCornerCount ].SetVertex( vertex );
	pCorners[ pCornerCount ].SetEdge( edge );
	pCorners[ pCornerCount ].SetTypeNumber( type );
	pCorners[ pCornerCount ].SetLink( CORNER_NO_LINK );
	pCorners[ pCornerCount ].SetEnabled( true );
	pCornerCount++;
}



dedaiSpaceMeshFace &dedaiSpaceMesh::GetFaceAt( int index ) const{
	if( index < 0 || index >= pFaceCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pFaces[ index ];
}

void dedaiSpaceMesh::AddFace(){
	if( pFaceCount == pFaceSize ){
		int newSize = pFaceSize + 10;
		dedaiSpaceMeshFace *newArray = new dedaiSpaceMeshFace[ newSize ];
		if( pFaces ){
			if( pFaceSize > 0 ){
				memcpy( newArray, pFaces, sizeof( dedaiSpaceMeshFace ) * pFaceSize );
			}
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = newSize;
	}
	
	pFaces[ pFaceCount ].SetIndex( pFaceCount );
	pFaces[ pFaceCount ].SetMesh( this );
	pFaces[ pFaceCount ].SetFirstCorner( 0 );
	pFaces[ pFaceCount ].SetCornerCount( 0 );
	pFaces[ pFaceCount ].SetEnabled( true );
	pFaceCount++;
}

dedaiSpaceMeshFace *dedaiSpaceMesh::GetFaceClosestTo( const decVector &position, float &distance ) const{
	// for the time being brute force
	dedaiSpaceMeshFace *bestFace = NULL;
	float bestDistSquared = 0.0f;
	int f, c;
	
	for( f=0; f<pFaceCount; f++ ){
		const dedaiSpaceMeshFace &face = pFaces[ f ];
		
		if( ! face.GetEnabled() ){
			continue;
		}
// 		if( ! ( face.GetMaximumExtend() >= testMinExtend && face.GetMinimumExtend() <= testMaxExtend ) ){
// 			continue;
// 		}
		
		const decVector &faceNormal = face.GetNormal();
		const dedaiSpaceMeshCorner * const corners = pCorners + face.GetFirstCorner();
		const int cornerCount = face.GetCornerCount();
		
		decVector testPos( position + faceNormal * ( face.GetDistance() - position * faceNormal ) );
		
		for( c=0; c<cornerCount; c++ ){
			const decVector &ev1 = pVertices[ corners[ c ].GetVertex() ];
			const decVector &ev2 = pVertices[ corners[ ( c + 1 ) % cornerCount ].GetVertex() ];
			const decVector edgeNormal( ( faceNormal % ( ev2 - ev1 ) ).Normalized() );
			const float lambda = edgeNormal * ( testPos - ev1 );
			
			if( lambda < 0.0f ){
				testPos -= edgeNormal * lambda;
			}
		}
		
		const float testDistSquared = ( testPos - position ).LengthSquared();
		
		if( ! bestFace || testDistSquared < bestDistSquared ){
			bestFace = pFaces + f;
			bestDistSquared = testDistSquared;
		}
	}
	
	if( bestFace ){
		distance = sqrtf( bestDistSquared );
	}
	return bestFace;
}



dedaiSpaceMeshFace *dedaiSpaceMesh::NearestPoint( const decVector &point, float radius,
decVector &nearestPosition, float &nearestDistSquared ) const{
	const decVector testMinExtend( point - decVector( radius, radius, radius ) );
	const decVector testMaxExtend( point + decVector( radius, radius, radius ) );
	const float radiusSquared = radius * radius;
	dedaiSpaceMeshFace *nearestFace = NULL;
	int f, c;
	
	nearestDistSquared = 0.0f;
	
	for( f=0; f<pFaceCount; f++ ){
		const dedaiSpaceMeshFace &face = pFaces[ f ];
		
		if( ! face.GetEnabled() ){
			continue;
		}
		if( ! ( face.GetMaximumExtend() >= testMinExtend && face.GetMinimumExtend() <= testMaxExtend ) ){
			continue;
		}
		
		const decVector &faceNormal = face.GetNormal();
		const dedaiSpaceMeshCorner * const corners = pCorners + face.GetFirstCorner();
		const int cornerCount = face.GetCornerCount();
		
		decVector testPos( point + faceNormal * ( face.GetDistance() - point * faceNormal ) );
		
		for( c=0; c<cornerCount; c++ ){
			const decVector &ev1 = pVertices[ corners[ c ].GetVertex() ];
			const decVector &ev2 = pVertices[ corners[ ( c + 1 ) % cornerCount ].GetVertex() ];
			const decVector edgeNormal( ( faceNormal % ( ev2 - ev1 ) ).Normalized() );
			const float lambda = edgeNormal * ( testPos - ev1 );
			
			if( lambda < 0.0f ){
				testPos -= edgeNormal * lambda;
			}
		}
		
		const float testDistSquared = ( testPos - point ).LengthSquared();
		
		if( testDistSquared <= radiusSquared && ( ! nearestFace || testDistSquared < nearestDistSquared ) ){
			nearestFace = pFaces + f;
			nearestDistSquared = testDistSquared;
			nearestPosition = testPos;
		}
	}
	
	return nearestFace;
}



dedaiSpaceMeshLink &dedaiSpaceMesh::GetLinkAt( int index ) const{
	if( index < 0 || index >= pLinkCount ){
		DETHROW( deeInvalidParam );
	}
	
	return pLinks[ index ];
}

dedaiSpaceMeshLink &dedaiSpaceMesh::GetLinkWith( const dedaiSpaceMeshLink &foreignLink ) const{
	return GetLinkAt( GetCornerAt( GetFaceAt( foreignLink.GetFace() ).GetFirstCorner() + foreignLink.GetCorner() ).GetLink() );
}

void dedaiSpaceMesh::AddLink( dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner ){
	AddLink( mesh, face, corner, ( pSpace.GetMatrix() * mesh->GetSpace().GetInverseMatrix() ).ToMatrix() );
}

void dedaiSpaceMesh::AddLink( dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner, const decMatrix &transform ){
	///* DEBUG */ if( HasLinkWith( mesh, face, corner ) ) DETHROW( deeInvalidAction ); /* DEBUG */
	
	if( pLinkCount == pLinkSize ){
		int newSize = pLinkSize + 10;
		dedaiSpaceMeshLink *newArray = new dedaiSpaceMeshLink[ newSize ];
		if( pLinks ){
			if( pLinkSize > 0 ){
				memcpy( newArray, pLinks, sizeof( dedaiSpaceMeshLink ) * pLinkSize );
			}
			delete [] pLinks;
		}
		pLinks = newArray;
		pLinkSize = newSize;
	}
	
	pLinks[ pLinkCount ].SetMesh( mesh );
	pLinks[ pLinkCount ].SetFace( face );
	pLinks[ pLinkCount ].SetCorner( corner );
	pLinks[ pLinkCount ].SetTransform( transform );
	pLinkCount++;
}

bool dedaiSpaceMesh::HasLinkWith( dedaiSpaceMesh *mesh, unsigned short face, unsigned short corner ) const{
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		if( pLinks[ i ].Matches( mesh, face, corner ) ){
			return true;
		}
	}
	
	return false;
}

void dedaiSpaceMesh::RemoveLinkWith( const dedaiSpaceMeshLink &foreignLink ){
	dedaiSpaceMeshCorner &corner = GetCornerAt( GetFaceAt( foreignLink.GetFace() ).GetFirstCorner() + foreignLink.GetCorner() );
	const int link = corner.GetLink();
	if( link == CORNER_NO_LINK ){
		return;
	}
	
	corner.SetLink( CORNER_NO_LINK );
	
	if( link == pLinkCount - 1 ){
		pLinkCount--;
		return; // link to remove is last link so nothing more to do
	}
	
	// link is not last. move the last link to this position and adjust corner
	dedaiSpaceMeshLink &linkTo = pLinks[ link ];
	linkTo = pLinks[ pLinkCount - 1 ];
	const dedaiSpaceMeshLink &foreignLinkTo = linkTo.GetForeignLink();
	pCorners[ pFaces[ foreignLinkTo.GetFace() ].GetFirstCorner() + foreignLinkTo.GetCorner() ].SetLink( link );
	pLinkCount--;
	
	// tell the owner links have to be updated the next time
	pSpace.LinksRemoves();
}

void dedaiSpaceMesh::RemoveAllLinks(){
	if( pLinkCount == 0 ){
		return;
	}
	
	int i;
	
	for( i=0; i<pLinkCount; i++ ){
		pLinks[ i ].GetMesh()->RemoveLinkWith( pLinks[ i ] );
	}
	
	for( i=0; i<pCornerCount; i++ ){
		pCorners[ i ].SetLink( CORNER_NO_LINK ); // after RemoveLinkWith since it needs our link
	}
	
	pLinkCount = 0;
	
	// tell the owner links have to be updated the next time
	pSpace.LinksRemoves();
}



void dedaiSpaceMesh::InitFromSpace(){
	if( pSpace.GetOwnerNavSpace() ){
		pInitFromNavSpace();
		
	}else if( pSpace.GetOwnerHTNavSpace() ){
		pInitFromHTNavSpace();
	}
	
// 	pVerifyInvariants();
}

void dedaiSpaceMesh::LinkToOtherMeshes(){
	const float snapDistance = pSpace.GetSnapDistance();
	const float snapAngle = pSpace.GetSnapAngle();
	const deWorld &engWorld = pSpace.GetParentWorld()->GetWorld();
	const decDVector minExtend = pSpace.GetMinimumExtends() - decDVector( snapDistance, snapDistance, snapDistance );
	const decDVector maxExtend = pSpace.GetMaximumExtends() + decDVector( snapDistance, snapDistance, snapDistance );
	
	// height terrain navspaces
	const dedaiHeightTerrain * const heightTerrain = pSpace.GetParentWorld()->GetHeightTerrain();
	if( heightTerrain ){
		const int sectorCount = heightTerrain->GetSectorCount();
		int i, j;
		
		for( i=0; i<sectorCount; i++ ){
			dedaiHeightTerrainSector &sector = *heightTerrain->GetSectorAt( i );
			const int navSpaceCount = sector.GetNavSpaceCount();
			for( j=0; j<navSpaceCount; j++ ){
				dedaiHeightTerrainNavSpace * const navspace = sector.GetNavSpaceAt( j );
				if( navspace == pSpace.GetOwnerHTNavSpace() ){
					continue;
				}
				
				dedaiSpace &space = *navspace->GetSpace();
				dedaiSpaceMesh * const mesh = space.GetMesh();
				if( ! mesh ){
					continue;
				}
				
				const decDVector &minExtend2 = space.GetMinimumExtends();
				const decDVector &maxExtend2 = space.GetMaximumExtends();
				if( maxExtend2 >= minExtend && minExtend2 <= maxExtend ){
					pLinkToMesh( mesh, snapDistance, snapAngle );
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
			dedaiSpaceMesh * const navmesh = space->GetMesh();
			
			if( navmesh ){
				const decDVector &minExtend2 = space->GetMinimumExtends();
				const decDVector &maxExtend2 = space->GetMaximumExtends();
				if( maxExtend2 >= minExtend && minExtend2 <= maxExtend ){
					pLinkToMesh( navmesh, snapDistance, snapAngle );
				}
			}
		}
		
		engNavSpace = engNavSpace->GetLLWorldNext();
	}
}

void dedaiSpaceMesh::UpdateBlocking(){
	// remove all created blocking elements
	pFaceCount = pBlockerBaseFace;
	pCornerCount = pBlockerBaseCorner;
	pEdgeCount = pBlockerBaseEdge;
	pVertexCount = pBlockerBaseVertex;
	
	// process overlapping blockers
	if( ! pSpace.GetParentWorld() ){
		return;
	}
	
	// create list of splitter volumes
	decConvexVolumeList splitterList;
	pSpace.AddBlockerSplitters( splitterList );
	pSpace.AddSpaceBlockerSplitters( splitterList );
	
	// TODO calculate extends of cut volumes to optimize
	
	// split faces using the list of splitter volumes
	const int splitterCount = splitterList.GetVolumeCount();
	if( splitterCount == 0 ){
		return;
	}
	
	int i, j;
	
	for( i=0; i<pBlockerBaseFace; i++ ){
		const dedaiSpaceMeshFace &face = pFaces[ i ];
		
		// NOTE we are not testing for face.GetEnabled() here since blocking is done on the
		//      initial face set which all are enabled before blocking is applied
		
		// TODO skip faces with extends not overlapping cut extends
		
		// init convex face list with face to split
		dedaiConvexFaceList convexFaceList;
		pInitConvexFaceListFromFace( convexFaceList, face );
		
		// split by all splitter volumes
		for( j=0; j<splitterCount; j++ ){
			convexFaceList.SplitByVolume( *splitterList.GetVolumeAt( j ) );
		}
		
		// if there is more than one face or the first face is not equal to the original face
		// then the splitting has affected the face. in this case add all faces as blocker faces
		if( pMatchesConvexFaceListMeshFace( convexFaceList, face ) ){
			continue;
		}
		
		// collapse redundant vertices to reduce the number of split faces
		if( pSpace.GetDEAI().GetDeveloperMode().GetQuickDebug() == 0
		|| pSpace.GetDEAI().GetDeveloperMode().GetQuickDebug() == 9 ){
			pOptimizeBlockedFaces( convexFaceList, face.GetCornerCount() );
		}
		
		// remove old face adding the new ones
		pDisableFace( i );
		pAddConvexFaces( convexFaceList, face );
		// after this call face reference is potentially invalid due to memory move
	}
}

void dedaiSpaceMesh::Clear(){
	RemoveAllLinks();
	pFaceCount = 0;
	pCornerCount = 0;
	pEdgeCount = 0;
	pVertexCount = 0;
}



void dedaiSpaceMesh::UpdateDDSSpaceShape(){
	deDebugDrawerShape * const ddsShape = pSpace.GetDDSSpace();
	deDebugDrawerShape * const ddsCorners = pSpace.GetDDSCorners();
	deDebugDrawerShape * const ddsNormals = pSpace.GetDDSNormals();
	deDebugDrawerShape * const ddsMismatching = pSpace.GetDDSMismatching();
	deDebugDrawerShape * const ddsHilightCostType = pSpace.GetDDSHilightCostType();
	deDebugDrawerShapeFace *ddsFace = NULL;
	unsigned short hilightCostType = ~0;
	const float normalLength = 0.1f;
	const float cornerCrossLength = 0.025f;
	const decVector cornerCross1( -cornerCrossLength, 0.0f, cornerCrossLength );
	const decVector cornerCross2( cornerCrossLength, 0.0f, cornerCrossLength );
	const decVector cornerCross3( -cornerCrossLength, 0.0f, -cornerCrossLength );
	const decVector cornerCross4( cornerCrossLength, 0.0, -cornerCrossLength );
	int f, c, i;
	
	if( ddsShape ){
		ddsShape->RemoveAllFaces();
		ddsShape->GetShapeList().RemoveAll();
	}
	if( ddsCorners ){
		ddsCorners->RemoveAllFaces();
		ddsCorners->GetShapeList().RemoveAll();
	}
	if( ddsNormals ){
		ddsNormals->RemoveAllFaces();
		ddsNormals->GetShapeList().RemoveAll();
	}
	if( ddsMismatching ){
		ddsMismatching->RemoveAllFaces();
		ddsMismatching->GetShapeList().RemoveAll();
	}
	if( ddsHilightCostType ){
		ddsHilightCostType->RemoveAllFaces();
		ddsHilightCostType->GetShapeList().RemoveAll();
		
		const int costType = pSpace.GetDEAI().GetDeveloperMode().GetSpaceHilightCostType();
		if( costType != -1 ){
			if( pSpace.GetLayer() ){
				hilightCostType = ( unsigned short )pSpace.GetLayer()->GetCostTable().GetTypeWith( costType, hilightCostType );
			}
		}
	}
	
	try{
		for( f=0; f<pFaceCount; f++ ){
			if( ! pFaces[ f ].GetEnabled() ){
				continue;
			}
			
			const unsigned short cornerCount = pFaces[ f ].GetCornerCount();
			const int firstCorner = pFaces[ f ].GetFirstCorner();
			
			if( cornerCount > 2 ){
				// face
				if( ddsShape ){
					ddsFace = new deDebugDrawerShapeFace;
					for( c=0; c<cornerCount; c++ ){
						ddsFace->AddVertex( pVertices[ pCorners[ firstCorner + c ].GetVertex() ] );
					}
					ddsFace->SetNormal( pFaces[ f ].GetNormal() );
					ddsShape->AddFace( ddsFace );
					ddsFace = NULL;
				}
				
				// corners
				if( ddsCorners ){
					for( c=0; c<cornerCount; c++ ){
						const decVector &position = pVertices[ pCorners[ firstCorner + c ].GetVertex() ];
						
						ddsFace = new deDebugDrawerShapeFace;
						ddsFace->AddVertex( position );
						ddsFace->AddVertex( position + cornerCross1 );
						ddsFace->AddVertex( position + cornerCross2 );
						ddsCorners->AddFace( ddsFace );
						ddsFace = NULL;
						
						ddsFace = new deDebugDrawerShapeFace;
						ddsFace->AddVertex( position );
						ddsFace->AddVertex( position + cornerCross4 );
						ddsFace->AddVertex( position + cornerCross3 );
						ddsCorners->AddFace( ddsFace );
						ddsFace = NULL;
					}
				}
				
				// normal
				if( ddsNormals ){
					ddsFace = new deDebugDrawerShapeFace;
					ddsFace->AddVertex( pFaces[ f ].GetCenter() + pFaces[ f ].GetNormal() * normalLength );
					ddsFace->AddVertex( pFaces[ f ].GetCenter() );
					ddsFace->AddVertex( pFaces[ f ].GetCenter() );
					ddsNormals->AddFace( ddsFace );
					ddsFace = NULL;
				}
				
				// hilight if required
				if( ddsHilightCostType ){
					if( hilightCostType != ( unsigned short )~0 && pFaces[ f ].GetTypeNumber() == hilightCostType ){
						ddsFace = new deDebugDrawerShapeFace;
						for( c=0; c<cornerCount; c++ ){
							ddsFace->AddVertex( pVertices[ pCorners[ firstCorner + c ].GetVertex() ] );
						}
						ddsFace->SetNormal( pFaces[ f ].GetNormal() );
						ddsHilightCostType->AddFace( ddsFace );
						ddsFace = NULL;
					}
				}
				
				// mismatching if required
				if( ddsMismatching ){
					for( c=0; c<cornerCount; c++ ){
						const unsigned short ei = pCorners[ firstCorner + c ].GetEdge();
						const dedaiSpaceMeshEdge &edge = pEdges[ ei ];
						const bool winding1 = ( edge.GetVertex1() == pCorners[ firstCorner + c ].GetVertex() );
						bool winding2 = ! winding1;
						
						const short linkIndex = pCorners[ firstCorner + c ].GetLink();
						
						if( linkIndex == -1 ){
							const short fi2 = ( edge.GetFace1() == f ) ? edge.GetFace2() : edge.GetFace1();
							
							if( fi2 != -1 ){
								const dedaiSpaceMeshFace &f2 = pFaces[ fi2 ];
								const int firstCorner2 = f2.GetFirstCorner();
								const int cornerCount2 = f2.GetCornerCount();
								
								for( i=0; i<cornerCount2; i++ ){
									if( pCorners[ firstCorner2 + i ].GetEdge() == ei ){
										winding2 = ( edge.GetVertex1() == pCorners[ firstCorner2 + i ].GetVertex() );
										break;
									}
								}
							}
							
						}else{
							/*
							const dedaiSpaceMesh &nm2 = *pLinks[ linkIndex ].GetMesh();
							const dedaiSpaceMeshCorner * const nm2Corners = nm2.GetCorners();
							const decVector * const nm2Vertices = nm2.GetVertices();
							const unsigned short fi2 = pLinks[ linkIndex ].GetFace();
							
							const dedaiSpaceMeshFace &f2 = nm2.GetFaces()[ fi2 ];
							
							if( f2.GetCornerCount() < 3 ){
								winding2 = ! winding1;
								
							}else{
								const decVector &f2v1 = nm2Vertices[ nm2Corners[ f2.GetFirstCorner() ].GetVertex() ];
								const decVector &f2v2 = nm2Vertices[ nm2Corners[ f2.GetFirstCorner() + 1 ].GetVertex() ];
								const decVector nref2 = f2.GetNormal() % ( f2v1 - f2.GetCenter() );
								winding2 = ( nref2 * ( f2v2 - f2.GetCenter() ) > 0.0f );
							}
							*/
						}
						
						if( winding1 == winding2 ){
							ddsFace = new deDebugDrawerShapeFace;
							for( c=0; c<cornerCount; c++ ){
								ddsFace->AddVertex( pVertices[ pCorners[ firstCorner + c ].GetVertex() ] );
							}
							ddsFace->SetNormal( pFaces[ f ].GetNormal() );
							ddsMismatching->AddFace( ddsFace );
							ddsFace = NULL;
							
							break;
						}
					}
				}
			}
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

void dedaiSpaceMesh::pInitFromNavSpace(){
	const deNavigationSpace &engNavSpace = pSpace.GetOwnerNavSpace()->GetNavigationSpace();
	const deNavigationSpaceCorner * const corners = engNavSpace.GetCorners();
	const deNavigationSpaceFace * const faces = engNavSpace.GetFaces();
	const decVector * const vertices = engNavSpace.GetVertices();
	const int faceCount = engNavSpace.GetFaceCount();
	unsigned short vertex1, vertex2;
	int cornerCount;
	int f, c, count;
	int edgeIndex;
	
	// make sure all is cleared first
	Clear();
	
	// adjust arrays their size we know already
	count = engNavSpace.GetVertexCount();
	if( count > pVertexSize ){
		decVector *newArray = new decVector[ count ];
		if( pVertices ){
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = count;
	}
	pVertexCount = count;
	
	count = engNavSpace.GetCornerCount();
	if( count > pCornerSize ){
		dedaiSpaceMeshCorner *newArray = new dedaiSpaceMeshCorner[ count ];
		if( pCorners ){
			delete [] pCorners;
		}
		pCorners = newArray;
		pCornerSize = count;
	}
	pCornerCount = count;
	
	count = faceCount;
	if( count > pFaceSize ){
		dedaiSpaceMeshFace *newArray = new dedaiSpaceMeshFace[ count ];
		if( pFaces ){
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = count;
	}
	pFaceCount = count;
	
	// copy vertices and corners
	count = engNavSpace.GetVertexCount();
	if( count > 0 ){
		memcpy( pVertices, engNavSpace.GetVertices(), sizeof( decVector ) * count );
	}
	
	count = engNavSpace.GetCornerCount();
	for( c=0; c<count; c++ ){
		pCorners[ c ].SetVertex( corners[ c ].GetVertex() );
		pCorners[ c ].SetTypeNumber( pSpace.AddTypeMapping( corners[ c ].GetType() ) );
		pCorners[ c ].SetLink( CORNER_NO_LINK );
		pCorners[ c ].SetEnabled( true );
	}
	
	// add faces. this builds also the edges the count of which we don't know so far
	int firstCorner = 0;
	for( f=0; f<faceCount; f++ ){
		const deNavigationSpaceFace &face = faces[ f ];
		
		cornerCount = face.GetCornerCount();
		
		if( cornerCount < 2 ){
			pSpace.GetDEAI().LogError( "Navigation mesh has degenrated faces." );
			DETHROW( deeInvalidParam );
		}
		
		pFaces[ f ].SetMesh( this );
		pFaces[ f ].SetIndex( ( unsigned short )f );
		pFaces[ f ].SetFirstCorner( firstCorner );
		pFaces[ f ].SetCornerCount( cornerCount );
		pFaces[ f ].SetTypeNumber( pSpace.AddTypeMapping( face.GetType() ) );
		pFaces[ f ].SetEnabled( true );
		
		decVector center;
		decVector minExtend;
		decVector maxExtend;
		for( c=0; c<cornerCount; c++ ){
			vertex1 = corners[ firstCorner + c ].GetVertex();
			vertex2 = corners[ firstCorner + ( ( c + 1 ) % cornerCount ) ].GetVertex();
			edgeIndex = IndexOfEdgeMatching( vertex1, vertex2 );
			
			if( edgeIndex == -1 ){
				AddEdge( vertex1, vertex2 );
				edgeIndex = pEdgeCount - 1;
				
				pEdges[ edgeIndex ].SetFace1( ( short )f );
				
			}else{
				if( pEdges[ edgeIndex ].GetFace2() == -1 ){
					pEdges[ edgeIndex ].SetFace2( ( short )f );
					
				}else{
					// non-manifold mesh. not usable
					pSpace.GetDEAI().LogError( "Navigation Mesh is non-manifold." );
					DETHROW( deeInvalidParam );
				}
			}
			
			pCorners[ firstCorner + c ].SetEdge( ( unsigned short )edgeIndex );
			
			const decVector &position = vertices[ vertex1 ];
			
			center += position;
			
			if( c == 0 ){
				minExtend = position;
				maxExtend = position;
				
			}else{
				minExtend.SetSmallest( position );
				maxExtend.SetLargest( position );
			}
		}
		
		pFaces[ f ].SetExtends( minExtend, maxExtend );
		pFaces[ f ].SetCenter( center / ( float )cornerCount );
		
		// calculate normal. it is possible multiple following edges are co-linear due to the
		// fact nav-mesh faces can have many corners as long as they are convex. if we pick the
		// first 3 corners it is possible we get two colinear edges. due to floating point
		// precision this can result in a normal which is not close enough to zero causing all
		// kinds of wrong results.
		// 
		// to get a good normal we pick the first two vertices (the first edge) and iterate
		// the remaining corners until we find the first one which is not colinear. what is
		// colinear is tricky to say especially with floating point precision problems. here
		// we pick the corner with the largest angle (abs(dot-product) is smallest). this
		// ensures we do not have to deal with fudge factors for the smallest angle to allow.
		const decVector &p1 = vertices[ corners[ firstCorner ].GetVertex() ];
		const decVector &p2 = vertices[ corners[ firstCorner + 1 ].GetVertex() ];
		const decVector edge1( ( p2 - p1 ).Normalized() );
		float bestAbsDot = 1.0f;
		decVector bestEdge2;
		int bestCorner = -1;
		
		for( c=2; c<cornerCount; c++ ){
			const decVector &p3 = vertices[ corners[ firstCorner + c ].GetVertex() ];
			const decVector edge2( ( p3 - p1 ).Normalized() );
			const float absDot = fabsf( edge1 * edge2 );
			
			if( absDot < bestAbsDot ){
				bestAbsDot = absDot;
				bestEdge2 = edge2;
				bestCorner = c;
			}
		}
		
		if( bestCorner == -1 ){
			pSpace.GetDEAI().LogErrorFormat( "Degenerated face %d", f );
			DETHROW( deeInvalidParam );
			//pFaces[ f ].SetNormal( decVector( 0.0f, 1.0f, 0.0f ) );
			
		}else{
			pFaces[ f ].SetNormal( edge1 % bestEdge2 );
		}
		
		pFaces[ f ].SetDistance( pFaces[ f ].GetNormal() * pFaces[ f ].GetCenter() );
		
		firstCorner += cornerCount;
	}
	
	// store base indices for blockers
	pBlockerBaseVertex = pVertexCount;
	pBlockerBaseEdge = pEdgeCount;
	pBlockerBaseCorner = pCornerCount;
	pBlockerBaseFace = pFaceCount;
}

void dedaiSpaceMesh::pInitFromHTNavSpace(){
	const dedaiHeightTerrainNavSpace &navspace = *pSpace.GetOwnerHTNavSpace();
	const dedaiHeightTerrainSector &sector = navspace.GetSector();
	
	const deHeightTerrain &engHT = sector.GetHeightTerrain().GetHeightTerrain();
	const int imageDim = engHT.GetSectorResolution();
	const float posScale = ( float )engHT.GetSectorSize() / ( float )( imageDim - 1 );
	const float posOffset = ( float )engHT.GetSectorSize() * 0.5f;
	
	const deNavigationSpaceFace * const faces = navspace.GetFaces();
	const unsigned short * const corners = navspace.GetCorners();
	const unsigned int * const vertices = navspace.GetVertices();
	const float * const heights = sector.GetHeights();
	const int vertexCount = navspace.GetVertexCount();
	const int cornerCount = navspace.GetCornerCount();
	const int faceCount = navspace.GetFaceCount();
	unsigned short vertex1, vertex2;
	int i, j, edgeIndex;
	
	// check for obvious bugs
	if( ! corners || ! vertices ){
		DETHROW( deeInvalidParam );
	}
	
	// make sure all is cleared first
	Clear();
	
	// adjust arrays their size we know already
	if( vertexCount > pVertexSize ){
		decVector * const newArray = new decVector[ vertexCount ];
		if( pVertices ){
			delete [] pVertices;
		}
		pVertices = newArray;
		pVertexSize = vertexCount;
	}
	for( i=0; i<vertexCount; i++ ){
		pVertices[ i ].x = posScale * ( float )( vertices[ i ] % imageDim ) - posOffset;
		pVertices[ i ].y = heights[ vertices[ i ] ];
		pVertices[ i ].z = posOffset - posScale * ( float )( vertices[ i ] / imageDim );
	}
	pVertexCount = vertexCount;
	
	if( cornerCount > pCornerSize ){
		dedaiSpaceMeshCorner * const newArray = new dedaiSpaceMeshCorner[ cornerCount ];
		if( pCorners ){
			delete [] pCorners;
		}
		pCorners = newArray;
		pCornerSize = cornerCount;
	}
	pCornerCount = cornerCount;
	for( i=0; i<cornerCount; i++ ){
		pCorners[ i ].SetVertex( corners[ i ] );
		pCorners[ i ].SetTypeNumber( CORNER_NO_COST );
		pCorners[ i ].SetLink( CORNER_NO_LINK );
		pCorners[ i ].SetEnabled( true );
	}
	
	if( faceCount > pFaceSize ){
		dedaiSpaceMeshFace * const newArray = new dedaiSpaceMeshFace[ faceCount ];
		if( pFaces ){
			delete [] pFaces;
		}
		pFaces = newArray;
		pFaceSize = faceCount;
	}
	pFaceCount = faceCount;
	
	// add faces. this builds also the edges the count of which we don't know so far
	int firstCorner = 0;
	for( i=0; i<faceCount; i++ ){
		const deNavigationSpaceFace &face = faces[ i ];
		const int faceCornerCount = face.GetCornerCount();
		
		if( faceCornerCount < 2 ){
			pSpace.GetDEAI().LogError( "Navigation mesh has degenrated faces." );
			DETHROW( deeInvalidParam );
		}
		
		pFaces[ i ].SetMesh( this );
		pFaces[ i ].SetIndex( ( unsigned short )i );
		pFaces[ i ].SetFirstCorner( firstCorner );
		pFaces[ i ].SetCornerCount( faceCornerCount );
		pFaces[ i ].SetTypeNumber( pSpace.AddTypeMapping( face.GetType() ) );
		pFaces[ i ].SetEnabled( true );
		
		decVector center;
		decVector minExtend;
		decVector maxExtend;
		
		for( j=0; j<faceCornerCount; j++ ){
			vertex1 = corners[ firstCorner + j ];
			vertex2 = corners[ firstCorner + ( ( j + 1 ) % faceCornerCount ) ];
			edgeIndex = IndexOfEdgeMatching( vertex1, vertex2 );
			
			if( edgeIndex == -1 ){
				AddEdge( vertex1, vertex2 );
				edgeIndex = pEdgeCount - 1;
				
				pEdges[ edgeIndex ].SetFace1( ( short )i );
				
			}else{
				if( pEdges[ edgeIndex ].GetFace2() == -1 ){
					pEdges[ edgeIndex ].SetFace2( ( short )i );
					
				}else{
					// non-manifold mesh. not usable
					pSpace.GetDEAI().LogError( "Navigation Mesh is non-manifold." );
					DETHROW( deeInvalidParam );
				}
			}
			
			pCorners[ firstCorner + j ].SetEdge( ( unsigned short )edgeIndex );
			
			const decVector &position = pVertices[ vertex1 ];
			
			center += position;
			
			if( j == 0 ){
				minExtend = position;
				maxExtend = position;
				
			}else{
				minExtend.SetSmallest( position );
				maxExtend.SetLargest( position );
			}
		}
		
		pFaces[ i ].SetExtends( minExtend, maxExtend );
		pFaces[ i ].SetCenter( center / ( float )faceCornerCount );
		
		// calculate normal. it is possible multiple following edges are co-linear due to the
		// fact nav-mesh faces can have many corners as long as they are convex. if we pick the
		// first 3 corners it is possible we get two colinear edges. due to floating point
		// precision this can result in a normal which is not close enough to zero causing all
		// kinds of wrong results.
		// 
		// to get a good normal we pick the first two vertices (the first edge) and iterate
		// the remaining corners until we find the first one which is not colinear. what is
		// colinear is tricky to say especially with floating point precision problems. here
		// we pick the corner with the largest angle (abs(dot-product) is smallest). this
		// ensures we do not have to deal with fudge factors for the smallest angle to allow.
		const decVector &p1 = pVertices[ corners[ firstCorner ] ];
		const decVector &p2 = pVertices[ corners[ firstCorner + 1 ] ];
		const decVector edge1( ( p2 - p1 ).Normalized() );
		float bestAbsDot = 1.0f;
		decVector bestEdge2;
		int bestCorner = -1;
		
		for( j=2; j<cornerCount; j++ ){
			const decVector &p3 = pVertices[ corners[ firstCorner + j ] ];
			const decVector edge2( ( p3 - p1 ).Normalized() );
			const float absDot = fabsf( edge1 * edge2 );
			
			if( absDot < bestAbsDot ){
				bestAbsDot = absDot;
				bestEdge2 = edge2;
				bestCorner = j;
			}
		}
		
		if( bestCorner == -1 ){
			pSpace.GetDEAI().LogErrorFormat( "Degenerated face %d", i );
			DETHROW( deeInvalidParam );
			//pFaces[ i ].SetNormal( decVector( 0.0f, 1.0f, 0.0f ) );
			
		}else{
			pFaces[ i ].SetNormal( edge1 % bestEdge2 );
		}
		
		pFaces[ i ].SetDistance( pFaces[ i ].GetNormal() * pFaces[ i ].GetCenter() );
		
		firstCorner += faceCornerCount;
	}
	
	// store base indices for blockers
	pBlockerBaseVertex = pVertexCount;
	pBlockerBaseEdge = pEdgeCount;
	pBlockerBaseCorner = pCornerCount;
	pBlockerBaseFace = pFaceCount;
}



void dedaiSpaceMesh::pInitConvexFaceListFromFace( dedaiConvexFaceList &list, const dedaiSpaceMeshFace &face ) const{
	const int firstCorner = face.GetFirstCorner();
	const int cornerCount = face.GetCornerCount();
	dedaiConvexFace *convexFace = NULL;
	int i;
	
	list.RemoveAllFaces();
	list.RemoveAllVertices();
	
	try{
		convexFace = new dedaiConvexFace;
		for( i=0; i<cornerCount; i++ ){
			list.AddVertex( pVertices[ pCorners[ firstCorner + i ].GetVertex() ] );
			convexFace->AddVertex( i );
		}
		
		convexFace->SetNormal( face.GetNormal() );
		list.AddFace( convexFace );
		
	}catch( const deException & ){
		if( convexFace ){
			delete convexFace;
		}
		throw;
	}
}

bool dedaiSpaceMesh::pMatchesConvexFaceListMeshFace( dedaiConvexFaceList &list, const dedaiSpaceMeshFace &face ) const{
	if( list.GetFaceCount() == 1 ){
		const dedaiConvexFace &convexFace = *list.GetFaceAt( 0 );
		const int cornerCount = face.GetCornerCount();
		int i;
		
		if( convexFace.GetVertexCount() == cornerCount ){
			const int firstCorner = face.GetFirstCorner();
			
			for( i=0; i<cornerCount; i++ ){
				const decVector &correctPosition = pVertices[ pCorners[ firstCorner + i ].GetVertex() ];
				const decVector &testPosition = list.GetVertexAt( convexFace.GetVertexAt( i ) );
				
				if( ! testPosition.IsEqualTo( correctPosition, THRESHOLD_EQUAL ) ){
					break;
				}
			}
			
			if( i == cornerCount ){
				return true;
			}
		}
	}
	
	return false;
}

void dedaiSpaceMesh::pOptimizeBlockedFaces( dedaiConvexFaceList &list, int initialVertexCount ) const{
	const float thresholdColinear = 0.005f; //0.001f;
	const float thresholdConvex = 0.005f; //0.001f;
	
	// collapse faces along connecting edges if the resulting face is convex again.
	// then collapse co-linear edges inside faces not connecting to other faces.
	// at all times initial vertices are not allowed to be touched.
	const int faceCount = list.GetFaceCount();
	int f, f2, v, v2;
	
	for( f=0; f<faceCount; f++ ){
		dedaiConvexFace &face = *list.GetFaceAt( f );
		
		int vertexCount = face.GetVertexCount();
		if( vertexCount < 3 ){
			continue;
		}
		
		int lastVertex = face.GetVertexAt( vertexCount - 1 );
		
		for( v=0; v<vertexCount; v++ ){
			const int vertex = face.GetVertexAt( v );
			
			// find second face containing edge from this vertex to the next one.
			const int nextCorner = ( v + 1 ) % vertexCount;
			const int nextVertex = face.GetVertexAt( nextCorner );
			dedaiConvexFace *otherFace = NULL;
			bool hasOtherVertex = false;
			int otherCorner = 0;
			
			for( f2=0; f2<faceCount; f2++ ){
				if( f2 == f ){
					continue;
				}
				
				dedaiConvexFace * const testFace = list.GetFaceAt( f2 );
				if( testFace->GetVertexCount() < 3 ){
					continue;
				}
				
				const int testCorner = testFace->IndexOfVertex( vertex );
				if( testCorner == -1 ){
					continue;
				}
				
				// for the single face test case we need to know if any face shares the vertex
				hasOtherVertex = true;
				
				// check if the face contains the connection edge starting at vertex. this check
				// is only required for the two face test case. because collapsing test is
				// order independent only face after this face need to be check which is faster
				if( f2 > f ){
					const int testVertexCount = testFace->GetVertexCount();
					const int testVertexPrev = testFace->GetVertexAt( ( testCorner - 1 + testVertexCount ) % testVertexCount );
					
					if( testVertexPrev != nextVertex ){
						continue;
					}
					
					otherFace = testFace;
					otherCorner = testCorner;
					break;
				}
			}
			
			// two faces case. possibly collapse two faces into one
			if( otherFace ){
				// verify if the two vertices neighbor to the connection edge are convex. for this
				// calculate the plane normal at the neighbor edges in the first face pointing
				// inside the first face and check if the other face neighbor vertices are on the
				// front side of these planes. of this is not the case joining the faces would
				// result in a concave face
				const int nextVertex2 = face.GetVertexAt( ( v + 2 ) % vertexCount );
				const decVector &position1 = list.GetVertexAt( lastVertex );
				const decVector &position2 = list.GetVertexAt( vertex );
				const decVector &position3 = list.GetVertexAt( nextVertex );
				const decVector &position4 = list.GetVertexAt( nextVertex2 );
				const decVector normal1( ( face.GetNormal() % ( position2 - position1 ) ).Normalized() );
				const decVector normal2( ( face.GetNormal() % ( position4 - position3 ) ).Normalized() );
				
				const int otherVertexCount = otherFace->GetVertexCount();
				const int otherVertexPrev2 = otherFace->GetVertexAt( ( otherCorner - 2 + otherVertexCount ) % otherVertexCount );
				const int otherVertexNext = otherFace->GetVertexAt( ( otherCorner + 1 ) % otherVertexCount );
				const decVector &otherPosition1 = list.GetVertexAt( otherVertexNext );
				const decVector &otherPosition2 = list.GetVertexAt( otherVertexPrev2 );
				
				if( normal1 * ( otherPosition1 - position2 ) < -thresholdConvex ){
					lastVertex = vertex;
					continue; // first edge would lead into a concave face
				}
				if( normal2 * ( otherPosition2 - position3 ) < -thresholdConvex ){
					lastVertex = vertex;
					continue; // second edge would lead into a concave face
				}
				
				// the joined face is convex. insert the vertices of the other face into this face
				// and remove the other face
				for( v2=0; v2<otherVertexCount-2; v2++ ){
					face.InsertVertex( nextCorner + v2, otherFace->GetVertexAt(
						( otherCorner + 1 + v2 ) % otherVertexCount ) );
				}
				
				otherFace->RemoveAllVertices();
				vertexCount += otherVertexCount - 2;
				
				// there are now two possible scenarios due to wrapping around. if the corners have
				// been added after the current vertex re-test the current vertex since a new face
				// could be touching this face or the newly added following up edge could be co-linear
				if( nextCorner > v ){
					// we re-test the current vertex so lastVertex stays the same
					v--;
					
				// if the corners have been added before the current vertex due to wrapping around
				// the situation is not so optimal. the current vertex counter has to be moved by
				// otherVertexCount - 2 and reduced by 1 to re-test the current vertex. the corners
				// added before the current vertex though would not be checked again missing more
				// opportunities to optimize. for this reason start again at 0 to check the newly
				// added corners. due to wrapping around the current vertex is vertexCount-1 in
				// this case and thus nextCorner is 0. this re-tests a second time some corners
				// but yields some more optimizations
				}else{
					lastVertex = vertex;
					v = -1; // so we end up with v=nextCorner=0 in the next round
				}
				
			// single face case
			}else{
				if( hasOtherVertex ){
					lastVertex = vertex;
					continue; // some other face shares this vertex so collapsing is not possible
				}
				
				// if vertex is an initial vertex collapsing is prohibited
				if( vertex < initialVertexCount ){
					lastVertex = vertex;
					continue;
				}
				
				const decVector &position1 = list.GetVertexAt( lastVertex );
				const decVector &position2 = list.GetVertexAt( vertex );
				const decVector &position3 = list.GetVertexAt( nextVertex );
				const decVector normal( ( face.GetNormal() % ( position2 - position1 ) ).Normalized() );
				
				if( fabsf( normal * ( position3 - position2 ) ) > thresholdColinear ){
					lastVertex = vertex;
					continue; // edges are not co-linear
				}
				
				// edges are co-linear. remove vertex. continue testing with the current vertex since
				// the next edge could be co-linear again
				face.RemoveVertexFrom( v );
				vertexCount--;
				if( vertexCount < 3 ){
					break;
				}
				// we re-test the current vertex so lastVertex stays the same
				v--;
			}
		}
	}
}

void dedaiSpaceMesh::pDisableFace( int faceIndex ){
	// remove the face from all edges
	dedaiSpaceMeshFace &face = pFaces[ faceIndex ];
	const int cornerCount = face.GetCornerCount();
	const int firstCorner = face.GetFirstCorner();
	int i;
	
	for( i=0; i<cornerCount; i++ ){
		dedaiSpaceMeshCorner &corner = pCorners[ firstCorner + i ];
		dedaiSpaceMeshEdge &edge = pEdges[ corner.GetEdge() ];
		
		if( edge.GetFace1() == faceIndex ){
			edge.SetFace1( edge.GetFace2() );
		}
		edge.SetFace2( -1 );
		
		corner.SetEnabled( false );
	}
	
	// disable the face. it will be no longer looked at during path finding
	face.SetEnabled( false );
}

void dedaiSpaceMesh::pAddConvexFaces( const dedaiConvexFaceList &list, const dedaiSpaceMeshFace &sourceFace ){
	// the AddFace call potentially moves memory invalidating sourceFace reference.
	// store all information required from it aside before doing this call
	const unsigned short typeNumber = sourceFace.GetTypeNumber();
	
	// sourceFace is now considered invalid. do not use it anymore
	const int faceCount = list.GetFaceCount();
	int i, j;
	
	for( i=0; i<faceCount; i++ ){
		const dedaiConvexFace &convexFace = *list.GetFaceAt( i );
		const int vertexCount = convexFace.GetVertexCount();
		
		// if there are not enough vertices ignore the face
		if( vertexCount < 3 ){
			continue;
		}
		
		// add a new face and init the parameters similar to InitFromNavSpace
		AddFace();
		dedaiSpaceMeshFace &newFace = pFaces[ pFaceCount - 1 ];
		
		newFace.SetTypeNumber( typeNumber );
		newFace.SetNormal( convexFace.GetNormal() );
		newFace.SetFirstCorner( pCornerCount );
		
		// add vertices and corners. the initial corner is set to have no edge since we set this
		// later. also init the remaining face parameters related to vertices. done here so we
		// do not need to loop a second time when we have the vertices in our hand already
		decVector center;
		decVector minExtend;
		decVector maxExtend;
		
		for( j=0; j<vertexCount; j++ ){
			const decVector &position = list.GetVertexAt( convexFace.GetVertexAt( j ) );
			
			int vertexIndex = IndexOfVertex( position );
			if( vertexIndex == -1 ){
				vertexIndex = pVertexCount;
				AddVertex( position );
			}
			
			AddCorner( ( unsigned short )vertexIndex, 0, CORNER_NO_COST );
			
			center += position;
			
			if( j == 0 ){
				minExtend = position;
				maxExtend = position;
				
			}else{
				minExtend.SetSmallest( position );
				maxExtend.SetLargest( position );
			}
		}
		
		newFace.SetExtends( minExtend, maxExtend );
		newFace.SetCenter( center / ( float )vertexCount );
		newFace.SetDistance( newFace.GetNormal() * newFace.GetCenter() );
		
		// add edges and update corners with them
		const int firstCorner = newFace.GetFirstCorner();
		
		for( j=0; j<vertexCount; j++ ){
			const unsigned short v1 = pCorners[ firstCorner + j ].GetVertex();
			const unsigned short v2 = pCorners[ firstCorner + ( j + 1 ) % vertexCount ].GetVertex();
			
			// find matching edge. if not found add a new one
			int edgeIndex = IndexOfEdgeMatching( v1, v2 );
			if( edgeIndex == -1 ){
				edgeIndex = pEdgeCount;
				AddEdge( v1, v2 );
			}
			
			// update corner with the found edge
			pCorners[ firstCorner + j ].SetEdge( ( unsigned short )edgeIndex );
			
			// update the faces on the edge to properly link the new face to the existing face
			dedaiSpaceMeshEdge &edge = pEdges[ edgeIndex ];
			
			if( edge.GetFace1() == -1 ){
				edge.SetFace1( pFaceCount - 1 );
				
			}else if( edge.GetFace2() == -1 ){
				edge.SetFace2( pFaceCount - 1 );
				
			}else{
				printf( "PROBLEM!\n" );
				//DETHROW( deeInvalidParam );
			}
		}
		
		// update the corner count. done at the end to make sure all visible corners are properly
		// initialized even in the case of an exception
		newFace.SetCornerCount( ( unsigned short )vertexCount );
	}
}

void dedaiSpaceMesh::pLinkToMesh( dedaiSpaceMesh *mesh, float snapDistance, float snapAngle ){
	const float snapDistance2 = mesh->GetSpace().GetSnapDistance();
	const float snapAngle2 = mesh->GetSpace().GetSnapAngle();
	const decMatrix matrix = ( pSpace.GetMatrix() * mesh->GetSpace().GetInverseMatrix() ).ToMatrix();
	const decMatrix matrixInverse = matrix.Invert();
	const dedaiSpaceMeshEdge *edges = mesh->GetEdges();
	const dedaiSpaceMeshFace *faces = mesh->GetFaces();
	const decVector *vertices = mesh->GetVertices();
	int edgeCount = mesh->GetEdgeCount();
	int i, j;
	
	const float useSnapDistance = decMath::max( snapDistance, snapDistance2 );
	const float useSnapAngle = decMath::max( snapAngle, snapAngle2 );
	const bool useSplitting = ( pSpace.GetBlockerShape().GetCount() > 0 );
	
	// this is now a slow test. each edge of this navigation mesh that is part of only one face is
	// tested against all edges of the other mesh which are part of only one face. this is an N^2
	// test which is slow but this could be later on improved by using an octree or some other
	// kind of data structure
	for( i=0; i<pCornerCount; i++ ){
		if( ! pCorners[ i ].GetEnabled() ){
			continue;
		}
		if( pCorners[ i ].GetLink() != CORNER_NO_LINK ){
			continue;
		}
		
		dedaiSpaceMeshEdge &localEdge = pEdges[ pCorners[ i ].GetEdge() ];
		
		// edge has to be belong to exactly one face to be linkable. if the first face is -1 the
		// edge is disabled and if the second face is not -1 the edge belongs already to two faces
		if( localEdge.GetFace1() == -1 || localEdge.GetFace2() != -1 ){
			continue;
		}
		
		// retrieve the position of the vertices in this navigation mesh transformed into the
		// coordinate system of the navigation mesh to link to
		const short localFaceIndex = localEdge.GetFace1(); // splitting the edge potentially invalides the reference
		
		const decVector &localVertex1 = pVertices[ localEdge.GetVertex1() ];
		const decVector &localVertex2 = pVertices[ localEdge.GetVertex2() ];
		
		const decVector testVertex1( matrix * localVertex1 );
		const decVector testVertex2( matrix * localVertex2 );
		const decVector testEdgeDir( testVertex2 - testVertex1 );
		const float testEdgeDirInvLenSquared = 1.0f / testEdgeDir.LengthSquared();
		
		// test every edge if it matches the source vertices
		bool retestCorner = false;
		
		for( j=0; j<edgeCount; j++ ){
			const dedaiSpaceMeshEdge &partnerEdge = edges[ j ];
			
			// only look at edges which are linkable. follows the same rules as above
			if( partnerEdge.GetFace1() == -1 || partnerEdge.GetFace2() != -1 ){
				continue;
			}
			
			// test the vertices for matching the edge in either direction
			const decVector &partnerVertex1 = vertices[ partnerEdge.GetVertex1() ];
			const decVector &partnerVertex2 = vertices[ partnerEdge.GetVertex2() ];
			
			const bool matchesA1 = partnerVertex1.IsEqualTo( testVertex1, useSnapDistance );
			const bool matchesA2 = partnerVertex2.IsEqualTo( testVertex2, useSnapDistance );
			const bool matchesB1 = partnerVertex1.IsEqualTo( testVertex2, useSnapDistance );
			const bool matchesB2 = partnerVertex2.IsEqualTo( testVertex1, useSnapDistance );
			
			// if both vertices match the edge we have found a link partner so end the search
			if( ( matchesA1 && matchesA2 ) || ( matchesB1 && matchesB2 ) ){
				// check if the angle between the faces is inside the snap angle
				const decVector testNormal( matrix.TransformNormal( pFaces[ localEdge.GetFace1() ].GetNormal() ) );
				
				if( fabsf( testNormal * faces[ partnerEdge.GetFace1() ].GetNormal() ) >= useSnapAngle ){
					j = edgeCount; // edge would match but snap angle is too large so no linking
				}
				
				break; // end search always
			}
			
			// if splitting is possible for this mesh is possible
			if( ! useSplitting ){
				continue;
			}
			
			// if one vertex matches there is the possibility for the non-matching vertex to be
			// located on the local edge thus requiring a split. this situation and an edge
			// matching both vertices can not happen at the same time so finding a split edge
			// ends the search
			if( ( matchesA1 || matchesA2 || matchesB1 || matchesB2 ) ){
				// determine the split vertex
				const decVector &splitVertex = ( ( matchesA2 || matchesB2 ) ? partnerVertex1 : partnerVertex2 );
				
				// check if the second vertex is located on the local edge. if this is the case
				// we have to split the edge to get a matching edge as above
				const decVector splitOffset( splitVertex - testVertex1 );
				const float splitFactor = ( testEdgeDir * splitOffset ) * testEdgeDirInvLenSquared;
				
				if( ( splitOffset - testEdgeDir * splitFactor ).Length() <= useSnapDistance ){
					// there are two possible situations. in the first case the split vertex lies
					// inside the local edge. in this case the local edge has to be split
					if( splitFactor > 0.0f && splitFactor < 1.0f ){
						// the vertex does touch the edge so split the edge. the new edge is placed
						// at the end of the list and checked again later. the current edge matches
						// now the local edge we are looking for so add a link and exit
						const decVector localSplitVertex( localVertex1 * ( 1.0f - splitFactor ) + localVertex2 * splitFactor );
						pSplitEdge( pCorners[ i ].GetEdge(), localSplitVertex );
						
						// if vertex at the corner is matching the current (shortened) edge can be linked.
						// if though the next corner vertex is matching the split edge moved to the end of
						// the list has to be linked. retest the corner since references to edges are
						// potentially invalid now due to SplitEdge and new linking opportunities might
						// have surfaced
						retestCorner = true;
						break;
						
					// in the other case the split vertex can lay on the partner edge. in this case the partner
					// edge has to split instead of the local one. if this case is missed a two-split case
					// can be handled as a two single split cases with the second single split being missed
					// by splitting the partner edge the linking can be properly handled. we know if the first
					// or second test vertex matches with the edge. depending on this the split factor has to
					// be past the opposite vertex for the opposite edge to be inside the partner edge
					}else{
						const bool matchesTv1 = matchesA1 | matchesB2;
						const bool matchesTv2 = matchesA2 | matchesB1;
						
						if( ( matchesTv1 && splitFactor > 1.0f ) || ( matchesTv2 && splitFactor < 0.0f ) ){
							// the non matching test vertex is located inside the partner edge so split it. the new
							// edge is placed at the end of the list and checked again later. the current edge matches
							// now the local edge we are looking for so add a link and exit
							mesh->pSplitEdge( j, matchesTv1 ? testVertex2 : testVertex1 );
							
							// after splitting edges the vertices, edges and faces pointers are potentially invalid
							edges = mesh->GetEdges();
							faces = mesh->GetFaces();
							vertices = mesh->GetVertices();
							edgeCount = mesh->GetEdgeCount();
							
							// test edge again since it falls now into the one-vertex-matches case
							j--;
						}
					}
				}
				
			// no vertex matches. it is possible the edge is located on and inside the edge. if
			// this is the case we can produce a new match by cutting the other mesh edge two
			// times and using this new edge to connect to
			}else{
				const decVector partnerEdgeDir( partnerVertex2 - partnerVertex1 );
				const float partnerEdgeDirInvLenSquared = 1.0f / partnerEdgeDir.LengthSquared();
				
				const decVector splitOffset1( testVertex1 - partnerVertex1 );
				const decVector splitOffset2( testVertex2 - partnerVertex1 );
				const float splitFactor1 = ( partnerEdgeDir * splitOffset1 ) * partnerEdgeDirInvLenSquared;
				const float splitFactor2 = ( partnerEdgeDir * splitOffset2 ) * partnerEdgeDirInvLenSquared;
				
				const bool canSplit1 = ( ( splitFactor1 > 0.0f && splitFactor1 < 1.0f ) 
					&& ( ( splitOffset1 - partnerEdgeDir * splitFactor1 ).Length() <= useSnapDistance ) );
				const bool canSplit2 = ( ( splitFactor2 > 0.0f && splitFactor2 < 1.0f ) 
					&& ( ( splitOffset2 - partnerEdgeDir * splitFactor2 ).Length() <= useSnapDistance ) );
				
				// if both vertices are inside the partner edge do a double split and link to the edge
				if( canSplit1 && canSplit2 ){
					const decVector partnerSplitVertex1( partnerVertex1 * ( 1.0f - splitFactor1 ) + partnerVertex2 * splitFactor1 );
					const decVector partnerSplitVertex2( partnerVertex1 * ( 1.0f - splitFactor2 ) + partnerVertex2 * splitFactor2 );
					
					if( splitFactor1 > splitFactor2 ){
						mesh->pSplitEdge( j, partnerSplitVertex1 );
						mesh->pSplitEdge( j, partnerSplitVertex2 );
						
					}else{
						mesh->pSplitEdge( j, partnerSplitVertex2 );
						mesh->pSplitEdge( j, partnerSplitVertex1 );
					}
					
					// after adding edges the vertices, edges and faces pointers are potentially invalid
					edges = mesh->GetEdges();
					faces = mesh->GetFaces();
					vertices = mesh->GetVertices();
					edgeCount = mesh->GetEdgeCount();
					
					// split edge can be linked
					j = edgeCount - 1;
					break;
					
				// if one vertex is inside split edge and re-try this edge
				}else if( canSplit1 || canSplit2 ){
					const float splitFactor = canSplit1 ? splitFactor1 : splitFactor2;
					const decVector partnerSplitVertex( partnerVertex1 * ( 1.0f - splitFactor ) + partnerVertex2 * splitFactor );
					
					mesh->pSplitEdge( j, partnerSplitVertex );
					
					// after adding edges the vertices, edges and faces pointers are potentially invalid
					edges = mesh->GetEdges();
					faces = mesh->GetFaces();
					vertices = mesh->GetVertices();
					edgeCount = mesh->GetEdgeCount();
					
					// test edge again since it falls now into the one-vertex-matches case
					j--;
					
				// no match so try the next edge
				}
			}
		}
		
		// retest corner if requested
		if( retestCorner ){
			i--;
			continue;
		}
		
		// a matching edge has been found either due to direct matching or by splitting
		if( j < edgeCount ){
			// add link to partner face
			const dedaiSpaceMeshEdge &partnerEdge = edges[ j ];
			const dedaiSpaceMeshFace &partnerFace = faces[ partnerEdge.GetFace1() ];
			const int cornerIndex = partnerFace.IndexOfCornerWithEdge( j );
			
			// DEBUG
			if( HasLinkWith( mesh, partnerEdge.GetFace1(), cornerIndex ) ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g) -> (%g,%g,%g): "
					"partnerEdge.face1=%d cornerIndex=%d",  pSpace.GetPosition().x,
					pSpace.GetPosition().y, pSpace.GetPosition().z, mesh->GetSpace().GetPosition().x,
					mesh->GetSpace().GetPosition().y, mesh->GetSpace().GetPosition().z,
					partnerEdge.GetFace1(), cornerIndex );
				pSpace.GetDEAI().LogException( deeInvalidAction( __FILE__, __LINE__ ) );
				continue;
			}
			// DEBUG
			
			AddLink( mesh, partnerEdge.GetFace1(), cornerIndex, matrix );
			pCorners[ i ].SetLink( ( short )( pLinkCount - 1 ) );
			
			// if the partner corner does not have a link back to us yet add it
			dedaiSpaceMeshCorner &corner = mesh->GetCornerAt( partnerFace.GetFirstCorner() + cornerIndex );
			
			if( corner.GetLink() == CORNER_NO_LINK ){
				mesh->AddLink( this, localFaceIndex, i - pFaces[ localFaceIndex ].GetFirstCorner(), matrixInverse );
				corner.SetLink( ( short )( mesh->GetLinkCount() - 1 ) );
			}
		}
	}
	
// 	pVerifyInvariants();
}

void dedaiSpaceMesh::pSplitEdge( int edgeIndex, const decVector &splitVertex ){
	if( pEdges[ edgeIndex ].GetFace2() != -1 ){
		DETHROW( deeInvalidParam ); // split only allowed on edges belonging to 1 face
	}
	
	dedaiSpaceMeshFace &face = pFaces[ pEdges[ edgeIndex ].GetFace1() ];
	
	// add split vertex
	const int splitVertexIndex = pVertexCount;
	AddVertex( splitVertex );
	
	// add split edge. choose the correct second vertex matching the face winding order
	// WARNING AddEdge potentially invalidates edge references
	const int faceCornerIndex = face.IndexOfCornerWithEdge( edgeIndex );
	const int faceFirstCorner = face.GetFirstCorner();
	const int splitEdgeIndex = pEdgeCount;
	
	if( pEdges[ edgeIndex ].GetVertex1() == pCorners[ faceFirstCorner + faceCornerIndex ].GetVertex() ){
		AddEdge( splitVertexIndex, pEdges[ edgeIndex ].GetVertex2() );
		
	}else{
		AddEdge( splitVertexIndex, pEdges[ edgeIndex ].GetVertex1() );
	}
	
	pEdges[ splitEdgeIndex ].SetFace1( pEdges[ edgeIndex ].GetFace1() );
	
	// find the face with the first corner equal to our first corner plus corner count if
	// existing. only then adding a corner spills into the next face forcing moving it
	int moveFaceIndex = -1;
	int i;
	for( i=0; i<pFaceCount; i++ ){
		const dedaiSpaceMeshFace &testFace = pFaces[ i ];
		if( ! testFace.GetEnabled() ){
			continue;
		}
		
		if( testFace.GetFirstCorner() == faceFirstCorner + face.GetCornerCount() ){
			moveFaceIndex = i;
			break;
		}
	}
	
	// add vertex to the face the edge belongs to. since the corners are in a linear array move
	// the corners of the next face to the end of the corner list. this gives room for the
	// corners of the affected face to grow by one and potentially some more if another split
	// happens.
	if( moveFaceIndex != -1 ){
		dedaiSpaceMeshFace &nextFace = pFaces[ moveFaceIndex ];
		const int cornerCount = nextFace.GetCornerCount();
		const int firstCorner = nextFace.GetFirstCorner();
		const int newFirstCorner = pCornerCount;
		int i;
		
		for( i=0; i<cornerCount; i++ ){
			dedaiSpaceMeshCorner &corner = pCorners[ firstCorner + i ]; // AddCorner invalidates reference
			const short linkIndex = corner.GetLink();
			const int newCornerIndex = pCornerCount;
			
			corner.SetEnabled( false );
			
			AddCorner( corner.GetVertex(), corner.GetEdge(), corner.GetTypeNumber() );
			pCorners[ newCornerIndex ].SetLink( linkIndex );
			
			// foreign link does not have to be updated since face index and corner index
			// relative to face do not change
		}
		
		nextFace.SetFirstCorner( newFirstCorner );
	}
	
	// add a corner if no previous slot can be used
	const int faceCornerCount = face.GetCornerCount();
	if( faceFirstCorner + faceCornerCount == pCornerCount ){
		AddCorner( 0, 0, 0 ); // dummy data, will be replaced right after
	}
	
	// now add the vertex in the face at the correct place
	for( i=faceCornerCount; i>faceCornerIndex+1; i-- ){
		dedaiSpaceMeshCorner &cornerTo = pCorners[ faceFirstCorner + i ];
		cornerTo = pCorners[ faceFirstCorner + i - 1 ];
		
		// update foreign link to point to the new corner index
		const short link = cornerTo.GetLink();
		if( link != CORNER_NO_LINK ){
			pLinks[ link ].GetForeignLink().SetCorner( i );
		}
	}
	
	const dedaiSpaceMeshCorner &corner1 = pCorners[ faceFirstCorner + faceCornerIndex ];
	dedaiSpaceMeshCorner &corner2 = pCorners[ faceFirstCorner + faceCornerIndex + 1 ];
	corner2.SetEdge( ( unsigned short )splitEdgeIndex );
	corner2.SetVertex( ( unsigned short )splitVertexIndex );
	corner2.SetTypeNumber( corner1.GetTypeNumber() );
	corner2.SetLink( CORNER_NO_LINK );
	corner2.SetEnabled( true );
	
	if( pEdges[ edgeIndex ].GetVertex1() == corner1.GetVertex() ){
		pEdges[ edgeIndex ].SetVertex2( ( unsigned short )splitVertexIndex );
		
	}else{
		pEdges[ edgeIndex ].SetVertex1( ( unsigned short )splitVertexIndex );
	}
	
	face.SetCornerCount( faceCornerCount + 1 );
	
// 	pVerifyInvariants();
}



void dedaiSpaceMesh::pVerifyInvariants() const{
	int f, f2, c, c2, l, e, e2, v, v2;
	
	// all vertices are unique
	for( v=0; v<pVertexCount; v++ ){
		for( v2=v+1; v2<pVertexCount; v2++ ){
			if( pVertices[ v ].IsEqualTo( pVertices[ v2 ], THRESHOLD_EQUAL ) ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Vertices %i and %i are equal",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, v, v2 );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
		}
	}
	
	// all corners belonging to enabled faces have to be enabled
	for( f=0; f<pFaceCount; f++ ){
		const dedaiSpaceMeshFace &face = pFaces[ f ];
		if( ! face.GetEnabled() ){
			continue;
		}
		for( c=0; c<face.GetCornerCount(); c++ ){
			const dedaiSpaceMeshCorner &corner = pCorners[ face.GetFirstCorner() + c ];
			if( ! corner.GetEnabled() ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Corner %i not enabled although used in face %i",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, c, f );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
		}
	}
	
	// all corners not belonging to enabled faces have to be disabled
	for( c=0; c<pCornerCount; c++ ){
		const dedaiSpaceMeshCorner &corner = pCorners[ c ];
		if( ! corner.GetEnabled() ){
			continue;
		}
		
		for( f=0; f<pFaceCount; f++ ){
			const dedaiSpaceMeshFace &face = pFaces[ f ];
			if( ! face.GetEnabled() ){
				continue;
			}
			
			if( c >= face.GetFirstCorner() && c < face.GetFirstCorner() + face.GetCornerCount() ){
				break;
			}
		}
		
		if( f == pFaceCount ){
			pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Corner %i enabled although not used in a face",
				pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, c );
			pDebugPrint();
			DETHROW( deeInvalidParam );
		}
	}
	
	// all edges on enabled faces have to belong to the face
	for( f=0; f<pFaceCount; f++ ){
		const dedaiSpaceMeshFace &face = pFaces[ f ];
		if( ! face.GetEnabled() ){
			continue;
		}
		
		for( c=0; c<face.GetCornerCount(); c++ ){
			const dedaiSpaceMeshCorner &corner = pCorners[ face.GetFirstCorner() + c ];
			if( corner.GetEdge() >= pEdgeCount ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Face %i corner %i has invalid edge index %i",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, f, c, corner.GetEdge() );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
			
			const dedaiSpaceMeshEdge &edge = pEdges[ corner.GetEdge() ];
			if( edge.GetFace1() != f && edge.GetFace2() != f ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Edge %i face %i corner %i does not link to face",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, corner.GetEdge(), c, f );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
		}
	}
	
	// all edges not belonging to enabled corners have both faces set to -1
	for( e=0; e<pEdgeCount; e++ ){
		const dedaiSpaceMeshEdge &edge = pEdges[ e ];
		
		for( c=0; c<pCornerCount; c++ ){
			const dedaiSpaceMeshCorner &corner = pCorners[ c ];
			if( ! corner.GetEnabled() ){
				continue;
			}
			if( corner.GetEdge() == e ){
				break;
			}
		}
		
		if( c != pCornerCount ){
			continue;
		}
		
		if( edge.GetFace1() != -1 ){
			pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Edge %i has face1 not -1 although not used by a corner",
				pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, e );
			pDebugPrint();
			DETHROW( deeInvalidParam );
		}
		if( edge.GetFace2() != -1 ){
			pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Edge %i has face2 not -1 although not used by a corner",
				pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, e );
			pDebugPrint();
			DETHROW( deeInvalidParam );
		}
	}
	
	// all edges are unique
	for( e=0; e<pEdgeCount; e++ ){
		const dedaiSpaceMeshEdge &edge = pEdges[ e ];
		
		for( e2=e+1; e2<pEdgeCount; e2++ ){
			const dedaiSpaceMeshEdge &edge2 = pEdges[ e2 ];
			if( ( edge.GetVertex1() == edge2.GetVertex1() && edge.GetVertex2() == edge2.GetVertex2() )
			|| ( edge.GetVertex1() == edge2.GetVertex2() && edge.GetVertex2() == edge2.GetVertex1() ) ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Edge %i and %i have same vertices",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, e, e2 );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
		}
	}
	
	// all enabled faces have unique corner ranges
	for( f=0; f<pFaceCount; f++ ){
		const dedaiSpaceMeshFace &face = pFaces[ f ];
		if( ! face.GetEnabled() ){
			continue;
		}
		
		for( f2=f+1; f2<pFaceCount; f2++ ){
			const dedaiSpaceMeshFace &face2 = pFaces[ f2 ];
			if( ! face2.GetEnabled() ){
				continue;
			}
			
			const int a1 = face.GetFirstCorner();
			const int b1 = face2.GetFirstCorner();
			const int a2 = a1 + face.GetCornerCount();
			const int b2 = b1 + face2.GetCornerCount();
			
			if( ( b1 < a2 && b2 > a1 ) || ( a1 < b2 && a2 > b1 ) ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Faces %i and %i overlap corner range",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, f, f2 );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
		}
	}
	
	// all enabled faces have no duplicate vertices
	for( f=0; f<pFaceCount; f++ ){
		const dedaiSpaceMeshFace &face = pFaces[ f ];
		if( ! face.GetEnabled() ){
			continue;
		}
		
		for( c=0; c<face.GetCornerCount(); c++ ){
			const int vertex = pCorners[ face.GetFirstCorner() + c ].GetVertex();
			
			for( c2=c+1; c2<face.GetCornerCount(); c2++ ){
				if( vertex == pCorners[ face.GetFirstCorner() + c2 ].GetVertex() ){
					pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Face %i has duplicate vertex %i",
						pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, f, vertex );
					pDebugPrint();
					DETHROW( deeInvalidParam );
				}
			}
		}
	}
	
	// edge loop in all enabled edges is correct
	for( f=0; f<pFaceCount; f++ ){
		const dedaiSpaceMeshFace &face = pFaces[ f ];
		if( ! face.GetEnabled() ){
			continue;
		}
		
		for( c=0; c<face.GetCornerCount(); c++ ){
			const dedaiSpaceMeshEdge &edge = pEdges[ pCorners[ face.GetFirstCorner() + c ].GetEdge() ];
			const dedaiSpaceMeshEdge &edge2 = pEdges[ pCorners[ face.GetFirstCorner() + ( ( c + 1 ) % face.GetCornerCount() ) ].GetEdge() ];
			
			if( edge.GetVertex1() != edge2.GetVertex1() && edge.GetVertex1() != edge2.GetVertex2()
			&& edge.GetVertex2() != edge2.GetVertex1() && edge.GetVertex2() != edge2.GetVertex2() ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Face %i edge loop %i/%i broken",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, f,
						pCorners[ face.GetFirstCorner() + c ].GetEdge(),
						pCorners[ face.GetFirstCorner() + ( ( c + 1 ) % face.GetCornerCount() ) ].GetEdge() );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
		}
	}
	
	// all edges assigned to faces do not contain a colinear vertex except the edge vertices
	for( e=0; e<pEdgeCount; e++ ){
		const dedaiSpaceMeshEdge &edge = pEdges[ e ];
		if( edge.GetFace1() == -1 && edge.GetFace2() == -1 ){
			continue;
		}
		
		const decVector &edgeStart = pVertices[ edge.GetVertex1() ];
		const decVector edgeDir( pVertices[ edge.GetVertex2() ] - edgeStart );
		const float edgeLenInvSquared = 1.0f / edgeDir.LengthSquared();
		
		for( v=0; v<pVertexCount; v++ ){
			if( v == edge.GetVertex1() || v == edge.GetVertex2() ){
				continue;
			}
			
			const float lambda = decMath::clamp( ( ( pVertices[ v ] - edgeStart ) * edgeDir ) * edgeLenInvSquared, 0.0f, 1.0f );
			const decVector projection( edgeStart + edgeDir * lambda );
			if( ( pVertices[ v ] - projection ).Length() < THRESHOLD_EQUAL ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Edge %i has co-linear vertex %i not part of the edge",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, e, v );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
		}
	}
	
	// all links are valid
	for( l=0; l<pLinkCount; l++ ){
		const dedaiSpaceMeshLink &link = pLinks[ l ];
		if( ! link.GetMesh() ){
			pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Link %i has NULL partner mesh",
				pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, l );
			pDebugPrint();
			DETHROW( deeInvalidParam );
		}
		
		const dedaiSpaceMesh &mesh = *link.GetMesh();
		if( link.GetFace() >= mesh.GetFaceCount() ){
			pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Link %i has invalid face index %i",
				pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, l, link.GetFace() );
			pDebugPrint();
			DETHROW( deeInvalidParam );
		}
		
		const dedaiSpaceMeshFace &face = mesh.GetFaceAt( link.GetFace() );
		if( ! face.GetEnabled() ){
			pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Link %i face %i is disabled",
				pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, l, link.GetFace() );
			pDebugPrint();
			DETHROW( deeInvalidParam );
		}
		
		if( link.GetCorner() >= face.GetCornerCount() ){
			pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Link %i has invalid corner %i",
				pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, l, link.GetCorner() );
			pDebugPrint();
			DETHROW( deeInvalidParam );
		}
	}
	
	// all enabled corners have valid links
	for( f=0; f<pFaceCount; f++ ){
		const dedaiSpaceMeshFace &face = pFaces[ f ];
		if( ! face.GetEnabled() ){
			continue;
		}
		
		for( c=0; c<face.GetCornerCount(); c++ ){
			const dedaiSpaceMeshCorner &corner = pCorners[ face.GetFirstCorner() + c ];
			if( corner.GetLink() == CORNER_NO_LINK ){
				continue;
			}
			if( corner.GetLink() < 0 || corner.GetLink() >= pLinkCount ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Corner %i has invalid link %i",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, c, corner.GetLink() );
				pDebugPrint();
				DETHROW( deeInvalidParam );
			}
			
			const dedaiSpaceMesh &mesh = *pLinks[ corner.GetLink() ].GetMesh();
			
			if( ! mesh.HasLinkWith( ( dedaiSpaceMesh* )this, ( unsigned short )f, ( unsigned short )c ) ){
				pSpace.GetDEAI().LogErrorFormat( "Space (%g,%g,%g): Face %i corner %i link has no partner link",
					pSpace.GetPosition().x, pSpace.GetPosition().y, pSpace.GetPosition().z, f, c );
				pDebugPrint();
				mesh.pDebugPrint();
				DETHROW( deeInvalidParam );
			}
		}
	}
}

void dedaiSpaceMesh::pDebugPrint() const{
	const decDVector &position = pSpace.GetPosition();
	int i;
	
	if( pSpace.GetOwnerNavSpace() ){
		pSpace.GetDEAI().LogErrorFormat( "NavSpace Mesh %p (%g,%g,%g):", this, position.x, position.y, position.z );
		
	}else{
		pSpace.GetDEAI().LogErrorFormat( "HTNavSpace Mesh %p (%g,%g,%g):", this, position.x, position.y, position.z );
	}
	
	for( i=0; i<pVertexCount; i++ ){
		pSpace.GetDEAI().LogErrorFormat( "- Vertex %i: (%g,%g,%g)", i,
			pVertices[ i ].x, pVertices[ i ].y, pVertices[ i ].z );
	}
	for( i=0; i<pCornerCount; i++ ){
		pSpace.GetDEAI().LogErrorFormat( "- Corner %i: vertex=%i edge=%i type=%i link=%i enabled=%d", i,
			pCorners[ i ].GetVertex(), pCorners[ i ].GetEdge(), pCorners[ i ].GetTypeNumber(),
			pCorners[ i ].GetLink(), pCorners[ i ].GetEnabled() );
	}
	for( i=0; i<pEdgeCount; i++ ){
		pSpace.GetDEAI().LogErrorFormat( "- Edge %i: vertex1=%i vertex2=%i face1=%i face2=%d", i,
			pEdges[ i ].GetVertex1(), pEdges[ i ].GetVertex2(), pEdges[ i ].GetFace1(), pEdges[ i ].GetFace2() );
	}
	for( i=0; i<pFaceCount; i++ ){
		pSpace.GetDEAI().LogErrorFormat( "- Face %i: corners=(%i->%i(%i)) enabled=%d", i,
			pFaces[ i ].GetFirstCorner(), pFaces[ i ].GetFirstCorner() + pFaces[ i ].GetCornerCount() - 1,
			pFaces[ i ].GetCornerCount(), pFaces[ i ].GetEnabled() );
	}
	for( i=0; i<pLinkCount; i++ ){
		pSpace.GetDEAI().LogErrorFormat( "- Link %i: mesh=%p face=%i corner=%i", i,
			pLinks[ i ].GetMesh(), pLinks[ i ].GetFace(), pLinks[ i ].GetCorner() );
	}
}
