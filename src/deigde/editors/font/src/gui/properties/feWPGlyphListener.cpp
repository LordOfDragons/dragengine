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

#include "feWPGlyph.h"
#include "feWPGlyphListener.h"
#include "../../font/feFont.h"
#include "../../font/glyph/feFontGlyphSelection.h"

#include <dragengine/common/exceptions.h>



// Class feWPGlyphListener
////////////////////////////

// Constructor, destructor
////////////////////////////

feWPGlyphListener::feWPGlyphListener( feWPGlyph &panel ) :
pPanel( panel ){
}

feWPGlyphListener::~feWPGlyphListener(){
}



// Management
///////////////

void feWPGlyphListener::GlyphStructureChanged( feFont *font ){
	pPanel.UpdateGlyphList();
}

void feWPGlyphListener::GlyphChanged( feFont *font, feFontGlyph *glyph ){
	if( glyph == pPanel.GetGlyph() ){
		pPanel.UpdateGlyph();
	}
}

void feWPGlyphListener::ActiveGlyphChanged( feFont *font ){
	pPanel.SetGlyph( font->GetGlyphSelection().GetActiveGlyph() );
}
