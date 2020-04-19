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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deoglCapsTextureFormat.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUnicodeString.h>



// Class deoglCapsTextureFormat
/////////////////////////////////

// Constructor, destructor
////////////////////////////

deoglCapsTextureFormat::deoglCapsTextureFormat( GLint format, GLenum pixelFormat, GLenum pixelType,
int bitsPerPixel, bool isDepth, bool isDepthFloat, bool isCompressed, const char *name ) :
pFormat( format ),
pPixelFormat( pixelFormat ),
pPixelType( pixelType ),
pBitsPerPixel( bitsPerPixel ),
pIsDepth( isDepth ),
pIsDepthFloat( isDepthFloat ),
pIsCompressed( isCompressed ),
pName( name ){
}

deoglCapsTextureFormat::~deoglCapsTextureFormat(){
}



// Management
///////////////

void deoglCapsTextureFormat::WriteToString( decUnicodeString &buffer ) const{
	buffer.AppendFromUTF8( pName.GetString() );
	buffer.AppendFromUTF8( ",\n" );
}
