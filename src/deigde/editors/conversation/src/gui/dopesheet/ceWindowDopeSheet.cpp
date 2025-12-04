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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ceWDSVAPreview.h"
#include "ceWindowDopeSheet.h"
#include "ceWindowDopeSheetListener.h"
#include "lane/ceWDSLaneWord.h"
#include "lane/ceWDSLaneFacePose.h"
#include "lane/ceWDSLaneGesture.h"
#include "lane/ceWDSLaneBodyLookAt.h"
#include "lane/ceWDSLaneHeadLookAt.h"
#include "lane/ceWDSLaneEyesLookAt.h"
#include "../ceWindowMain.h"
#include "../properties/ceWindowProperties.h"
#include "../properties/ceWPTopic.h"
#include "../../conversation/ceConversation.h"
#include "../../conversation/action/ceCAActorSpeak.h"
#include "../../conversation/action/ceConversationAction.h"
#include "../../conversation/camerashot/ceCameraShot.h"
#include "../../conversation/file/ceConversationFile.h"
#include "../../conversation/playback/cePlayback.h"
#include "../../conversation/playback/cePlaybackActionStackEntry.h"
#include "../../conversation/topic/ceConversationTopic.h"
#include "../../undosys/cameraShot/ceUCCShotSetParameter.h"

#include <deigde/environment/igdeEnvironment.h>
#include <deigde/gui/igdeUIHelper.h>
#include <deigde/gui/igdeComboBox.h>
#include <deigde/gui/igdeContainerReference.h>
#include <deigde/gui/igdeLabel.h>
#include <deigde/gui/igdeScrollBar.h>
#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeAction.h>
#include <deigde/gui/event/igdeComboBoxListener.h>
#include <deigde/gui/event/igdeScrollBarListener.h>
#include <deigde/gui/event/igdeMouseKeyListener.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/layout/igdeContainerFixed.h>
#include <deigde/gui/layout/igdeContainerBox.h>
#include <deigde/gui/layout/igdeContainerBorder.h>
#include <deigde/gui/menu/igdeMenuCascade.h>
#include <deigde/gui/menu/igdeMenuCascadeReference.h>
#include <deigde/gui/theme/igdeGuiTheme.h>
#include <deigde/gui/theme/propertyNames.h>
#include <deigde/gui/theme/themeNames.h>

#include <dragengine/deEngine.h>
#include <dragengine/common/exceptions.h>
#include <dragengine/resources/canvas/deCanvasView.h>
#include <dragengine/resources/canvas/deCanvasPaint.h>
#include <dragengine/resources/canvas/deCanvasImage.h>
#include <dragengine/resources/canvas/deCanvasManager.h>
#include <dragengine/resources/canvas/deCanvasRenderWorld.h>
#include <dragengine/resources/canvas/deCanvasText.h>
#include <dragengine/resources/font/deFont.h>
#include <dragengine/resources/sound/deSound.h>



// Actions
////////////

namespace{

class cBaseAction : public igdeAction{
protected:
	ceWindowDopeSheet &pWindow;
	
public:
	cBaseAction( ceWindowDopeSheet &window, const char *text, igdeIcon *icon, const char *description ) :
	igdeAction( text, icon, description ),
	pWindow( window ){ }
	
	virtual void OnAction(){
		ceConversation * const conversation = pWindow.GetConversation();
		if( conversation ){
			OnAction( *conversation );
		}
	}
	
	virtual void OnAction( ceConversation &conversation ) = 0;
	
	virtual void Update(){
		ceConversation * const conversation = pWindow.GetConversation();
		if( conversation ){
			Update( *conversation );
			
		}else{
			SetEnabled( false );
			SetSelected( false );
		}
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( true );
		SetSelected( false );
	}
};

class cActionPlayAction : public cBaseAction{
public:
	cActionPlayAction( ceWindowDopeSheet &window ) : cBaseAction( window, "",
		window.GetWindowMain().GetIconPlayAction(), "Play selected action" ){
	}
	
	virtual void OnAction( ceConversation &conversation ){
		ceConversationAction * const action = pWindow.GetAction();
		if( action ){
			conversation.GetPlayback()->PlaySingleAction( action, 0.0f );
		}
	}
	
