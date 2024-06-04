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
#include "feFontGlyphSelection.h"
#include "../feFont.h"

#include <dragengine/common/exceptions.h>



// Class feFontGlyphSelection
///////////////////////////////

// Constructor, destructor
////////////////////////////

feFontGlyphSelection::feFontGlyphSelection( feFont *parentFont ){
	if( ! parentFont ) DETHROW( deeInvalidParam );
	
	pParentFont = parentFont;
	pActive = NULL;
}

feFontGlyphSelection::~feFontGlyphSelection(){
	Reset();
}



// Management
///////////////

int feFontGlyphSelection::GetSelectedGlyphCount() const{
	return pSelected.GetGlyphCount();
}

feFontGlyph *feFontGlyphSelection::GetSelectedGlyphAt( int index ) const{
	return pSelected.GetGlyphAt( index );
}

bool feFontGlyphSelection::IsGlypthSelected( feFontGlyph *glyph ) const{
	return pSelected.HasGlyph( glyph );
}

int feFontGlyphSelection::IndexOfGlyph( feFontGlyph *glyph ) const{
	return pSelected.IndexOfGlyph( glyph );
}

void feFontGlyphSelection::AddGlyphToSelection( feFontGlyph *glyph ){
	if( ! glyph ) DETHROW( deeInvalidParam );
	
	if( ! IsGlypthSelected( glyph ) ){
		pSelected.AddGlyph( glyph );
		glyph->SetSelected( true );
	}
	
	pParentFont->NotifyGlyphSelectionChanged();
	
	if( ! pActive ){
		SetActiveGlyph( glyph );
	}
}

void feFontGlyphSelection::RemoveGlyphFromSelection( feFontGlyph *glyph ){
	if( ! glyph ) DETHROW( deeInvalidParam );
	
	int index = pSelected.IndexOfGlyph( glyph );
	
	glyph->SetSelected( false );
	if( glyph->GetActive() ){
		ActivateNextGlyph();
	}
	
	if( index != -1 ){
		pSelected.RemoveGlyph( glyph );
	}
	
	pParentFont->NotifyGlyphSelectionChanged();
}

void feFontGlyphSelection::RemoveAllGlyphsFromSelection(){
	SetActiveGlyph( NULL );
	pSelected.RemoveAllGlyphs();
	
	pParentFont->NotifyGlyphSelectionChanged();
}

void feFontGlyphSelection::GetSelectedList( feFontGlyphList &list ) const{
	list = pSelected;
}



bool feFontGlyphSelection::HasActiveGlyph() const{
	return pActive != NULL;
}

void feFontGlyphSelection::SetActiveGlyph( feFontGlyph *glyph ){
	if( glyph != pActive ){
		if( pActive ){
			pActive->SetActive( false );
			pActive->FreeReference();
		}
		
		pActive = glyph;
		
		if( glyph ){
			glyph->AddReference();
			glyph->SetActive( true );
		}
		
		pParentFont->NotifyActiveGlyphChanged();
	}
}

void feFontGlyphSelection::ActivateNextGlyph(){
	int g, count = pSelected.GetGlyphCount();
	feFontGlyph *nextGlyph = NULL;
	feFontGlyph *glyph;
	
	for( g=0; g<count; g++ ){
		glyph = pSelected.GetGlyphAt( g );
		
		if( pActive != glyph ){
			nextGlyph = glyph;
			break;
		}
	}
	
	SetActiveGlyph( nextGlyph );
}

void feFontGlyphSelection::Reset(){
	SetActiveGlyph( NULL );
	RemoveAllGlyphsFromSelection();
}
