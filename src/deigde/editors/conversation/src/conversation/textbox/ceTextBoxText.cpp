/* 
 * Drag[en]gine IGDE Conversation Editor
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

#include "ceTextBox.h"
#include "ceTextBoxText.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasText.h>



// Class ceTextBoxText
////////////////////////

// Constructor, destructor
////////////////////////////

ceTextBoxText::ceTextBoxText() :
pCanvasView( NULL ){
}

ceTextBoxText::~ceTextBoxText(){
	if( pCanvasView ){
		pCanvasView->FreeReference();
	}
}



// Management
///////////////

void ceTextBoxText::SetName( const decUnicodeString &name ){
	pName = name;
}

void ceTextBoxText::SetText( const decUnicodeString &text ){
	pText = text;
}

void ceTextBoxText::Layout( const ceTextBox &textBox ){
	// create canvas if not existing
	deCanvasManager &canvasManager = *textBox.GetEngine().GetCanvasManager();
	if( ! pCanvasView ){
		pCanvasView = canvasManager.CreateCanvasView();
	}
	
	// clear canvas
	pCanvasView->RemoveAllCanvas();
	pCanvasView->SetSize( decPoint( 10, 1 ) );
	
	// get font. if not existing exit since we can not render anything this way
	deFont * const font = textBox.GetFont();
	if( ! font ){
		return;
	}
	
	// determine required height
	deCanvasView &parentVide = *textBox.GetCanvasView();
	const decPoint &parentSize = parentVide.GetSize();
	deCanvasText *canvasText = NULL;
	
	const decString name( pName.ToUTF8() );
	const decString text( pText.ToUTF8() );
	const int padding = textBox.GetPadding();
	const int offset = textBox.GetTextOffset();
	
	const int textHeight = textBox.GetTextSize();
	const int top = 0;
	const int left = padding;
	const int right = parentSize.x - padding;
	
	pCanvasView->SetSize( decPoint( parentSize.x, textHeight ) );
	
	try{
		// create name canvas
		canvasText = canvasManager.CreateCanvasText();
		canvasText->SetColor( textBox.GetTextColor() );
		canvasText->SetFont( font );
		canvasText->SetFontSize( ( float )textBox.GetTextSize() );
		canvasText->SetText( name );
		canvasText->SetOrder( 0.0f );
		canvasText->SetPosition( decPoint( left, top ) );
		canvasText->SetSize( decPoint( offset, textHeight ) );
		pCanvasView->AddCanvas( canvasText );
		canvasText->FreeReference();
		canvasText = NULL;
		
		// create text canvas
		canvasText = canvasManager.CreateCanvasText();
		canvasText->SetColor( textBox.GetTextColor() );
		canvasText->SetFont( font );
		canvasText->SetFontSize( ( float )textBox.GetTextSize() );
		canvasText->SetText( text );
		canvasText->SetOrder( 1.0f );
		canvasText->SetPosition( decPoint( left + offset, top ) );
		canvasText->SetSize( decPoint( right - offset - left, textHeight ) );
		pCanvasView->AddCanvas( canvasText );
		canvasText->FreeReference();
		canvasText = NULL;
		
	}catch( const deException & ){
		if( canvasText ){
			canvasText->FreeReference();
		}
		throw;
	}
}
