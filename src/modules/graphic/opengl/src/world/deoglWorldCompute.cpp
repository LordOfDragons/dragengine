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

#include "deoglRWorld.h"
#include "deoglWorldCompute.h"
#include "../capabilities/deoglCapabilities.h"
#include "../renderthread/deoglRenderThread.h"
#include "../renderthread/deoglRTRenderers.h"
#include "../renderthread/deoglRTLogger.h"
#include "../rendering/deoglRenderCompute.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"

#include <dragengine/common/exceptions.h>



// Class deoglWorldCompute
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglWorldCompute::deoglWorldCompute( deoglRWorld &world ) :
pWorld( world ),
pFullUpdateLimit( 0 ),
pFullUpdateFactor( 0.2f ),
pUpdateElementCount( 0 ),
pFullUpdateGeometryLimit( 0 ),
pFullUpdateGeometryFactor( 0.25f ),
pUpdateElementGeometryCount( 0 )
{
	deoglRenderThread &renderThread = world.GetRenderThread();
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pSSBOElements.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBOElements->SetRowMajor( rowMajor );
	pSSBOElements->SetParameterCount( 11 );
	pSSBOElements->GetParameterAt( espeMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOElements->GetParameterAt( espeFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( espeMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOElements->GetParameterAt( espeUpdateIndex ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( espeLayerMask ).SetAll( deoglSPBParameter::evtInt, 2, 1, 1 );
	pSSBOElements->GetParameterAt( espeFirstGeometry ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( espeGeometryCount ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( espeLodFactors ).SetAll( deoglSPBParameter::evtFloat, 4, 1, 1 );
	pSSBOElements->GetParameterAt( espeHighestLod ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( espeCullResult ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( espeLodIndex ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->MapToStd140();
	
	pSSBOElementGeometries.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBOElementGeometries->SetRowMajor( rowMajor );
	pSSBOElementGeometries->SetParameterCount( 9 );
	pSSBOElementGeometries->GetParameterAt( espetElement ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElementGeometries->GetParameterAt( espetLod ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElementGeometries->GetParameterAt( espetRenderFilter ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElementGeometries->GetParameterAt( espetSkinTexture ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElementGeometries->GetParameterAt( espetPipelineBase ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElementGeometries->GetParameterAt( espetVao ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElementGeometries->GetParameterAt( espetInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElementGeometries->GetParameterAt( espetSPBInstance ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElementGeometries->GetParameterAt( espetTUCs ).SetAll( deoglSPBParameter::evtInt, 4, 1, 1 );
	pSSBOElementGeometries->MapToStd140();
	
	pSharedSPBGeometries.TakeOver( new deoglSharedBlockSPB( pSSBOElementGeometries ) );
}

deoglWorldCompute::~deoglWorldCompute(){
	// quick dispose allows elements like components to drop out of the world quickly.
	// in this case though they do not remove themselves from the compute world properly.
	// to avoid problems remove all elements before cleaning up
	const int count = pElements.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deoglWorldComputeElement &element = *( deoglWorldComputeElement* )pElements.GetAt( i );
		element.SetIndex( -1 );
		element.SetWorldCompute( nullptr );
		element.GetSPBGeometries() = nullptr;
	}
	pElements.RemoveAll();
}



// Management
///////////////

void deoglWorldCompute::Prepare(){
	if( pUpdateElements.GetCount() < pFullUpdateLimit
	&& pElements.GetCount() <= pSSBOElements->GetElementCount() ){
		pUpdateSSBOElements();
		
	}else{
		pFullUpdateSSBOElements();
	}
	
	pUpdateFullUpdateLimits();
}

void deoglWorldCompute::PrepareGeometries(){
	if( pUpdateElementGeometries.GetCount() < pFullUpdateGeometryLimit ){
		pUpdateSSBOElementGeometries();
		// pFullUpdateSSBOElementGeometries();
		
	}else{
		pFullUpdateSSBOElementGeometries();
	}
	
	pUpdateFullUpdateGeometryLimits();
}



int deoglWorldCompute::GetElementCount() const{
	return pElements.GetCount();
}

deoglWorldComputeElement &deoglWorldCompute::GetElementAt( int index ) const{
	return *( deoglWorldComputeElement* )pElements.GetAt( index );
}

void deoglWorldCompute::AddElement( deoglWorldComputeElement *element ){
	DEASSERT_NOTNULL( element );
	DEASSERT_TRUE( element->GetIndex() == -1 )
	
	const int index = pElements.GetCount();
	
	pElements.Add( element );
	element->SetWorldCompute( this );
	element->SetIndex( index );
	element->SetUpdateRequired( true );
	
	if( pUpdateElements.GetCount() < pFullUpdateLimit ){
		pUpdateElements.Add( element );
	}
}

void deoglWorldCompute::UpdateElement( deoglWorldComputeElement *element ){
	DEASSERT_NOTNULL( element );
	DEASSERT_TRUE( element->GetIndex() != -1 )
	
	if( element->GetUpdateRequired() ){
		return;
	}
	
	element->SetUpdateRequired( true );
	
	if( pUpdateElements.GetCount() < pFullUpdateLimit ){
		pUpdateElements.Add( element );
	}
}

void deoglWorldCompute::RemoveElement( deoglWorldComputeElement *element ){
	DEASSERT_NOTNULL( element );
	
	const int index = element->GetIndex();
	DEASSERT_TRUE( index != -1 )
	
	const int last = pElements.GetCount() - 1;
	
	element->GetSPBGeometries() = nullptr;
	element->SetIndex( -1 );
	element->SetWorldCompute( nullptr );
	element->SetUpdateRequired( false );
	element->SetUpdateGeometriesRequired( false );
	
	if( index < last ){
		deoglWorldComputeElement * const swap = ( deoglWorldComputeElement* )pElements.GetAt( last );
		pElements.SetAt( index, swap );
		swap->SetIndex( index );
		
		if( ! swap->GetUpdateRequired() ){
			swap->SetUpdateRequired( true );
			
			if( pUpdateElements.GetCount() < pFullUpdateLimit ){
				pUpdateElements.Add( element );
			}
		}
	}
	
	pElements.RemoveFrom( last );
}

int deoglWorldCompute::GetElementGeometryCount() const{
	return GetElementCount() > 0 ? pSSBOElementGeometries->GetElementCount() : 0;
}

void deoglWorldCompute::UpdateElementGeometries( deoglWorldComputeElement *element ){
	DEASSERT_NOTNULL( element );
	DEASSERT_TRUE( element->GetIndex() != -1 )
	
	if( pFullUpdateGeometryFactor == 0 || element->GetUpdateGeometriesRequired() ){
		return;
	}
	
	element->SetUpdateGeometriesRequired( true );
	
	if( pUpdateElementGeometries.GetCount() < pFullUpdateGeometryLimit ){
		pUpdateElementGeometries.Add( element );
	}
}



// Private Functions
//////////////////////

void deoglWorldCompute::pUpdateSSBOElements(){
	const int count = pUpdateElements.GetCount();
	
	const deoglRenderCompute &renderCompute = pWorld.GetRenderThread().GetRenderers().GetCompute();
	deoglSPBlockSSBO &ssbo = renderCompute.GetSSBOUpdateElements();
	
	if( count > ssbo.GetElementCount() ){
		ssbo.SetElementCount( count );
	}
	
	const deoglSPBMapBuffer mapped( ssbo );
	deoglWorldComputeElement::sDataElement * const data =
		( deoglWorldComputeElement::sDataElement* )ssbo.GetWriteBuffer();
	int i;
	
	memset( data, 0, ssbo.GetElementStride() * count );
	
	for( i=0; i<count; i++ ){
		deoglWorldComputeElement &element = *( ( deoglWorldComputeElement* )pUpdateElements.GetAt( i ) );
		pUpdateSSBOElement( element, data[ i ] );
		data[ i ].updateIndex = ( uint32_t )element.GetIndex();
	}
	
	pUpdateElementCount = count;
	pUpdateElements.RemoveAll();
}

void deoglWorldCompute::pFullUpdateSSBOElements(){
	pUpdateElementCount = 0;
	pUpdateElements.RemoveAll();
	
	const int count = pElements.GetCount();
	if( count == 0 ){
		return;
	}
	
	if( count > pSSBOElements->GetElementCount() ){
		pSSBOElements->SetElementCount( count + 100 );
	}
	
	const deoglSPBMapBuffer mapped( pSSBOElements );
	deoglWorldComputeElement::sDataElement * const data =
		( deoglWorldComputeElement::sDataElement* )pSSBOElements->GetWriteBuffer();
	
	memset( data, 0, pSSBOElements->GetElementStride() * count );
	
	int i;
	for( i=0; i<count; i++ ){
		pUpdateSSBOElement( *( deoglWorldComputeElement* )pElements.GetAt( i ), data[ i ] );
	}
}

void deoglWorldCompute::pUpdateSSBOElement( deoglWorldComputeElement &element,
deoglWorldComputeElement::sDataElement &data ){
	element.UpdateData( data );
	element.SetUpdateRequired( false );
	
	pCheckElementGeometryCount( element, data );
	
	if( element.GetSPBGeometries() ){
		data.firstGeometry = ( uint32_t )element.GetSPBGeometries()->GetIndex();
	}
}

void deoglWorldCompute::pCheckElementGeometryCount( deoglWorldComputeElement &element,
deoglWorldComputeElement::sDataElement &data ){
	// check if geometry count changed
	int oldGeometryCount = 0;
	if( element.GetSPBGeometries() ){
		oldGeometryCount = element.GetSPBGeometries()->GetCount();
	}
	
	const int geometryCount = data.geometryCount;
	if( geometryCount == oldGeometryCount ){
		return;
	}
	
	if( geometryCount == 0 ){
		element.GetSPBGeometries() = nullptr;
		return;
	}
	
	if( geometryCount < oldGeometryCount ){
		// if the geometry count is less we can keep using the same spb element. this does waste
		// some slots but chances are the geometry count increses again soon most probably back
		// to the old count. this is typically the case with outline geometries
		return;
	}
	
	// first drop the spb element. this returns it to the spb. this is required since the
	// returned spb element could be merged producing a larger empty space we can consume
	element.GetSPBGeometries() = nullptr;
	element.GetSPBGeometries() = pSharedSPBGeometries->GetElement( geometryCount );
	
	// if no new element can be obtained we have to grow the spb. this requires a full update
	// so we can stop trying to do partial updates in the future
	if( element.GetSPBGeometries() ){
		return;
	}
	
	pSSBOElementGeometries->SetElementCount( pSSBOElementGeometries->GetElementCount() + geometryCount + 100 );
	pFullUpdateGeometryLimit = 0;
	
	element.GetSPBGeometries() = pSharedSPBGeometries->GetElement( geometryCount );
}



void deoglWorldCompute::pUpdateSSBOElementGeometries(){
		// decTimer timer;
	const int elementCount = pUpdateElementGeometries.GetCount();
	int i;
	
	pUpdateElementGeometryCount = 0;
	
	for( i=0; i<elementCount; i++ ){
		const deoglWorldComputeElement &element = *( ( deoglWorldComputeElement* )pUpdateElementGeometries.GetAt( i ) );
		if( element.GetSPBGeometries() ){
			pUpdateElementGeometryCount += element.GetSPBGeometries()->GetCount();
		}
	}
	
	if( pUpdateElementGeometryCount == 0 ){
		pUpdateElementGeometries.RemoveAll();
		return;
	}
	
	const deoglRenderCompute &renderCompute = pWorld.GetRenderThread().GetRenderers().GetCompute();
	deoglSPBlockSSBO &ssboData = renderCompute.GetSSBOUpdateElementGeometries();
	deoglSPBlockSSBO &ssboIndex = renderCompute.GetSSBOUpdateIndices();
	
	if( pUpdateElementGeometryCount > ssboData.GetElementCount() ){
		ssboData.SetElementCount( pUpdateElementGeometryCount );
	}
	
	const int countIndex = ( pUpdateElementGeometryCount - 1 ) / 4 + 1;
	if( countIndex > ssboIndex.GetElementCount() ){
		ssboIndex.SetElementCount( countIndex );
	}
	
	const deoglSPBMapBuffer mappedData( ssboData ), mappedIndex( ssboIndex );
	deoglWorldComputeElement::sDataElementGeometry * const dataData =
		( deoglWorldComputeElement::sDataElementGeometry* )ssboData.GetWriteBuffer();
	uint32_t * const dataIndex = ( uint32_t* )ssboIndex.GetWriteBuffer();
	
	memset( dataData, 0, ssboData.GetElementStride() * pUpdateElementGeometryCount );
	memset( dataIndex, 0, ssboIndex.GetElementStride() * countIndex );
	
	int j, nextIndex = 0;
	
	for( i=0; i<elementCount; i++ ){
		deoglWorldComputeElement &element = *( ( deoglWorldComputeElement* )pUpdateElementGeometries.GetAt( i ) );
		if( ! element.GetSPBGeometries() ){
			continue;
		}
		
		element.UpdateDataGeometries( dataData + nextIndex );
		element.SetUpdateGeometriesRequired( false );
		
		const deoglSharedBlockSPBElement &spbel = element.GetSPBGeometries();
		const int firstGeometry = spbel.GetIndex();
		const int geometryCount = spbel.GetCount();
		
		for( j=0; j<geometryCount; j++ ){
			dataIndex[ nextIndex++ ] = ( uint32_t )( firstGeometry + j );
		}
	}
		// pWorld.GetRenderThread().GetLogger().LogInfoFormat("pUpdateSSBOElementGeometries: Write %dys (%d)", (int)(timer.GetElapsedTime() * 1e6f), pUpdateElementGeometryCount);
		// mappedData.Unmap();
		// mappedIndex.Unmap();
		// pWorld.GetRenderThread().GetLogger().LogInfoFormat("pUpdateSSBOElementGeometries: Upload %dys", (int)(timer.GetElapsedTime() * 1e6f));
	
	pUpdateElementGeometries.RemoveAll();
}

void deoglWorldCompute::pFullUpdateSSBOElementGeometries(){
	pUpdateElementGeometryCount = 0;
	pUpdateElementGeometries.RemoveAll();
	
	const int elementCount = pElements.GetCount();
	if( elementCount == 0 ){
		return;
	}
	
		// decTimer timer;
	const deoglSPBMapBuffer mapped( pSSBOElementGeometries );
	deoglWorldComputeElement::sDataElementGeometry * const data =
		( deoglWorldComputeElement::sDataElementGeometry* )pSSBOElementGeometries->GetWriteBuffer();
	
	pSSBOElementGeometries->Clear();
	
	int i;
	for( i=0; i<elementCount; i++ ){
		deoglWorldComputeElement &element = *( ( deoglWorldComputeElement* )pElements.GetAt( i ) );
		if( element.GetSPBGeometries() ){
			element.UpdateDataGeometries( data + element.GetSPBGeometries()->GetIndex() );
			element.SetUpdateGeometriesRequired( false );
		}
	}
		// pWorld.GetRenderThread().GetLogger().LogInfoFormat( "pFullUpdateSSBOElementGeometries: Write %dys", (int)(timer.GetElapsedTime() * 1e6f) );
		// mapped.Unmap();
		// pWorld.GetRenderThread().GetLogger().LogInfoFormat( "pFullUpdateSSBOElementGeometries: Upload %dys", (int)(timer.GetElapsedTime() * 1e6f) );
}



void deoglWorldCompute::pUpdateFullUpdateLimits(){
	pFullUpdateLimit = decMath::max( pFullUpdateLimit,
		( int )( pFullUpdateFactor * ( float )pElements.GetCount() ) );
}

void deoglWorldCompute::pUpdateFullUpdateGeometryLimits(){
	// this is count of elements not geometries
	pFullUpdateGeometryLimit = decMath::max( pFullUpdateGeometryLimit,
		( int )( pFullUpdateGeometryFactor * ( float )pElements.GetCount() ) );
}
