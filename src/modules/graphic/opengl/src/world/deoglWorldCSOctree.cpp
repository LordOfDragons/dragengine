/* 
 * Drag[en]gine OpenGL Graphic Module
 *
 * Copyright (C) 2023, Roland Plüss (roland@rptd.ch)
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

#include "deoglWorldOctree.h"
#include "deoglWorldCSOctree.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>



// Class deoglWorldCSOctree
/////////////////////////////

void deoglWorldCSOctree::sCSNode::SetExtends( const decDVector &a, const decDVector &b ){
	minExtendX = ( GLfloat )a.x;
	minExtendY = ( GLfloat )a.y;
	minExtendZ = ( GLfloat )a.z;
	maxExtendX = ( GLfloat )b.x;
	maxExtendY = ( GLfloat )b.y;
	maxExtendZ = ( GLfloat )b.z;
}

void deoglWorldCSOctree::sCSElement::SetExtends( const decDVector &a, const decDVector &b ){
	minExtendX = ( GLfloat )a.x;
	minExtendY = ( GLfloat )a.y;
	minExtendZ = ( GLfloat )a.z;
	maxExtendX = ( GLfloat )b.x;
	maxExtendY = ( GLfloat )b.y;
	maxExtendZ = ( GLfloat )b.z;
}

void deoglWorldCSOctree::sCSElement::SetLayerMask( const decLayerMask &a ){
	layerMask[ 0 ] = ( uint32_t )( a.GetMask() >> 32 );
	layerMask[ 1 ] = ( uint32_t )a.GetMask();
}

// Constructors and Destructors
/////////////////////////////////

deoglWorldCSOctree::deoglWorldCSOctree( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pPtrNode( nullptr ),
pPtrElement( nullptr ),
pNodeCount( 0 ),
pElementCount( 0 ),
pNextNode( 0 ),
pNextElement( 0 ),
pElementLinks( nullptr ),
pElementLinkSize( 0 )
{
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pSSBONodes.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBONodes->SetRowMajor( rowMajor );
	pSSBONodes->SetParameterCount( 5 );
	pSSBONodes->GetParameterAt( ecsnpMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBONodes->GetParameterAt( ecsnpFirstNode ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBONodes->GetParameterAt( ecsnpMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBONodes->GetParameterAt( ecsnpChildNodeCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBONodes->GetParameterAt( ecsnpElementCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBONodes->MapToStd140();
	pSSBONodes->SetBindingPoint( 1 );
	
	pSSBOElements.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBOElements->SetRowMajor( rowMajor );
	pSSBOElements->SetParameterCount( 5 );
	pSSBOElements->GetParameterAt( ecsepMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOElements->GetParameterAt( ecsepElementIndex ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( ecsepMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOElements->GetParameterAt( ecsepFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( ecsepLayerMask ).SetAll( deoglSPBParameter::evtInt, 2, 1, 1 );
	pSSBOElements->MapToStd140();
	pSSBOElements->SetBindingPoint( 0 );
}

deoglWorldCSOctree::~deoglWorldCSOctree(){
	EndWriting();
	
	if( pElementLinks ){
		delete [] pElementLinks;
	}
}



// Management
///////////////

void deoglWorldCSOctree::SetReferencePosition( const decDVector &position ){
	pReferencePosition = position;
}

void deoglWorldCSOctree::Clear(){
	DEASSERT_NULL( pPtrNode )
	
	pNodeCount = 0;
	pElementCount = 0;
	pNextNode = 0;
	pNextElement = 0;
}

void deoglWorldCSOctree::BeginWriting( int nodeCount, int elementCount ){
	DEASSERT_TRUE( nodeCount >= 0 )
	DEASSERT_TRUE( elementCount >= 0 )
	
	EndWriting();
	
	pNodeCount = nodeCount;
	pElementCount = elementCount;
	pNextNode = 0;
	pNextElement = 0;
	
	if( nodeCount == 0 && elementCount == 0 ){
		return;
	}
	
	pSSBONodes->SetElementCount( nodeCount );
	pSSBOElements->SetElementCount( elementCount );
	
	pSSBONodes->MapBuffer();
	pSSBOElements->MapBuffer();
	
	pPtrNode = ( sCSNode* )pSSBONodes->GetWriteBuffer();
	pPtrElement = ( sCSElement* )pSSBOElements->GetWriteBuffer();
	
	if( elementCount > pElementLinkSize ){
		if( pElementLinks ){
			delete pElementLinks;
			pElementLinks = nullptr;
			pElementLinkSize = 0;
		}
		
		pElementLinks = new sElementLink[ elementCount ];
		pElementLinkSize = elementCount;
	}
}

void deoglWorldCSOctree::EndWriting(){
	if( pPtrElement ){
		pNextElement = 0;
		pPtrElement = nullptr;
		pSSBOElements->UnmapBuffer();
	}
	if( pPtrNode ){
		pNextNode = 0;
		pPtrNode = nullptr;
		pSSBONodes->UnmapBuffer();
	}
}



deoglWorldCSOctree::sCSNode &deoglWorldCSOctree::GetNodeAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pNodeCount )
	DEASSERT_NOTNULL( pPtrNode )
	return pPtrNode[ index ];
}

int deoglWorldCSOctree::NextNode(){
	DEASSERT_TRUE( pNextNode < pNodeCount )
	return pNextNode++;
}

deoglWorldCSOctree::sCSNode &deoglWorldCSOctree::NextNodeRef(){
	return pPtrNode[ NextNode() ];
}

void deoglWorldCSOctree::AdvanceNextData( int amount ){
	DEASSERT_TRUE( amount >= 0 )
	DEASSERT_TRUE( pNextNode + amount <= pNodeCount )
	pNextNode += amount;
}


deoglWorldCSOctree::sCSElement &deoglWorldCSOctree::GetElementAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pElementCount )
	DEASSERT_NOTNULL( pPtrElement )
	return pPtrElement[ index ];
}

int deoglWorldCSOctree::NextElement( eCSElementTypes type, const void *link ){
	DEASSERT_TRUE( pNextElement < pElementCount )
	DEASSERT_NOTNULL( link );
	
	pElementLinks[ pNextElement ].type = type;
	pElementLinks[ pNextElement ].element = link;
	return pNextElement++;
}

deoglWorldCSOctree::sCSElement &deoglWorldCSOctree::NextElementRef( eCSElementTypes type, const void *link ){
	return pPtrElement[ NextElement( type, link ) ];
}

const deoglWorldCSOctree::sElementLink &deoglWorldCSOctree::GetLinkAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pElementCount )
	return pElementLinks[ index ];
}
