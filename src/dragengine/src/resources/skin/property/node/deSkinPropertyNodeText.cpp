/* 
 * Drag[en]gine Game Engine
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

#include "deSkinPropertyNodeText.h"
#include "deSkinPropertyNodeVisitor.h"
#include "../../../font/deFont.h"
#include "../../../../common/exceptions.h"



// Class deSkinPropertyNodeText
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deSkinPropertyNodeText::deSkinPropertyNodeText() :
pFont( NULL ),
pFontSize( 10.0f ){
}

deSkinPropertyNodeText::~deSkinPropertyNodeText(){
	if( pFont ){
		pFont->FreeReference();
	}
}



// Management
///////////////

void deSkinPropertyNodeText::SetPath( const char *path ){
	pPath = path;
}

void deSkinPropertyNodeText::SetFont( deFont *font ){
	if( font == pFont ){
		return;
	}
	
	if( pFont ){
		pFont->FreeReference();
	}
	
	pFont = font;
	
	if( font ){
		font->AddReference();
	}
}

void deSkinPropertyNodeText::SetFontSize( float size ){
	pFontSize = decMath::max( size, 0.0f );
}

void deSkinPropertyNodeText::SetText( const char *text ){
	pText = text;
}

void deSkinPropertyNodeText::SetColor( const decColor &color ){
	pColor = color;
}



// Visiting
/////////////

void deSkinPropertyNodeText::Visit( deSkinPropertyNodeVisitor &visitor ){
	visitor.VisitText( *this );
}
