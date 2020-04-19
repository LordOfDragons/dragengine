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

#include "dedaiPathFinderNavMesh.h"
#include "dedaiPathFinderFunnel.h"
#include "dedaiPathFinderPointList.h"
#include "../dedaiNavigator.h"
#include "../costs/dedaiCostTable.h"
#include "../spaces/dedaiSpace.h"
#include "../spaces/mesh/dedaiSpaceMesh.h"
#include "../spaces/mesh/dedaiSpaceMeshCorner.h"
#include "../spaces/mesh/dedaiSpaceMeshEdge.h"
#include "../spaces/mesh/dedaiSpaceMeshFace.h"
#include "../spaces/mesh/dedaiSpaceMeshLink.h"
#include "../layer/dedaiLayer.h"
#include "../../deDEAIModule.h"
#include "../../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/debug/deDebugDrawerShape.h>
#include <dragengine/resources/debug/deDebugDrawerShapeFace.h>
#include <dragengine/resources/navigation/navigator/deNavigator.h>
#include <dragengine/resources/world/deWorld.h>



/* NOTES
 * =====
 * 
 * nav-mesh: each face has a type number. defines what kind of cost to use in a generic way. game
 *           simply defines types and assigns them a number. type numbers have to be unique across
 *           all nav-meshes in the same world.
 * navigator: list of type costs <typeNumber,costFix,costPerMeter>. for each face the type numer
 *            is checked and the matching cost used. the costFix defines the fix cost for traveling
 *            through this face and costPerMeter a relative cost. the final cost for a face is thus
 *            cost = costFix + costPerMeter * distance.
 *            also cost multipliers for steepness (angle of face) and other modifiers.
 *            fog-of-war infos.
 */

// #define DEBUG 1



// Class dedaiPathFinderNavMesh
/////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiPathFinderNavMesh::dedaiPathFinderNavMesh() :
pWorld( NULL ),
pNavigator( NULL ),

pStartFace( NULL ),
pEndFace( NULL ),

pPathPoints( NULL ),
pPathPointCount( 0 ),
pPathPointSize( 0 ),

pDDSListOpen( NULL ),
pDDSListClosed( NULL ){
}

dedaiPathFinderNavMesh::~dedaiPathFinderNavMesh(){
	if( pPathPoints ){
		delete [] pPathPoints;
	}
}



// Management
///////////////

void dedaiPathFinderNavMesh::SetWorld( dedaiWorld *world ){
	pWorld = world;
}

void dedaiPathFinderNavMesh::SetNavigator( dedaiNavigator *navigator ){
	pNavigator = navigator;
}

void dedaiPathFinderNavMesh::SetStartPoint( const decDVector &point ){
	pStartPoint = point;
}

void dedaiPathFinderNavMesh::SetEndPoint( const decDVector &point ){
	pEndPoint = point;
}



void dedaiPathFinderNavMesh::FindPath(){
	if( ! pNavigator || ! pWorld ){
		return;
	}
	
	try{
		pFindFacePath();
		pFindRealPath();
		pUpdateDDSListOpen();
		pUpdateDDSListClosed();
		pClearLists();
		
	}catch( const deException & ){
		pClearLists();
		throw;
	}
}



void dedaiPathFinderNavMesh::AddPathPoint( const decDVector &point ){
	if( pPathPointCount == pPathPointSize ){
		int newSize = pPathPointCount + 10;
		decDVector *newArray = new decDVector[ newSize ];
		if( pPathPoints ){
			memcpy( newArray, pPathPoints, sizeof( decDVector ) * pPathPointCount );
			delete [] pPathPoints;
		}
		pPathPoints = newArray;
		pPathPointSize = newSize;
	}
	
	pPathPoints[ pPathPointCount++ ] = point;
}

void dedaiPathFinderNavMesh::RemoveAllPathPoints(){
	pPathPointCount = 0;
}



void dedaiPathFinderNavMesh::UpdateDDSShapeFaces( deDebugDrawerShape &dds ){
	dds.RemoveAllFaces();
	dds.GetShapeList().RemoveAll();
	
	if( pPathFaces.GetCount() > 0 ){
		const decDMatrix matrix = decDMatrix::CreateTranslation( -pStartPoint );
		deDebugDrawerShapeFace *ddsFace = NULL;
		const int count = pPathFaces.GetCount();
		int i, j;
		
		try{
			for( i=0; i<count; i++ ){
				const dedaiSpaceMeshFace &face = *( ( dedaiSpaceMeshFace* )pPathFaces.GetAt( i ) );
				const unsigned short cornerCount = face.GetCornerCount();
				
				if( cornerCount > 2 ){
					const decMatrix transform = ( face.GetMesh()->GetSpace().GetMatrix() * matrix ).ToMatrix();
					const dedaiSpaceMeshCorner * const corners = face.GetMesh()->GetCorners();
					const decVector * const vertices = face.GetMesh()->GetVertices();
					const int firstCorner = face.GetFirstCorner();
					
					ddsFace = new deDebugDrawerShapeFace;
					for( j=0; j<cornerCount; j++ ){
						ddsFace->AddVertex( transform * vertices[ corners[ firstCorner + j ].GetVertex() ] );
					}
					ddsFace->SetNormal( face.GetNormal() );
					dds.AddFace( ddsFace );
					ddsFace = NULL;
				}
			}
			
		}catch( const deException & ){
			if( ddsFace ){
				delete ddsFace;
			}
			throw;
		}
	}
}



void dedaiPathFinderNavMesh::SetDDSListOpen( deDebugDrawerShape* dds ){
	pDDSListOpen = dds;
}

void dedaiPathFinderNavMesh::SetDDSListClosed( deDebugDrawerShape *dds ){
	pDDSListClosed = dds;
}




// Private Functions
//////////////////////

void dedaiPathFinderNavMesh::pClearLists(){
	int i, count;
	
	count = pListClosed.GetCount();
	for( i=0; i<count; i++ ){
		( ( dedaiSpaceMeshFace* )pListClosed.GetAt( i ) )->ClearPath();
	}
	pListClosed.RemoveAll();
	
	count = pListOpen.GetCount();
	for( i=0; i<count; i++ ){
		( ( dedaiSpaceMeshFace* )pListOpen.GetAt( i ) )->ClearPath();
	}
	pListOpen.RemoveAll();
}

