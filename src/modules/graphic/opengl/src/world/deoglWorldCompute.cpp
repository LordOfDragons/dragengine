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
#include "../rendering/deoglRenderCompute.h"
#include "../shaders/paramblock/deoglSPBMapBuffer.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/common/utils/decLayerMask.h>



// deoglWorldCompute::sDataElement
////////////////////////////////////

void deoglWorldCompute::sDataElement::SetExtends( const decDVector &a, const decDVector &b ){
	minExtendX = ( GLfloat )a.x;
	minExtendY = ( GLfloat )a.y;
	minExtendZ = ( GLfloat )a.z;
	maxExtendX = ( GLfloat )b.x;
	maxExtendY = ( GLfloat )b.y;
	maxExtendZ = ( GLfloat )b.z;
}

void deoglWorldCompute::sDataElement::SetLayerMask( const decLayerMask &a ){
	layerMask[ 0 ] = ( uint32_t )( a.GetMask() >> 32 );
	layerMask[ 1 ] = ( uint32_t )a.GetMask();
}

void deoglWorldCompute::sDataElement::SetEmptyLayerMask(){
	layerMask[ 0 ] = 0;
	layerMask[ 1 ] = 0;
}



// deoglWorldCompute::Element
///////////////////////////////

deoglWorldCompute::Element::Element( deoglWorldCompute::eElementTypes type, const void *owner ) :
pType( type ),
pOwner( owner ),
pIndex( -1 ),
pUpdateRequired( false )
{
	DEASSERT_NOTNULL( owner );
}

deoglWorldCompute::Element::~Element(){
}

void deoglWorldCompute::Element::SetIndex( int index ){
	pIndex = index;
}

void deoglWorldCompute::Element::SetUpdateRequired( bool updateRequired ){
	pUpdateRequired = updateRequired;
}



// Class deoglWorldCompute
////////////////////////////

// Constructors and Destructors
/////////////////////////////////

deoglWorldCompute::deoglWorldCompute( deoglRWorld &world ) :
pWorld( world ),
pFullUpdateLimit( 0 ),
pFullUpdateFactor( 0.2f )
{
	deoglRenderThread &renderThread = world.GetRenderThread();
	const bool rowMajor = renderThread.GetCapabilities().GetUBOIndirectMatrixAccess().Working();
	
	pSSBOElements.TakeOver( new deoglSPBlockSSBO( renderThread ) );
	pSSBOElements->SetRowMajor( rowMajor );
	pSSBOElements->SetParameterCount( 5 );
	pSSBOElements->GetParameterAt( espeMinExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOElements->GetParameterAt( espeFlags ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( espeMaxExtend ).SetAll( deoglSPBParameter::evtFloat, 3, 1, 1 );
	pSSBOElements->GetParameterAt( espeUpdateIndex ).SetAll( deoglSPBParameter::evtInt, 1, 1, 1 );
	pSSBOElements->GetParameterAt( espeLayerMask ).SetAll( deoglSPBParameter::evtInt, 2, 1, 1 );
	pSSBOElements->MapToStd140();
	pSSBOElements->SetBindingPoint( 0 );
}

deoglWorldCompute::~deoglWorldCompute(){
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
	
	pUpdateFullUpdateLimit();
}



int deoglWorldCompute::GetElementCount() const{
	return pElements.GetCount();
}

deoglWorldCompute::Element &deoglWorldCompute::GetElementAt( int index ) const{
	return *( Element* )pElements.GetAt( index );
}

void deoglWorldCompute::AddElement( Element *element ){
	DEASSERT_NOTNULL( element );
	DEASSERT_TRUE( element->GetIndex() == -1 )
	
	const int index = pElements.GetCount();
	
	pElements.Add( element );
	element->SetIndex( index );
	element->SetUpdateRequired( true );
	
	if( pUpdateElements.GetCount() < pFullUpdateLimit ){
		pUpdateElements.Add( element );
	}
}

void deoglWorldCompute::UpdateElement( Element *element ){
	DEASSERT_NOTNULL( element );
	DEASSERT_TRUE( element->GetIndex() != -1 )
	
	if( element->GetIndex() == -1 || element->GetUpdateRequired() ){
		return;
	}
	
	element->SetUpdateRequired( true );
	
	if( pUpdateElements.GetCount() < pFullUpdateLimit ){
		pUpdateElements.Add( element );
	}
}

void deoglWorldCompute::RemoveElement( Element *element ){
	DEASSERT_NOTNULL( element );
	
	const int index = element->GetIndex();
	DEASSERT_TRUE( index != -1 )
	
	const int last = pElements.GetCount() - 1;
	
	element->SetIndex( -1 );
	element->SetUpdateRequired( false );
	
	if( index < last ){
		Element * const swap = ( Element* )pElements.GetAt( last );
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
	sDataElement * const data = ( sDataElement* )ssbo.GetWriteBuffer();
	int i;
	
	for( i=0; i<count; i++ ){
		Element &element = *( ( Element* )pUpdateElements.GetAt( i ) );
		element.UpdateData( *this, data[ i ] );
		data[ i ].updateIndex = ( uint32_t )element.GetIndex();
	}
	
	pUpdateElementCount = count;
}

void deoglWorldCompute::pFullUpdateSSBOElements(){
	const int count = pElements.GetCount();
	if( count == 0 ){
		return;
	}
	
	int i;
	
	if( count > pSSBOElements->GetElementCount() ){
		pSSBOElements->SetElementCount( count + 100 );
	}
	
	const deoglSPBMapBuffer mapped( pSSBOElements );
	sDataElement * const data = ( sDataElement* )pSSBOElements->GetWriteBuffer();
	
	for( i=0; i<count; i++ ){
		Element &element = *( ( Element* )pElements.GetAt( i ) );
		element.UpdateData( *this, data[ i ] );
		element.SetUpdateRequired( false );
	}
	
	pUpdateElementCount = 0;
}

void deoglWorldCompute::pUpdateFullUpdateLimit(){
	pFullUpdateLimit = decMath::max( pFullUpdateLimit,
		( int )( pFullUpdateFactor * ( float )pElements.GetCount() ) );
}
