/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2021, Roland Plüss (roland@rptd.ch)
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

#include "deoglBVH.h"
#include "deoglBVHNode.h"

#include <dragengine/common/exceptions.h>


// Class deoglBVH
///////////////////

// Constructor, destructor
////////////////////////////

deoglBVH::deoglBVH() :
pNodes( NULL ),
pNodeCount( 0 ),
pNodeSize ( 0 ),
pPrimitives( NULL ),
pPrimitiveCount( 0 ),
pPrimitiveSize( 0 ){
}

deoglBVH::~deoglBVH(){
	if( pNodes ){
		delete [] pNodes;
	}
	if( pPrimitives ){
		delete [] pPrimitives;
	}
}



// Management
///////////////

deoglBVHNode &deoglBVH::GetNodeAt( int index ) const{
	if( index < 0 || index >= pNodeCount ){
		DETHROW( deeInvalidParam );
	}
	return pNodes[ index ];
}

int deoglBVH::GetPrimitiveAt( int index ) const{
	if( index < 0 || index >= pPrimitiveCount ){
		DETHROW( deeInvalidParam );
	}
	return pPrimitives[ index ];
}

deoglBVHNode *deoglBVH::GetRootNode() const{
	if( pNodeCount == 0 ){
		return NULL;
	}
	return pNodes;
}

void deoglBVH::Clear(){
	pNodeCount = 0;
	pPrimitiveCount = 0;
}

void deoglBVH::Build( const sBuildPrimitive *primitives, int primitiveCount, int maxDepth ){
	if( primitiveCount < 0 ){
		DETHROW_INFO( deeInvalidParam, "primitiveCount < 0" );
	}
	if( primitiveCount > 0 && ! primitives ){
		DETHROW_INFO( deeInvalidParam, "primitiveCount > 0 && primitives is null" );
	}
	if( maxDepth < 1 ){
		DETHROW_INFO( deeInvalidParam, "maxDepth < 1" );
	}
	
	Clear();
	
	if( primitiveCount == 0 ){
		return;
	}
	
	pInitPrimitives( primitiveCount );
	
	pAddNode();
	deoglBVHNode &rootNode = pNodes[ 0 ];
	rootNode.SetFirstIndex( 0 );
	rootNode.SetPrimitiveCount( primitiveCount );
	
	pBuildNode( primitives, primitiveCount, 0, maxDepth - 1 );
}



// Protected Functions
////////////////////////

void deoglBVH::pAddNode(){
	if( pNodeCount == pNodeSize ){
		const int newSize = pNodeCount + 20;
		deoglBVHNode * const newArray = new deoglBVHNode[ newSize ];
		if( pNodes ){
			memcpy( newArray, pNodes, sizeof( deoglBVHNode ) * pNodeCount );
			delete [] pNodes;
		}
		pNodes = newArray;
		pNodeSize = newSize;
	}
	
	pNodes[ pNodeCount++ ].Clear();
}

void deoglBVH::pInitPrimitives( int primitiveCount ){
	if( primitiveCount > pPrimitiveSize ){
		int * const newArray = new int[ primitiveCount ];
		if( pPrimitives ){
			delete [] pPrimitives;
		}
		pPrimitives = newArray;
		pPrimitiveSize = primitiveCount;
	}
	
	for( pPrimitiveCount=0; pPrimitiveCount<primitiveCount; pPrimitiveCount++ ){
		pPrimitives[ pPrimitiveCount ] = pPrimitiveCount;
	}
}

