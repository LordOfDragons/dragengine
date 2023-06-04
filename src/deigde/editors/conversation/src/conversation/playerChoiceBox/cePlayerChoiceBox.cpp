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

#include "cePCBOption.h"
#include "cePlayerChoiceBox.h"
#include "../ceConversation.h"
#include "../playback/cePlayback.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include "../playback/cePlaybackActionStack.h"
#include "../action/ceConversationAction.h"



// Definitions
////////////////

#define LOGSOURCE "Conversation Editor"



// Class cePlayerChoiceBox
////////////////////

// Constructor, destructor
////////////////////////////

cePlayerChoiceBox::cePlayerChoiceBox( ceConversation &conversation ) :
pConversation( conversation ),

pEngFont( NULL ),

pBackgroundColor( 0.0f, 0.0f, 0.0f, 0.5f ),
pTextColor( 1.0f, 1.0f, 1.0f, 1.0f ),
pSelectedBackgroundColor( 0.0f, 0.0f, 0.5f, 0.5f ),
pSelectedTextColor( 1.0f, 0.5f, 0.5f, 1.0f ),
pTextSize( 18 ),
pPadding( 10 ),
pPlaybackStackDepth( -1 ),

pSelectedOption( -1 ),

pCanvasView( NULL )
{
	try{
		pCanvasView = conversation.GetEngine()->GetCanvasManager()->CreateCanvasView();
		pCanvasView->SetOrder( 11.0f );
		
		SetPathFont( "/igde/fonts/sans_9_border.defont" );
		
	}catch( const deException & ){
		pCleanUp();
		throw;
	}
}

cePlayerChoiceBox::~cePlayerChoiceBox(){
	pCleanUp();
}



// Management
///////////////

void cePlayerChoiceBox::SetPathFont( const char *path ){
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

void cePlayerChoiceBox::SetBackgroundColor( const decColor &color ){
	pBackgroundColor = color;
}

void cePlayerChoiceBox::SetTextColor( const decColor &color ){
	if( color.IsEqualTo( pTextColor ) ){
		return;
	}
	
	pTextColor = color;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetTextSize( int size ){
	if( size < 1 ){
		size = 1;
	}
	
	if( size == pTextSize ){
		return;
	}
	
	pTextSize = size;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetSelectedBackgroundColor( const decColor &color ){
	if( color.IsEqualTo( pSelectedBackgroundColor ) ){
		return;
	}
	
	pSelectedBackgroundColor = color;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetSelectedTextColor( const decColor &color ){
	if( color.IsEqualTo( pSelectedTextColor ) ){
		return;
	}
	
	pSelectedTextColor = color;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetPadding( int padding ){
	if( padding < 0 ){
		padding = 0;
	}
	
	if( padding == pPadding ){
		return;
	}
	
	pPadding = padding;
	UpdateCanvas();
}

void cePlayerChoiceBox::SetPlaybackStackDepth( int depth ){
	pPlaybackStackDepth = depth;
}

void cePlayerChoiceBox::SetSelectedOption( int index ){
	if( index < -1 || index >= pOptions.GetCount() ){
		DETHROW( deeInvalidParam );
	}
	
	if( index == pSelectedOption ){
		return;
	}
	
	pSelectedOption = index;
	UpdateCanvas();
}

void cePlayerChoiceBox::Clear(){
	pOptions.RemoveAll();
	pSelectedOption = -1;
	UpdateCanvas();
}

int cePlayerChoiceBox::IndexOfOptionAt( int x, int y ) const{
	deCanvasView * const parentView = pCanvasView->GetParentView();
	if( ! parentView ){
		return -1;
	}
	
	const decPoint &boxPosition = pCanvasView->GetPosition();
	const int count = pOptions.GetCount();
	const decPoint point( x, y );
	int i;
	
	for( i=0; i<count; i++ ){
		const deCanvasView * const canvasOption = pOptions.GetAt( i )->GetCanvasView();
		if( ! canvasOption ){
			continue;
		}
		
		const decPoint position( boxPosition + canvasOption->GetPosition() );
		const decPoint &size = canvasOption->GetSize();
		
		if( point >= position && point < position + size ){
			return i;
		}
	}
	
	return -1;
}

void cePlayerChoiceBox::SelectOptionAt( int x, int y ){
	const int index = IndexOfOptionAt( x, y );
	
	if( index != -1 ){
		SetSelectedOption( index );
	}
}

void cePlayerChoiceBox::UpdateCanvas(){
	// if there is no parent canvas there is no use in updating anything
	deCanvasView * const parentView = pCanvasView->GetParentView();
	if( ! parentView ){
		return;
	}
	
	// remove all text canvas
	pCanvasView->RemoveAllCanvas();
	
	// make sure the selected option is in valid range
	const int count = pOptions.GetCount();
	
	if( count > 0 ){
		pSelectedOption = decMath::clamp( pSelectedOption, 0, count - 1 );
		
	}else{
		pSelectedOption = -1;
	}
	
	// calculate required box size
	int boxHeight = pPadding * 2;
	int i;
	
	for( i=0; i<count; i++ ){
		cePCBOption &option = *pOptions.GetAt( i );
		
		option.Layout( *this, i == pSelectedOption );
		boxHeight += option.GetCanvasView()->GetSize().y;
	}
	
	boxHeight = decMath::max( boxHeight, pPadding * 2 );
	
	// resize canvas
	const decPoint &parentSize = parentView->GetSize();
	
	pCanvasView->SetPosition( decPoint( 0, ( parentSize.y - boxHeight ) / 2 ) );
	pCanvasView->SetSize( decPoint( parentSize.x, boxHeight ) );
	
	// add option canvas
	int y = pPadding;
	
	for( i=0; i<count; i++ ){
		deCanvasView * const textCanvas = pOptions.GetAt( i )->GetCanvasView();
		const int height = textCanvas->GetSize().y;
		
		textCanvas->SetPosition( decPoint( 0, y ) );
		textCanvas->SetOrder( ( float )( 1 + i ) );
		pCanvasView->AddCanvas( textCanvas );
		
		y += height;
	}
}



// Private Functions
//////////////////////

void cePlayerChoiceBox::pCleanUp(){
	pOptions.RemoveAll();
	
	if( pCanvasView ){
		pCanvasView->FreeReference();
	}
	
	if( pEngFont ){
		pEngFont->FreeReference();
	}
}



void cePlayerChoiceBox::pUpdateFont(){
	deFont *font = NULL;
	
	try{
		if( ! pPathFont.IsEmpty() ){
			font = pConversation.GetEngine()->GetFontManager()->LoadFont( pPathFont.GetString(), "/" );
		}
		
		if( pEngFont ){
			pEngFont->FreeReference();
		}
		pEngFont = font;
		
	}catch( const deException &e ){
		if( font ){
			font->FreeReference();
		}
		pConversation.GetLogger()->LogException( LOGSOURCE, e );
	}
}