void dedaiPathFinderNavMesh::pFindFacePath(){
	const bool improvedSearchMode = false;
	
	const float maxOutsideDistance = pNavigator->GetNavigator().GetMaxOutsideDistance();
	const float blockingCost = pNavigator->GetNavigator().GetBlockingCost();
	dedaiSpaceMeshFace *testFace, *nextFace;
	float fixCost, costPerMeter;
	unsigned short c, endCorner;
// 	unsigned short linkedCorner;
	float distance = 0.0f;
	decVector entryPoint;
	float gcost = 0.0f;
	int f, faceCount;
	
	pPathFaces.RemoveAll();
	
	pStartFace = pNavigator->GetLayer()->GetMeshFaceClosestTo( pStartPoint, distance );
	
	if( distance > maxOutsideDistance ){
		pStartFace = NULL;
		pEndFace = NULL;
		
	}else{
		pEndFace = pNavigator->GetLayer()->GetMeshFaceClosestTo( pEndPoint, distance );
		
		if( distance > maxOutsideDistance ){
			pStartFace = NULL;
			pEndFace = NULL;
		}
	}
	
#ifdef DEBUG
	deDEAIModule &module = pWorld->GetDEAI();
	module.LogInfo( "Find Path:" );
	if( pStartFace ){
		const decDVector c = pStartFace->GetMesh()->GetSpace().GetMatrix() * pStartFace->GetCenter();
		module.LogInfoFormat( "   Start: nm=%p f=%i (%.3f,%.3f,%.3f)", pStartFace->GetMesh(),
			pStartFace->GetIndex(), c.x, c.y, c.z );
	}else{
		module.LogInfoFormat( "   Start: nm=NULL" );
	}
	if( pEndFace ){
		const decDVector c = pEndFace->GetMesh()->GetSpace().GetMatrix() * pEndFace->GetCenter();
		module.LogInfoFormat( "   End: nm=%p f=%i (%.3f,%.3f,%.3f)", pEndFace->GetMesh(),
			pEndFace->GetIndex(), c.x, c.y, c.z );
	}else{
		module.LogInfoFormat( "   End: nm=NULL" );
	}
#endif
	
	if( pEndFace != pStartFace ){
		const decDVector targetEnd = pEndFace->GetMesh()->GetSpace().GetMatrix() * pEndFace->GetCenter();
		const decDVector targetStart = pStartFace->GetMesh()->GetSpace().GetMatrix() * pStartFace->GetCenter();
		
		pListOpen.Add( pStartFace );
		pStartFace->SetPathParent( NULL );
		pStartFace->SetPathCostG( 0.0f );
		pStartFace->SetPathCostH( ( float )( ( targetEnd - targetStart ).Length() ) );
		pStartFace->SetPathCostF( pStartFace->GetPathCostH() );
		pStartFace->SetPathType( dedaiSpaceMeshFace::epftOpen );
		if( improvedSearchMode ){
			pStartFace->SetEntryPoint( ( pStartFace->GetMesh()->GetSpace().GetInverseMatrix() * pStartPoint ).ToVector() );
		}
#ifdef DEBUG
		{ const decDVector c = pStartFace->GetMesh()->GetSpace().GetMatrix() * pStartFace->GetCenter();
		module.LogInfoFormat( "   Open Add Face: nm=%p f=%i (%.3f,%.3f,%.3f)", pStartFace->GetMesh(),
			pStartFace->GetIndex(), c.x, c.y, c.z ); }
#endif
		testFace = pStartFace;
		
		while( testFace ){
#ifdef DEBUG
			{ const decDVector c = testFace->GetMesh()->GetSpace().GetMatrix() * testFace->GetCenter();
			module.LogInfoFormat( "   Testing Face: nm=%p f=%i (%.3f,%.3f,%.3f)", testFace->GetMesh(),
				testFace->GetIndex(), c.x, c.y, c.z ); }
#endif
			const dedaiSpaceMeshCorner * const corners = testFace->GetMesh()->GetCorners();
			const dedaiSpaceMeshEdge * const edges = testFace->GetMesh()->GetEdges();
			const dedaiSpaceMeshLink * const links = testFace->GetMesh()->GetLinks();
			dedaiSpaceMeshFace * const faces = testFace->GetMesh()->GetFaces();
			
			endCorner = testFace->GetFirstCorner() + testFace->GetCornerCount();
			
			for( c=testFace->GetFirstCorner(); c<endCorner; c++ ){
				const dedaiSpaceMeshCorner &corner = corners[ c ];
				const dedaiSpaceMeshEdge &edge = edges[ corner.GetEdge() ];
				
				// determine if this edge leads somewhere. this can be inside the same navigation mesh if the edge has
				// a second face assigned or inside another navigation mesh if the corner has a link
				nextFace = NULL;
// 				linkedCorner = 0;
				
				if( edge.GetFace2() == -1 ){
					if( corner.GetLink() != -1 ){
						const dedaiSpaceMeshLink &link = links[ corner.GetLink() ];
						
						nextFace = link.GetMesh()->GetFaces() + link.GetFace();
// 						linkedCorner = link.GetCorner();
					}
					
				}else{
					if( edge.GetFace1() == testFace->GetIndex() ){
						nextFace = faces + edge.GetFace2();
						
					}else{
						nextFace = faces + edge.GetFace1();
					}
				}
				
				// path can continue here if there is a next face
				if( nextFace ){
					// calculate costs only if the face is not on the closed list
					if( nextFace->GetPathType() != dedaiSpaceMeshFace::epftClosed ){
						pNavigator->GetCostParametersFor( nextFace->GetTypeNumber(), fixCost, costPerMeter );
						gcost = testFace->GetPathCostG();
						
						// apply fix cost only if the next face has a different type number. if applied always
						// split faces apply the fix cost multiple times falsifying the result. forcing the
						// rule to apply the fix cost only on type number changes is the only correct way
						if( nextFace->GetTypeNumber() != testFace->GetTypeNumber() ){
							gcost += fixCost;
						}
						
						/*
						// this is not correct (and a bad idea). if faces are split the movement cost increases
						// due to crossing additional edges. this shifts the favor to non-split faces or path
						// where larger faces without cuts in them are located. besides handling corner type
						// assignment in 3d modeling applications is a problem too. so drop it altogether.
						
						if( corner.GetTypeNumber() != CORNER_NO_COST ){
							gcost += pNavigator->GetFixCostFor( corner.GetTypeNumber() );
						}
						*/
						
						// apply cost per meter. applying this always works correctly with split faces. the
						// original algorithm uses the distance between face centers. this works well if the
						// faces all are similar in shape and size. in the geneal case though this leads to
						// wrong results and bad initial path choice. the situation can be improved by not
						// using the face center but a point on the entry edge along the connection line
						// between the two face centers. correct calculation requires intersecting this line
						// with a plane along the edge oriented towards the exit face center or comparing the
						// angles between the this line and the edge corners. both solutions are time consuming
						// and in the end we only need some point on the edge located around the correct point
						// to obtain a better result. in this case a simple and fast approximation can be used.
						// both face centers are projected onto the edge and averaged. the result is clamped
						// to the edge. this point is good enough to obtain better results at little cost.
						// this can even go as simple as using the center of the edge, it still works better
						// than using the face center. the resulting point is stored in the face and used
						// instead of the face center for future cost calculations. for the starting face the
						// entry point is set to the start point. if the face parent changes the entry point
						// is updated too
						// 
						// NOTE using the center of the edge has a nice additional affect over all other
						//      solutions in that the entry point can be calculate across different spaces
						//      without taking the detour over world space conversation using matrices
						if( testFace->GetMesh() == nextFace->GetMesh() ){
							if( improvedSearchMode ){
								const decVector &edgeV1 = testFace->GetMesh()->GetVertices()[ edge.GetVertex1() ];
								const decVector &edgeV2 = testFace->GetMesh()->GetVertices()[ edge.GetVertex2() ];
								decVector edgeDir( edgeV2 - edgeV1 );
								const float edgeLen = edgeDir.Length();
								edgeDir /= edgeLen;
								//entryPoint = edgeV1 + edgeDir * decMath::clamp(
								//	( edgeDir * ( testFace->GetEntryPoint() - edgeV1 )
								//	+ edgeDir * ( nextFace->GetCenter() - edgeV1 ) ) * 0.5f, 0.0f, edgeLen );
								entryPoint = edgeV1 + edgeDir * decMath::clamp(
									edgeDir * ( ( testFace->GetEntryPoint() + nextFace->GetCenter() ) * 0.5f ), 0.0f, edgeLen );
								
								/*
								const decVector &edgeV1 = testFace->GetMesh()->GetVertices()[ edge.GetVertex1() ];
								const decVector &edgeV2 = testFace->GetMesh()->GetVertices()[ edge.GetVertex2() ];
								entryPoint = ( edgeV2 - edgeV1 ) * 0.5f;
								*/
								
							}else{
								gcost += costPerMeter * ( nextFace->GetCenter() - testFace->GetCenter() ).Length();
							}
							
						}else{
							if( improvedSearchMode ){
								const decDVector nextPoint( testFace->GetMesh()->GetSpace().GetInverseMatrix() *
									( nextFace->GetMesh()->GetSpace().GetMatrix() * nextFace->GetCenter() ) );
								const decVector &edgeV1 = testFace->GetMesh()->GetVertices()[ edge.GetVertex1() ];
								const decVector &edgeV2 = testFace->GetMesh()->GetVertices()[ edge.GetVertex2() ];
								decVector edgeDir( edgeV2 - edgeV1 );
								const float edgeLen = edgeDir.Length();
								edgeDir /= edgeLen;
								entryPoint = edgeV1 + edgeDir * decMath::clamp(
									edgeDir * ( ( testFace->GetEntryPoint() + nextPoint ) * 0.5f ), 0.0f, edgeLen );
								
								/*
								const dedaiSpaceMeshEdge &linkedEdge = nextFace->GetMesh()->GetEdges()[
									nextFace->GetMesh()->GetCorners()[ nextFace->GetFirstCorner() + linkedCorner ].GetEdge() ];
								const decVector &edgeV1 = nextFace->GetMesh()->GetVertices()[ linkedEdge.GetVertex1() ];
								const decVector &edgeV2 = nextFace->GetMesh()->GetVertices()[ linkedEdge.GetVertex2() ];
								entryPoint = ( edgeV2 - edgeV1 ) * 0.5f;
								*/
								
							}else{
								const decDVector testFaceCenter = testFace->GetMesh()->GetSpace().GetMatrix() * testFace->GetCenter();
								const decDVector nextFaceCenter = nextFace->GetMesh()->GetSpace().GetMatrix() * nextFace->GetCenter();
								gcost += costPerMeter * ( float )( ( nextFaceCenter - testFaceCenter ).Length() );
							}
						}
					}
					
					// add it to the list if not on one already
					if( nextFace->GetPathType() == dedaiSpaceMeshFace::epftFree ){
						nextFace->SetPathParent( testFace );
						nextFace->SetPathCostG( gcost );
						if( improvedSearchMode ){
							nextFace->SetEntryPoint( entryPoint );
							const decDVector testFaceCenter = testFace->GetMesh()->GetSpace().GetMatrix() * entryPoint;
							nextFace->SetPathCostH( ( float )( ( targetEnd - testFaceCenter ).Length() ) );
						}else{
							const decDVector testFaceCenter = testFace->GetMesh()->GetSpace().GetMatrix() * testFace->GetCenter();
							nextFace->SetPathCostH( ( float )( ( targetEnd - testFaceCenter ).Length() ) );
						}
						nextFace->SetPathCostF( gcost + nextFace->GetPathCostH() );
						
						// add to open list if the cost is not larger than the blocking cost
						if( nextFace->GetPathCostF() < blockingCost ){
							pListOpen.Add( nextFace );
							nextFace->SetPathType( dedaiSpaceMeshFace::epftOpen );
#ifdef DEBUG
							{ const decDVector c = nextFace->GetMesh()->GetSpace().GetMatrix() * nextFace->GetCenter();
							module.LogInfoFormat( "   Open Add Face: %p:%i (p=%p:%i c=(%g,%g,%g) t=%i) (%.3f,%.3f,%.3f)",
								nextFace->GetMesh(), nextFace->GetIndex(), testFace->GetMesh(), testFace->GetIndex(),
								nextFace->GetPathCostF(), nextFace->GetPathCostG(), nextFace->GetPathCostH(),
								nextFace->GetTypeNumber(), c.x, c.y, c.z ); }
#endif
						// if the cost is larger than the blocking cost this face can not be crossed. in this case
						// add it to the closed list so it is not tested anymore in the future
						}else{
							pListClosed.Add( nextFace );
							nextFace->SetPathType( dedaiSpaceMeshFace::epftClosed );
#ifdef DEBUG
							{ const decDVector c = nextFace->GetMesh()->GetSpace().GetMatrix() * nextFace->GetCenter();
							module.LogInfoFormat( "   Blocking: Closed Add Face: %p:%i (p=%p:%i c=(%g,%g,%g) t=%i) (%.3f,%.3f,%.3f)",
								nextFace->GetMesh(), nextFace->GetIndex(), testFace->GetMesh(), testFace->GetIndex(),
								nextFace->GetPathCostF(), nextFace->GetPathCostG(), nextFace->GetPathCostH(),
								nextFace->GetTypeNumber(), c.x, c.y, c.z ); }
#endif
						}
						
					}else if( nextFace->GetPathType() == dedaiSpaceMeshFace::epftOpen ){
						if( gcost < nextFace->GetPathCostG() ){
							nextFace->SetPathParent( testFace );
							nextFace->SetPathCostG( gcost );
							nextFace->SetPathCostF( gcost + nextFace->GetPathCostH() );
							if( improvedSearchMode ){
								nextFace->SetEntryPoint( entryPoint );
							}
#ifdef DEBUG
							{ const decDVector c = nextFace->GetMesh()->GetSpace().GetMatrix() * nextFace->GetCenter();
							module.LogInfoFormat( "   Improve Parent Path: %p:%i (p=%p:%i c=(%g,%g,%g) t=%i) (%.3f,%.3f,%.3f)",
								nextFace->GetMesh(), nextFace->GetIndex(), testFace->GetMesh(), testFace->GetIndex(),
								nextFace->GetPathCostF(), nextFace->GetPathCostG(), nextFace->GetPathCostH(),
								nextFace->GetTypeNumber(), c.x, c.y, c.z ); }
#endif
						}
					}
				}
			}
			
			pListOpen.RemoveFrom( pListOpen.IndexOf( testFace ) );
#ifdef DEBUG
			{ const decDVector c = testFace->GetMesh()->GetSpace().GetMatrix() * testFace->GetCenter();
			module.LogInfoFormat( "   Open Remove Face: %p:%i (p=%p:%i c=(%g,%g,%g)) (%.3f,%.3f,%.3f)",
				testFace->GetMesh(), testFace->GetIndex(), testFace->GetPathParent()?testFace->GetPathParent()->GetMesh():NULL,
			  testFace->GetPathParent()?testFace->GetPathParent()->GetIndex():-1, testFace->GetPathCostF(),
			  testFace->GetPathCostG(), testFace->GetPathCostH(), c.x, c.y, c.z ); }
#endif
			pListClosed.Add( testFace );
#ifdef DEBUG
			{ const decDVector c = testFace->GetMesh()->GetSpace().GetMatrix() * testFace->GetCenter();
			module.LogInfoFormat( "   Closed Add Face: %p:%i (p=%p:%i c=(%g,%g,%g)) (%.3f,%.3f,%.3f)",
				testFace->GetMesh(), testFace->GetIndex(), testFace->GetPathParent()?testFace->GetPathParent()->GetMesh():NULL,
			  testFace->GetPathParent()?testFace->GetPathParent()->GetIndex():-1, testFace->GetPathCostF(),
			  testFace->GetPathCostG(), testFace->GetPathCostH(), c.x, c.y, c.z ); }
#endif
			
			if( testFace == pEndFace ){
				break;
			}
			
			faceCount = pListOpen.GetCount();
			if( faceCount == 0 ){
				testFace = NULL;
				
			}else{
				testFace = ( dedaiSpaceMeshFace* )pListOpen.GetAt( faceCount - 1 );
				for( f=faceCount-2; f>=0; f-- ){
					nextFace = ( dedaiSpaceMeshFace* )pListOpen.GetAt( f );
					
					if( nextFace->GetPathCostF() < testFace->GetPathCostF() ){
						testFace = nextFace;
					}
				}
			}
		}
		
#ifdef DEBUG
		module.LogInfo( "   Debug:" );
		module.LogInfo( "      Open List:" );
		for( c=0; c<pListOpen.GetCount(); c++ ){
			testFace = ( dedaiSpaceMeshFace* )pListOpen.GetAt( c );
			const decDVector c = testFace->GetMesh()->GetSpace().GetMatrix() * testFace->GetCenter();
			module.LogInfoFormat( "         Face: %p:%i p=%p:%i c=(%g,%g,%g) (%.3f,%.3f,%.3f)",
				testFace->GetMesh(), testFace->GetIndex(), testFace->GetPathParent()?testFace->GetPathParent()->GetMesh():NULL,
				testFace->GetPathParent()?testFace->GetPathParent()->GetIndex():-1, testFace->GetPathCostF(),
				testFace->GetPathCostG(), testFace->GetPathCostH(), c.x, c.y, c.z );
		}
		module.LogInfo( "      Closed List:" );
		for( c=0; c<pListClosed.GetCount(); c++ ){
			testFace = ( dedaiSpaceMeshFace* )pListClosed.GetAt( c );
			const decDVector c = testFace->GetMesh()->GetSpace().GetMatrix() * testFace->GetCenter();
			module.LogInfoFormat( "         Face: %p:%i p=%p:%i c=(%g,%g,%g) (%.3f,%.3f,%.3f)",
				testFace->GetMesh(), testFace->GetIndex(), testFace->GetPathParent()?testFace->GetPathParent()->GetMesh():NULL,
				testFace->GetPathParent()?testFace->GetPathParent()->GetIndex():-1, testFace->GetPathCostF(),
				testFace->GetPathCostG(), testFace->GetPathCostH(), c.x, c.y, c.z );
		}
#endif
	}
	
	if( pListClosed.GetCount() > 0 ){
		testFace = ( dedaiSpaceMeshFace* )pListClosed.GetAt( pListClosed.GetCount() - 1 );
		
		// if the last face in the closed list is not the end face then there is no path to get
		// to the end face using the current configuration. in this case look for the face with
		// the smallest f-cost value and use this as the end face. this at last yields a path
		// which gets us the closest to the face
		//if( testFace != pEndFace ){
		//}
		
		if( testFace == pEndFace ){
			faceCount = 0;
			nextFace = testFace;
			while( nextFace ){
				faceCount++;
				nextFace = nextFace->GetPathParent();
				pPathFaces.Add( NULL );
			}
			
			for( faceCount--; faceCount>=0; faceCount-- ){
				pPathFaces.SetAt( faceCount, testFace );
				testFace = testFace->GetPathParent();
			}
		}
	}
#ifdef DEBUG
	module.LogInfo( "      Path Faces:" );
	for( f=0; f<pPathFaces.GetCount(); f++ ){
		testFace = ( dedaiSpaceMeshFace* )pPathFaces.GetAt( f );
		const decDVector c = testFace->GetMesh()->GetSpace().GetMatrix() * testFace->GetCenter();
		module.LogInfoFormat( "         Face: %p:%i p=(%p:%i) c=(%g,%g,%g) (%.3f,%.3f,%.3f)",
			testFace->GetMesh(), testFace->GetIndex(), testFace->GetPathParent()?testFace->GetPathParent()->GetMesh():NULL,
			testFace->GetPathParent()?testFace->GetPathParent()->GetIndex():-1, testFace->GetPathCostF(),
			testFace->GetPathCostG(), testFace->GetPathCostH(), c.x, c.y, c.z );
	}
#endif
}

