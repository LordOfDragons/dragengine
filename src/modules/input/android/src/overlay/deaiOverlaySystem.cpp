/* 
 * Drag[en]gine Android Input Module
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

#include <stdlib.h>

#include "deaiInputBinding.h"
#include "deaiOverlaySystem.h"
#include "deaiOverlayCirclePad.h"
#include "deaiOverlayActionButton.h"
#include "../deAndroidInput.h"
#include "../deaiDeviceKeyboard.h"
#include "../deaiDeviceAxis.h"
#include "../deaiDeviceButton.h"
#include "../deaiDeviceMouse.h"
#include "../deaiDeviceManager.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/input/deInputEvent.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>



// Class deaiOverlaySystem
////////////////////////////

// Constructor, destructor
////////////////////////////

deaiOverlaySystem::deaiOverlaySystem( deAndroidInput &androidInput ) :
pAndroidInput( androidInput ),

pCanvas( NULL ),
pInHorizontalMode( false )
{
	deaiOverlayActionButton *actionButton = NULL;
	deaiOverlayCirclePad *circlePad = NULL;
	
	decPoint screenSize( androidInput.GetScreenSize() );
	pInHorizontalMode = screenSize.x > screenSize.y;
	if( pInHorizontalMode ){
		const int temporary = screenSize.x;
		screenSize.x = screenSize.y;
		screenSize.y = temporary;
	}
	
	try{
		pCanvas = androidInput.GetGameEngine()->GetCanvasManager()->CreateCanvasView();
		pCanvas->SetSize( screenSize );
		
		const deaiDeviceMouse &mouse = *androidInput.GetDevices().GetMouse();
		const deaiDeviceKeyboard &keyboard = *androidInput.GetDevices().GetKeyboard();
		
		circlePad = new deaiOverlayCirclePad( androidInput );
		circlePad->SetKeyCodeLeft( deaiInputBinding( keyboard.GetID(),
			keyboard.GetButtonAt( keyboard.IndexOfButtonWithKeyCode(
				deInputEvent::ekcY ) ).GetID() ) );
		circlePad->SetKeyCodeUp( deaiInputBinding( keyboard.GetID(),
			keyboard.GetButtonAt( keyboard.IndexOfButtonWithKeyCode(
				deInputEvent::ekcD ) ).GetID() ) );
		circlePad->SetKeyCodeRight( deaiInputBinding( keyboard.GetID(),
			keyboard.GetButtonAt( keyboard.IndexOfButtonWithKeyCode(
				deInputEvent::ekcV ) ).GetID() ) );
		circlePad->SetKeyCodeDown( deaiInputBinding( keyboard.GetID(),
			keyboard.GetButtonAt( keyboard.IndexOfButtonWithKeyCode(
				deInputEvent::ekcC ) ).GetID() ) );
		circlePad->SetLayoutHorizontal( deaiLayout(
			decPoint( 20, screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().y ),
			circlePad->GetLayoutHorizontal().GetSize() ) );
		circlePad->SetLayoutVertical( deaiLayout(
			decPoint( 20, screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().y ),
			circlePad->GetLayoutHorizontal().GetSize() ) );
		pCanvas->AddCanvas( circlePad->GetCanvas() );
		pOverlays.Add( circlePad );
		circlePad->FreeReference();
		circlePad = NULL;
		
		circlePad = new deaiOverlayCirclePad( androidInput );
		circlePad->SetAxisLeftRight( deaiInputBinding( mouse.GetID(),
			mouse.GetAxisAt( 0 ).GetID(), false ) ); // mouse left-right
		circlePad->SetAxisUpDown( deaiInputBinding( mouse.GetID(),
			mouse.GetAxisAt( 1 ).GetID(), true ) ); // mouse up-down
		circlePad->SetLayoutHorizontal( deaiLayout(
			decPoint( screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().x,
				screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().y ),
			circlePad->GetLayoutHorizontal().GetSize() ) );
		circlePad->SetLayoutVertical( deaiLayout(
			decPoint( screenSize.x - 20 - circlePad->GetLayoutHorizontal().GetSize().x,
				screenSize.y - 20 - circlePad->GetLayoutHorizontal().GetSize().y ),
			circlePad->GetLayoutHorizontal().GetSize() ) );
		pCanvas->AddCanvas( circlePad->GetCanvas() );
		pOverlays.Add( circlePad );
		circlePad->FreeReference();
		circlePad = NULL;
		
		actionButton = new deaiOverlayActionButton( androidInput );
		actionButton->SetBinding( deaiInputBinding( mouse.GetID(),
			mouse.GetButtonAt( 0 ).GetID() ) ); // left mouse button
		actionButton->SetText( "LMB" );
		actionButton->SetColor( decColor( 1.0f, 0.0f, 0.0f ) );
		actionButton->SetLayoutHorizontal( deaiLayout(
			decPoint( screenSize.y - 416, screenSize.x - 512 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		actionButton->SetLayoutVertical( deaiLayout(
			decPoint( screenSize.x - 416, screenSize.y - 512 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		pCanvas->AddCanvas( actionButton->GetCanvas() );
		pOverlays.Add( actionButton );
		actionButton->FreeReference();
		actionButton = NULL;
		
		actionButton = new deaiOverlayActionButton( androidInput );
		actionButton->SetBinding( deaiInputBinding( mouse.GetID(),
			mouse.GetButtonAt( 1 ).GetID() ) ); // right mouse button
		actionButton->SetText( "RMB" );
		actionButton->SetColor( decColor( 1.0f, 0.0f, 0.0f ) );
		actionButton->SetLayoutHorizontal( deaiLayout(
			decPoint( screenSize.y - 142, screenSize.x - 512 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		actionButton->SetLayoutVertical( deaiLayout(
			decPoint( screenSize.x - 142, screenSize.y - 512 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		pCanvas->AddCanvas( actionButton->GetCanvas() );
		pOverlays.Add( actionButton );
		actionButton->FreeReference();
		actionButton = NULL;
		
		actionButton = new deaiOverlayActionButton( androidInput );
		actionButton->SetBinding( deaiInputBinding( mouse.GetID(),
			mouse.GetButtonAt( 2 ).GetID() ) ); // middle mouse button
		actionButton->SetText( "MMB" );
		actionButton->SetColor( decColor( 1.0f, 0.0f, 0.0f ) );
		actionButton->SetLayoutHorizontal( deaiLayout(
			decPoint( screenSize.y - 278, screenSize.x - 543 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		actionButton->SetLayoutVertical( deaiLayout(
			decPoint( screenSize.x - 278, screenSize.y - 543 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		pCanvas->AddCanvas( actionButton->GetCanvas() );
		pOverlays.Add( actionButton );
		actionButton->FreeReference();
		actionButton = NULL;
		
		actionButton = new deaiOverlayActionButton( androidInput );
		actionButton->SetBinding( deaiInputBinding( keyboard.GetID(),
			keyboard.GetButtonAt( keyboard.IndexOfButtonWithKeyCode(
				deInputEvent::ekcQ ) ).GetID() ) );
		actionButton->SetText( "Quit" );
		actionButton->SetColor( decColor( 0.0f, 0.5f, 1.0f ) );
		actionButton->SetLayoutHorizontal( deaiLayout(
			decPoint( screenSize.y - 517, screenSize.x - 390 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		actionButton->SetLayoutVertical( deaiLayout(
			decPoint( screenSize.x - 517, screenSize.y - 390 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		pCanvas->AddCanvas( actionButton->GetCanvas() );
		pOverlays.Add( actionButton );
		actionButton->FreeReference();
		actionButton = NULL;
		
		actionButton = new deaiOverlayActionButton( androidInput );
		actionButton->SetBinding( deaiInputBinding( keyboard.GetID(),
			keyboard.GetButtonAt( keyboard.IndexOfButtonWithKeyCode(
				deInputEvent::ekcR ) ).GetID() ) );
		actionButton->SetText( "Debug" );
		actionButton->SetColor( decColor( 0.0f, 0.5f, 1.0f ) );
		actionButton->SetLayoutHorizontal( deaiLayout(
			decPoint( screenSize.y - 544, screenSize.x - 239 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		actionButton->SetLayoutVertical( deaiLayout(
			decPoint( screenSize.x - 544, screenSize.y - 239 ),
			actionButton->GetLayoutHorizontal().GetSize() ) );
		pCanvas->AddCanvas( actionButton->GetCanvas() );
		pOverlays.Add( actionButton );
		actionButton->FreeReference();
		actionButton = NULL;
		
		UpdateBindingIndices();
		UpdateFromLayout();
		
	}catch( const deException & ){
		if( actionButton ){
			actionButton->FreeReference();
		}
		if( circlePad ){
			circlePad->FreeReference();
		}
		pCleanUp();
		throw;
	}
}

deaiOverlaySystem::~deaiOverlaySystem(){
	pCleanUp();
}



// Management
///////////////

void deaiOverlaySystem::UpdateContent(){
	const decPoint screenSize( pAndroidInput.GetScreenSize() );
	SetInHorizontalMode( screenSize.x > screenSize.y );
	
	const int count = pOverlays.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		pOverlays.GetAt( i )->UpdateContent();
	}
}

void deaiOverlaySystem::ScreenSizeChanged(){
}

void deaiOverlaySystem::SetInHorizontalMode( bool inHorizontalMode ){
	if( inHorizontalMode == pInHorizontalMode ){
		return;
	}
	
	pInHorizontalMode = inHorizontalMode;
	UpdateFromLayout();
}

void deaiOverlaySystem::UpdateBindingIndices(){
	const int count = pOverlays.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		pOverlays.GetAt( i )->UpdateBindingIndices();
	}
}

void deaiOverlaySystem::UpdateFromLayout(){
	pCanvas->SetSize( pAndroidInput.GetScreenSize() );
	
	const int count = pOverlays.GetCount();
	int i;
	
	if( pInHorizontalMode ){
		for( i=0; i<count; i++ ){
			pOverlays.GetAt( i )->UpdateFromHorizontalLayout();
		}
		
	}else{
		for( i=0; i<count; i++ ){
			pOverlays.GetAt( i )->UpdateFromVerticalLayout();
		}
	}
}



bool deaiOverlaySystem::OnTouch( int pointerId, const decPoint &position ){
	const int count = pOverlays.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		deaiOverlay &overlay = *pOverlays.GetAt( i );
		if( overlay.OnTouch( pointerId, position - overlay.GetCanvas()->GetPosition() ) ){
			return true;
		}
	}
	
	return false;
}

bool deaiOverlaySystem::OnMove( int pointerId, const decPoint &position ){
	const int count = pOverlays.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deaiOverlay &overlay = *pOverlays.GetAt( i );
		if( overlay.GetPointer() == pointerId ){
			overlay.OnMove( position - overlay.GetCanvas()->GetPosition() );
			return true;
		}
	}
	
	return false;
}

bool deaiOverlaySystem::OnRelease( int pointerId ){
	const int count = pOverlays.GetCount();
	int i;
	for( i=0; i<count; i++ ){
		deaiOverlay &overlay = *pOverlays.GetAt( i );
		if( overlay.GetPointer() == pointerId ){
			overlay.OnRelease();
			return true;
		}
	}
	
	return false;
}



// Private Functions
//////////////////////

void deaiOverlaySystem::pCleanUp(){
	pOverlays.RemoveAll();
	
	if( pCanvas ){
		pCanvas->RemoveAllCanvas();
		pCanvas->FreeReference();
	}
}
