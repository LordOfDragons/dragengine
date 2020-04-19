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

#include "cePCBOption.h"
#include "cePlayerChoiceBox.h"
#include "../action/ceCAPlayerChoice.h"
#include "../action/ceCAPlayerChoiceOption.h"
#include "../ceConversation.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>



// Class cePCBOption
//////////////////////

// Constructor, destructor
////////////////////////////

cePCBOption::cePCBOption() :
pAction( NULL ),
pActionOption( NULL ),
pCanvasView( NULL ){
}

cePCBOption::~cePCBOption(){
	if( pCanvasView ){
		pCanvasView->FreeReference();
	}
	SetActionOption( NULL, NULL );
}



// Management
///////////////

void cePCBOption::SetText( const decUnicodeString &text ){
	pText = text;
}

void cePCBOption::SetActionOption( ceCAPlayerChoice *action, ceCAPlayerChoiceOption *option ){
	if( action != pAction ){
		if( pAction ){
			pAction->FreeReference();
		}
		
		pAction = action;
		
		if( action ){
			action->AddReference();
		}
	}
	
	if( option != pActionOption ){
		if( pActionOption ){
			pActionOption->FreeReference();
		}
		
		pActionOption = option;
		
		if( option ){
			option->AddReference();
		}
	}
}



void cePCBOption::Layout( const cePlayerChoiceBox &pcbox, bool selected ){
	// create canvas if not existing
	deCanvasManager &canvasManager = *pcbox.GetConversation().GetEngine()->GetCanvasManager();
	if( ! pCanvasView ){
		pCanvasView = canvasManager.CreateCanvasView();
	}
	
	// clear canvas
	pCanvasView->RemoveAllCanvas();
	pCanvasView->SetSize( decPoint( 10, 1 ) );
	
	// get font. if not existing exit since we can not render anything this way
	deFont * const font = pcbox.GetFont();
	if( ! font ){
		return;
	}
	
	// determine required height
	deCanvasView &parentVide = *pcbox.GetCanvasView();
	const decPoint &parentSize = parentVide.GetSize();
	deCanvasText *canvasText = NULL;
	deCanvasPaint *canvasBackground = NULL;
	
	const decString text( pText.ToUTF8() );
	
	const int textHeight = pcbox.GetTextSize();
	
	pCanvasView->SetSize( decPoint( parentSize.x, textHeight ) );
	
	try{
		canvasBackground = canvasManager.CreateCanvasPaint();
		canvasBackground->SetShapeType( deCanvasPaint::estRectangle );
		if( selected ){
			canvasBackground->SetFillColor( pcbox.GetSelectedBackgroundColor() );
		}else{
			canvasBackground->SetFillColor( pcbox.GetBackgroundColor() );
		}
		canvasBackground->SetLineColor( decColor( canvasBackground->GetFillColor(), 0.0f ) );
		canvasBackground->SetThickness( 0.0f );
		canvasBackground->SetOrder( 0.0f );
		canvasBackground->SetSize( pCanvasView->GetSize() );
		pCanvasView->AddCanvas( canvasBackground );
		canvasBackground->FreeReference();
		canvasBackground = NULL;
		
		canvasText = canvasManager.CreateCanvasText();
		if( selected ){
			canvasText->SetColor( pcbox.GetSelectedTextColor() );
		}else{
			canvasText->SetColor( pcbox.GetTextColor() );
		}
		canvasText->SetFont( font );
		canvasText->SetFontSize( textHeight );
		canvasText->SetText( text );
		canvasText->SetOrder( 0.0f );
		canvasText->SetSize( pCanvasView->GetSize() );
		pCanvasView->AddCanvas( canvasText );
		canvasText->FreeReference();
		canvasText = NULL;
		
	}catch( const deException & ){
		if( canvasText ){
			canvasText->FreeReference();
		}
		if( canvasBackground ){
			canvasBackground->FreeReference();
		}
		throw;
	}
}
