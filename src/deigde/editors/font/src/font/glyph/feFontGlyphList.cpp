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
#include "feFontGlyphList.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>



// Class feFontGlyph
//////////////////////

// Constructor, destructor
////////////////////////////

feFontGlyphList::feFontGlyphList(){
}

feFontGlyphList::~feFontGlyphList(){
}



// Management
///////////////

int feFontGlyphList::GetGlyphCount() const{
	return pGlyphs.GetCount();
}

feFontGlyph *feFontGlyphList::GetGlyphAt(int index) const{
	return (feFontGlyph*)pGlyphs.GetAt(index);
}

feFontGlyph *feFontGlyphList::GetGlyphWithCode(int code) const{
	int g, count = pGlyphs.GetCount();
	feFontGlyph *glyph;
	
	for(g=0; g<count; g++){
		glyph = (feFontGlyph*)pGlyphs.GetAt(g);
		if(glyph->GetCode() == code){
			return glyph;
		}
	}
	
	return NULL;
}

int feFontGlyphList::IndexOfGlyph(feFontGlyph *glyph) const{
	return pGlyphs.IndexOf(glyph);
}

int feFontGlyphList::IndexOfGlyphWithCode(int code) const{
	int g, count = pGlyphs.GetCount();
	
	for(g=0; g<count; g++){
		if(((feFontGlyph*)pGlyphs.GetAt(g))->GetCode() == code){
			return g;
		}
	}
	
	return -1;
}

bool feFontGlyphList::HasGlyph(feFontGlyph *glyph) const{
	return pGlyphs.Has(glyph);
}

bool feFontGlyphList::HasGlyphWithCode(int code) const{
	int g, count = pGlyphs.GetCount();
	
	for(g=0; g<count; g++){
		if(((feFontGlyph*)pGlyphs.GetAt(g))->GetCode() == code){
			return true;
		}
	}
	
	return false;
}

void feFontGlyphList::AddGlyph(feFontGlyph *glyph){
	if(!glyph || HasGlyphWithCode(glyph->GetCode())) DETHROW(deeInvalidParam);
	
	pGlyphs.Add(glyph);
}

void feFontGlyphList::RemoveGlyph(feFontGlyph *glyph){
	if(!glyph) DETHROW(deeInvalidParam);
	
	pGlyphs.Remove(glyph);
}

void feFontGlyphList::RemoveGlyphWithCode (int code) {
	pGlyphs.RemoveIfPresent(GetGlyphWithCode(code));
}

void feFontGlyphList::RemoveAllGlyphs(){
	pGlyphs.RemoveAll();
}



feFontGlyphList &feFontGlyphList::operator=(const feFontGlyphList &list){
	pGlyphs = list.pGlyphs;
	return *this;
}
