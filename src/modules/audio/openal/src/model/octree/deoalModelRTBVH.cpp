/* 
 * Drag[en]gine OpenAL Audio Module
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
#include <stdlib.h>
#include <string.h>

#include "deoalModelRTBVH.h"
#include "../deoalAModel.h"
#include "../deoalModelFace.h"

#include <dragengine/common/exceptions.h>



// Class deoalModelRTBVH
//////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoalModelRTBVH::deoalModelRTBVH() :
pFaces( NULL ),
pFaceCount( 0 ),
pFaceSize( 0 ),

pNodes( NULL ),
pNodeCount( 0 ),
pNodeSize( 0 ),

pBuildNodes( NULL ),
pBuildNodeCount( 0 ),
pBuildNodeSize( 0 ),

pBuildFaces( NULL ),
pBuildFaceCount( 0 ),
pBuildFaceSize( 0 ),

pIndexNode( 0 ),
pIndexFace( 0 ){
}

deoalModelRTBVH::~deoalModelRTBVH(){
	pCleanUp();
}



// Management
///////////////

void deoalModelRTBVH::Build( const deoalModelFace *faces, int faceCount ){
	pNodeCount = 0;
	pFaceCount = 0;
	pBuildFaceCount = 0;
	pBuildNodeCount = 0;
	if( faceCount == 0 ){
		return;
	}
	
	if( faceCount > pBuildFaceSize ){
		if( pBuildFaces ){
			delete [] pBuildFaces;
			pBuildFaces = NULL;
			pBuildFaceSize = 0;
		}
		pBuildFaces = new sBuildFace[ faceCount ];
		pBuildFaceSize = faceCount;
	}
	pBuildFaceCount = faceCount;
	
	int i;
	for( i=0; i<pBuildFaceCount; i++ ){
		const deoalModelFace &modelFace = faces[ i ];
		sBuildFace &buildFace = pBuildFaces[ i ];
		buildFace.minExtend = modelFace.GetMinExtend();
		buildFace.maxExtend = modelFace.GetMaxExtend();
		buildFace.center = ( buildFace.minExtend + buildFace.maxExtend ) * 0.5f;
		buildFace.modelFace = &modelFace;
		buildFace.next = i + 1;
	}
	pBuildFaces[ pBuildFaceCount - 1 ].next = -1;
	
	// build root node
	pAddBuildNode();
	sBuildNode &rootNode = pBuildNodes[ 0 ];
	rootNode.faceCount = pBuildFaceCount;
	rootNode.firstFace = 0;
	rootNode.lastFace = pBuildFaceCount - 1;
	pUpdateNodeExtends( rootNode );
	
	// split root node
	pSplitNode( 0 );
	
	// build visit arrays
	if( pBuildNodeCount > pNodeSize ){
		if( pNodes ){
			delete [] pNodes;
			pNodes = NULL;
			pNodeSize = 0;
		}
		pNodes = new sNode[ pBuildNodeCount ];
		pNodeSize = pBuildNodeCount;
	}
	pNodeCount = pBuildNodeCount;
	
	if( pBuildFaceCount > pFaceSize ){
		if( pFaces ){
			delete [] pFaces;
			pFaces = NULL;
			pFaceSize = 0;
		}
		pFaces = new sFace[ pBuildFaceCount ];
		pFaceSize = pBuildFaceCount;
	}
	pFaceCount = pBuildFaceCount;
	
	pIndexNode = 0;
	pIndexFace = 0;
	pBuildVisitNode( pBuildNodes[ 0 ] );
}

void deoalModelRTBVH::DropBuildData(){
	if( pBuildNodes ){
		delete [] pBuildNodes;
		pBuildNodes = NULL;
		pBuildNodeCount = 0;
		pBuildNodeSize = 0;
	}
	if( pBuildFaces ){
		delete [] pBuildFaces;
		pBuildFaces = NULL;
		pBuildFaceCount = 0;
		pBuildFaceSize = 0;
	}
}



// Private Functions
//////////////////////

void deoalModelRTBVH::pCleanUp(){
	if( pNodes ){
		delete [] pNodes;
	}
	if( pFaces ){
		delete [] pFaces;
	}
	if( pBuildNodes ){
		delete [] pBuildNodes;
	}
	if( pBuildFaces ){
		delete [] pBuildFaces;
	}
}

int deoalModelRTBVH::pAddBuildNode(){
	if( pBuildNodeCount == pBuildNodeSize ){
		const int size = pBuildNodeSize + 100;
		sBuildNode * const nodes = new sBuildNode[ size ];
		if( pBuildNodes ){
			memcpy( nodes, pBuildNodes, sizeof( sBuildNode ) * pBuildNodeSize );
			delete [] pBuildNodes;
		}
		pBuildNodes = nodes;
		pBuildNodeSize = size;
	}
	
	const int index = pBuildNodeCount++;
	sBuildNode &node = pBuildNodes[ index ];
	node.child1 = -1;
	node.child2 = -1;
	node.firstFace = -1;
	node.lastFace = -1;
	node.faceCount = 0;
	
	return index;
}

void deoalModelRTBVH::pUpdateNodeExtends( sBuildNode &node ) const{
	if( node.firstFace == -1 ){
		node.center.SetZero();
		node.halfSize.SetZero();
		return;
	}
	
	const sBuildFace &firstFace = pBuildFaces[ node.firstFace ];
	decVector minExtend( firstFace.minExtend );
	decVector maxExtend( firstFace.maxExtend );
	int faceIndex = firstFace.next;
	
	while( faceIndex != -1 ){
		const sBuildFace &face = pBuildFaces[ faceIndex ];
		minExtend.SetSmallest( face.minExtend );
		maxExtend.SetLargest( face.maxExtend );
		faceIndex = face.next;
	}
	
	node.center = ( minExtend + maxExtend ) * 0.5f;
	node.halfSize = ( maxExtend - minExtend ) * 0.5f;
}

void deoalModelRTBVH::pSplitNode( int nodeIndex ){
	if( pBuildNodes[ nodeIndex ].faceCount < 3 ){
		return;
	}
	
	// find center extends. we need to use this instead of the node extends since only this
	// shows us in corner cases if a split is possible
	const sBuildFace &firstFace = pBuildFaces[ pBuildNodes[ nodeIndex ].firstFace ];
	decVector centerMin( firstFace.center );
	decVector centerMax( firstFace.center );
	int faceIndex = firstFace.next;
	
	while( faceIndex != -1 ){
		const sBuildFace &face = pBuildFaces[ faceIndex ];
		centerMin.SetSmallest( face.center );
		centerMax.SetLargest( face.center );
		faceIndex = face.next;
	}
	
	// find primary axis
	const decVector centerSize( centerMax - centerMin );
	const float splitThreshold = 0.1f; //0.001f; // 0.01f
	float splitPosition;
	int primaryAxis;
	
	if( centerSize.x >= centerSize.y ){
		if( centerSize.x >= centerSize.z ){
			if( centerSize.x < splitThreshold ){
				return;
			}
			splitPosition = ( centerMin.x + centerMax.x ) * 0.5f;
			primaryAxis = 0;
			
		}else{
			if( centerSize.z < splitThreshold ){
				return;
			}
			splitPosition = ( centerMin.z + centerMax.z ) * 0.5f;
			primaryAxis = 2;
		}
		
	}else if( centerSize.y >= centerSize.z ){
		if( centerSize.y < splitThreshold ){
			return;
		}
		splitPosition = ( centerMin.y + centerMax.y ) * 0.5f;
		primaryAxis = 1;
		
	}else{
		if( centerSize.z < splitThreshold ){
			return;
		}
		splitPosition = ( centerMin.z + centerMax.z ) * 0.5f;
		primaryAxis = 2;
	}
	
	// create child nodes
	const int childIndex1 = pAddBuildNode();
	const int childIndex2 = pAddBuildNode();
	
	sBuildNode &node = pBuildNodes[ nodeIndex ];
	sBuildNode &child1 = pBuildNodes[ childIndex1 ];
	sBuildNode &child2 = pBuildNodes[ childIndex2 ];
	
	faceIndex = node.firstFace;
	
	switch( primaryAxis ){
	case 0: // x largest
		while( faceIndex != -1 ){
			sBuildFace &face = pBuildFaces[ faceIndex ];
			const int nextFaceIndex = face.next;
			face.next = -1;
			
			if( face.center.x < splitPosition ){
				pNodeAddFace( child1, faceIndex );
				
			}else{
				pNodeAddFace( child2, faceIndex );
			}
			
			faceIndex = nextFaceIndex;
		}
		break;
		
	case 1: // y largest
		while( faceIndex != -1 ){
			sBuildFace &face = pBuildFaces[ faceIndex ];
			const int nextFaceIndex = face.next;
			face.next = -1;
			
			if( face.center.y < splitPosition ){
				pNodeAddFace( child1, faceIndex );
				
			}else{
				pNodeAddFace( child2, faceIndex );
			}
			
			faceIndex = nextFaceIndex;
		}
		break;
		
	case 2: // z largest
		while( faceIndex != -1 ){
			sBuildFace &face = pBuildFaces[ faceIndex ];
			const int nextFaceIndex = face.next;
			face.next = -1;
			
			if( face.center.z < splitPosition ){
				pNodeAddFace( child1, faceIndex );
				
			}else{
				pNodeAddFace( child2, faceIndex );
			}
			
			faceIndex = nextFaceIndex;
		}
		break;
	}
	
	node.faceCount = 0;
	node.firstFace = -1;
	node.lastFace = -1;
	node.child1 = childIndex1;
	node.child2 = childIndex2;
	
	pUpdateNodeExtends( child1 );
	pUpdateNodeExtends( child2 );
	
	pSplitNode( childIndex1 );
	pSplitNode( childIndex2 );
}

void deoalModelRTBVH::pNodeAddFace( sBuildNode &node, int faceIndex ){
	if( node.lastFace != -1 ){
		pBuildFaces[ node.lastFace ].next = faceIndex;
		
	}else{
		node.firstFace = faceIndex;
	}
	
	node.lastFace = faceIndex;
	node.faceCount++;
}

void deoalModelRTBVH::pBuildVisitNode( const sBuildNode &buildNode ){
	sNode &visitNode = pNodes[ pIndexNode++ ];
	visitNode.center = buildNode.center;
	visitNode.halfSize = buildNode.halfSize;
	
	// components
	if( buildNode.faceCount > 0 ){
		visitNode.firstFace = pIndexFace;
		visitNode.faceCount = buildNode.faceCount;
		visitNode.node1 = -1;
		visitNode.node2 = -1;
		
		int indexBuildFace = buildNode.firstFace;
		while( indexBuildFace != -1 ){
			const sBuildFace &buildFace = pBuildFaces[ indexBuildFace ];
			sFace &visitFace = pFaces[ pIndexFace++ ];
			const deoalModelFace &modelFace = *buildFace.modelFace;
			
			visitFace.normal = modelFace.GetNormal();
			visitFace.baseVertex = modelFace.GetVertex1();
			visitFace.edgeNormal[ 0 ] = modelFace.GetEdge1Normal();
			visitFace.edgeNormal[ 1 ] = modelFace.GetEdge2Normal();
			visitFace.edgeNormal[ 2 ] = modelFace.GetEdge3Normal();
			visitFace.edgeDistance[ 0 ] = modelFace.GetEdge1DistanceSafe();
			visitFace.edgeDistance[ 1 ] = modelFace.GetEdge2DistanceSafe();
			visitFace.edgeDistance[ 2 ] = modelFace.GetEdge3DistanceSafe();
			visitFace.indexFace = modelFace.GetIndex();
			visitFace.indexTexture = modelFace.GetTexture();
			
			indexBuildFace = buildFace.next;
		}
		
	// child nodes
	}else if( buildNode.child1 != -1 ){
		visitNode.firstFace = -1;
		visitNode.faceCount = 0;
		
		visitNode.node1 = pIndexNode;
		pBuildVisitNode( pBuildNodes[ buildNode.child1 ] );
		
		visitNode.node2 = pIndexNode;
		pBuildVisitNode( pBuildNodes[ buildNode.child2 ] );
		
	// this should never happen
	}else{
		DETHROW( deeInvalidParam );
	}
}
