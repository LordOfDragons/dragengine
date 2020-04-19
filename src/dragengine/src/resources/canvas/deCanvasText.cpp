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

#include "deCanvasText.h"
#include "deCanvasVisitor.h"
#include "../font/deFont.h"
#include "../../common/exceptions.h"



// Class deCanvasText
////////////////////////

// Constructors and Destructors
/////////////////////////////////

deCanvasText::deCanvasText( deCanvasManager *manager ) :
deCanvas( manager ),

pFontSize( 10.0f ){
}

deCanvasText::~deCanvasText(){
}



// Management
///////////////

void deCanvasText::SetFont( deFont *font ){
	if( font == pFont ){
		return;
	}
	
	pFont = font;
	
	NotifyContentChanged();
}

void deCanvasText::SetFontSize( float size ){
	if( size < 0.0f ){
		size = 0.0f;
	}
	
	if( fabsf( size - pFontSize ) < FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pFontSize = size;
	NotifyContentChanged();
}

void deCanvasText::SetText( const char *text ){
	if( ! text ){
		DETHROW( deeInvalidParam );
	}
	
	if( pText == text ){
		return;
	}
	
	pText = text;
	NotifyContentChanged();
}

void deCanvasText::SetColor( const decColor &color ){
	if( pColor.IsEqualTo( color ) ){
		return;
	}
	
	pColor = color;
	NotifyContentChanged();
}



// Visiting
/////////////

void deCanvasText::Visit( deCanvasVisitor &visitor ){
	visitor.VisitText( *this );
}
