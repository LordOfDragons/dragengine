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

#include "feFontGlyphSelection.h"
#include "../feFont.h"

#include <dragengine/common/exceptions.h>



// Class feFontGlyphSelection
///////////////////////////////

// Constructor, destructor
////////////////////////////

feFontGlyphSelection::feFontGlyphSelection(feFont *parentFont){
	DEASSERT_NOTNULL(parentFont)
	pParentFont = parentFont;
}

feFontGlyphSelection::~feFontGlyphSelection(){
	Reset();
}



// Management
///////////////

void feFontGlyphSelection::Add(feFontGlyph *glyph){
	DEASSERT_NOTNULL(glyph)
	if(!pSelected.Add(glyph)){
		return;
	}
	
	glyph->SetSelected(true);
	
	pParentFont->NotifyGlyphSelectionChanged();
	
	if(!pActive){
		SetActive(glyph);
	}
}

void feFontGlyphSelection::Remove(feFontGlyph *glyph){
	if(!pSelected.Remove(glyph)){
		return;
	}
	
	glyph->SetSelected(false);
	if(glyph->GetActive()){
		ActivateNextGlyph();
	}
	
	pParentFont->NotifyGlyphSelectionChanged();
}

void feFontGlyphSelection::RemoveAll(){
	if(pSelected.IsEmpty()){
		return;
	}
	
	SetActive(nullptr);
	pSelected.RemoveAll();
	
	pParentFont->NotifyGlyphSelectionChanged();
}


void feFontGlyphSelection::SetActive(feFontGlyph *glyph){
	if(glyph != pActive){
		if(pActive){
			pActive->SetActive(false);
		}
		
		pActive = glyph;
		
		if(glyph){
			glyph->SetActive(true);
		}
		
		pParentFont->NotifyActiveGlyphChanged();
	}
}

void feFontGlyphSelection::ActivateNextGlyph(){
	int g, count = pSelected.GetCount();
	feFontGlyph *nextGlyph = nullptr;
	
	for(g=0; g<count; g++){
		const feFontGlyph::Ref &glyph = pSelected.GetAt(g);
		if(glyph != pActive){
			nextGlyph = glyph;
			break;
		}
	}
	
	SetActive(nextGlyph);
}

void feFontGlyphSelection::Reset(){
	SetActive(nullptr);
	RemoveAll();
}
