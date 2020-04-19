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

#include "igdeGDCCTexture.h"

#include <dragengine/common/exceptions.h>



// Class igdeGDCCTexture
//////////////////////////

// Constructor, destructor
////////////////////////////

igdeGDCCTexture::igdeGDCCTexture() :
pScale( 1.0f, 1.0f ),
pRotation( 0.0f ),
pColorTint( 1.0f, 1.0f, 1.0f ){
}

igdeGDCCTexture::igdeGDCCTexture( const igdeGDCCTexture &texture ) :
pName( texture.pName ),
pPathSkin( texture.pPathSkin ),
pOffset( texture.pOffset ),
pScale( texture.pScale ),
pRotation( texture.pRotation ),
pColorTint( texture.pColorTint ),
pProperties( texture.pProperties ){
}

igdeGDCCTexture::~igdeGDCCTexture(){
}



// Management
///////////////

void igdeGDCCTexture::SetName( const char *name ){
	pName = name;
}

void igdeGDCCTexture::SetPathSkin( const char *path ){
	pPathSkin = path;
}

void igdeGDCCTexture::SetOffset( const decVector2 &offset ){
	pOffset = offset;
}

void igdeGDCCTexture::SetScale( const decVector2 &scale ){
	pScale = scale;
}

void igdeGDCCTexture::SetRotation( float rotation ){
	pRotation = rotation;
}

void igdeGDCCTexture::SetColorTint( const decColor &color ){
	pColorTint = color;
}

void igdeGDCCTexture::SetProperties( const decStringDictionary &properties ){
	pProperties = properties;
}