void dedaiPathFinderNavMesh::pFindRealPath(){
	const int faceCount = pPathFaces.GetCount();
	const float threshold = 0.001f;
	
	RemoveAllPathPoints();
	
	if( faceCount > 1 ){
		dedaiPathFinderPointList pointListLeft, pointListRight;
		dedaiSpaceMeshCorner *curCorner, *nextCorner;
		dedaiSpaceMesh *curNavMesh, *nextNavMesh;
		int i, curFaceEdge, nextFaceEdge = -1;
		dedaiSpaceMeshFace *curFace, *nextFace;
		dedaiSpaceMeshEdge *curEdge; //, *nextEdge;
		unsigned short compareVertexIndex;
		decVector vertexLeft, vertexRight;
		dedaiPathFinderFunnel funnel;
		unsigned short vertexIndex;
		bool inverseOrder = false;
		bool initFunnel = true;
		decVector testVertex;
		decVector testNormal;
		decDVector pathPoint;
		float testDistance;
		int bestPointIndex;
		int j, pointCount;
		decVector normal;
		bool testResult;
#ifdef DEBUG
		int DL, DR;
		deDEAIModule &module = pWorld->GetDEAI();
#endif
		float len;
		
		bool withFixing = true;
		
		//AddPathPoint( pStartPoint ); //nah, we are there already usually
		
		for( i=0; i<faceCount; i++ ){
			if( i < faceCount - 1 ){
				curFace = ( dedaiSpaceMeshFace* )pPathFaces.GetAt( i );
				nextFace = ( dedaiSpaceMeshFace* )pPathFaces.GetAt( i + 1 );
				
				curFaceEdge = pFindEdgeLeadingToFace( *curFace, *nextFace );
				nextFaceEdge = pFindEdgeLeadingToFace( *nextFace, *curFace );
				if( curFaceEdge == -1 || nextFaceEdge == -1 ){
					pNavigator->GetDEAI().LogWarn( "Broken Nav-Path, Stopping" );
					return;
				}
				
				curNavMesh = curFace->GetMesh();
				curCorner = curNavMesh->GetCorners() + ( curFace->GetFirstCorner() + curFaceEdge );
				curEdge = curNavMesh->GetEdges() + curCorner->GetEdge();
				vertexIndex = curCorner->GetVertex();
				testResult = ( vertexIndex == curEdge->GetVertex1() );
				if( inverseOrder ){
					testResult = ! testResult;
				}
				if( testResult ){
					vertexLeft = curNavMesh->GetVertices()[ curEdge->GetVertex1() ];
					vertexRight = curNavMesh->GetVertices()[ curEdge->GetVertex2() ];
					compareVertexIndex = curEdge->GetVertex1();
#ifdef DEBUG
					DL=curEdge->GetVertex1();
					DR=curEdge->GetVertex2();
#endif
					
				}else{
					vertexLeft = curNavMesh->GetVertices()[ curEdge->GetVertex2() ];
					vertexRight = curNavMesh->GetVertices()[ curEdge->GetVertex1() ];
					compareVertexIndex = curEdge->GetVertex2();
#ifdef DEBUG
					DL=curEdge->GetVertex2();
					DR=curEdge->GetVertex1();
#endif
				}
				
				nextNavMesh = nextFace->GetMesh();
				nextCorner = nextNavMesh->GetCorners() + ( nextFace->GetFirstCorner() + nextFaceEdge );
				//nextEdge = nextNavMesh->GetEdges() + nextCorner->GetEdge();
				vertexIndex = nextCorner->GetVertex();
				if( nextNavMesh == curNavMesh ){
					inverseOrder = ( vertexIndex == compareVertexIndex );
					
				}else{
					const decMatrix &matrix = curNavMesh->GetLinkAt( curCorner->GetLink() ).GetTransform();
					inverseOrder = ( matrix * vertexLeft ).IsEqualTo( nextNavMesh->GetVertices()[ vertexIndex ], threshold );
				}
				
			}else{
				curFace = ( dedaiSpaceMeshFace* )pPathFaces.GetAt( i );
				curNavMesh = curFace->GetMesh();
				
				vertexLeft = ( curFace->GetMesh()->GetSpace().GetInverseMatrix() * pEndPoint ).ToVector();
				vertexRight = vertexLeft;
				
				// stop compiler complaining about potentially unused variables as they are not
				nextFace = NULL;
				curFaceEdge = 0;
#ifdef DEBUG
				DL = 0;
				DR = 0;
#endif
			}
			
			if( initFunnel ){
				if( i < faceCount - 1 ){
					const decDMatrix &invMatrix = curNavMesh->GetSpace().GetInverseMatrix();
					
					funnel.SetMesh( curNavMesh );
					funnel.SetOrigin( ( invMatrix * pStartPoint ).ToVector() );
					funnel.SetGoal( ( invMatrix * pEndPoint ).ToVector() );
					funnel.SetLeftCorner( vertexLeft );
					funnel.SetRightCorner( vertexRight );
					funnel.UpdateLeftPlane( curFace->GetNormal() );
					funnel.UpdateRightPlane( curFace->GetNormal() );
					initFunnel = false;
					
#ifdef DEBUG
					const decDVector c = curFace->GetMesh()->GetSpace().GetMatrix() * curFace->GetCenter();
					module.LogInfoFormat( "   InitFunnel: %u(%u)=>%u(%u) l=%u r=%u i=%c (%.3f,%.3f,%.3f)", curFace->GetIndex(),
						curFaceEdge, nextFace->GetIndex(), nextFaceEdge, DL, DR, inverseOrder?'t':'f', c.x, c.y, c.z );
				}else{
					module.LogInfoFormat( "   InitFunnel: %u", curFace->GetIndex() );
#endif
				}
				
			}else{
#ifdef DEBUG
				if( i < faceCount - 1 ){
					const decDVector c = curFace->GetMesh()->GetSpace().GetMatrix() * curFace->GetCenter();
					module.LogInfoFormat( "   UpdateFunnel: %u(%u)=>%u(%u) l=%u r=%u i=%c (%.3f,%.3f,%.3f)", curFace->GetIndex(),
						curFaceEdge, nextFace->GetIndex(), nextFaceEdge, DL, DR, inverseOrder?'t':'f', c.x, c.y, c.z );
					
				}else{
					module.LogInfoFormat( "   UpdateFunnel: %u", curFace->GetIndex() );
				}
#endif
				// if the navigation mesh changed transform also the funnel
				if( curNavMesh != funnel.GetMesh() ){
#ifdef DEBUG
					module.LogInfoFormat( "   TransformFunnel: nm=%p => nm=%p", funnel.GetMesh(), curNavMesh );
#endif
					// how can this be changed to reuse the per-link saved transform matrix?
					const decMatrix matrix = ( funnel.GetMesh()->GetSpace().GetMatrix() * curNavMesh->GetSpace().GetInverseMatrix() ).ToMatrix();
					pointListRight.Transform( matrix );
					pointListLeft.Transform( matrix );
					funnel.Transform( matrix );
					funnel.SetMesh( curNavMesh );
				}
				
				// if the left vertex is the same as the left funnel vertex nothing is going to change
				if( ! vertexLeft.IsEqualTo( funnel.GetLeftCorner(), threshold ) ){
					// if the left vertex is on the front side of the left funnel plane the funnel has to be updated
					if( funnel.GetLeftNormal() * vertexLeft > funnel.GetLeftDistance() ){
						// clear the left point list as no point in there can be better than the left vertex
						pointListLeft.RemoveAll();
						
						// if the left vertex is on the front side of the right funnel plane the left vertex improves
						// the funnel without invalidating it. the left funnel plane is updated
						if( funnel.GetRightNormal() * vertexLeft > funnel.GetRightDistance() ){
							normal = curFace->GetNormal() % ( vertexLeft - funnel.GetOrigin() );
							normal.Normalize();
							funnel.SetLeftNormal( normal );
							funnel.SetLeftCorner( vertexLeft );
#ifdef DEBUG
							module.LogInfo( "   ImproveFunnel: left" );
#endif
							
						// if the left vertex is on the back side of the right funnel plane the new left funnel plane
						// would invalidate the funnel as it is on the back of the right funnel plane. the funnel has
						// to be fixed now
						}else{
#ifdef DEBUG
							module.LogInfo( "   AddNextCorner: right funnel corner" );
#endif
							pathPoint = curFace->GetMesh()->GetSpace().GetMatrix() * decDVector( funnel.GetRightCorner() );
							if( pPathPointCount == 0 || ! pathPoint.IsEqualTo( pPathPoints[ pPathPointCount - 1 ], threshold ) ){
								AddPathPoint( pathPoint );
							}
							
							funnel.SetOrigin( funnel.GetRightCorner() );
							funnel.SetLeftCorner( vertexLeft );
							funnel.UpdateLeftPlane( curFace->GetNormal() );
							
							if( ! withFixing ){
								funnel.SetRightCorner( vertexRight );
								funnel.UpdateRightPlane( curFace->GetNormal() );
								
							}else{
							
							// if there are though now entries in the right point list the funnel is potentially
							// incorrect. this happenes if the path leads around a curved corner. in this case
							// the funnel algorithm misses the correct shape. to fix the funnel the entries in
							// the right point list are processed from the start of the list to the end
							while( true ){
								// find the point from the right point list that is the farthest away from the
								// right funnel plane. for this each point in turn is tested if it is on the
								// front side of the plane defined by the best found point. this way the plane
								// radially drifts farther away from the right funnel plane until the best plane
								// has been found. all points before this found point are removed from the list
								// as they are for sure worse than the found point. to start the test the right
								// funnel plane is used. this way one side of the upcoming funnel boundary check
								// we get for free
								pointCount = pointListRight.GetCount();
								bestPointIndex = -1;
								testDistance = 0.0f; // required as compiler thinks otherwise it's potentially unused
								
								for( j=0; j<pointCount; j++ ){
									testVertex = pointListRight.GetAt( j );
									
									if( bestPointIndex == -1 || testNormal * testVertex > testDistance ){
										// NOTE normalizing is not required. currently used only to catch zero-length
										testNormal = curFace->GetNormal() % ( funnel.GetOrigin() - testVertex );
										len = testNormal.Length();
										if( len > FLOAT_SAFE_EPSILON ){
											testNormal /= len;
											
										}else{
											continue;
										}
										testDistance = testNormal * funnel.GetOrigin();
										bestPointIndex = j;
									}
								}
								
								if( bestPointIndex == -1 ){
#ifdef DEBUG
									module.LogInfoFormat( "      FixingFunnel(%i): removing all points (best not found)", pointListRight.GetCount() );
#endif
									pointListRight.RemoveAll();
									funnel.SetRightCorner( vertexRight );
									break;
								}
								
#ifdef DEBUG
								module.LogInfoFormat( "      FixingFunnel(%i): removing %i points (best %i)", pointListRight.GetCount(), bestPointIndex, bestPointIndex );
#endif
								pointListRight.RemoveFirst( bestPointIndex );
								testVertex = pointListRight.GetFirst();
								
								// if the best point is inside the funnel boundaries no further fixing is required.
								// since we use the right funnel plane already in the above test we only have to
								// test the left side now
								if( funnel.GetLeftNormal() * testVertex > funnel.GetLeftDistance() ){
#ifdef DEBUG
									module.LogInfoFormat( "      FixingFunnel(%i): funnel valid", pointListRight.GetCount() );
#endif
									funnel.SetRightCorner( testVertex );
									break;
									
								// if the best point is outside the boundaries the funnel has to be fixed, a new corner
								// point added and another turn taken. the funnel is fixed by using the best point as
								// the new funnel origin. the right and left funnel vertex stays the same but their
								// planes have to be updated
								}else{
#ifdef DEBUG
									module.LogInfoFormat( "      FixingFunnel(%i): removing point (funnel updated)", pointListRight.GetCount() );
#endif
									pointListRight.RemoveFirst();
#ifdef DEBUG
									module.LogInfo( "         AddNextCorner: found point" );
#endif
									pathPoint = curFace->GetMesh()->GetSpace().GetMatrix() * decDVector( testVertex );
									if( pPathPointCount == 0 || ! pathPoint.IsEqualTo( pPathPoints[ pPathPointCount - 1 ], threshold ) ){
										AddPathPoint( pathPoint );
									}
									funnel.SetOrigin( testVertex );
									funnel.UpdateLeftPlane( curFace->GetNormal() );
								}
							}
							
							funnel.UpdateLeftPlane( curFace->GetNormal() );
							funnel.UpdateRightPlane( curFace->GetNormal() );
							}
						}
						
					// if the left vertex is on the back side of the left funnel plane the left vertex has to be
					// stored in the left point list to fix the funnel later on if a right vertex invalidates
					// the funnel
					}else{
						pointListLeft.Add( vertexLeft );
#ifdef DEBUG
						module.LogInfoFormat( "   AddPointListLeft: %i(%i)", DL, pointListLeft.GetCount() );
#endif
					}
					
#ifdef DEBUG
				}else{
					module.LogInfoFormat( "   SameLeft: %i", DL );
#endif
				}
				
				// if the right vertex is the same as the right funnel vertex nothing is going to change
				if( ! vertexRight.IsEqualTo( funnel.GetRightCorner(), threshold ) ){
					// if the right vertex is on the front side of the right funnel plane the funnel has to be updated
					if( funnel.GetRightNormal() * vertexRight > funnel.GetRightDistance() ){
						// clear the right point list as no point in there can be better than the right vertex
						pointListRight.RemoveAll();
						
						// if the right vertex is on the front side of the left funnel plane the right vertex improves
						// the funnel without invalidating it. the right funnel plane is updated and the right point
						// list cleared since they can never be a better choice with the new right funnel plane in place
						if( funnel.GetLeftNormal() * vertexRight > funnel.GetLeftDistance() ){
							normal = curFace->GetNormal() % ( funnel.GetOrigin() - vertexRight );
							normal.Normalize();
							funnel.SetRightNormal( normal );
							funnel.SetRightCorner( vertexRight );
#ifdef DEBUG
							module.LogInfo( "   ImproveFunnel: right" );
#endif
							
						// if the right vertex is on the back side of the left funnel plane the new right funnel plane
						// would invalidate the funnel as it is on the back of the left funnel plane. the funnel has
						// to be fixed now
						}else{
#ifdef DEBUG
							module.LogInfo( "   AddNextCorner: left funnel corner" );
#endif
							pathPoint = curFace->GetMesh()->GetSpace().GetMatrix() * decDVector( funnel.GetLeftCorner() );
							if( pPathPointCount == 0 || ! pathPoint.IsEqualTo( pPathPoints[ pPathPointCount - 1 ], threshold ) ){
								AddPathPoint( pathPoint );
							}
							
							funnel.SetOrigin( funnel.GetLeftCorner() );
							funnel.SetRightCorner( vertexRight );
							funnel.UpdateRightPlane( curFace->GetNormal() );
							
							if( ! withFixing ){
								funnel.SetLeftCorner( vertexLeft );
								funnel.UpdateLeftPlane( curFace->GetNormal() );
								
							}else{
							
							// if there are though now entries in the left point list the funnel is potentially
							// incorrect. this happenes if the path leads around a curved corner. in this case
							// the funnel algorithm misses the correct shape. to fix the funnel the entries in
							// the left point list are processed from the start of the list to the end
							while( true ){
								// find the point from the left point list that is the farthest away from the
								// left funnel plane. for this each point in turn is tested if it is on the
								// front side of the plane defined by the best found point. this way the plane
								// radially drifts farther away from the left funnel plane until the best plane
								// has been found. all points before this found point are removed from the list
								// as they are for sure worse than the found point. to start the test the left
								// funnel plane is used. this way one side of the upcoming funnel boundary check
								// we get for free
								pointCount = pointListLeft.GetCount();
								bestPointIndex = -1;
								testDistance = 0.0f; // required as compiler thinks otherwise it's potentially unused
								
								for( j=0; j<pointCount; j++ ){
									testVertex = pointListLeft.GetAt( j );
									
									if( bestPointIndex == -1 || testNormal * testVertex > testDistance ){
										// NOTE normalizing is not required. currently used only to catch zero-length
										testNormal = curFace->GetNormal() % ( testVertex - funnel.GetOrigin() );
										len = testNormal.Length();
										if( len > FLOAT_SAFE_EPSILON ){
											testNormal /= len;
											
										}else{
											continue;
										}
										testDistance = testNormal * funnel.GetOrigin();
										bestPointIndex = j;
									}
								}
								
								if( bestPointIndex == -1 ){
#ifdef DEBUG
									module.LogInfoFormat( "      FixingFunnel(%i): removing all points (best not found)", pointListLeft.GetCount() );
#endif
									pointListLeft.RemoveAll();
									funnel.SetLeftCorner( vertexLeft );
									break;
								}
								
#ifdef DEBUG
								module.LogInfoFormat( "      FixingFunnel(%i): removing %i points (best %i)", pointListLeft.GetCount(), bestPointIndex, bestPointIndex );
#endif
								pointListLeft.RemoveFirst( bestPointIndex );
								testVertex = pointListLeft.GetFirst();
								
								// if the best point is inside the funnel boundaries no further fixing is required.
								// since we use the left funnel plane already in the above test we only have to
								// test the right side now
								if( funnel.GetRightNormal() * testVertex > funnel.GetRightDistance() ){
#ifdef DEBUG
									module.LogInfoFormat( "      FixingFunnel(%i): funnel valid", pointListLeft.GetCount() );
#endif
									funnel.SetLeftCorner( testVertex );
									break;
									
								// if the best point is outside the boundaries the funnel has to be fixed, a new corner
								// point added and another turn taken. the funnel is fixed by using the best point as
								// the new funnel origin. the right and left funnel vertex stays the same but their
								// planes have to be updated
								}else{
#ifdef DEBUG
									module.LogInfoFormat( "      FixingFunnel(%i): removing point (funnel updated)", pointListLeft.GetCount() );
#endif
									pointListLeft.RemoveFirst();
#ifdef DEBUG
									pWorld->GetDEAI().LogInfo( "         AddNextCorner: found point" );
#endif
									pathPoint = curFace->GetMesh()->GetSpace().GetMatrix() * decDVector( testVertex );
									if( pPathPointCount == 0 || ! pathPoint.IsEqualTo( pPathPoints[ pPathPointCount - 1 ], threshold ) ){
										AddPathPoint( pathPoint );
									}
									funnel.SetOrigin( testVertex );
									funnel.UpdateRightPlane( curFace->GetNormal() );
								}
							}
							
							funnel.UpdateLeftPlane( curFace->GetNormal() );
							funnel.UpdateRightPlane( curFace->GetNormal() );
							}
						}
						
					// if the right vertex is on the back side of the right funnel plane the right vertex has to be
					// stored in the right point list to fix the funnel later on if a left vertex invalidates the funnel
					}else{
						pointListRight.Add( vertexRight );
#ifdef DEBUG
						module.LogInfoFormat( "   AddPointListRight: %i(%i)", DR, pointListRight.GetCount() );
#endif
					}
					
#ifdef DEBUG
				}else{
					module.LogInfoFormat( "   SameRight: %i", DR );
#endif
				}
			}
		}
		
#ifdef DEBUG
		module.LogInfo( "   FinishFunnel" );
#endif
	}
	
	if( pPathPointCount == 0 || ! pEndPoint.IsEqualTo( pPathPoints[ pPathPointCount - 1 ], threshold ) ){
		AddPathPoint( pEndPoint );
	}
}

