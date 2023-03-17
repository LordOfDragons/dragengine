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

#include "deoglSharedBlockSPB.h"
#include "deoglSharedBlockSPBElement.h"
#include "../deoglShaderParameterBlock.h"
#include "../../../renderthread/deoglRenderThread.h"

#include <dragengine/common/exceptions.h>



// Class deoglSharedBlockSPB
//////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedBlockSPB::deoglSharedBlockSPB( deoglShaderParameterBlock *parameterBlock ) :
pParameterBlock( parameterBlock ),
pSize( parameterBlock->GetElementCount() ),
pUsedElementCount( 0 ),
pFreeElementCount( pSize )
{
	DEASSERT_TRUE( parameterBlock->GetElementCount() >= 1 )
	
	// add empty block for the element space
	pElements.Add( deObject::Ref::New( new deoglSharedBlockSPBElement( *this, 0, pFreeElementCount ) ) );
}

deoglSharedBlockSPB::~deoglSharedBlockSPB(){
}



// Management
///////////////

deoglSharedBlockSPBElement *deoglSharedBlockSPB::GetElement( int count ){
	DEASSERT_TRUE( count > 0 )
	
	pCheckSize();
	
	// find empty element with enough free space. if found the empty element is removed from
	// the list of empty elements if required
	const int index = pIndexOfEmptyElementWithMinCount( count );
	if( index == -1 ){
		return nullptr;
	}
	
	deoglSharedBlockSPBElement * const element = ( deoglSharedBlockSPBElement* )pElements.GetAt( index );
	
	// if empty block is larger than requested size add empty block with
	// remaining empty space right after this block
	if( element->GetCount() > count ){
		const deObject::Ref emptyElement( deObject::Ref::New( new deoglSharedBlockSPBElement(
			*this, element->GetIndex() + count, element->GetCount() - count ) ) );
		
		if( index + 1 < pElements.GetCount() ){
			// not the last element filling up to the available space
			pElements.Insert( emptyElement, index + 1 );
			pEmptyElements.Add( emptyElement );
			
		}else{
			// last element filling up to the available space
			pElements.Add( emptyElement );
		}
	}
	
	// turn the block into a non-empty block with the requested size
	element->SetCount( count );
	element->SetEmpty( false );
	
	// book keeping
	pFreeElementCount -= count;
	pUsedElementCount += count;
	
	return element;
}

void deoglSharedBlockSPB::ReturnElement( deoglSharedBlockSPBElement *element ){
	int index = pElements.IndexOf( element );
	DEASSERT_TRUE( index != -1 )
	
	bool addToEmptyElements = index < pElements.GetCount() - 1;
	const int count = element->GetCount();
	bool merged = false;
	
	// turn the block into an empty block
	element->SetEmpty( true );
	
	// if the previous block is empty merge this block with the previous block
	if( index > 0 ){
		deoglSharedBlockSPBElement * const mergeElement =
			( deoglSharedBlockSPBElement* )pElements.GetAt( index - 1 );
		if( mergeElement->GetEmpty() ){
			mergeElement->SetCount( mergeElement->GetCount() + count );
			
			pElements.RemoveFrom( index );
			
			addToEmptyElements = false;
			merged = true;
			element = mergeElement;
			index--;
			
			// if the previous empty block becomes the last block filling up space up the to
			// available space remove it from the empty block list
			if( index == pElements.GetCount() - 1 ){
				pEmptyElements.RemoveFrom( pEmptyElements.IndexOf( mergeElement ) );
			}
		}
	}
	
	// if the next block is empty merge the next block with this block
	if( index < pElements.GetCount() - 1 ){
		deoglSharedBlockSPBElement * const mergeElement =
			( deoglSharedBlockSPBElement* )pElements.GetAt( index + 1 );
		if( mergeElement->GetEmpty() ){
			element->SetCount( count + mergeElement->GetCount() );
			
			// if the next empty block is not the last block filling up space up the to
			// available space remove it from the empty block list
			if( index + 1 < pElements.GetCount() - 1 ){
				pEmptyElements.RemoveFrom( pEmptyElements.IndexOf( mergeElement ) );
			}
			
			pElements.RemoveFrom( index + 1 );
			
			// if the merged empty block is the last block filling up space up to available
			// space do not add it to the empty block list
			if( index == pElements.GetCount() - 1 ){
				addToEmptyElements = false;
				
				if( merged ){
					// in this situation the element has been merge with both the previous and
					// next empty element. in this case the previous empty element becomes the
					// last element filling up space up to available space and has to be
					// removed from the empty element list
					pEmptyElements.RemoveFrom( pEmptyElements.IndexOf( element ) );
				}
			}
		}
	}
	
	if( addToEmptyElements ){
		pEmptyElements.Add( element );
	}
	
	// book keeping
	pUsedElementCount -= count;
	pFreeElementCount += count;
}

int deoglSharedBlockSPB::pIndexOfEmptyElementWithMinCount( int count ){
	// check if an empty block with enough space can be reused. this list does not include
	// the last empty block filling the space up to the available space. this is done like
	// this to ensure anything but the last space filler block is used if possible
	const int emptyCount = pEmptyElements.GetCount();
	int i;
	
	for( i=0; i<emptyCount; i++ ){
		deoglSharedBlockSPBElement * const element =
			( deoglSharedBlockSPBElement* )pEmptyElements.GetAt( i );
		if( element->GetCount() >= count ){
			pEmptyElements.RemoveFrom( i );
			return pElements.IndexOf( element );
		}
	}
	
	// check if the last empty block filling up space up to the available space is usable
	deoglSharedBlockSPBElement * const element =
		( deoglSharedBlockSPBElement* )pElements.GetAt( pElements.GetCount() - 1 );
	if( element->GetEmpty() && element->GetCount() >= count ){
		return pElements.GetCount() - 1;
	}
	
	// no empty block found with enough free space
	return -1;
}

void deoglSharedBlockSPB::pCheckSize(){
	const int change = pParameterBlock->GetElementCount() - pSize;
	if( change == 0 ){
		return;
	}
	
	DEASSERT_TRUE( change > 0 )
	
	deoglSharedBlockSPBElement * const element =
		( deoglSharedBlockSPBElement* )pElements.GetAt( pElements.GetCount() - 1 );
	if( element->GetEmpty() ){
		element->SetCount( element->GetCount() + change );
		
	}else{
		pElements.Add( deObject::Ref::New( new deoglSharedBlockSPBElement( *this, pSize, change ) ) );
	}
	
	pFreeElementCount += change;
	pSize = pParameterBlock->GetElementCount();
}
