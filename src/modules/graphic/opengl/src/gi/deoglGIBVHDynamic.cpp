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
		pTBONodeBox->SetDataCount( bvhLocal.GetTBONodeBox()->GetDataCount() );
		
		pTBOVertex = new deoglDynamicTBOFloat32( bvhLocal.GetRenderThread(), 4 );
		pTBOVertex->SetDataCount( bvhLocal.GetTBOVertex()->GetDataCount() );
		
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
	pWriteNodeExtends( 0, pMinExtend, pMaxExtend );
	
	if( pBlockNode ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockNode )->WriteToTBO();
	}
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
	
	if( pBlockVertex ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockVertex )->WriteToTBO();
	}
}

void deoglGIBVHDynamic::UpdateVertices( const oglVector3 *positions, int count ){
	if( count != pTBOVertex->GetPixelCount() ){
		DETHROW( deeInvalidParam );
	}
	
	float *data = pTBOVertex->GetDataFloat();
	int i;
	for( i=0; i<count; i++, data+=4 ){
		const oglVector3 &position = positions[ i ];
		data[ 0 ] = position.x;
		data[ 1 ] = position.y;
		data[ 2 ] = position.z;
		// data[ 3 ] = 0.0f;
	}
	
	if( pBlockVertex ){
		( ( deoglDynamicTBOBlock* )( deObject* )pBlockVertex )->WriteToTBO();
	}
}



deoglDynamicTBOBlock *deoglGIBVHDynamic::GetBlockNode(){
	if( ! pBlockNode ){
		pBlockNode.TakeOver( pGIBVHLocal.GetRenderThread().GetGI().GetBVHShared().GetSharedTBONode()
			->AddBlock( pGIBVHLocal.GetTBOIndex(), pTBONodeBox ) );
	}
	return ( deoglDynamicTBOBlock* )( deObject* )pBlockNode;
}

deoglDynamicTBOBlock *deoglGIBVHDynamic::GetBlockVertex(){
	if( ! pBlockVertex ){
		pBlockVertex.TakeOver( pGIBVHLocal.GetRenderThread().GetGI().GetBVHShared().GetSharedTBOVertex()
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
		      pWriteNodeExtends( firstIndex, minExtendLeft, maxExtendLeft );
		
		const deoglBVHNode &nodeRight = pGIBVHLocal.GetBVH().GetNodeAt( firstIndex + 1 );
		decVector minExtendRight, maxExtendRight;
		pCalcNodeExtends( nodeRight, minExtendRight, maxExtendRight );
		      pWriteNodeExtends( firstIndex + 1, minExtendRight, maxExtendRight );
		
		minExtend = minExtendLeft.Smallest( minExtendRight );
		maxExtend = maxExtendLeft.Largest( maxExtendRight );
		
	}else{
		const uint16_t *face = pGIBVHLocal.GetTBOFace()->GetDataUInt() + firstIndex * 4;
		const float * const vertices = pTBOVertex->GetDataFloat();
		int i;
		
		for( i=0; i<primitiveCount; i++, face+=4 ){
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
		
		// make sure boundaries have at least a minimum thickness or else ray casting code
		// can fail to detect the box. slightly enlarging the box is fine enough and makes
		// hitting boxes more robust
		const float margin = 1e-5f; // 0.01mm
		const decVector enlarge( decVector().Largest( decVector( margin, margin, margin ) - ( maxExtend - minExtend ) ) * 0.5f );
		minExtend -= enlarge;
		maxExtend += enlarge;
	}
}

void deoglGIBVHDynamic::pWriteNodeExtends( int index, const decVector &minExtend, const decVector &maxExtend ){
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