	virtual void Update( const ceConversation & ){
		SetEnabled( pWindow.GetAction() );
	}
};

class cActionPlayFromHere : public cBaseAction{
public:
	cActionPlayFromHere( ceWindowDopeSheet &window ) : cBaseAction( window, "",
		window.GetWindowMain().GetIconPlayFromHere(), "Play onwards from selected action" ){
	}
	
	virtual void OnAction( ceConversation & ){
		pWindow.GetWindowMain().GetWindowProperties().GetPanelTopic().PlayActionFromHere();
	}
	
	virtual void Update( const ceConversation &conversation ){
		SetEnabled( conversation.GetPlayback()->GetMainActionStack()->IsNotEmpty() );
	}
};

class cActionPause : public cBaseAction{
public:
	cActionPause( ceWindowDopeSheet &window ) : cBaseAction( window, "",
		window.GetWindowMain().GetIconPlayPause(), "Pause/Resume playing" ){
	}
	
	virtual void OnAction( ceConversation &conversation ){
		if( pWindow.GetTopic() ){
			conversation.GetPlayback()->SetPaused( ! conversation.GetPlayback()->GetPaused() );
		}
	}
	
	virtual void Update( const ceConversation &conversation ){
		SetEnabled( conversation.GetPlayback()->GetMainActionStack()->IsNotEmpty() );
		SetSelected( conversation.GetPlayback()->GetPaused() );
	}
};

class cActionSelectCurAction : public cBaseAction{
public:
	cActionSelectCurAction( ceWindowDopeSheet &window ) : cBaseAction( window, "",
		window.GetWindowMain().GetIconPlaySelectCurAction(), "Select currently played back action" ){
	}
	
	virtual void OnAction( ceConversation &conversation ){
		const cePlayback &playback = *conversation.GetPlayback();
		ceConversationAction * const action = playback.GetLastPlayedAction();
		ceConversationTopic * const topic = playback.GetLastPlayedActionTopic();
		if( ! action || ! topic ){
			return;
		}
		
		ceConversationFile * const file = topic->GetFile();
		if( ! file ){
			return;
		}
		
		conversation.SetActiveFile( file );
		file->SetActiveTopic( topic );
		
		pWindow.GetWindowMain().GetWindowProperties().GetPanelTopic().LocateAction( action );
	}
	
	virtual void Update( const ceConversation &conversation ){
		SetEnabled( conversation.GetPlayback()->GetMainActionStack()->IsNotEmpty() );
	}
};

class cScrollTime : public igdeScrollBarListener{
	ceWindowDopeSheet &pWindow;
	
public:
	cScrollTime( ceWindowDopeSheet &window ) : pWindow( window ){ }
	
	virtual void OnValueChanged( igdeScrollBar* ){
		ceConversation * const conversation = pWindow.GetConversation();
		if( conversation ){
			pWindow.OnTimeChanged();
		}
	}
};

class cComboTimeScale : public igdeComboBoxListener{
	ceWindowDopeSheet &pWindow;
	
public:
	cComboTimeScale( ceWindowDopeSheet &window ) : pWindow( window ){ }
	
	virtual void OnTextChanged( igdeComboBox *comboBox ){
		ceConversation * const conversation = pWindow.GetConversation();
		if( conversation ){
			pWindow.SetZoomTime( 0.01f * decMath::clamp( comboBox->GetText().ToInt(), 10, 800 ) );
		}
	}
};



class cMouseKeyListener : public igdeMouseKeyListener{
	ceWindowDopeSheet &pWindow;
	igdeMouseKeyListener::Ref pDragListener;
	
public:
	cMouseKeyListener( ceWindowDopeSheet &window ) : pWindow( window ){}
	
	igdeMouseKeyListener *GetListenerAtPosition( const decPoint &position ) const{
		if( position.y < pWindow.GetVAPreviewHeight() ){
			return pWindow.GetVAPreview().GetMouseKeyListener();
			
		}else{
			ceWDSLane * const lane = pWindow.GetLaneAtPosition( position );
			if( lane ){
				return lane->GetMouseKeyListener();
			}
		}
		
		return NULL;
	}
	
