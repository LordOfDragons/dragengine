/* 
 * Drag[en]gine IGDE World Editor
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

#include "meView3D.h"
#include "meWindowMain.h"
#include "meView3DListener.h"
#include "vieweditor/meViewEditorSelect.h"
#include "vieweditor/meViewEditorMove.h"
#include "vieweditor/meViewEditorRotate.h"
#include "vieweditor/meViewEditorScale.h"
#include "vieweditor/meViewEditorAddNew.h"
#include "vieweditor/meViewEditorHeightPaint.h"
#include "vieweditor/meViewEditorMaskPaint.h"
#include "vieweditor/meViewEditorVisibilityPaint.h"
#include "vieweditor/meViewEditorNavSpaceEdit.h"
#include "../configuration/meConfiguration.h"
#include "../world/meCamera.h"
#include "../world/meWorld.h"
#include "../world/meWorldGuiParameters.h"

#include <deigde/engine/igdeEngineController.h>
#include <deigde/gui/event/igdeMouseKeyListener.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/logger/deLogger.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/font/deFontManager.h>
#include <dragengine/resources/rendering/deRenderWindow.h>
#include <dragengine/resources/camera/deCamera.h>
#include <dragengine/resources/world/deWorld.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasPaintReference.h>



// Events
///////////

namespace {

class cEditorInteraction : public igdeMouseKeyListener {
	meView3D &pView;
	
public:
	cEditorInteraction( meView3D &view ) : pView( view ){ }
	
public:
	void OnButtonPress( igdeWidget*, int button, const decPoint &position, int modifiers ) override{
		if( ! pView.GetEditor() ){
			return;
		}
		
		switch( button ){
		case deInputEvent::embcLeft:
			pView.GetEditor()->OnLeftMouseButtonPress( position.x, position.y,
				( modifiers & deInputEvent::esmShift ) == deInputEvent::esmShift,
				( modifiers & deInputEvent::esmControl ) == deInputEvent::esmControl );
			break;
			
		case deInputEvent::embcRight:
			pView.GetEditor()->OnRightMouseButtonPress( position.x, position.y,
				( modifiers & deInputEvent::esmShift ) == deInputEvent::esmShift,
				( modifiers & deInputEvent::esmControl ) == deInputEvent::esmControl );
			break;
			
		default:
			break;
		}
	}
	
	void OnButtonRelease( igdeWidget*, int button, const decPoint &position, int modifiers ) override{
		if( ! pView.GetEditor() ){
			return;
		}
		
		switch( button ){
		case deInputEvent::embcLeft:
			pView.GetEditor()->OnLeftMouseButtonRelease( position.x, position.y,
				( modifiers & deInputEvent::esmShift ) == deInputEvent::esmShift,
				( modifiers & deInputEvent::esmControl ) == deInputEvent::esmControl );
			break;
			
		case deInputEvent::embcRight:
			pView.GetEditor()->OnRightMouseButtonRelease( position.x, position.y,
				( modifiers & deInputEvent::esmShift ) == deInputEvent::esmShift,
				( modifiers & deInputEvent::esmControl ) == deInputEvent::esmControl );
			break;
			
		default:
			break;
		}
	}
	
	void OnMouseMoved(igdeWidget*, const decPoint &position, int modifiers ) override{
		if( ! pView.GetEditor() ){
			return;
		}
		
		pView.GetEditor()->OnMouseMove( position.x, position.y,
			( modifiers & deInputEvent::esmShift ) == deInputEvent::esmShift,
			( modifiers & deInputEvent::esmControl ) == deInputEvent::esmControl );
	}
	
	void OnMouseWheeled( igdeWidget*, const decPoint &, const decPoint &change, int modifiers ) override{
		if( ! pView.GetEditor() ){
			return;
		}
		
		pView.GetEditor()->OnMouseWheel( change.y,
			( modifiers & deInputEvent::esmShift ) == deInputEvent::esmShift,
			( modifiers & deInputEvent::esmControl ) == deInputEvent::esmControl );
	}
	
	void OnKeyPress( igdeWidget*, deInputEvent::eKeyCodes keyCode, int ) override{
		if( ! pView.GetEditor() ){
			return;
		}
		
		pView.GetEditor()->OnKeyPress( keyCode, pView.GetEditor()->GetShiftStart(),
			pView.GetEditor()->GetControlStart() );
	}
	
	void OnKeyRelease( igdeWidget*, deInputEvent::eKeyCodes keyCode, int ) override{
		if( ! pView.GetEditor() ){
			return;
		}
		
		pView.GetEditor()->OnKeyRelease( keyCode, pView.GetEditor()->GetShiftStart(),
			pView.GetEditor()->GetControlStart() );
	}
	
	void OnMouseEnter( igdeWidget* ) override{
		if( pView.GetEditor() ){
			pView.GetEditor()->OnMouseEnter();
		}
	}
	
	void OnMouseLeave ( igdeWidget* ) override{
		if( pView.GetEditor() ){
			pView.GetEditor()->OnMousLeave();
		}
	}
};

}



// Class meView3D
///////////////////

// Constructor, destructor
////////////////////////////

meView3D::meView3D( meWindowMain &windowMain ) :
igdeViewRenderWindow( windowMain.GetEnvironment() ),
pWindowMain( windowMain ),
pListener( NULL ),
pWorld( NULL ),
pEditor( NULL )
{
	int i;
	for( i=0; i<30; i++ ){
		pFPSHistory[ i ] = 0;
	}
	pFPSRedrawCanvasDelay = 1.0f;
	
	pFontStats.TakeOver( GetEngine()->GetFontManager()->LoadFont( "/igde/fonts/sans_10.defont", "/" ) );
	
	pListener = new meView3DListener( *this );
	
	pListenerEditor.TakeOver( new cEditorInteraction( *this ) );
	AddListener( pListenerEditor );
}

meView3D::~meView3D(){
	SetWorld( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
}



// Management
///////////////

void meView3D::ResetView(){
	if( pWorld ){
		pWorld->GetFreeRoamingCamera()->Reset();
	}
}

void meView3D::SetWorld( meWorld *world ){
	if( world == pWorld ){
		return;
	}
	
	if( pEditor ){
		delete pEditor;
		pEditor = NULL;
	}
	
	SetRenderWorld( NULL );
	
	if( pWorld ){
		pWorld->RemoveNotifier( pListener );
		pWorld->FreeReference();
	}
	
	pWorld = world;
	
	if( world ){
		world->AddReference();
		world->AddNotifier( pListener );
		ModeChanged();
		ActiveCameraChanged();
	}
}



void meView3D::OnFrameUpdate( float elapsed ){
	igdeViewRenderWindow::OnFrameUpdate( elapsed );
	
	if( pWorld ){
		pWorld->UpdateDEWorld( elapsed );
		
		// update fps
		const float fpsElapsed = decMath::max( elapsed, 0.001f );
		int i, statsFPS = 0;
		
		for( i=29; i>1; i-- ) pFPSHistory[ i ] = pFPSHistory[ i - 1 ];
		pFPSHistory[ i ] = ( int )( 1.0f / fpsElapsed );
		for( i=0; i<30; i++ ) statsFPS += pFPSHistory[ i ];
		statsFPS /= 30;
		pFPSRedrawCanvasDelay -= fpsElapsed;
		
		if( pFPSRedrawCanvasDelay <= 0.0f ){
			pFPSRedrawCanvasDelay = 1.0f;
			
			if( pCanvasFPSText ){
				decString text;
				text.Format( "FPS: %i", statsFPS );
				pCanvasFPSText->SetText( text );
			}
		}
	}
	
	if( pEditor ){
		pEditor->OnFrameUpdate( elapsed );
	}
}



void meView3D::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	
	if( ! pCanvasFPS ){
		pCanvasFPS.TakeOver( GetEngine()->GetCanvasManager()->CreateCanvasView() );
		pCanvasFPS->SetOrder( 10.0f );
		pCanvasFPS->SetPosition( decPoint( 5, 5 ) );
		pCanvasFPS->SetSize( decPoint( pFontStats->GetLineHeight() * 4, pFontStats->GetLineHeight() ) );
		AddCanvas( pCanvasFPS );
		
		deCanvasPaintReference canvasBackground;
		canvasBackground.TakeOver( GetEngine()->GetCanvasManager()->CreateCanvasPaint() );
		canvasBackground->SetShapeType( deCanvasPaint::estRectangle );
		canvasBackground->SetFillColor( decColor( 0.0f, 0.0f, 0.0f, 0.5f ) );
		canvasBackground->SetLineColor( decColor( 0.0f, 0.0f, 0.0f, 0.0f ) );
		canvasBackground->SetThickness( 0.0f );
		canvasBackground->SetOrder( 0.0f );
		canvasBackground->SetSize( pCanvasFPS->GetSize() );
		pCanvasFPS->AddCanvas( canvasBackground );
	}
	
	if( ! pCanvasFPSText ){
		pCanvasFPSText.TakeOver( GetEngine()->GetCanvasManager()->CreateCanvasText() );
		pCanvasFPSText->SetColor( decColor( 1.0f, 1.0f, 1.0f, 1.0f ) );
		pCanvasFPSText->SetFont( pFontStats );
		pCanvasFPSText->SetFontSize( ( float )pFontStats->GetLineHeight() );
		pCanvasFPSText->SetOrder( 1.0f );
		pCanvasFPSText->SetPosition( decPoint( 1, 0 ) );
		pCanvasFPSText->SetSize( pCanvasFPS->GetSize() - decPoint( 2, 0 ) );
		pCanvasFPS->AddCanvas( pCanvasFPSText );
	}
	
	ActiveCameraChanged();
	
	// has to be done here since creating editors creates also canvas
	ModeChanged();
}

void meView3D::OnResize(){
	if( pEditor ){
		pEditor->OnResize();
	}
}



void meView3D::ModeChanged(){
	if( pEditor ){
		delete pEditor;
		pEditor = NULL;
	}
	
	if( ! pWorld ){
		return;
	}
	
	if( ! GetRenderWindowCanvas() ){
		// check required since ModeChanged() can be called during construction time where
		// no canvas is yet present
		return;
	}
	
	switch( pWorld->GetGuiParameters().GetWorkMode() ){
	case meWorldGuiParameters::ewmSelect:
		pEditor = new meViewEditorSelect( *this );
		break;
		
	case meWorldGuiParameters::ewmMove:
		pEditor = new meViewEditorMove( *this );
		break;
		
	case meWorldGuiParameters::ewmRotate:
		pEditor = new meViewEditorRotate( *this );
		break;
		
	case meWorldGuiParameters::ewmScale:
		pEditor = new meViewEditorScale( *this );
		break;
		
	case meWorldGuiParameters::ewmAddNew:
		pEditor = new meViewEditorAddNew( *this );
		break;
		
	case meWorldGuiParameters::ewmHeightPaint:
		pEditor = new meViewEditorHeightPaint( *this );
		break;
		
	case meWorldGuiParameters::ewmMaskPaint:
		pEditor = new meViewEditorMaskPaint( *this );
		break;
		
	case meWorldGuiParameters::ewmVisibilityPaint:
		pEditor = new meViewEditorVisibilityPaint( *this );
		break;
		
	case meWorldGuiParameters::ewmNavSpaceEdit:
		pEditor = new meViewEditorNavSpaceEdit( *this );
		break;
		
	default:
		// if we don't know what to do stick to navigation. this can never be wrong
		pEditor = new meViewEditorNavigation( *this );
	}
}

void meView3D::ActiveCameraChanged(){
	if( pWorld ){
		SetRenderWorld( pWorld->GetActiveCamera()->GetEngineCamera() );
		
	}else{
		SetRenderWorld( NULL );
	}
}
