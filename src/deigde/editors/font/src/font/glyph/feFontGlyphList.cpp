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

feFontGlyph *feFontGlyphList::GetGlyphAt( int index ) const{
	return ( feFontGlyph* )pGlyphs.GetAt( index );
}

feFontGlyph *feFontGlyphList::GetGlyphWithCode( int code ) const{
	int g, count = pGlyphs.GetCount();
	feFontGlyph *glyph;
	
	for( g=0; g<count; g++ ){
		glyph = ( feFontGlyph* )pGlyphs.GetAt( g );
		if( glyph->GetCode() == code ){
			return glyph;
		}
	}
	
	return NULL;
}

int feFontGlyphList::IndexOfGlyph( feFontGlyph *glyph ) const{
	return pGlyphs.IndexOf( glyph );
}

int feFontGlyphList::IndexOfGlyphWithCode( int code ) const{
	int g, count = pGlyphs.GetCount();
	
	for( g=0; g<count; g++ ){
		if( ( ( feFontGlyph* )pGlyphs.GetAt( g ) )->GetCode() == code ){
			return g;
		}
	}
	
	return -1;
}

bool feFontGlyphList::HasGlyph( feFontGlyph *glyph ) const{
	return pGlyphs.Has( glyph );
}

bool feFontGlyphList::HasGlyphWithCode( int code ) const{
	int g, count = pGlyphs.GetCount();
	
	for( g=0; g<count; g++ ){
		if( ( ( feFontGlyph* )pGlyphs.GetAt( g ) )->GetCode() == code ){
			return true;
		}
	}
	
	return false;
}

void feFontGlyphList::AddGlyph( feFontGlyph *glyph ){
	if( ! glyph || HasGlyphWithCode( glyph->GetCode() ) ) DETHROW( deeInvalidParam );
	
	pGlyphs.Add( glyph );
}

void feFontGlyphList::RemoveGlyph( feFontGlyph *glyph ){
	if( ! glyph ) DETHROW( deeInvalidParam );
	
	pGlyphs.Remove( glyph );
}

void feFontGlyphList::RemoveGlyphWithCode ( int code ) {
	pGlyphs.RemoveIfPresent( GetGlyphWithCode( code ) );
}

void feFontGlyphList::RemoveAllGlyphs(){
	pGlyphs.RemoveAll();
}



feFontGlyphList &feFontGlyphList::operator=( const feFontGlyphList &list ){
	pGlyphs = list.pGlyphs;
	return *this;
}