	virtual void OnButtonPress( igdeWidget *widget, int button, const decPoint &position, int modifiers ){
		pDragListener = GetListenerAtPosition( position );
		if( pDragListener){
			pDragListener->OnButtonPress( widget, button, position, modifiers );
		}
	}
	
	virtual void OnButtonRelease( igdeWidget *widget, int button, const decPoint &position, int modifiers ){
		if( pDragListener ){
			pDragListener->OnButtonRelease( widget, button, position, modifiers );
			pDragListener = NULL;
		}
	}
	
	virtual void OnDoubleClicked( igdeWidget *widget, int button, const decPoint &position, int modifiers ){
		pDragListener = GetListenerAtPosition( position );
		if( pDragListener ){
			pDragListener->OnDoubleClicked( widget, button, position, modifiers );
		}
	}
	
	virtual void OnMouseMoved( igdeWidget *widget, const decPoint &position, int modifiers ){
		igdeMouseKeyListener *listener = pDragListener;
		if( ! listener ){
			listener = GetListenerAtPosition( position );
		}
		if( listener ){
			listener->OnMouseMoved( widget, position, modifiers );
		}
	}
	
	virtual void OnMouseWheeled( igdeWidget *widget, const decPoint &position, const decPoint &change, int modifiers ){
		igdeMouseKeyListener *listener = pDragListener;
		if( ! listener ){
			listener = GetListenerAtPosition( position );
		}
		if( listener ){
			listener->OnMouseWheeled( widget, position, change, modifiers );
		}
	}
};

}


// class ceWindowDopeSheet::cDopeSheet
////////////////////////////////////////

ceWindowDopeSheet::cDopeSheet::cDopeSheet( ceWindowDopeSheet &window ) :
igdeViewRenderWindow( window.GetEnvironment() ), pWindow( window ){
}

ceWindowDopeSheet::cDopeSheet::~cDopeSheet(){
}

void ceWindowDopeSheet::cDopeSheet::CreateCanvas(){
	igdeViewRenderWindow::CreateCanvas();
	GetCanvasRenderWorld()->SetVisible( false );
	pWindow.CreateDopeSheetCanvas( *this );
}

void ceWindowDopeSheet::cDopeSheet::OnResize(){
	igdeViewRenderWindow::OnResize();
	pWindow.ResizeDopeSheetCanvas();
}



// Class ceWindowDopeSheet
////////////////////////////

// Constructor, destructor
////////////////////////////

