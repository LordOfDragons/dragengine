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

#include "dedaiHeightTerrain.h"
#include "dedaiHeightTerrainSector.h"
#include "dedaiHeightTerrainNavSpace.h"
#include "../spaces/dedaiSpace.h"
#include "../../deDEAIModule.h"
#include "../../world/dedaiWorld.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpace.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrain.h>
#include <dragengine/resources/terrain/heightmap/deHeightTerrainNavSpaceEdge.h>
#include <dragengine/resources/navigation/space/deNavigationSpaceFace.h>



// Class dedaiHeightTerrainNavSpace
/////////////////////////////////////

// Constructors and Destructors
/////////////////////////////////

dedaiHeightTerrainNavSpace::dedaiHeightTerrainNavSpace( dedaiHeightTerrainSector &sector,
	const deHeightTerrainNavSpace &navigationSpace ) :
pSector( sector ),
pNavigationSpace( navigationSpace ),

pVertices( NULL ),
pVertexCount( 0 ),
pVertexSize( 0 ),

pEdges( NULL ),

pCorners( NULL ),
pCornerCount( 0 ),
pCornerSize( 0 ),

pFaces( NULL ),
pFaceCount( 0 ),
pFaceSize( 0 ),

pSpace( NULL )
{
	try{
		pUpdateCorners();
		pUpdateEdges();
		
		pSpace = new dedaiSpace( sector.GetHeightTerrain().GetDEAI() );
		pSpace->SetOwnerHTNavSpace( this );
		pSpace->SetParentWorld( sector.GetHeightTerrain().GetParentWorld() );
		pSpace->SetPosition( sector.GetPosition() );
		pSpace->SetLayerNumber( navigationSpace.GetLayer() );
		pSpace->SetType( navigationSpace.GetType() );
		pSpace->SetSnapDistance( navigationSpace.GetSnapDistance() );
		pSpace->SetSnapAngle( navigationSpace.GetSnapAngle() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

dedaiHeightTerrainNavSpace::~dedaiHeightTerrainNavSpace(){
	pCleanUp();
}



// Management
///////////////



// Notifications
//////////////////

void dedaiHeightTerrainNavSpace::ParentWorldChanged(){
	pSpace->SetParentWorld( pSector.GetHeightTerrain().GetParentWorld() );
}

void dedaiHeightTerrainNavSpace::HeightChanged( const decPoint &from, const decPoint &to ){
	pSpace->OwnerLayoutChanged();
}

void dedaiHeightTerrainNavSpace::LayerChanged(){
	pSpace->SetLayerNumber( pNavigationSpace.GetLayer() );
}

void dedaiHeightTerrainNavSpace::TypeChanged(){
	pUpdateCorners();
	pUpdateEdges();
	pSpace->SetType( pNavigationSpace.GetType() );
}

void dedaiHeightTerrainNavSpace::SnappingChanged(){
	pSpace->SetSnapDistance( pNavigationSpace.GetSnapDistance() );
	pSpace->SetSnapAngle( pNavigationSpace.GetSnapAngle() );
}

void dedaiHeightTerrainNavSpace::LayoutChanged(){
	pUpdateCorners();
	pUpdateEdges();
	pSpace->OwnerLayoutChanged();
}



// Private Functions
//////////////////////

void dedaiHeightTerrainNavSpace::pCleanUp(){
	if( pSpace ){
		delete pSpace;
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

void dedaiHeightTerrainNavSpace::pUpdateCorners(){
	if( pNavigationSpace.GetType() != deNavigationSpace::estMesh ){
		return;
	}
	
	pVertexCount = 0;
	pCornerCount = 0;
	pFaceCount = 0;
	
	
	
#if 0
	const int faceCount = pNavigationSpace.GetFaceCount();
	if( faceCount == 0 ){
		return;
	}
	
	const deNavigationSpaceFace * const faces = pNavigationSpace.GetFaces();
	const unsigned int * const corners = pNavigationSpace.GetCorners();
	const int cornerCount = pNavigationSpace.GetCornerCount();
	int i, j;
	
	// NOTE vertices array is usually less than corners but never larger. allocating it
	//      with corner count size avoids re-allocating the array multiple times. after
	//      the real count is determined the array could be shrunk
	// NOTE caching this data is possible. this would also provide the shrinking
	if( cornerCount > pVertexSize ){
		if( pVertices ){
			delete [] pVertices;
			pVertices = NULL;
			pVertexSize = 0;
		}
		
		pVertices = new unsigned int[ cornerCount ];
		pVertexSize = cornerCount;
	}
	
	if( cornerCount > pCornerSize ){
		if( pCorners ){
			delete [] pCorners;
			pCorners = NULL;
			pCornerSize = 0;
		}
		
		pCorners = new unsigned short[ cornerCount ];
		pCornerSize = cornerCount;
	}
	
	if( faceCount > pFaceSize ){
		if( pFaces ){
			delete [] pFaces;
			pFaces = NULL;
			pFaceSize = 0;
		}
		
		pFaces = new deNavigationSpaceFace[ faceCount ];
		pFaceSize = faceCount;
	}
	
	// add corners and shared vertices
	for( i=0; i<cornerCount; i++ ){
		const unsigned int navpoint = corners[ i ];
		
		for( j=0; j<pVertexCount; j++ ){
			if( pVertices[ j ] == navpoint ){
				break;
			}
		}
		
		if( j == pVertexCount ){
			pVertices[ j ] = navpoint;
			pVertexCount++;
		}
		
		pCorners[ i ] = ( unsigned short )j;
	}
	pCornerCount = cornerCount;
	
	for( i=0; i<faceCount; i++ ){
		pFaces[ i ].SetType( faces[ i ].GetType() );
		pFaces[ i ].SetCornerCount( faces[ i ].GetCornerCount() );
	}
	pFaceCount = faceCount;
#endif
	
	
	
	
	const int orgFaceCount = pNavigationSpace.GetFaceCount();
	if( orgFaceCount == 0 ){
		return;
	}
	
	// build split corners and faces. for height terrain mappers can define faces
	// with more than 3 corners. it is though nearly impossible to produce fully
	// coplanar polygons this way. having non-coplanar polygons in the blocker
	// phase causes various problems and incorrect result. for this reason all
	// faces are split into 3-corner faces. there might be a few faces being
	// coplanar but the chance is so low it does not make sense to bother not
	// splitting those
	const deNavigationSpaceFace * const faces = pNavigationSpace.GetFaces();
	const unsigned int * const corners = pNavigationSpace.GetCorners();
	int cornerCount = 0;
	int faceCount = 0;
	int i, j, k;
	
	for( i=0; i<orgFaceCount; i++ ){
		if( faces[ i ].GetCornerCount() < 3 ){
			continue;
		}
		
		faceCount += faces[ i ].GetCornerCount() - 2;
		cornerCount += ( faces[ i ].GetCornerCount() - 2 ) * 3;
	}
	
	// NOTE vertices array is usually less than corners but never larger. allocating it
	//      with corner count size avoids re-allocating the array multiple times. after
	//      the real count is determined the array could be shrunk
	// NOTE caching this data is possible. this would also provide the shrinking
	if( cornerCount > pVertexSize ){
		if( pVertices ){
			delete [] pVertices;
			pVertices = NULL;
			pVertexSize = 0;
		}
		
		pVertices = new unsigned int[ cornerCount ];
		pVertexSize = cornerCount;
	}
	
	if( cornerCount > pCornerSize ){
		if( pCorners ){
			delete [] pCorners;
			pCorners = NULL;
			pCornerSize = 0;
		}
		
		pCorners = new unsigned short[ cornerCount ];
		pCornerSize = cornerCount;
	}
	
	if( faceCount > pFaceSize ){
		if( pFaces ){
			delete [] pFaces;
			pFaces = NULL;
			pFaceSize = 0;
		}
		
		pFaces = new deNavigationSpaceFace[ faceCount ];
		pFaceSize = faceCount;
	}
	
	// add corners, faces and shared vertices.
	int firstCorner = 0;
	
	for( i=0; i<orgFaceCount; i++ ){
		const deNavigationSpaceFace &face = faces[ i ];
		
		const int faceCornerCount = face.GetCornerCount();
		if( faceCornerCount < 3 ){
			firstCorner += faceCornerCount;
			continue;
		}
		
		const unsigned int navpointFirst = corners[ firstCorner ];
		for( j=0; j<pVertexCount; j++ ){
			if( pVertices[ j ] == navpointFirst ){
				break;
			}
		}
		if( j == pVertexCount ){
			pVertices[ pVertexCount++ ] = navpointFirst;
		}
		const unsigned short vertexFirst = ( unsigned short )j;
		
		const unsigned int navpointSecond = corners[ firstCorner + 1 ];
		for( j=0; j<pVertexCount; j++ ){
			if( pVertices[ j ] == navpointSecond ){
				break;
			}
		}
		if( j == pVertexCount ){
			pVertices[ pVertexCount++ ] = navpointSecond;
		}
		unsigned short vertexLast = ( unsigned short )j;
		
		for( j=2; j<faceCornerCount; j++ ){
			const unsigned int navpoint = corners[ firstCorner + j ];
			
			for( k=0; k<pVertexCount; k++ ){
				if( pVertices[ k ] == navpoint ){
					break;
				}
			}
			if( k == pVertexCount ){
				pVertices[ pVertexCount++ ] = navpoint;
			}
			
			pCorners[ pCornerCount++ ] = vertexFirst;
			pCorners[ pCornerCount++ ] = vertexLast;
			vertexLast = ( unsigned short )k;
			pCorners[ pCornerCount++ ] = vertexLast;
			
			pFaces[ pFaceCount ].SetType( face.GetType() );
			pFaces[ pFaceCount ].SetCornerCount( 3 );
			pFaceCount++;
		}
		
		firstCorner += faceCornerCount;
	}
}

void dedaiHeightTerrainNavSpace::pUpdateEdges(){
	if( pNavigationSpace.GetType() != deNavigationSpace::estGrid ){
		return;
	}
	
	if( pEdges ){
		delete [] pEdges;
		pEdges = NULL;
	}
	pVertexCount = 0;
	
	const int edgeCount = pNavigationSpace.GetEdgeCount();
	if( edgeCount == 0 ){
		return;
	}
	
	const deHeightTerrainNavSpaceEdge * const edges = pNavigationSpace.GetEdges();
	int i, j;
	
	// NOTE vertices array is usually less than 2 times edge count but never larger.
	//      allocating it with 2 times edge count size avoids re-allocating the array
	//      multiple times. after the real count is determined the array could be shrunk
	// NOTE caching this data is possible. this would also provide the shrinking
	pEdges = new sEdge[ edgeCount ];
	
	const int maxVertexCount = edgeCount * 2;
	if( maxVertexCount > pVertexSize ){
		if( pVertices ){
			delete [] pVertices;
			pVertices = NULL;
			pVertexSize = 0;
		}
		
		pVertices = new unsigned int[ maxVertexCount ];
		pVertexSize = maxVertexCount;
	}
	
	// add edges and shared vertices
	for( i=0; i< edgeCount; i++ ){
		const unsigned int navpoint1 = edges[ i ].GetPoint1();
		const unsigned int navpoint2 = edges[ i ].GetPoint2();
		
		// vertex 1
		for( j=0; j<pVertexCount; j++ ){
			if( pVertices[ j ] == navpoint1 ){
				break;
			}
		}
		if( j == pVertexCount ){
			pVertices[ j ] = navpoint1;
			pVertexCount++;
		}
		pEdges[ i ].vertex1 = ( unsigned short )j;
		
		// vertex 2
		for( j=0; j<pVertexCount; j++ ){
			if( pVertices[ j ] == navpoint2 ){
				break;
			}
		}
		if( j == pVertexCount ){
			pVertices[ j ] = navpoint2;
			pVertexCount++;
		}
		pEdges[ i ].vertex2 = ( unsigned short )j;
	}
}
