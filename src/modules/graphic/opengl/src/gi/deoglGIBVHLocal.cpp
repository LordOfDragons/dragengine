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
#include "../model/deoglModelLOD.h"
#include "../model/face/deoglModelFace.h"
#include "../renderthread/deoglRenderThread.h"
#include "../utils/bvh/deoglBVHNode.h"
#include "../tbo/deoglDynamicTBOFloat32.h"
#include "../tbo/deoglDynamicTBOFloat16.h"
#include "../tbo/deoglDynamicTBOUInt32.h"
#include "../tbo/deoglDynamicTBOUInt16.h"
#include "../tbo/deoglDynamicTBOBlock.h"
#include "../tbo/deoglDynamicTBOShared.h"

#include <dragengine/common/exceptions.h>


// Class deoglGIBVHLocal
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglGIBVHLocal::deoglGIBVHLocal( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pTBONodeBox( NULL ),
pTBOIndex( NULL ),
pTBOFace( NULL ),
pTBOVertex( NULL ),
pTBOTexCoord( NULL )
{
	try{
		pTBONodeBox = new deoglDynamicTBOFloat32( renderThread, 4 );
		pTBOIndex = new deoglDynamicTBOUInt16( renderThread, 2 );
		pTBOFace = new deoglDynamicTBOUInt16( renderThread, 4 );
		pTBOVertex = new deoglDynamicTBOFloat32( renderThread, 4 );
		pTBOTexCoord = new deoglDynamicTBOFloat16( renderThread, 2 );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

deoglGIBVHLocal::~deoglGIBVHLocal(){
	pCleanUp();
}



// Management
///////////////

void deoglGIBVHLocal::Clear(){
	DropBlocks();
	pBVH.Clear();
	pTBOVertex->Clear();
	pTBOTexCoord->Clear();
	pTBOFace->Clear();
	pTBOIndex->Clear();
	pTBONodeBox->Clear();
}

void deoglGIBVHLocal::BuildBVH( const deoglBVH::sBuildPrimitive *primitives,
int primitiveCount, int maxDepth ){
	pBVH.Build( primitives, primitiveCount, maxDepth );
}

void deoglGIBVHLocal::UpdateBVHExtends(){
	deoglBVHNode * const rootNode = pBVH.GetRootNode();
	if( rootNode ){
		pUpdateBVHExtends( *rootNode );
	}
}

void deoglGIBVHLocal::TBOAddVertex( const decVector &position ){
	pTBOVertex->AddVec4( position, 0.0f );
}

void deoglGIBVHLocal::TBOAddVertices( const oglModelPosition *positions, int count ){
	int i;
	for( i=0; i<count; i++ ){
		TBOAddVertex( positions[ i ].position );
	}
}

void deoglGIBVHLocal::TBOAddFace( int vertex1, int vertex2, int vertex3, int material,
const decVector2 &texCoord1, const decVector2 &texCoord2, const decVector2 &texCoord3 ){
	pTBOFace->AddVec4( vertex1, vertex2, vertex3, material );
	
	pTBOTexCoord->AddVec2( texCoord1 );
	pTBOTexCoord->AddVec2( texCoord2 );
	pTBOTexCoord->AddVec2( texCoord3 );
}

void deoglGIBVHLocal::TBOAddFaces( const deoglModelFace *faces, const oglModelVertex *vertices,
const decVector2 *texCoords ){
	const int * const primitives = pBVH.GetPrimitives();
	const int primitiveCount = pBVH.GetPrimitiveCount();
	int i;
	
	for( i=0; i<primitiveCount; i++ ){
		const deoglModelFace &face = faces[ primitives[ i ] ];
		const oglModelVertex &v1 = vertices[ face.GetVertex1() ];
		const oglModelVertex &v2 = vertices[ face.GetVertex2() ];
		const oglModelVertex &v3 = vertices[ face.GetVertex3() ];
		
		TBOAddFace( v1.position, v2.position, v3.position, face.GetTexture(),
			texCoords[ v1.texcoord ], texCoords[ v2.texcoord ], texCoords[ v3.texcoord ] );
	}
}

void deoglGIBVHLocal::TBOAddBVH(){
	const deoglBVHNode * const nodes = pBVH.GetNodes();
	const int nodeCount = pBVH.GetNodeCount();
	int i;
	
	for( i=0; i<nodeCount; i++ ){
		const deoglBVHNode &node = nodes[ i ];
		pTBONodeBox->AddVec4( node.GetMinExtend(), 0.0f );
		pTBONodeBox->AddVec4( node.GetMaxExtend(), 0.0f );
		pTBOIndex->AddVec2( node.GetFirstIndex(), node.GetPrimitiveCount() );
	}
}

void deoglGIBVHLocal::TBOBVHUpdateNodeExtends(){
	const deoglBVHNode * const nodes = pBVH.GetNodes();
	const int nodeCount = pBVH.GetNodeCount();
	float *nodeBox = pTBONodeBox->GetDataFloat();
	int i;
	
	for( i=0; i<nodeCount; i++, nodeBox+=8 ){
		const deoglBVHNode &node = nodes[ i ];
		
		const decVector &minExtend = node.GetMinExtend();
		nodeBox[ 0 ] = minExtend.x;
		nodeBox[ 1 ] = minExtend.y;
		nodeBox[ 2 ] = minExtend.z;
		
		const decVector &maxExtend = node.GetMaxExtend();
		nodeBox[ 4 ] = maxExtend.x;
		nodeBox[ 5 ] = maxExtend.y;
		nodeBox[ 6 ] = maxExtend.z;
	}
}



deoglDynamicTBOBlock *deoglGIBVHLocal::GetBlockNodeBox(){
	if( ! pBlockNodeBox ){
		pBlockNodeBox.TakeOver( pRenderThread.GetGI().GetBVH()
			.GetSharedTBONodeBox()->AddBlock( pTBONodeBox ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockNodeBox;
}

deoglDynamicTBOBlock *deoglGIBVHLocal::GetBlockIndex(){
	if( ! pBlockIndex ){
		pBlockIndex.TakeOver( pRenderThread.GetGI().GetBVH()
			.GetSharedTBONodeBox()->AddBlock( pTBOIndex ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockIndex;
}

deoglDynamicTBOBlock *deoglGIBVHLocal::GetBlockFace(){
	if( ! pBlockFace ){
		pBlockFace.TakeOver( pRenderThread.GetGI().GetBVH()
			.GetSharedTBONodeBox()->AddBlock( pTBOFace ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockFace;
}

deoglDynamicTBOBlock *deoglGIBVHLocal::GetBlockVertex(){
	if( ! pBlockVertex ){
		pBlockVertex.TakeOver( pRenderThread.GetGI().GetBVH()
			.GetSharedTBONodeBox()->AddBlock( pTBOVertex ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockVertex;
}

deoglDynamicTBOBlock *deoglGIBVHLocal::GetBlockTexCoord(){
	if( ! pBlockTexCoord ){
		pBlockTexCoord.TakeOver( pRenderThread.GetGI().GetBVH()
			.GetSharedTBONodeBox()->AddBlock( pTBOTexCoord ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockTexCoord;
}

void deoglGIBVHLocal::DropBlocks(){
	if( pBlockNodeBox ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockNodeBox )->Drop();
		pBlockNodeBox = NULL;
	}
	if( pBlockIndex ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockIndex )->Drop();
		pBlockIndex = NULL;
	}
	if( pBlockFace ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockFace )->Drop();
		pBlockFace = NULL;
	}
	if( pBlockVertex ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockVertex )->Drop();
		pBlockVertex = NULL;
	}
	if( pBlockTexCoord ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockTexCoord )->Drop();
		pBlockTexCoord = NULL;
	}
}



// Private Functions
//////////////////////

void deoglGIBVHLocal::pCleanUp(){
	DropBlocks();
	
	if( pTBONodeBox ){
		pTBONodeBox->FreeReference();
	}
	if( pTBOIndex ){
		pTBOIndex->FreeReference();
	}
	if( pTBOFace ){
		pTBOFace->FreeReference();
	}
	if( pTBOVertex ){
		pTBOVertex->FreeReference();
	}
	if( pTBOTexCoord ){
		pTBOTexCoord->FreeReference();
	}
}

void deoglGIBVHLocal::pUpdateBVHExtends( deoglBVHNode &node ){
	const int primitiveCount = node.GetPrimitiveCount();
	const int firstIndex = node.GetFirstIndex();
	
	if( primitiveCount == 0 ){
		deoglBVHNode &nodeLeft = pBVH.GetNodeAt( firstIndex );
		deoglBVHNode &nodeRight = pBVH.GetNodeAt( firstIndex + 1 );
		
		pUpdateBVHExtends( nodeLeft );
		pUpdateBVHExtends( nodeRight );
		
		node.SetExtends( nodeLeft.GetMinExtend().Smallest( nodeRight.GetMinExtend() ),
			nodeLeft.GetMaxExtend().Largest( nodeRight.GetMaxExtend() ) );
		
	}else{
		const int * const primitives = pBVH.GetPrimitives();
		const float * const vertices = pTBOVertex->GetDataFloat();
		const uint16_t * const faces = pTBOFace->GetDataUInt();
		decVector minExtend, maxExtend;
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
		
		node.SetExtends( minExtend, maxExtend );
	}
}
