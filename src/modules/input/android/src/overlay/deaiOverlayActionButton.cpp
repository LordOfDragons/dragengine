/*
 * MIT License
 *
 * Copyright (C) 2024, DragonDreams GmbH (info@dragondreams.ch)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <stdlib.h>

#include "deaiOverlayActionButton.h"
#include "../deAndroidInput.h"
#include "../deaiDeviceButton.h"
#include "../deaiDeviceKeyboard.h"
#include "../deaiDeviceMouse.h"
#include "../deaiDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/common/string/unicode/decUTF8Decoder.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontGlyph.h>
#include <dragengine/resources/image/deImage.h>
#include <dragengine/resources/image/deImageManager.h>



// Class deaiOverlayActionButton
//////////////////////////////////

// Constructor, destructor
////////////////////////////

deaiOverlayActionButton::deaiOverlayActionButton( deAndroidInput &androidInput ) :
deaiOverlay( androidInput ),

pRadius( 64 ),
pCenter( 64, 64 ),

pPressed( false ),

pCImage( NULL ),
pCLabel( NULL ),

pDirtyLabelSize( true )
{
	deCanvasManager &canvasManager = *androidInput.GetGameEngine()->GetCanvasManager();
	deImageManager &imageManager = *androidInput.GetGameEngine()->GetImageManager();
	deImage *image = NULL;
	
	const decPoint size( pRadius * 2, pRadius * 2 );
	
	SetLayoutHorizontal( deaiLayout( decPoint(), size ) );
	SetLayoutVertical( deaiLayout( decPoint(), size ) );
	
	try{
		GetCanvas()->SetSize( size );
		
		pCImage = canvasManager.CreateCanvasImage();
		pCImage->SetSize( size );
		image = imageManager.LoadImage( &androidInput.GetVFS(),
			"/share/images/actionbutton.png", "/" );
		pCImage->SetImage( image );
		image->FreeReference();
		image = NULL;
		GetCanvas()->AddCanvas( pCImage );
		
		pCLabel = canvasManager.CreateCanvasText();
		pCLabel->SetSize( size );
		pCLabel->SetColor( decColor( 1.0f, 1.0f, 1.0f ) );
		pCLabel->SetFont( androidInput.GetDefaultFont() );
		GetCanvas()->AddCanvas( pCLabel );
		
	}catch( const deException & ){
		if( image ){
			image->FreeReference();
		}
		pCleanUp();
		throw;
	}
}

deaiOverlayActionButton::~deaiOverlayActionButton(){
	pCleanUp();
}



// Management
///////////////

void deaiOverlayActionButton::SetBinding( const deaiInputBinding &binding ){
	pBinding = binding;
}



void deaiOverlayActionButton::SetRadius( int radius ){
	radius = decMath::max( radius, 0 );
	if( radius == pRadius ){
		return;
	}
	
	pRadius = radius;
	
	//GetCanvas()->SetSize( decPoint( pRadius * 2, pRadius * 2 ) );
	//pCImage->SetSize( decPoint( pRadius * 2, pRadius * 2 ) );
	
	// TODO
}

void deaiOverlayActionButton::SetCenter( const decPoint &center ){
	if( center == pCenter ){
		return;
	}
	
	pCenter = center;
	
	// TODO
}



const decString &deaiOverlayActionButton::GetText() const{
	return pCLabel->GetText();
}

void deaiOverlayActionButton::SetText( const char *text ){
	pCLabel->SetText( text );
	pDirtyLabelSize = true;
}

const decColor &deaiOverlayActionButton::GetColor() const{
	return pCLabel->GetColor();
}

void deaiOverlayActionButton::SetColor( const decColor &color ){
	pCLabel->SetColor( color );
}

deFont *deaiOverlayActionButton::GetFont() const{
	return pCLabel->GetFont();
}

void deaiOverlayActionButton::SetFont( deFont *font ){
	pCLabel->SetFont( font );
	pDirtyLabelSize = true;
}



void deaiOverlayActionButton::SetPressed( bool pressed ){
	if( pressed == pPressed ){
		return;
	}
	
	pPressed = pressed;
	
	pBinding.ChangeButtonState( GetAndroidInput(), pressed );
}



void deaiOverlayActionButton::UpdateBindingIndices(){
	pBinding.UpdateIndices( GetAndroidInput().GetDevices() );
}

void deaiOverlayActionButton::UpdateContent(){
	pUpdateLabelSize();
}

bool deaiOverlayActionButton::OnTouch( int pointerId, const decPoint &position ){
	if( ( int )( ( position - pCenter ).Length() ) > pRadius ){
		return false;
	}
	
	SetPointer( pointerId, position );
	SetPressed( true );
	return true;
}

void deaiOverlayActionButton::OnMove( const decPoint &position ){
	PointerMove( position );
	
	// no. keep the button pressed while moving. otherwise it is annoying if the button
	// releases and presses again because the finger moved a bit
	/*
	if( pPressed ){
		SetPressed( ( int )( ( position - pCenter ).Length() ) <= pRadius );
	}
	*/
}

void deaiOverlayActionButton::OnRelease(){
	SetPressed( false );
	deaiOverlay::OnRelease();
}



// Private Functions
//////////////////////

void deaiOverlayActionButton::pCleanUp(){
	if( pCLabel ){
		pCLabel->FreeReference();
	}
	if( pCImage ){
		pCImage->FreeReference();
	}
}

void deaiOverlayActionButton::pUpdateLabelSize(){
	if( ! pDirtyLabelSize ){
		return;
	}
	
	pDirtyLabelSize = false;
	
	deFont * const font = pCLabel->GetFont();
	if( ! font ){
		return;
	}
	
	decUTF8Decoder decoder;
	int width = 0;
	
	decoder.SetString( pCLabel->GetText() );
	
	while( true ){
		const int character = decoder.DecodeNextCharacter();
		if( character == -1 ){
			break; // end of string
		}
		
		width += font->GetGlyph( character ).GetAdvance();
	}
	
	const int lineHeight = font->GetLineHeight();
	const float textRadius = decPoint( width, lineHeight ).Length();
	if( textRadius < 1.0f ){
		return;
	}
	
	deAndroidInput &module = GetAndroidInput();
	const float scaleFactor = ( float )( pRadius * 2 - 10 ) / textRadius;
	pCLabel->SetFontSize( ( float )lineHeight * scaleFactor );
	pCLabel->SetSize( decPoint( ( float )width * scaleFactor, pCLabel->GetFontSize() ) );
	pCLabel->SetPosition( pCenter - pCLabel->GetSize() / 2 );
	module.LogInfoFormat( "text size(%i,%i)", pCLabel->GetSize().x, pCLabel->GetSize().y );
}