ceWindowDopeSheet::ceWindowDopeSheet( ceWindowMain &windowMain ) :
igdeContainerFlow( windowMain.GetEnvironment(), igdeContainerFlow::eaX, igdeContainerFlow::esLast, 2 ),
pWindowMain( windowMain ),
pListener( NULL ),
pConversation( NULL ),
pZoomTime( 1.0f ),
pPixelPerSecond( 300.0f ),
pSecondPerPixel( 1.0f / pPixelPerSecond ),
pVAPreviewHeight( 40 ),
pVAPreview( NULL )
{
	igdeEnvironment &env = windowMain.GetEnvironment();
	igdeUIHelper &helper = env.GetUIHelper();
	igdeContainerReference panel, panel2, panel3, panel4, panel5;
	int i;
	
	pListener = new ceWindowDopeSheetListener( *this );
	
	SetWidgetGuiThemeName( igdeGuiThemeNames::properties );
	
	pGetFontText();
	
	// actions
	pActionPlayAction.TakeOver( new cActionPlayAction( *this ) );
	pActionPlayFromhere.TakeOver( new cActionPlayFromHere( *this ) );
	pActionPause.TakeOver( new cActionPause( *this ) );
	pActionSelectCurAction.TakeOver( new cActionSelectCurAction( *this ) );
	
	
	// lanes
	deObjectReference lane;
	lane.TakeOver( new ceWDSLaneWord( *this, 0, "Word", "Speech animation." ) );
	pLanes.Add( lane );
	lane.TakeOver( new ceWDSLaneFacePose( *this, 1, "Face Pose", "Facial animation." ) );
	pLanes.Add( lane );
	lane.TakeOver( new ceWDSLaneGesture( *this, 2, "Gesture", "Gesture playback." ) );
	pLanes.Add( lane );
	lane.TakeOver( new ceWDSLaneBodyLookAt( *this, 3, "Body Look-At", "Body orientation." ) );
	pLanes.Add( lane );
	lane.TakeOver( new ceWDSLaneHeadLookAt( *this, 4, "Head Look-At", "Head orientation." ) );
	pLanes.Add( lane );
	lane.TakeOver( new ceWDSLaneEyesLookAt( *this, 5, "Eyes Look-At", "Eyes orientation. If empty uses Head Look-At." ) );
	pLanes.Add( lane );
	
	// voice audio preview
	pVAPreview = new ceWDSVAPreview( *this );
	
	
	// buttons on the left
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaYCenter, igdeContainerFlow::esNone ) );
	helper.Button( panel, pBtnPlayAction, pActionPlayAction );
	helper.Button( panel, pBtnPlayFromHere, pActionPlayFromhere );
	helper.Button( panel, pBtnPlayPause, pActionPause );
	helper.Button( panel, pBtnPlaySelectCurAction, pActionSelectCurAction );
	AddChild( panel );
	
	// scroll bar, dope sheet lane titles and dope sheet content in the center
	panel.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esFirst, 2 ) );
	
	panel2.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esLast, 2 ) );
	
	panel3.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY, igdeContainerFlow::esLast ) );
	helper.Spacer( panel3, decPoint( 10, pVAPreviewHeight ) );
	
	panel4.TakeOver( new igdeContainerBox( env, igdeContainerBox::eaY ) );
	for( i=0; i<pLanes.GetCount(); i++ ){
		const ceWDSLane &lane2 = *( ( ceWDSLane* )pLanes.GetAt( i ) );
		panel5.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaY ) );
		helper.Spacer( panel5, decPoint( 10, 2 ) );
		helper.Label( panel5, lane2.GetLabel(), lane2.GetDescription(), igdeLabel::eaLeft | igdeLabel::eaMiddle );
		helper.Spacer( panel5, decPoint( 10, 2 ) );
		panel4->AddChild( panel5 );
	}
	panel3->AddChild( panel4 );
	panel2->AddChild( panel3 );
	
	pDopeSheet.TakeOver( new cDopeSheet( *this ) );
	igdeMouseKeyListener::Ref mklistener;
	mklistener.TakeOver( new cMouseKeyListener( *this ) );
	( ( cDopeSheet& )( igdeWidget& )pDopeSheet ).AddListener( mklistener );
	panel2->AddChild( pDopeSheet );
	
	panel->AddChild( panel2 );
	
	// scroll and zoom frame on bottom
	panel2.TakeOver( new igdeContainerFlow( env, igdeContainerFlow::eaX, igdeContainerFlow::esLast, 2 ) );
	
	helper.Label( panel2, "Time-Scale:", "Time scaling" );
	helper.ComboBox( panel2, true, "Time scaling", pCBTimeScale, new cComboTimeScale( *this ) );
	pCBTimeScale->AddItem( "12" );
	pCBTimeScale->AddItem( "25" );
	pCBTimeScale->AddItem( "33" );
	pCBTimeScale->AddItem( "50" );
	pCBTimeScale->AddItem( "66" );
	pCBTimeScale->AddItem( "75" );
	pCBTimeScale->AddItem( "100" );
	pCBTimeScale->AddItem( "150" );
	pCBTimeScale->AddItem( "200" );
	pCBTimeScale->AddItem( "300" );
	pCBTimeScale->AddItem( "400" );
	pCBTimeScale->AddItem( "600" );
	pCBTimeScale->AddItem( "800" );
	pCBTimeScale->SetText( "100" );
	
	helper.ScrollBar( panel2, true, 0, 1, 1, 0, pSBTime, new cScrollTime( *this ) );
	panel->AddChild( panel2 );
	
	AddChild( panel );
}

ceWindowDopeSheet::~ceWindowDopeSheet(){
	SetConversation( NULL );
	
	if( pListener ){
		pListener->FreeReference();
	}
	if( pVAPreview ){
		delete pVAPreview;
	}
}



// Management
///////////////

