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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "feFontGlyph.h"
#include "../feFont.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class feFontGlyph
//////////////////////

// Constructor, destructor
////////////////////////////

feFontGlyph::feFontGlyph(){
	pParentFont = NULL;
	
	pCode = 0;
	pU = 0;
	pV = 0;
	pWidth =10;
	pBearing = 0;
	pAdvance = 10;
	
	pSelected = false;
	pActive = false;
}

feFontGlyph::~feFontGlyph(){
}



// Management
///////////////

void feFontGlyph::SetParentFont( feFont* font ){
	pParentFont = font;
}



void feFontGlyph::SetCode( int code ){
	if( code < 0 ) DETHROW( deeInvalidParam );
	
	if( code != pCode ){
		pCode = code;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetU( int u ){
	if( u != pU ){
		pU = u;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetV( int v ){
	if( v != pV ){
		pV = v;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetWidth( int width ){
	if( width < 0 ) DETHROW( deeInvalidParam );
	
	if( width != pWidth ){
		pWidth = width;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetBearing( int bearing ){
	if( bearing != pBearing ){
		pBearing = bearing;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetAdvance( int advance ){
	if( advance < 0 ) DETHROW( deeInvalidParam );
	
	if( advance != pAdvance ){
		pAdvance = advance;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetSelected( bool selected ){
	pSelected = selected;
}

void feFontGlyph::SetActive( bool active ){
	pActive = active;
}

feFontGlyph* feFontGlyph::Copy() const{
	feFontGlyph *glyph = NULL;
	
	try{
		glyph = new feFontGlyph;
		if( ! glyph ) DETHROW( deeOutOfMemory );
		
		glyph->pCode = pCode;
		glyph->pU = pU;
		glyph->pV = pV;
		glyph->pWidth = pWidth;
		glyph->pBearing = pBearing;
		glyph->pAdvance = pAdvance;
		
	}catch( const deException & ){
		if( glyph ) delete glyph;
		throw;
	}
	
	return glyph;
}



void feFontGlyph::NotifyGlyphChanged(){
	if( pParentFont ){
		pParentFont->SetChanged( true );
		pParentFont->NotifyGlyphChanged( this );
	}
}
