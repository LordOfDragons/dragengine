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
	pWidth = 10;
	pHeight = 0;
	pBearing = 0;
	pBearingY = 0;
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
	DEASSERT_TRUE(width >= 0)
	
	if( width != pWidth ){
		pWidth = width;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetHeight(int height){
	DEASSERT_TRUE(height >= 0)
	
	if(height != pHeight){
		pHeight = height;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetBearing( int bearing ){
	if( bearing != pBearing ){
		pBearing = bearing;
		NotifyGlyphChanged();
	}
}

void feFontGlyph::SetBearingY(int bearing){
	if(bearing != pBearingY){
		pBearingY = bearing;
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
		glyph->pHeight = pHeight;
		glyph->pBearing = pBearing;
		glyph->pBearingY = pBearingY;
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