int dedaiPathFinderNavMesh::pFindEdgeLeadingToFace( const dedaiSpaceMeshFace &face, const dedaiSpaceMeshFace &targetFace ) const{
	const dedaiSpaceMesh &navmesh = *face.GetMesh();
	const dedaiSpaceMeshCorner * const corners = navmesh.GetCorners() + face.GetFirstCorner();
	const unsigned int cornerCount = face.GetCornerCount();
	const int targetIndex = targetFace.GetIndex();
	unsigned int i;
	
	if( face.GetMesh() == targetFace.GetMesh() ){
		const dedaiSpaceMeshEdge * const edges = navmesh.GetEdges();
		
		for( i=0; i<cornerCount; i++ ){
			const dedaiSpaceMeshEdge &edge = edges[ corners[ i ].GetEdge() ];
			
			if( edge.GetFace1() == targetIndex || edge.GetFace2() == targetIndex ){
				return i;
			}
		}
		
	}else{
		const dedaiSpaceMeshLink * const links = navmesh.GetLinks();
		dedaiSpaceMesh * const targetNavMesh = targetFace.GetMesh();
		
		for( i=0; i<cornerCount; i++ ){
			if( corners[ i ].GetLink() == CORNER_NO_LINK ){
				continue;
			}
			
			const dedaiSpaceMeshLink &link = links[ corners[ i ].GetLink() ];
			if( link.GetMesh() == targetNavMesh && link.GetFace() == targetIndex ){
				return i;
			}
		}
	}
	
	return -1;
}

