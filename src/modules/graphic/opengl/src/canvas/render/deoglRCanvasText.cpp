/* 
 * Drag[en]gine OpenGL Graphic Module
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

#include "deoglRCanvasText.h"
#include "../../font/deoglRFont.h"
#include "../../rendering/deoglRenderCanvas.h"
#include "../../rendering/deoglRenderCanvasContext.h"
#include "../../renderthread/deoglRenderThread.h"
#include "../../renderthread/deoglRTRenderers.h"

#include <dragengine/common/exceptions.h>



// Class deoglRCanvasText
///////////////////////////

// Constructor, destructor
////////////////////////////

deoglRCanvasText::deoglRCanvasText( deoglRenderThread &renderThread ) :
deoglRCanvas( renderThread ),
pFont( NULL ),
pFontSize( 1 ){
	LEAK_CHECK_CREATE( renderThread, CanvasText );
}

deoglRCanvasText::~deoglRCanvasText(){
	LEAK_CHECK_FREE( GetRenderThread(), CanvasText );
	if( pFont ){
		pFont->FreeReference();
	}
}



// Management
///////////////

void deoglRCanvasText::SetFont( deoglRFont *font ) {
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

void deoglRCanvasText::SetFontSize( float size ){
	pFontSize = decMath::max( size, 0.0f );
}

void deoglRCanvasText::SetText( const char *text ) {
	pText = text;
}

void deoglRCanvasText::SetColor( const decColor &color ){
	pColor = color;
}



void deoglRCanvasText::Render( const deoglRenderCanvasContext &context ){
	if( ! pFont || pText.IsEmpty() ){
		return;
	}
	
	const deoglRenderCanvasContext textContext( context, *this );
	GetRenderThread().GetRenderers().GetCanvas().DrawCanvasText( textContext, *this );
}
