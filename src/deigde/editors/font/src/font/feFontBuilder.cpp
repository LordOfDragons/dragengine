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

feFontBuilder::feFontBuilder(feFont *font){
	if(!font) DETHROW(deeInvalidParam);
	
	pFont = font;
}

feFontBuilder::~feFontBuilder(){
}



// Management
///////////////

void feFontBuilder::BuildFont(deFont *engFont){
	if(!engFont){
		DETHROW(deeInvalidParam);
	}
	
	const int glyphCount = pFont->GetGlyphs().GetCount();
	
	// set font properties
	engFont->SetIsColorFont(pFont->GetColorFont());
	engFont->SetLineHeight(pFont->GetLineHeight());
	
	// set font image
	const feFontImage &fontImage = *pFont->GetFontImage();
	
	engFont->SetImage(fontImage.GetEngineImage());
	engFont->SetImagePath(fontImage.GetFilename());
	
	// set glyphs
	engFont->SetGlyphCount(glyphCount);
	
	int i;
	for(i=0; i<glyphCount; i++){
		const feFontGlyph &glyph = *pFont->GetGlyphs().GetAt(i);
		
		deFontGlyph &engGlyph = engFont->GetGlyphAt(i);
		
		engGlyph.SetUnicode(glyph.GetCode());
		engGlyph.SetX(glyph.GetU());
		engGlyph.SetY(glyph.GetV());
		engGlyph.SetZ(0);
		engGlyph.SetWidth(glyph.GetWidth());
		engGlyph.SetHeight(pFont->GetLineHeight());
		engGlyph.SetBearing(glyph.GetBearing());
		engGlyph.SetAdvance(glyph.GetAdvance());
		
	}
}
