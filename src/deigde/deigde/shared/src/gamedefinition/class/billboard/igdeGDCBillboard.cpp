/* 
 * Drag[en]gine IGDE
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

#include "igdeGDCBillboard.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/billboard/deBillboard.h>



// Class igdeGDCBillboard
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCBillboard::igdeGDCBillboard() :
pAxis( 0.0f, 1.0f, 0.0f ),
pSize( 1.0f, 1.0f ),
pLocked( true ),
pSpherical( true ),
pSizeFixedToScreen( false ),

pDoNotScale( false ),
pPartialHide( false ),
pRenderEnvMap( true ){
}

igdeGDCBillboard::igdeGDCBillboard( const igdeGDCBillboard &billboard ) :
pSkinPath( billboard.pSkinPath ),
pAxis( billboard.pAxis ),
pSize( billboard.pSize ),
pOffset( billboard.pOffset ),
pLocked( billboard.pLocked ),
pSpherical( billboard.pSpherical ),
pSizeFixedToScreen( billboard.pSizeFixedToScreen ),

pDoNotScale( billboard.pDoNotScale ),
pPartialHide( billboard.pPartialHide ),
pRenderEnvMap( billboard.pRenderEnvMap ),
pPosition( billboard.pPosition ),
pBoneName( billboard.pBoneName )
{
	int i;
	for( i=0; i<=epAttachPosition; i++ ){
		pPropertyNames[ i ] = billboard.pPropertyNames[ i ];
	}
}

igdeGDCBillboard::~igdeGDCBillboard(){
}



// Management
///////////////

void igdeGDCBillboard::SetSkinPath( const char *path ){
	pSkinPath = path;
}

void igdeGDCBillboard::SetAxis( const decVector &axis ){
	pAxis = axis;
}

void igdeGDCBillboard::SetSize( const decVector2 &size ){
	pSize = decVector2( FLOAT_SAFE_EPSILON, FLOAT_SAFE_EPSILON ).Largest( size );
}

void igdeGDCBillboard::SetOffset( const decVector2 &offset ){
	pOffset = offset;
}

void igdeGDCBillboard::SetLocked( bool locked ){
	pLocked = locked;
}

void igdeGDCBillboard::SetSpherical( bool spherical ){
	pSpherical = spherical;
}

void igdeGDCBillboard::SetSizeFixedToScreen( bool sizeFixedToScreen ){
	pSizeFixedToScreen = sizeFixedToScreen;
}



void igdeGDCBillboard::SetDoNotScale( bool doNotScale ){
	pDoNotScale = doNotScale;
}

void igdeGDCBillboard::SetPartialHide( bool partiallyHidden ){
	pPartialHide = partiallyHidden;
}

void igdeGDCBillboard::SetRenderEnvMap( bool renderEnvMap ){
	pRenderEnvMap = renderEnvMap;
}

void igdeGDCBillboard::SetPosition( const decDVector &position ){
	pPosition = position;
}

void igdeGDCBillboard::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}



bool igdeGDCBillboard::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &igdeGDCBillboard::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void igdeGDCBillboard::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool igdeGDCBillboard::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachPosition; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}
