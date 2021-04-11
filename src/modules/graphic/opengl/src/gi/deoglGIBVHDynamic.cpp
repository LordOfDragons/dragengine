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

#include "deoglGI.h"
#include "deoglGIBVH.h"
#include "deoglGIBVHLocal.h"
#include "deoglGIBVHDynamic.h"
#include "../model/deoglModelLOD.h"
#include "../renderthread/deoglRenderThread.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOUInt16.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../tbo/deoglDynamicTBOShared.h"
#include "../utils/bvh/deoglBVHNode.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIBVHDynamic
////////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIBVHDynamic::deoglGIBVHDynamic( deoglGIBVHLocal &bvhLocal ) :
pGIBVHLocal( bvhLocal ),
pTBONodeBox( NULL ),
pTBOVertex( NULL ),
pBlockUsageCount( 0 )
{
	try{
		pTBONodeBox = new deoglDynamicTBOFloat32( bvhLocal.GetRenderThread(), 4 );
		pTBONodeBox->IncreaseDataCount( bvhLocal.GetTBONodeBox()->GetDataCount() );
		
		pTBOVertex = new deoglDynamicTBOFloat32( bvhLocal.GetRenderThread(), 4 );
		pTBOVertex->IncreaseDataCount( bvhLocal.GetTBOVertex()->GetDataCount() );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGIBVHDynamic::~deoglGIBVHDynamic(){
	pCleanUp();
}



// Management
///////////////

void deoglGIBVHDynamic::UpdateBVHExtends(){
	const deoglBVHNode * const rootNode = pGIBVHLocal.GetBVH().GetRootNode();
	if( ! rootNode ){
		return;
	}
	
	pCalcNodeExtends( *rootNode, pMinExtend, pMaxExtend );
	pWriteNodeExtend( 0, pMinExtend, pMaxExtend );
}

void deoglGIBVHDynamic::UpdateVertices( const oglModelPosition *positions, int count ){
	if( count != pTBOVertex->GetPixelCount() ){
		DETHROW( deeInvalidParam );
	}
	
	float *data = pTBOVertex->GetDataFloat();
	int i;
	for( i=0; i<count; i++, data+=4 ){
		const decVector &position = positions[ i ].position;
		data[ 0 ] = position.x;
		data[ 1 ] = position.y;
		data[ 2 ] = position.z;
		// data[ 3 ] = 0.0f;
	}
}

void deoglGIBVHDynamic::UpdateVertices( const oglVector *positions, int count ){
	if( count != pTBOVertex->GetPixelCount() ){
		DETHROW( deeInvalidParam );
	}
	
	float *data = pTBOVertex->GetDataFloat();
	int i;
	for( i=0; i<count; i++, data+=4 ){
		const oglVector &position = positions[ i ];
		data[ 0 ] = position.x;
		data[ 1 ] = position.y;
		data[ 2 ] = position.z;
		// data[ 3 ] = 0.0f;
	}
}



deoglDynamicTBOBlock *deoglGIBVHDynamic::GetBlockNode(){
	if( ! pBlockNode ){
		pBlockNode.TakeOver( pGIBVHLocal.GetRenderThread().GetGI().GetBVH().GetSharedTBONode()
			->AddBlock( pGIBVHLocal.GetTBOIndex(), pTBONodeBox ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockNode;
}

deoglDynamicTBOBlock *deoglGIBVHDynamic::GetBlockVertex(){
	if( ! pBlockVertex ){
		pBlockVertex.TakeOver( pGIBVHLocal.GetRenderThread().GetGI().GetBVH().GetSharedTBOVertex()
			->AddBlock( pTBOVertex ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockVertex;
}

void deoglGIBVHDynamic::DropBlocks(){
	if( pBlockNode ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockNode )->Drop();
		pBlockNode = NULL;
	}
	if( pBlockVertex ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockVertex )->Drop();
		pBlockVertex = NULL;
	}
}

void deoglGIBVHDynamic::AddBlockUsage(){
	pBlockUsageCount++;
}

void deoglGIBVHDynamic::RemoveBlockUsage(){
	if( pBlockUsageCount == 0 ){
		return;
	}
	
	pBlockUsageCount--;
	if( pBlockUsageCount == 0 ){
		DropBlocks();
	}
}



// Private Functions
//////////////////////

void deoglGIBVHDynamic::pCleanUp(){
	DropBlocks();
	
	if( pTBONodeBox ){
		pTBONodeBox->FreeReference();
	}
	if( pTBOVertex ){
		pTBOVertex->FreeReference();
	}
}

void deoglGIBVHDynamic::pCalcNodeExtends( const deoglBVHNode &node, decVector &minExtend, decVector &maxExtend ){
	const int primitiveCount = node.GetPrimitiveCount();
	const int firstIndex = node.GetFirstIndex();
	
	if( primitiveCount == 0 ){
		const deoglBVHNode &nodeLeft = pGIBVHLocal.GetBVH().GetNodeAt( firstIndex );
		decVector minExtendLeft, maxExtendLeft;
		pCalcNodeExtends( nodeLeft, minExtendLeft, maxExtendLeft );
		pWriteNodeExtend( firstIndex, minExtendLeft, maxExtendLeft );
		
		const deoglBVHNode &nodeRight = pGIBVHLocal.GetBVH().GetNodeAt( firstIndex + 1 );
		decVector minExtendRight, maxExtendRight;
		pCalcNodeExtends( nodeRight, minExtendRight, maxExtendRight );
		pWriteNodeExtend( firstIndex + 1, minExtendRight, maxExtendRight );
		
		minExtend = minExtendLeft.Smallest( minExtendRight );
		maxExtend = maxExtendLeft.Largest( maxExtendRight );
		
	}else{
		const int * const primitives = pGIBVHLocal.GetBVH().GetPrimitives();
		const float * const vertices = pTBOVertex->GetDataFloat();
		const uint16_t * const faces = pGIBVHLocal.GetTBOFace()->GetDataUInt();
		int i;
		
		for( i=0; i<primitiveCount; i++ ){
			const uint16_t * const face = faces + primitives[ i ] * 4;
			const float * const v1 = vertices + face[ 0 ] * 4;
			const decVector p1( v1[ 0 ], v1[ 1 ], v1[ 2 ] );
			if( i > 0 ){
				minExtend.SetSmallest( p1 );
				maxExtend.SetLargest( p1 );
				
			}else{
				minExtend = maxExtend = p1;
			}
			
			const float * const v2 = vertices + face[ 1 ] * 4;
			const decVector p2( v2[ 0 ], v2[ 1 ], v2[ 2 ] );
			minExtend.SetSmallest( p2 );
			maxExtend.SetLargest( p2 );
			
			const float * const v3 = vertices + face[ 2 ] * 4;
			const decVector p3( v3[ 0 ], v3[ 1 ], v3[ 2 ] );
			minExtend.SetSmallest( p3 );
			maxExtend.SetLargest( p3 );
		}
	}
}

void deoglGIBVHDynamic::pWriteNodeExtend( int index, const decVector &minExtend, const decVector &maxExtend ){
	float * const data = pTBONodeBox->GetDataFloat() + index * 8;
	
	data[ 0 ] = minExtend.x;
	data[ 1 ] = minExtend.y;
	data[ 2 ] = minExtend.z;
	//data[ 3 ] = 0.0f;
	
	data[ 4 ] = maxExtend.x;
	data[ 5 ] = maxExtend.y;
	data[ 6 ] = maxExtend.z;
	//data[ 7 ] = 0.0f;
}
