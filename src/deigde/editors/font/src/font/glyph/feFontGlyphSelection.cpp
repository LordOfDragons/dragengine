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