void ceWindowDopeSheet::SetConversation( ceConversation *conversation ){
	if( conversation == pConversation ){
		return;
	}
	
	pVAPreview->SetSpeaker( NULL );
	
	if( pConversation ){
		pConversation->RemoveListener( pListener );
		pConversation->FreeReference();
	}
	
	pConversation = conversation;
	
	if( conversation ){
		conversation->AddListener( pListener );
		conversation->AddReference();
		
		pVAPreview->SetSpeaker( conversation->GetEngineSpeakerVAPreview() );
	}
	
	OnPlaybackChanged();
	OnActionChanged();
}



ceConversationFile *ceWindowDopeSheet::GetFile() const{
	return pConversation ? pConversation->GetActiveFile() : NULL;
}

ceConversationTopic *ceWindowDopeSheet::GetTopic() const{
	ceConversationFile * const file = GetFile();
	return file ? file->GetActiveTopic() : NULL;
}

ceConversationAction *ceWindowDopeSheet::GetAction() const{
	ceConversationTopic * const topic = GetTopic();
	return topic ? topic->GetActiveAction() : NULL;
}

ceCAActorSpeak *ceWindowDopeSheet::GetActionASpeak() const{
	ceConversationAction * const action = GetAction();
	return action && action->GetType() == ceConversationAction::eatActorSpeak
		? ( ceCAActorSpeak* )action : NULL;
}

ceCameraShot *ceWindowDopeSheet::GetCameraShot() const{
	return pConversation ? pConversation->GetActiveCameraShot() : NULL;
}

int ceWindowDopeSheet::GetScrollTime() const{
	return pSBTime->GetValue();
}

void ceWindowDopeSheet::SetZoomTime( float zoom ){
	if( fabsf( zoom - pZoomTime ) <= FLOAT_SAFE_EPSILON ){
		return;
	}
	
	pZoomTime = zoom;
	
	int initZoomPixPerSec = 300;
	pPixelPerSecond = pZoomTime * initZoomPixPerSec;
	pSecondPerPixel = 1.0f / pPixelPerSecond;
	
	decString text;
	text.Format( "%d", ( int )( zoom * 100.0f + 0.5f ) );
	pCBTimeScale->SetText( text );
	
	pUpdateScrollbars();
	OnTimeChanged();
	if( pVAPreview ){
		pVAPreview->InvalidatePreview();
	}
}

float ceWindowDopeSheet::GetMaximumLinesTime() const{
	const int laneCount = pLanes.GetCount();
	float maxTime = 0.0f;
	int i;
	
	for( i=0; i<laneCount; i++ ){
		maxTime = decMath::max( maxTime, ( ( ceWDSLane* )pLanes.GetAt( i ) )->GetMaximumLineTime() );
	}
	
	return maxTime;
}

float ceWindowDopeSheet::GetTimeForX( int x ) const{
	return pSecondPerPixel * ( pSBTime->GetValue() + x );
}

int ceWindowDopeSheet::GetXForTime( float time ) const{
	return ( int )( pPixelPerSecond * time + 0.5f ) - pSBTime->GetValue();
}



ceWDSLane &ceWindowDopeSheet::GetLane( eLanes lane ) const{
	return *( ( ceWDSLane* )pLanes.GetAt( lane ) );
}

ceWDSLane *ceWindowDopeSheet::GetLaneAtPosition( const decPoint &position ) const{
	const int laneCount = pLanes.GetCount();
	int i;
	
	for( i=0; i<laneCount; i++ ){
		ceWDSLane * const lane = ( ceWDSLane* )pLanes.GetAt( i );
		deCanvasView * const canvas = lane->GetCanvas();
		if( canvas && position >= canvas->GetPosition() && position < canvas->GetPosition() + canvas->GetSize() ){
			return lane;
		}
	}
	
	return NULL;
}

decPoint ceWindowDopeSheet::GetSizeDopeSheet() const{
	if( pDopeSheet ){
		return ( ( cDopeSheet& )( igdeWidget& )pDopeSheet ).GetRenderAreaSize();
		
	}else{
		return decPoint( 100, 10 );
	}
}

