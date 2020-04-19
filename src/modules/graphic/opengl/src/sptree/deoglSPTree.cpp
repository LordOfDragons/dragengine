/* 
 * Drag[en]gine OpenGL Graphic Module
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
#include "deoglSPTree.h"
#include "deoglSPTreeNode.h"
#include "dragengine/common/exceptions.h"



// Class deoglSPTree
//////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglSPTree::deoglSPTree( const decVector &minExtend, const decVector &maxExtend, const decPoint3 &nodeCount ){
	decVector size = maxExtend - minExtend;
	int i;
	
	pNodeSize.x = size.x / ( float )nodeCount.x;
	pNodeSize.y = size.y / ( float )nodeCount.y;
	pNodeSize.z = size.z / ( float )nodeCount.z;
	
	pNodeCount = nodeCount;
	
	pMinExtend = minExtend;
	pMaxExtend = maxExtend;
	pNodes = NULL;
	
	pTotalNodeCount = nodeCount.x * nodeCount.y * nodeCount.z;
	pStride = nodeCount.x * nodeCount.y;
	
	pNodes = new deoglSPTreeNode*[ pTotalNodeCount ];
	if( ! pNodes ) DETHROW( deeOutOfMemory );
	for( i=0; i<pTotalNodeCount; i++ ) pNodes[ i ] = NULL;
}

deoglSPTree::~deoglSPTree(){
	ClearAllNodes();
	if( pNodes ) delete [] pNodes;
}



// Management
///////////////

bool deoglSPTree::IsIndexValid( const decPoint3 &index ) const{
	if( index.x < 0 || index.x >= pNodeCount.x ) return false;
	if( index.y < 0 || index.y >= pNodeCount.y ) return false;
	if( index.z < 0 || index.z >= pNodeCount.z ) return false;
	
	return true;
}

void deoglSPTree::IndexOfNodeAt( decPoint3 &index, const decVector &position ) const{
	index.x = ( int )( ( position.x - pMinExtend.x ) / pNodeSize.x );
	if( index.x >= 0 && index.x < pNodeCount.x ){
		index.y = ( int )( ( position.y - pMinExtend.y ) / pNodeSize.y );
		if( index.y >= 0 && index.y < pNodeCount.y ){
			index.z = ( int )( ( position.z - pMinExtend.z ) / pNodeSize.z );
			if( index.z >= 0 && index.z < pNodeCount.z ){
				return;
			}
		}
	}
	
	index.x = -1;
	index.y = -1;
	index.z = -1;
}

deoglSPTreeNode *deoglSPTree::GetNodeAt( const decPoint3 &index ) const{
	if( ! IsIndexValid( index ) ) DETHROW( deeInvalidParam );
	
	return pNodes[ pStride * index.z + pNodeCount.x * index.y + index.x ];
}

void deoglSPTree::SetNodeAt( const decPoint3 &index, deoglSPTreeNode *node ){
	if( ! IsIndexValid( index ) ) DETHROW( deeInvalidParam );
	int realIndex = pStride * index.z + pNodeCount.x * index.y + index.x;
	
	if( pNodes[ realIndex ] ){
		delete pNodes[ realIndex ];
	}
	
	pNodes[ realIndex ] = node;
}

void deoglSPTree::ClearAllNodes(){
	int i, count = pNodeCount.x * pNodeCount.y * pNodeCount.z;
	
	for( i=0; i<count; i++ ){
		if( pNodes[ i ] ){
			delete pNodes[ i ];
			pNodes[ i ] = NULL;
		}
	}
}
