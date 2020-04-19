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

#include "feFontNotifier.h"



// Class feFontNotifier
////////////////////////

// Constructor, destructor
////////////////////////////

feFontNotifier::feFontNotifier(){
}

feFontNotifier::~feFontNotifier(){
}



// Notifications
//////////////////

void feFontNotifier::ModeChanged( feFont *font ){
}

void feFontNotifier::StateChanged( feFont *font ){
}

void feFontNotifier::UndoChanged( feFont *font ){
}



void feFontNotifier::FontChanged( feFont *font ){
}

void feFontNotifier::ImageChanged( feFont *font, feFontImage *image ){
}

void feFontNotifier::GlyphStructureChanged( feFont *font ){
}

void feFontNotifier::GlyphChanged( feFont *font, feFontGlyph *glyph ){
}

void feFontNotifier::GlyphSelectionChanged( feFont *font ){
}

void feFontNotifier::ActiveGlyphChanged( feFont *font ){
}