void dedaiPathFinderNavMesh::pUpdateDDSListOpen(){
	if( ! pDDSListOpen ){
		return;
	}
	
	pDDSListOpen->RemoveAllFaces();
	pDDSListOpen->GetShapeList().RemoveAll();
	
	if( pListOpen.GetCount() > 0 ){
		dedaiSpace &space = ( ( dedaiSpaceMeshFace* )pListOpen.GetAt( 0 ) )->GetMesh()->GetSpace();
		const decDMatrix &invMatrix = space.GetInverseMatrix();
		deDebugDrawerShapeFace *ddsFace = NULL;
		const int count = pListOpen.GetCount();
		int i, j;
		
		pDDSListOpen->SetPosition( space.GetMatrix().GetPosition() );
		pDDSListOpen->SetOrientation( space.GetMatrix().ToQuaternion() );
		
		try{
			for( i=0; i<count; i++ ){
				const dedaiSpaceMeshFace &face = *( ( dedaiSpaceMeshFace* )pListOpen.GetAt( i ) );
				const unsigned short cornerCount = face.GetCornerCount();
				
				if( cornerCount > 2 ){
					const decMatrix transform = ( face.GetMesh()->GetSpace().GetMatrix() * invMatrix ).ToMatrix();
					const dedaiSpaceMeshCorner * const corners = face.GetMesh()->GetCorners();
					const decVector * const vertices = face.GetMesh()->GetVertices();
					const int firstCorner = face.GetFirstCorner();
					
					ddsFace = new deDebugDrawerShapeFace;
					for( j=0; j<cornerCount; j++ ){
						ddsFace->AddVertex( transform * vertices[ corners[ firstCorner + j ].GetVertex() ] );
					}
					ddsFace->SetNormal( face.GetNormal() );
					pDDSListOpen->AddFace( ddsFace );
					ddsFace = NULL;
				}
			}
			
		}catch( const deException & ){
			if( ddsFace ){
				delete ddsFace;
			}
			throw;
		}
	}
}