int ceWindowDopeSheet::GetLaneHeight() const{
	return decMath::max( ( GetSizeDopeSheet().y - pVAPreviewHeight ) / pLanes.GetCount(), 1 );
}




void ceWindowDopeSheet::OnActionChanged(){
	const int laneCount = pLanes.GetCount();
	int i;
	for( i=0; i<laneCount; i++ ){
		( ( ceWDSLane* )pLanes.GetAt( i ) )->OnActionChanged();
	}
	
	pUpdateScrollbars();
	pRebuildTimeLinesAndLabels();
	if( pVAPreview ){
		pVAPreview->OnActionChanged();
	}
	pUpdateActions();
}

void ceWindowDopeSheet::OnPlaybackChanged(){
	pUpdateActions();
}

void ceWindowDopeSheet::CreateDopeSheetCanvas( igdeViewRenderWindow &view ){
	igdeEnvironment &env = GetEnvironment();
	const decColor colorBackground( env.GetSystemColor( igdeEnvironment::escWidgetBackground ) );
	const decColor colorBorder( env.GetSystemColor( igdeEnvironment::escWidgetShadow ) );
	const decColor colorPreviewTime( 1.0f, 0.0f, 0.0f );
	
	deCanvasManager &canvasManager = *GetEngine()->GetCanvasManager();
	deCanvasView &content = *view.GetRenderWindowCanvas();
	
	pCanvasPanelDopeSheet.TakeOver( canvasManager.CreateCanvasPaint() );
	pCanvasPanelDopeSheet->SetFillColor( colorBorder );
	pCanvasPanelDopeSheet->SetThickness( 0 );
	pCanvasPanelDopeSheet->SetOrder( ( float )content.GetCanvasCount() );
	view.AddCanvas( pCanvasPanelDopeSheet );
	
	pCanvasPanelVAPreview.TakeOver( canvasManager.CreateCanvasPaint() );
	pCanvasPanelVAPreview->SetFillColor( colorBackground );
	pCanvasPanelVAPreview->SetThickness( 0 );
	pCanvasPanelVAPreview->SetOrder( ( float )content.GetCanvasCount() );
	view.AddCanvas( pCanvasPanelVAPreview );
	
	pCanvasPanelVAPreviewLine.TakeOver( canvasManager.CreateCanvasPaint() );
	pCanvasPanelVAPreviewLine->SetFillColor( colorBorder );
	pCanvasPanelVAPreviewLine->SetThickness( 0 );
	pCanvasPanelVAPreviewLine->SetOrder( ( float )content.GetCanvasCount() );
	view.AddCanvas( pCanvasPanelVAPreviewLine );
	
	pCanvasTimeLines.TakeOver( canvasManager.CreateCanvasView() );
	pCanvasTimeLines->SetOrder( ( float )content.GetCanvasCount() );
	view.AddCanvas( pCanvasTimeLines );
	
	pCanvasTimeLineLabels.TakeOver( canvasManager.CreateCanvasView() );
	pCanvasTimeLineLabels->SetOrder( ( float )content.GetCanvasCount() );
	view.AddCanvas( pCanvasTimeLineLabels );
	
	const int laneCount = pLanes.GetCount();
	int i;
	for( i=0; i<laneCount; i++ ){
		ceWDSLane &lane = *( ( ceWDSLane* )pLanes.GetAt( i ) );
		lane.CreateCanvas();
		if( lane.GetCanvas() ){
			lane.GetCanvas()->SetOrder( ( float )content.GetCanvasCount() );
			view.AddCanvas( lane.GetCanvas() );
		}
	}
	
	pCanvasVAPreviewTime.TakeOver( canvasManager.CreateCanvasPaint() );
	pCanvasVAPreviewTime->SetFillColor( colorPreviewTime );
	pCanvasVAPreviewTime->SetThickness( 0 );
	pCanvasVAPreviewTime->SetOrder( ( float )content.GetCanvasCount() );
	view.AddCanvas( pCanvasVAPreviewTime );
	
	ResizeDopeSheetCanvas();
}

