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
#include <string.h>

#include "feUGlyphSetV.h"
#include "../../font/feFont.h"
#include "../../font/glyph/feFontGlyph.h"

#include <dragengine/common/exceptions.h>



// Class feUGlyphSetV
///////////////////////

// Constructor, destructor
////////////////////////////

feUGlyphSetV::feUGlyphSetV( feFontGlyph *glyph, int newV ){
	SetShortInfo( "GLyph set v" );
	
	pOldV = glyph->GetV();
	pNewV = newV;
	
	pGlyph = glyph;
	glyph->AddReference();
}

feUGlyphSetV::~feUGlyphSetV(){
	if( pGlyph ) pGlyph->FreeReference();
}



// Management
///////////////

void feUGlyphSetV::Undo(){
	pGlyph->SetV( pOldV );
}

void feUGlyphSetV::Redo(){
	pGlyph->SetV( pNewV );
}
