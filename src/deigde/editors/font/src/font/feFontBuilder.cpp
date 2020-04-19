/* 
 * Drag[en]gine IGDE Font Editor
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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "feFont.h"
#include "feFontBuilder.h"
#include "glyph/feFontGlyph.h"
#include "image/feFontImage.h"

#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/common/exceptions.h>



// Class feFontBuilder
////////////////////////

// Constructor, destructor
////////////////////////////

feFontBuilder::feFontBuilder( feFont *font ){
	if( ! font ) DETHROW( deeInvalidParam );
	
	pFont = font;
}

feFontBuilder::~feFontBuilder(){
}



// Management
///////////////

void feFontBuilder::BuildFont( deFont *engFont ){
	if( ! engFont ){
		DETHROW( deeInvalidParam );
	}
	
	const int glyphCount = pFont->GetGlyphCount();
	
	// set font properties
	engFont->SetIsColorFont( pFont->GetColorFont() );
	engFont->SetLineHeight( pFont->GetLineHeight() );
	
	// set font image
	const feFontImage &fontImage = *pFont->GetFontImage();
	
	engFont->SetImage( fontImage.GetEngineImage() );
	engFont->SetImagePath( fontImage.GetFilename() );
	
	// set glyphs
	engFont->SetGlyphCount( glyphCount );
	
	int i;
	for( i=0; i<glyphCount; i++ ){
		const feFontGlyph &glyph = *pFont->GetGlyphAt( i );
		
		deFontGlyph &engGlyph = engFont->GetGlyphAt( i );
		
		engGlyph.SetUnicode( glyph.GetCode() );
		engGlyph.SetX( glyph.GetU() );
		engGlyph.SetY( glyph.GetV() );
		engGlyph.SetZ( 0 );
		engGlyph.SetWidth( glyph.GetWidth() );
		engGlyph.SetBearing( glyph.GetBearing() );
		engGlyph.SetAdvance( glyph.GetAdvance() );
		
	}
}