void ceWindowDopeSheet::ResizeDopeSheetCanvas(){
	const decPoint sizeDopeSheet( GetSizeDopeSheet() );
	
	if( pCanvasPanelDopeSheet ){
		pCanvasPanelDopeSheet->SetPosition( decPoint( 0, pVAPreviewHeight ) );
		pCanvasPanelDopeSheet->SetSize( decPoint( sizeDopeSheet.x, sizeDopeSheet.y - pVAPreviewHeight ) );
	}
	if( pCanvasPanelVAPreview ){
		pCanvasPanelVAPreview->SetPosition( decPoint( 0, 0 ) );
		pCanvasPanelVAPreview->SetSize( decPoint( sizeDopeSheet.x, pVAPreviewHeight ) );
	}
	if( pCanvasPanelVAPreviewLine ){
		pCanvasPanelVAPreviewLine->SetPosition( decPoint( 0, pVAPreviewHeight - 1 ) );
		pCanvasPanelVAPreviewLine->SetSize( decPoint( sizeDopeSheet.x, 1 ) );
	}
	if( pCanvasTimeLines ){
		pCanvasTimeLines->SetPosition( decPoint( 0, 0 ) );
		pCanvasTimeLines->SetSize( decPoint( sizeDopeSheet.x, sizeDopeSheet.y ) );
	}
	if( pCanvasTimeLineLabels ){
		pCanvasTimeLineLabels->SetPosition( decPoint( 0, 0 ) );
		pCanvasTimeLineLabels->SetSize( decPoint( sizeDopeSheet.x, sizeDopeSheet.y ) );
	}
	
	const int laneCount = pLanes.GetCount();
	int i;
	for( i=0; i<laneCount; i++ ){
		( ( ceWDSLane* )pLanes.GetAt( i ) )->RebuildCanvas();
	}
	
	pUpdateScrollbars();
	pRebuildTimeLinesAndLabels();
	pUpdateCanvasVAPreviewTime();
}

void ceWindowDopeSheet::OnTimeChanged(){
	const int laneCount = pLanes.GetCount();
	int i;
	for( i=0; i<laneCount; i++ ){
		( ( ceWDSLane* )pLanes.GetAt( i ) )->OnTimeChanged();
	}
	
	pRebuildTimeLinesAndLabels();
	pUpdateCanvasVAPreviewTime();
}



void ceWindowDopeSheet::OnAfterEngineStart(){
	( ( cDopeSheet& )( igdeWidget& )pDopeSheet ).OnAfterEngineStart();
}

void ceWindowDopeSheet::OnBeforeEngineStop(){
	( ( cDopeSheet& )( igdeWidget& )pDopeSheet ).OnBeforeEngineStop();
}

void ceWindowDopeSheet::OnFrameUpdate( float elapsed ){
	( ( cDopeSheet& )( igdeWidget& )pDopeSheet ).OnFrameUpdate( elapsed );
}

void ceWindowDopeSheet::SetEnableRendering( bool enable ){
	( ( cDopeSheet& )( igdeWidget& )pDopeSheet ).SetEnableRendering( enable );
}



// Protected Functions
////////////////////////

void ceWindowDopeSheet::pGetFontText(){
	const igdeGuiTheme &guitheme = *GetGuiTheme();
	igdeFont::sConfiguration configuration;
	GetEnvironment().GetApplicationFont( configuration );
	
	if( guitheme.HasProperty( igdeGuiThemePropertyNames::textFieldFontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::textFieldFontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::textFieldFontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::textFieldFontSize, 1.0f );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSizeAbsolute ) ){
		configuration.size = ( float )guitheme.GetIntProperty(
			igdeGuiThemePropertyNames::fontSizeAbsolute, 0 );
		
	}else if( guitheme.HasProperty( igdeGuiThemePropertyNames::fontSize ) ){
		configuration.size *= guitheme.GetFloatProperty(
			igdeGuiThemePropertyNames::fontSize, 1.0f );
	}
	
	pFontText = GetEnvironment().GetSharedFont( configuration );
}

void ceWindowDopeSheet::pUpdateActions(){
	pActionPlayAction->Update();
	pActionPlayFromhere->Update();
	pActionPause->Update();
	pActionSelectCurAction->Update();
}

