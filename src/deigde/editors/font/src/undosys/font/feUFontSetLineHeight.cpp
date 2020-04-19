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

#include "feUFontSetLineHeight.h"
#include "../../font/feFont.h"

#include <dragengine/common/exceptions.h>



// Class feUFontSetLineHeight
///////////////////////////////

// Constructor, destructor
////////////////////////////

feUFontSetLineHeight::feUFontSetLineHeight( feFont *font, int newLineHeight ) :
pFont( font )
{
	if( newLineHeight < 1 ) DETHROW( deeInvalidParam );
	
	SetShortInfo( "Font set line height" );
	
	pOldHeight = font->GetLineHeight();
	pNewHeight = newLineHeight;
}

feUFontSetLineHeight::~feUFontSetLineHeight(){
}



// Management
///////////////

void feUFontSetLineHeight::Undo(){
	pFont->SetLineHeight( pOldHeight );
}

void feUFontSetLineHeight::Redo(){
	pFont->SetLineHeight( pNewHeight );
}
