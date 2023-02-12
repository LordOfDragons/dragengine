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

#include <dragengine/common/exceptions.h>



// Class deoglSharedBlockSPBElement::Ref
//////////////////////////////////////////

deoglSharedBlockSPBElement::Ref::Ref() :
pElement( nullptr ){
}

deoglSharedBlockSPBElement::Ref::Ref( const Ref & ){
	DETHROW( deeInvalidAction );
}

deoglSharedBlockSPBElement::Ref::~Ref(){
	if( pElement ){
		pElement->Return();
	}
}

deoglSharedBlockSPBElement::Ref::operator deoglSharedBlockSPBElement *() const{
	return pElement;
}

deoglSharedBlockSPBElement::Ref::operator deoglSharedBlockSPBElement &() const{
	DEASSERT_NOTNULL( pElement );
	return *pElement;
}

deoglSharedBlockSPBElement *deoglSharedBlockSPBElement::Ref::operator->() const{
	DEASSERT_NOTNULL( pElement );
	return pElement;
}

deoglSharedBlockSPBElement::Ref &deoglSharedBlockSPBElement::Ref::operator=( deoglSharedBlockSPBElement *element ){
	if( element == pElement ){
		return *this;
	}
	
	if( pElement ){
		pElement->Return();
	}
	
	pElement = element;
	return *this;
}

bool deoglSharedBlockSPBElement::Ref::operator!() const{
	return pElement == nullptr;
}

deoglSharedBlockSPBElement::Ref::operator bool() const{
	return pElement != nullptr;
}



// Class deoglSharedBlockSPBElement
/////////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSharedBlockSPBElement::deoglSharedBlockSPBElement( deoglSharedBlockSPB &spb, int index, int count ) :
pSPB( spb ),
pIndex( index ),
pCount( count ),
pEmpty( true )
{
	DEASSERT_TRUE( count > 0 )
}

deoglSharedBlockSPBElement::~deoglSharedBlockSPBElement(){
}



// Management
///////////////

void deoglSharedBlockSPBElement::SetCount( int count ){
	DEASSERT_TRUE( count > 0 )
	pCount = count;
}

void deoglSharedBlockSPBElement::SetEmpty( bool empty ){
	pEmpty = empty;
}

deoglShaderParameterBlock &deoglSharedBlockSPBElement::MapBuffer() const{
	pSPB.GetParameterBlock()->MapBuffer( pIndex, pCount );
	return *pSPB.GetParameterBlock();
}

void deoglSharedBlockSPBElement::Return() {
	pSPB.ReturnElement( this );
}
