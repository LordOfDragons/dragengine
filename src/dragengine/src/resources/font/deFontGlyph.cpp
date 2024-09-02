/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
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
