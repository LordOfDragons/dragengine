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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceTextBox.h"
#include "ceTextBoxText.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class ceTextBox
////////////////////

// Constructor, destructor
////////////////////////////

ceTextBox::ceTextBox( deEngine &engine, deLogger &logger ) :
pEngine( engine ),
pLogger( logger ),

pEngFont( NULL ),

pBackgroundColor( 0.0f, 0.0f, 0.0f, 0.5f ),
pTextColor( 1.0f, 1.0f, 1.0f, 1.0f ),
pTextSize( 18 ),
pPadding( 10 ),
pTextOffset( 200 ),

pCanvasView( NULL )
{
	try{
		pCanvasView = engine.GetCanvasManager()->CreateCanvasView();
		pCanvasView->SetOrder( 10.0f );
		
		SetPathFont( "/igde/fonts/sans_9_border.defont" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

ceTextBox::~ceTextBox(){
	pCleanUp();
}



// Management
///////////////

void ceTextBox::SetPathFont( const char *path ){
	if( ! path ){
		DETHROW( deeInvalidParam );
	}
	
	if( pPathFont.Equals( path ) ){
		return;
	}
	
	pPathFont = path;
	pUpdateFont();
	UpdateCanvas();
}

void ceTextBox::SetBackgroundColor( const decColor &color ){
	if( color.IsEqualTo( pBackgroundColor ) ){
		return;
	}
	
	pBackgroundColor = color;
	UpdateCanvas();
}

void ceTextBox::SetTextColor( const decColor &color ){
	if( color.IsEqualTo( pTextColor ) ){
		return;
	}
	
	pTextColor = color;
	UpdateCanvas();
}

void ceTextBox::SetTextSize( int size ){
	if( size < 1 ){
		size = 1;
	}
	
	if( size == pTextSize ){
		return;
	}
	
	pTextSize = size;
	UpdateCanvas();
}

void ceTextBox::SetPadding( int padding ){
	if( padding < 0 ){
		padding = 0;
	}
	
	if( padding == pPadding ){
		return;
	}
	
	pPadding = padding;
	UpdateCanvas();
}

void ceTextBox::SetTextOffset( int offset ){
	if( offset < 0 ){
		offset = 0;
	}
	
	if( offset == pTextOffset ){
		return;
	}
	
	pTextOffset = offset;
	UpdateCanvas();
}



void ceTextBox::UpdateCanvas(){
	// if there is no parent canvas there is no use in updating anything
	deCanvasView * const parentView = pCanvasView->GetParentView();
	if( ! parentView ){
		return;
	}
	
	// remove all text canvas
	pCanvasView->RemoveAllCanvas();
	
	// calculate required box size
	const int count = pTexts.GetCount();
	int boxHeight = pPadding * 2;
	int i;
	
	for( i=0; i<count; i++ ){
		ceTextBoxText &text = *pTexts.GetAt( i );
		
		text.Layout( *this );
		boxHeight += text.GetCanvasView()->GetSize().y;
	}
	
	// keep box at a height of at least 20 pixel
	if( pEngFont ){
		boxHeight = decMath::max( boxHeight, pPadding * 2 + pTextSize );
	}
	
	// resize canvas
	const decPoint &parentSize = parentView->GetSize();
	
	pCanvasView->SetSize( decPoint( parentSize.x, boxHeight ) );
	pCanvasView->SetPosition( decPoint( 0, parentSize.y - boxHeight ) );
	
	// add background color canvas
	deCanvasPaint *canvasBackground = NULL;
	try{
		canvasBackground = pEngine.GetCanvasManager()->CreateCanvasPaint();
		canvasBackground->SetShapeType( deCanvasPaint::estRectangle );
		canvasBackground->SetFillColor( pBackgroundColor );
		canvasBackground->SetLineColor( decColor( pBackgroundColor, 0.0f ) );
		canvasBackground->SetThickness( 0.0f );
		canvasBackground->SetOrder( 0.0f );
		canvasBackground->SetSize( pCanvasView->GetSize() );
		pCanvasView->AddCanvas( canvasBackground );
		canvasBackground->FreeReference();
		
	}catch( const deException & ){
		if( canvasBackground ){
			canvasBackground->FreeReference();
		}
		throw;
	}
	
	// add text canvas
	int y = pPadding;
	
	for( i=0; i<count; i++ ){
		deCanvasView * const textCanvas = pTexts.GetAt( i )->GetCanvasView();
		const int height = textCanvas->GetSize().y;
		
		textCanvas->SetPosition( decPoint( 0, y ) );
		textCanvas->SetOrder( ( float )( 1 + i ) );
		pCanvasView->AddCanvas( textCanvas );
		
		y += height;
	}
}



// Private Functions
//////////////////////

void ceTextBox::pCleanUp(){
	pTexts.RemoveAll();
	
	if( pCanvasView ){
		pCanvasView->FreeReference();
	}
	
	if( pEngFont ){
		pEngFont->FreeReference();
	}
}



void ceTextBox::pUpdateFont(){
	deFont *font = NULL;
	
	try{
		if( ! pPathFont.IsEmpty() ){
			font = pEngine.GetFontManager()->LoadFont( pPathFont, "/" );
		}
		
		if( pEngFont ){
			pEngFont->FreeReference();
		}
		pEngFont = font;
		
	}catch( const deException &e ){
		if( font ){
			font->FreeReference();
		}
		pLogger.LogException( LOGSOURCE, e );
	}
}