void deoglBVH::pBuildNode( const sBuildPrimitive *primitives, int primitiveCount, int node, int maxDepth ){
	const int nodePrimitiveCount = pNodes[ node ].GetPrimitiveCount();
	const int nodeFirstIndex = pNodes[ node ].GetFirstIndex();
	int i;
	
	// calculate boundaries. we need this for both cases
	const int * const nodePrimitives = pPrimitives + nodeFirstIndex;
	const sBuildPrimitive &firstPrimitive = primitives[ nodePrimitives[ 0 ] ];
	decVector minExtend( firstPrimitive.minExtend );
	decVector maxExtend( firstPrimitive.maxExtend );
	
	for( i=1; i<nodePrimitiveCount; i++ ){
		const sBuildPrimitive &nodePrimitive = primitives[ nodePrimitives[ i ] ];
		minExtend.SetSmallest( nodePrimitive.minExtend );
		maxExtend.SetLargest( nodePrimitive.maxExtend );
	}
	
	// make sure boundaries have at least a minimum thickness or else ray casting code
	// can fail to detect the box. slightly enlarging the box is fine enough and makes
	// hitting boxes more robust
	const float margin = 1e-5f; // 0.01mm
	const decVector enlarge( decVector().Largest( decVector( margin, margin, margin ) - ( maxExtend - minExtend ) ) * 0.5f );
	minExtend -= enlarge;
	maxExtend += enlarge;
	
	pNodes[ node ].SetExtends( minExtend, maxExtend );
	
	const decVector nodeSize( maxExtend - minExtend );
	
	// if more than 1 primtive is located in the node, the maximum depth has been reached
	// or the node size becomes too small keep the node a primitive only node and stop
	// building this branch of the tree
	if( nodePrimitiveCount < 2 || maxDepth == 0
	|| ! ( nodeSize > decVector( FLOAT_SAFE_EPSILON, FLOAT_SAFE_EPSILON, FLOAT_SAFE_EPSILON ) ) ){
		return;
	}
	
	// create two child nodes. pAddNode potentially moves memory
	const int indexLeftNode = pNodeCount;
	pAddNode(); // add left node
	pAddNode(); // add right node
	
	// distribute primitives across children
	int walkerLeft = nodeFirstIndex;
	int walkerRight = nodeFirstIndex + nodePrimitiveCount - 1;
	
	if( nodeSize.x > nodeSize.y && nodeSize.x > nodeSize.z ){
		// split across X axis
		const float splitCenter = ( minExtend.x + maxExtend.x ) * 0.5f;
		while( walkerLeft <= walkerRight ){
			const int temp = pPrimitives[ walkerLeft ];
			
			if( primitives[ temp ].center.x < splitCenter ){
				walkerLeft++;
				
			}else{
				pPrimitives[ walkerLeft ] = pPrimitives[ walkerRight ];
				pPrimitives[ walkerRight-- ] = temp;
			}
		}
		
	}else if( nodeSize.y > nodeSize.z ){
		// split across Y axis
		const float splitCenter = ( minExtend.y + maxExtend.y ) * 0.5f;
		while( walkerLeft <= walkerRight ){
			const int temp = pPrimitives[ walkerLeft ];
			
			if( primitives[ temp ].center.y < splitCenter ){
				walkerLeft++;
				
			}else{
				pPrimitives[ walkerLeft ] = pPrimitives[ walkerRight ];
				pPrimitives[ walkerRight-- ] = temp;
			}
		}
		
	}else{
		// split across Z axis
		const float splitCenter = ( minExtend.z + maxExtend.z ) * 0.5f;
		while( walkerLeft <= walkerRight ){
			const int temp = pPrimitives[ walkerLeft ];
			
			if( primitives[ temp ].center.z < splitCenter ){
				walkerLeft++;
				
			}else{
				pPrimitives[ walkerLeft ] = pPrimitives[ walkerRight ];
				pPrimitives[ walkerRight-- ] = temp;
			}
		}
	}
	
	// update primitive indices of child nodes
	deoglBVHNode &leftNode = pNodes[ indexLeftNode ];
	deoglBVHNode &rightNode = pNodes[ indexLeftNode + 1 ];
	
	leftNode.SetFirstIndex( nodeFirstIndex );
	leftNode.SetPrimitiveCount( walkerLeft - nodeFirstIndex );
	rightNode.SetFirstIndex( nodeFirstIndex + leftNode.GetPrimitiveCount() );
	rightNode.SetPrimitiveCount( nodePrimitiveCount - leftNode.GetPrimitiveCount() );
	
	// if a node winds up empty we can not separate the primitives. drop the two child
	// nodes and keep the node a primitives only node
	if( leftNode.GetPrimitiveCount() == 0 || rightNode.GetPrimitiveCount() == 0 ){
		pNodeCount -= 2;
		return;
	}
	
	// otherwise continue building with the child nodes. since pAddNode potentially
	// moved memory we can not use leftNode and rightNode reference from here on
	pNodes[ node ].SetFirstIndex( pNodeCount - 2 );
	pNodes[ node ].SetPrimitiveCount( 0 );
	
	maxDepth--;
	pBuildNode( primitives, primitiveCount, indexLeftNode, maxDepth );
	pBuildNode( primitives, primitiveCount, indexLeftNode + 1, maxDepth );
}