void ceWindowDopeSheet::pUpdateScrollbars(){
	float playtime = 0.0f;
	
	// speech time if an audio clip is loaded otherwise a default value
	ceCAActorSpeak * const action = GetActionASpeak();
	if( action && action->GetEngineSound() ){
		playtime = action->GetEngineSound()->GetPlayTime();
	}
	
	// maximum length of all lines
	const float maxLineTime = GetMaximumLinesTime();
	if( maxLineTime > playtime ){
		playtime = maxLineTime;
	}
	
	// ontop of the found playtime add 2 seconds to have some space for dragging.
	playtime += 2.0f;
	
	// update the scrollbars
	if( pSBTime ){
		const int range = decMath::max( ( int )( playtime * pPixelPerSecond ) - GetSizeDopeSheet().x, 0 );
		const int page = decMath::max( GetSizeDopeSheet().x / 2, 1 );
		pSBTime->SetUpper( range + page );
		pSBTime->SetPageSize( page );
	}
}

void ceWindowDopeSheet::pRebuildTimeLinesAndLabels(){
	if( ! pCanvasTimeLines || ! pCanvasTimeLineLabels ){
		return;
	}
	
	pCanvasTimeLines->RemoveAllCanvas();
	pCanvasTimeLineLabels->RemoveAllCanvas();
	
	const decPoint sizeDopeSheet( GetSizeDopeSheet() );
	const int timeLast = ( int )GetTimeForX( sizeDopeSheet.x );
	const int timeFirst = ( int )GetTimeForX( 0 );
	decString text;
	int i;
	
	for( i=timeFirst; i<=timeLast; i++ ){
		if( i - timeFirst == pTimeLines.GetCount() ){
			deCanvasPaintReference canvas;
			canvas.TakeOver( GetEngine()->GetCanvasManager()->CreateCanvasPaint() );
			canvas->SetFillColor( decColor( 0.0f, 0.0f, 0.0f ) );
			//GetEnvironment().GetSystemColor( igdeEnvironment::escWidgetShadow ) );
			canvas->SetThickness( 0 );
			pTimeLines.Add( ( deCanvasPaint* )canvas );
		}
		
		if( i - timeFirst == pTimeLineLabels.GetCount() ){
			deFont * const font = pFontText->GetEngineFont();
			
			deCanvasTextReference canvas;
			canvas.TakeOver( GetEngine()->GetCanvasManager()->CreateCanvasText() );
			canvas->SetFont( font );
			canvas->SetFontSize( ( float )font->GetLineHeight() );
			canvas->SetColor( GetEnvironment().GetSystemColor( igdeEnvironment::escWidgetForeground ) );
			pTimeLineLabels.Add( ( deCanvasText* )canvas );
		}
		
		deCanvasPaint * const canvasLine = ( deCanvasPaint* )pTimeLines.GetAt( i - timeFirst );
		canvasLine->SetPosition( decPoint( GetXForTime( ( float )i ), 0 ) );
		canvasLine->SetSize( decPoint( 1, sizeDopeSheet.y ) );
		canvasLine->SetOrder( ( float )pCanvasTimeLines->GetCanvasCount() );
		pCanvasTimeLines->AddCanvas( canvasLine );
		
		deCanvasText * const canvasText = ( deCanvasText* )pTimeLineLabels.GetAt( i - timeFirst );
		text.Format( "%ds", i );
		canvasText->SetText( text );
		canvasText->SetPosition( canvasLine->GetPosition() + decPoint( 2, 2 ) );
		canvasText->SetSize( decPoint( 80, ( int )canvasText->GetFontSize() ) ); // TODO add method to get text size
		canvasText->SetOrder( ( float )pCanvasTimeLines->GetCanvasCount() );
		pCanvasTimeLines->AddCanvas( canvasText );
	}
}

void ceWindowDopeSheet::pUpdateCanvasVAPreviewTime(){
	if( ! pCanvasVAPreviewTime || ! pVAPreview ){
		return;
	}
	
	pCanvasVAPreviewTime->SetPosition( decPoint( GetXForTime( pVAPreview->GetCurTime() ), 0 ) );
	pCanvasVAPreviewTime->SetSize( decPoint( 1, GetSizeDopeSheet().y ) );
}
