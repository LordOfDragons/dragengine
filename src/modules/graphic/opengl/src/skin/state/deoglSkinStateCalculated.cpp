/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglSkinStateCalculated.h"
#include "../deoglSkinCalculatedProperty.h"
#include "../../texture/texture2d/deoglTexture.h"

#include <dragengine/common/exceptions.h>



// Class deoglSkinStateCalculated
///////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglSkinStateCalculated::deoglSkinStateCalculated() :
pTexture( nullptr ),
pProperty( nullptr ){
}

deoglSkinStateCalculated::~deoglSkinStateCalculated(){
	if( pTexture ){
		delete pTexture;
	}
}



// Management
///////////////

void deoglSkinStateCalculated::SetColor( const decColor &color ){
	pColor = color;
}

void deoglSkinStateCalculated::SetTexture( deoglTexture* texture ) {
	if( texture == pTexture ){
		return;
	}
	
	if( pTexture ){
		delete pTexture;
	}
	pTexture = texture;
}

void deoglSkinStateCalculated::SetProperty( deoglSkinCalculatedProperty *property ){
	pProperty = property;
}

void deoglSkinStateCalculated::Update( deoglSkinState &skinState ){
	if( pProperty ){
		pColor = pProperty->Calculate( skinState );
		
	}else{
		pColor.Set( 0.0f, 0.0f, 0.0f, 1.0f );
	}
}
