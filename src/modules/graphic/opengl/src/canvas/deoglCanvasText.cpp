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

#include "deoglCanvasText.h"
#include "render/deoglRCanvasText.h"
#include "../deGraphicOpenGl.h"
#include "../font/deoglFont.h"
#include "../font/deoglRFont.h"

#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvas.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/font/deFont.h>



// Class deoglCanvasText
//////////////////////////

// Constructor, destructor
////////////////////////////

deoglCanvasText::deoglCanvasText( deGraphicOpenGl &ogl, deCanvasText &canvas ) :
deoglCanvas( ogl, canvas ),
pCanvasText( canvas ),
pRCanvasText( NULL ){
}

deoglCanvasText::~deoglCanvasText(){
}



// Management
///////////////

void deoglCanvasText::DropRCanvas(){
	pRCanvasText = NULL;
	deoglCanvas::DropRCanvas();
}

void deoglCanvasText::SyncContentToRender(){
	const deFont * const font = pCanvasText.GetFont();
	deoglRFont *rfont = NULL;
	
	if( font ){
		deoglFont &oglFont = *( ( deoglFont* )font->GetPeerGraphic() );
		rfont = oglFont.GetRFont();
	}
	
	pRCanvasText->SetFont( rfont );
	pRCanvasText->SetFontSize( pCanvasText.GetFontSize() );
	pRCanvasText->SetColor( pCanvasText.GetColor() );
	pRCanvasText->SetText( pCanvasText.GetText() );
}



// Protected Functions
////////////////////////

deoglRCanvas *deoglCanvasText::CreateRCanvas(){
	pRCanvasText = new deoglRCanvasText( GetOgl().GetRenderThread() );
	return pRCanvasText;
}
