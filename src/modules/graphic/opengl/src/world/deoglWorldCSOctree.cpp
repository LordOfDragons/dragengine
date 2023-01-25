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

void deoglWorldCSOctree::sCSData::SetExtends( const decDVector &minExtend, const decDVector &maxExtend ){
	minExtendX = ( GLfloat )minExtend.x;
	minExtendY = ( GLfloat )minExtend.y;
	minExtendZ = ( GLfloat )minExtend.z;
	maxExtendX = ( GLfloat )maxExtend.x;
	maxExtendY = ( GLfloat )maxExtend.y;
	maxExtendZ = ( GLfloat )maxExtend.z;
}

void deoglWorldCSOctree::sCSData::SetLayerMask( const decLayerMask &layerMask ){
	layerMaskUpper = ( uint32_t )( layerMask.GetMask() >> 32 );
	layerMaskLower = ( uint32_t )layerMask.GetMask();
}

// Constructors and Destructors
/////////////////////////////////

deoglWorldCSOctree::deoglWorldCSOctree( deoglRenderThread &renderThread ) :
pRenderThread( renderThread ),
pPtrData( nullptr ),
pNodeCount( 0 ),
pElementCount( 0 ),
pNextData( 0 ),
pNextElement( 0 ),
pElementLinks( nullptr ),
pElementLinkSize( 0 ){
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
	DEASSERT_NULL( pPtrData )
	
	pNodeCount = 0;
	pElementCount = 0;
	pNextData = 0;
	pNextElement = 0;
}

void deoglWorldCSOctree::BeginWriting( int nodeCount, int elementCount ){
	DEASSERT_TRUE( nodeCount >= 0 )
	DEASSERT_TRUE( elementCount >= 0 )
	DEASSERT_NULL( pPtrData )
	
	pSSBOData->SetElementCount( nodeCount );
	pNodeCount = nodeCount;
	
	pElementCount = elementCount;
	
	pNextData = 0;
	pNextElement = 0;
	
	if( nodeCount == 0 && elementCount == 0 ){
		return;
	}
	
	if( ! pSSBOData ){
		pSSBOData.TakeOver( new deoglSPBlockSSBO( pRenderThread ) );
		pSSBOData->SetRowMajor( pRenderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working() );
		pSSBOData->SetParameterCount( 3 );
		pSSBOData->GetParameterAt( 0 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 );
		pSSBOData->GetParameterAt( 1 ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 );
		pSSBOData->GetParameterAt( 2 ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
		pSSBOData->MapToStd140();
		pSSBOData->SetBindingPoint( 0 );
	}
	
	pSSBOData->MapBuffer();
	
	pPtrData = ( sCSData* )pSSBOData->GetWriteBuffer();
	
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
	if( ! pPtrData ){
		return;
	}
	
	pNextData = 0;
	pNextElement = 0;
	pPtrData = nullptr;
	
	pSSBOData->UnmapBuffer();
}

deoglWorldCSOctree::sCSData &deoglWorldCSOctree::GetDataAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pNodeCount )
	DEASSERT_NOTNULL( pPtrData )
	return pPtrData[ index ];
}

int deoglWorldCSOctree::NextData(){
	DEASSERT_TRUE( pNextData < pNodeCount )
	return pNextData++;
}

deoglWorldCSOctree::sCSData &deoglWorldCSOctree::NextDataRef(){
	DEASSERT_TRUE( pNextData < pNodeCount )
	return pPtrData[ pNextData++ ];
}

void deoglWorldCSOctree::AdvanceNextData( int amount ){
	DEASSERT_TRUE( amount >= 0 )
	DEASSERT_TRUE( pNextData + amount <= pNodeCount )
	pNextData += amount;
}

int deoglWorldCSOctree::NextElement( eCSElementTypes type, const void *link ){
	DEASSERT_TRUE( pNextElement < pElementCount )
	DEASSERT_NOTNULL( link );
	
	pElementLinks[ pNextElement ].type = type;
	pElementLinks[ pNextElement ].element = link;
	return pNextElement++;
}

const deoglWorldCSOctree::sElementLink &deoglWorldCSOctree::GetLinkAt( int index ) const{
	DEASSERT_TRUE( index >= 0 )
	DEASSERT_TRUE( index < pElementCount )
	return pElementLinks[ index ];
}
