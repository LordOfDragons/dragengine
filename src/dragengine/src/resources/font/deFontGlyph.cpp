/* 
 * Drag[en]gine Game Engine
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

#include "deFontGlyph.h"
#include "../../common/exceptions.h"



// Class deFontGlyph
//////////////////////

// Constructors and Destructors
/////////////////////////////////

deFontGlyph::deFontGlyph() :
pUnicode( 0 ),
pX( 0 ),
pY( 0 ),
pZ( 0 ),
pWidth( 0 ),
pBearing( 0 ),
pAdvance( 0 ){
}

deFontGlyph::~deFontGlyph(){
}



// Management
///////////////

void deFontGlyph::SetUnicode( int unicode ){
	if( unicode < 0 ){
		DETHROW( deeInvalidParam );
	}
	pUnicode = unicode;
}

void deFontGlyph::SetX( int x ){
	pX = x;
}

void deFontGlyph::SetY( int y ){
	pY = y;
}

void deFontGlyph::SetZ( int z ){
	if( z < 0 ){
		DETHROW( deeInvalidParam );
	}
	pZ = z;
}

void deFontGlyph::SetWidth( int width ){
	if( width < 0 ){
		DETHROW( deeInvalidParam );
	}
	pWidth = width;
}

void deFontGlyph::SetBearing( int bearing ){
	pBearing = bearing;
}

void deFontGlyph::SetAdvance( int advance ){
	if( advance < 0 ){
		DETHROW( deeInvalidParam );
	}
	pAdvance = advance;
}
