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

#include "gdeOCComponentTexture.h"

#include <dragengine/common/exceptions.h>



// Class gdeOCComponentTexture
////////////////////////////////

// Constructor, destructor
////////////////////////////

gdeOCComponentTexture::gdeOCComponentTexture() :
pScale( 1.0f, 1.0f ),
pRotation( 0.0f ),
pColorTint( 1.0f, 1.0f, 1.0f ){
}

gdeOCComponentTexture::gdeOCComponentTexture( const char *name ) :
pName( name ),
pScale( 1.0f, 1.0f ),
pRotation( 0.0f ),
pColorTint( 1.0f, 1.0f, 1.0f ){
}

gdeOCComponentTexture::gdeOCComponentTexture( const gdeOCComponentTexture &texture ) :
pName( texture.pName ),
pPathSkin( texture.pPathSkin ),
pOffset( texture.pOffset ),
pScale( texture.pScale ),
pRotation( texture.pRotation ),
pColorTint( texture.pColorTint ),
pProperties( texture.pProperties ){
}

gdeOCComponentTexture::~gdeOCComponentTexture(){
}



// Management
///////////////

void gdeOCComponentTexture::SetName( const char *name ){
	pName = name;
}

void gdeOCComponentTexture::SetPathSkin( const char *path ){
	pPathSkin = path;
}

void gdeOCComponentTexture::SetOffset( const decVector2 &offset ){
	pOffset = offset;
}

void gdeOCComponentTexture::SetScale( const decVector2 &scale ){
	pScale = scale;
}

void gdeOCComponentTexture::SetRotation( float rotation ){
	pRotation = rotation;
}

void gdeOCComponentTexture::SetColorTint( const decColor &color ){
	pColorTint = color;
}

void gdeOCComponentTexture::SetActiveProperty( const char *property ){
	pActiveProperty = property;
}
