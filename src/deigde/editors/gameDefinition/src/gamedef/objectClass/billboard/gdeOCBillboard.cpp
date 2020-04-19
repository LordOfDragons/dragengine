/* 
 * Drag[en]gine IGDE Game Definition Editor
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

#include "gdeOCBillboard.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCBillboard
/////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCBillboard::gdeOCBillboard() :
pAxis( 0.0f, 1.0f, 0.0f ),
pSize( 1.0f, 1.0f ),
pLocked( true ),
pSpherical( true ),
pSizeFixedToScreen( false ),
pDoNotScale( false ),
pPartialHide( false ),
pRenderEnvMap( true ){
}

gdeOCBillboard::gdeOCBillboard( const gdeOCBillboard &billboard ) :
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

gdeOCBillboard::~gdeOCBillboard(){
}



// Management
///////////////

void gdeOCBillboard::SetSkinPath( const char *path ){
	pSkinPath = path;
}

void gdeOCBillboard::SetAxis( const decVector &axis ){
	pAxis = axis;
}

void gdeOCBillboard::SetSize( const decVector2 &size ){
	pSize = decVector2( 0.001f, 0.001f ).Largest( size );
}

void gdeOCBillboard::SetOffset( const decVector2 &offset ){
	pOffset = offset;
}

void gdeOCBillboard::SetLocked( bool locked ){
	pLocked = locked;
}

void gdeOCBillboard::SetSpherical( bool spherical ){
	pSpherical = spherical;
}

void gdeOCBillboard::SetSizeFixedToScreen( bool sizeFixedToScreen ){
	pSizeFixedToScreen = sizeFixedToScreen;
}

void gdeOCBillboard::SetDoNotScale( bool doNotScale ){
	pDoNotScale = doNotScale;
}

void gdeOCBillboard::SetPartialHide( bool partiallyHidden ){
	pPartialHide = partiallyHidden;
}

void gdeOCBillboard::SetRenderEnvMap( bool renderEnvMap ){
	pRenderEnvMap = renderEnvMap;
}

void gdeOCBillboard::SetPosition( const decDVector &position ){
	pPosition = position;
}

void gdeOCBillboard::SetBoneName( const char *boneName ){
	pBoneName = boneName;
}




bool gdeOCBillboard::IsPropertySet( eProperties property ) const{
	return ! pPropertyNames[ property ].IsEmpty();
}

const decString &gdeOCBillboard::GetPropertyName( eProperties property ) const{
	return pPropertyNames[ property ];
}

void gdeOCBillboard::SetPropertyName( eProperties property, const char *name ){
	pPropertyNames[ property ] = name;
}

bool gdeOCBillboard::HasPropertyWithName( const char *name ) const{
	int i;
	for( i=0; i<=epAttachPosition; i++ ){
		if( pPropertyNames[ i ] == name ){
			return true;
		}
	}
	
	return false;
}
