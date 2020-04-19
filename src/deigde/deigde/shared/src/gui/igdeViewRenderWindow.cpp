/* 
 * Drag[en]gine IGDE
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

#include "igdeContainer.h"
#include "igdeViewRenderWindow.h"
#include "event/igdeMouseKeyListener.h"
#include "native/fox/igdeNativeFoxRenderView.h"
#include "../engine/igdeEngineController.h"
#include "../environment/igdeEnvironment.h"

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>



// Class igdeViewRenderWindow
///////////////////////////////

// Constructor, destructor
////////////////////////////

igdeViewRenderWindow::igdeViewRenderWindow( igdeEnvironment &environment ) :
igdeWidget( environment ),
pEnableRendering( false ),
pEngineRunning( false ){
}

igdeViewRenderWindow::~igdeViewRenderWindow(){
}



// Management
///////////////

void igdeViewRenderWindow::SetEnableRendering( bool enable ){
	pEnableRendering = enable;
	
	if( pRenderWindow ){
		igdeNativeFoxRenderView * const native = ( igdeNativeFoxRenderView* )GetNativeWidget();
		pRenderWindow->SetPaint( enable && native && native->shown() );
	}
}

bool igdeViewRenderWindow::GetCanRender() const{
	const igdeNativeFoxRenderView * const native = ( const igdeNativeFoxRenderView* )GetNativeWidget();
	return native && native->GetCanRender();
}

decPoint igdeViewRenderWindow::GetRenderAreaSize() const{
	const igdeNativeFoxRenderView * const native = ( igdeNativeFoxRenderView* )GetNativeWidget();
	return native ? decPoint( native->getWidth(), native->getHeight() ) : decPoint();
}



void igdeViewRenderWindow::ClearErrorRenderWindow(){
	// TODO
}



void igdeViewRenderWindow::OnAfterEngineStart(){
	igdeNativeFoxRenderView * const native = ( igdeNativeFoxRenderView* )GetNativeWidget();
	if( native ){
		native->AttachRenderWindow();
	}
	pEngineRunning = true;
}

void igdeViewRenderWindow::OnBeforeEngineStop(){
	igdeNativeFoxRenderView * const native = ( igdeNativeFoxRenderView* )GetNativeWidget();
	if( native ){
		native->DetachRenderWindow();
	}
	pEngineRunning = false;
}

void igdeViewRenderWindow::OnFrameUpdate( float ){
	if( ! pRenderWindow ){
		return;
	}
	
	// window updates are rendered to the window by the graphic module. FOX does not notice
	// this update so we have to tell it outself. for this we need to check if the window is
	// visible on screen. this check is required since there is no way to figure out otherwise
	// if the window is actually visible on screen or hidden (either itself or a parent
	// somewhere up the chain). disabling rendering here is important since otherwise strange
	// UI artifacts can happen
	igdeNativeFoxRenderView * const native = ( igdeNativeFoxRenderView* )GetNativeWidget();
	pRenderWindow->SetPaint( pEnableRendering && native && native->IsReallyVisible() );
	
	// update only if painting is enabled
	if( pRenderWindow->GetPaint() ){
		native->update();
	}
}



void igdeViewRenderWindow::SetRenderWorld( deCamera *camera ){
	deCanvasRenderWorld * const canvasRenderWorld = GetCanvasRenderWorld();
	if( canvasRenderWorld ){
		canvasRenderWorld->SetCamera( camera );
	}
}

void igdeViewRenderWindow::AddCanvas( deCanvas *canvas ){
	if( ! canvas || ! pRenderWindow ){
		DETHROW( deeInvalidParam );
	}
	
	deCanvasView * canvasView = pRenderWindow->GetCanvasView();
	if( canvasView ){
		canvasView->AddCanvas( canvas );
	}
}

void igdeViewRenderWindow::RemoveCanvas( deCanvas *canvas ){
	if( ! canvas || ! pRenderWindow ){
		DETHROW( deeInvalidParam );
	}
	
	deCanvasView * canvasView = pRenderWindow->GetCanvasView();
	if( canvasView ){
		canvasView->RemoveCanvas( canvas );
	}
}

deCanvasView *igdeViewRenderWindow::GetRenderWindowCanvas() const{
	return pRenderWindow ? pRenderWindow->GetCanvasView() : NULL;
}

deCanvasRenderWorld *igdeViewRenderWindow::GetCanvasRenderWorld() const{
	return ( deCanvasRenderWorld* )( deCanvas* )pCanvasRenderWorld;
}

deCanvasPaint *igdeViewRenderWindow::GetCanvasBackground() const{
	return ( deCanvasPaint* )( deCanvas* )pCanvasBackground;
}

void igdeViewRenderWindow::CreateCanvas(){
	if( ! pCanvasBackground ){
		pCanvasBackground.TakeOver( GetEngineController().GetEngine()->
			GetCanvasManager()->CreateCanvasPaint() );
		deCanvasPaint &background = ( deCanvasPaint& )( deCanvas& )pCanvasBackground;
		background.SetOrder( 0.0f );
		background.SetShapeType( deCanvasPaint::estRectangle );
		background.SetThickness( 0.0f );
		background.SetFillColor( GetEnvironment().GetSystemColor( igdeEnvironment::escWidgetShadow ) );
		AddCanvas( pCanvasBackground );
	}
	if( ! pCanvasRenderWorld ){
		pCanvasRenderWorld.TakeOver( GetEngineController().GetEngine()->
			GetCanvasManager()->CreateCanvasRenderWorld() );
		pCanvasRenderWorld->SetOrder( 1.0f );
		AddCanvas( pCanvasRenderWorld );
	}
}

void igdeViewRenderWindow::GrabInput(){
	igdeNativeFoxRenderView * const native = ( igdeNativeFoxRenderView* )GetNativeWidget();
	if( native ){
		native->GrabInput();
	}
}

void igdeViewRenderWindow::ReleaseInput(){
	igdeNativeFoxRenderView * const native = ( igdeNativeFoxRenderView* )GetNativeWidget();
	if( native ){
		native->ReleaseInput();
	}
}



void igdeViewRenderWindow::AddListener( igdeMouseKeyListener *listener ){
	if( ! listener ){
		DETHROW( deeInvalidParam );
	}
	pListeners.Add( listener );
}

void igdeViewRenderWindow::RemoveListener( igdeMouseKeyListener *listener ){
	pListeners.Remove( listener );
}

void igdeViewRenderWindow::NotifyKeyPress( deInputEvent::eKeyCodes keyCode, int key ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeMouseKeyListener* )listeners.GetAt( i ) )->OnKeyPress( this, keyCode, key );
	}
}

void igdeViewRenderWindow::NotifyKeyRelease( deInputEvent::eKeyCodes keyCode, int key ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeMouseKeyListener* )listeners.GetAt( i ) )->OnKeyRelease( this, keyCode, key );
	}
}

void igdeViewRenderWindow::NotifyButtonPress( int button, const decPoint &position, int modifiers ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeMouseKeyListener* )listeners.GetAt( i ) )->OnButtonPress( this,
			button, position, modifiers );
	}
}

void igdeViewRenderWindow::NotifyButtonRelease( int button, const decPoint &position, int modifiers ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeMouseKeyListener* )listeners.GetAt( i ) )->OnButtonRelease( this,
			button, position, modifiers );
	}
}

void igdeViewRenderWindow::NotifyDoubleClicked( int button, const decPoint &position, int modifiers ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeMouseKeyListener* )listeners.GetAt( i ) )->OnDoubleClicked( this,
			button, position, modifiers );
	}
}

void igdeViewRenderWindow::NotifyMouseMoved( const decPoint &position, int modifiers ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeMouseKeyListener* )listeners.GetAt( i ) )->OnMouseMoved( this, position, modifiers );
	}
}

void igdeViewRenderWindow::NotifyMouseWheeled( const decPoint &position, const decPoint &change, int modifiers ){
	const decObjectOrderedSet listeners( pListeners );
	const int count = listeners.GetCount();
	int i;
	
	for( i=0; i<count; i++ ){
		( ( igdeMouseKeyListener* )listeners.GetAt( i ) )->OnMouseWheeled( this, position, change, modifiers );
	}
}



void igdeViewRenderWindow::CreateNativeWidget(){
	if( GetNativeWidget() ){
		return;
	}
	
	if( ! GetParent() ){
		DETHROW( deeInvalidParam );
	}
	
	FXComposite * const nativeParent = ( FXComposite* )GetParent()->GetNativeContainer();
	if( ! nativeParent ){
		DETHROW( deeInvalidParam );
	}
	
	igdeNativeFoxRenderView * const native = new igdeNativeFoxRenderView(
		*this, nativeParent, igdeUIFoxHelper::GetChildLayoutFlags( this ) );
	SetNativeWidget( native );
	
	if( ! nativeParent->id() ){
		DETHROW( deeInvalidParam );
	}
	
	native->create();
}

void igdeViewRenderWindow::DropNativeWidget(){
	if( GetNativeWidget() ){
		( ( igdeNativeFoxRenderView* )GetNativeWidget() )->DropNativeWindow();
	}
	
	igdeWidget::DropNativeWidget();
}

void igdeViewRenderWindow::DestroyNativeWidget(){
	if( ! GetNativeWidget() ){
		return;
	}
	
	delete ( igdeNativeFoxRenderView* )GetNativeWidget();
	DropNativeWidget();
}

void igdeViewRenderWindow::CreateAndAttachRenderWindow(){
	if( ! GetNativeWidget() ){
		DETHROW( deeNullPointer );
	}
	
	if( ! pRenderWindow ){
		pRenderWindow.TakeOver( GetEngineController().CreateRenderWindow( *this ) );
		pRenderWindow->SetPaint( pEnableRendering );
		CreateCanvas();
	}
	
	if( pEngineRunning ){
		( ( igdeNativeFoxRenderView* )GetNativeWidget() )->AttachRenderWindow();
	}
}