void dedaiPathFinderNavMesh::pUpdateDDSListClosed(){
	if( ! pDDSListClosed ){
		return;
	}
	
	pDDSListClosed->RemoveAllFaces();
	pDDSListClosed->GetShapeList().RemoveAll();
	
	if( pListClosed.GetCount() > 0 ){
		dedaiSpace &navspace = ( ( dedaiSpaceMeshFace* )pListClosed.GetAt( 0 ) )->GetMesh()->GetSpace();
		const decDMatrix &invMatrix = navspace.GetInverseMatrix();
		deDebugDrawerShapeFace *ddsFace = NULL;
		const int count = pListClosed.GetCount();
		int i, j;
		
		pDDSListClosed->SetPosition( navspace.GetMatrix().GetPosition() );
		pDDSListClosed->SetOrientation( navspace.GetMatrix().ToQuaternion() );
		
		try{
			for( i=0; i<count; i++ ){
				const dedaiSpaceMeshFace &face = *( ( dedaiSpaceMeshFace* )pListClosed.GetAt( i ) );
				const unsigned short cornerCount = face.GetCornerCount();
				
				if( cornerCount > 2 ){
					const decMatrix transform = ( face.GetMesh()->GetSpace().GetMatrix() * invMatrix ).ToMatrix();
					const dedaiSpaceMeshCorner * const corners = face.GetMesh()->GetCorners();
					const decVector * const vertices = face.GetMesh()->GetVertices();
					const int firstCorner = face.GetFirstCorner();
					
					ddsFace = new deDebugDrawerShapeFace;
					for( j=0; j<cornerCount; j++ ){
						ddsFace->AddVertex( transform * vertices[ corners[ firstCorner + j ].GetVertex() ] );
					}
					ddsFace->SetNormal( face.GetNormal() );
					pDDSListClosed->AddFace( ddsFace );
					ddsFace = NULL;
				}
			}
			
		}catch( const deException & ){
			if( ddsFace ){
				delete ddsFace;
			}
			throw;
		}
	}
}
