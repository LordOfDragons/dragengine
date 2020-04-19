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

#ifndef _CEWINDOWDOPESHEET_H_
#define _CEWINDOWDOPESHEET_H_

#include <deigde/gui/igdeButtonReference.h>
#include <deigde/gui/igdeComboBoxReference.h>
#include <deigde/gui/igdeScrollBarReference.h>
#include <deigde/gui/igdeWidgetReference.h>
#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/layout/igdeContainerFlow.h>
#include <deigde/gui/event/igdeActionReference.h>
#include <deigde/gui/resources/igdeFontReference.h>

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/resources/canvas/deCanvasPaintReference.h>
#include <dragengine/resources/canvas/deCanvasImageReference.h>
#include <dragengine/resources/canvas/deCanvasViewReference.h>
#include <dragengine/resources/sound/deSpeakerReference.h>
#include <dragengine/resources/sound/deSoundReference.h>


class ceWindowDopeSheetListener;
class ceViewDopeSheet;
class ceWindowMain;
class ceConversation;
class ceCameraShot;
class ceConversationFile;
class ceConversationTopic;
class ceConversationAction;
class ceCAActorSpeak;
class ceWDSLane;
class ceWDSVAPreview;



/**
 * \brief Dope Sheet Window.
 */
class ceWindowDopeSheet : public igdeContainerFlow{
public:
	/** \brief Lanes. */
	enum eLanes{
		elWord,
		elFacePose,
		elGesture,
		elBodyLookAt,
		elHeadLookAt,
		elEyesLookAt
	};
	
	/** \brief Dope sheet view. */
	class cDopeSheet : public igdeViewRenderWindow{
	private:
		ceWindowDopeSheet &pWindow;
		
	public:
		cDopeSheet( ceWindowDopeSheet &window );
		
	protected:
		virtual ~cDopeSheet();
		
	public:
		virtual void OnResize();
		virtual void CreateCanvas();
	};
	
	
	
private:
	ceWindowMain &pWindowMain;
	ceWindowDopeSheetListener *pListener;
	ceConversation *pConversation;
	
	igdeActionReference pActionPlayAction;
	igdeActionReference pActionPlayFromhere;
	igdeActionReference pActionPause;
	igdeActionReference pActionSelectCurAction;
	
	igdeButtonReference pBtnPlayAction;
	igdeButtonReference pBtnPlayFromHere;
	igdeButtonReference pBtnPlayPause;
	igdeButtonReference pBtnPlaySelectCurAction;
	
	igdeScrollBarReference pSBTime;
	igdeComboBoxReference pCBTimeScale;
	igdeWidgetReference pDopeSheet;
	
	decObjectList pTimeLineLabels;
	decObjectList pTimeLines;
	
	deCanvasViewReference pCanvasTimeLines;
	deCanvasViewReference pCanvasTimeLineLabels;
	deCanvasPaintReference pCanvasPanelDopeSheet;
	deCanvasPaintReference pCanvasPanelVAPreview;
	deCanvasPaintReference pCanvasPanelVAPreviewLine;
	deCanvasPaintReference pCanvasVAPreviewTime;
	
	float pZoomTime;
	float pPixelPerSecond;
	float pSecondPerPixel;
	int pVAPreviewHeight;
	
	decObjectList pLanes;
	ceWDSVAPreview *pVAPreview;
	
	igdeFontReference pFontText;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create window. */
	ceWindowDopeSheet( ceWindowMain &windowMain );
	
protected:
	/** \brief Clean up window. */
	virtual ~ceWindowDopeSheet();
	/*@}*/
	
	
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Main window. */
	inline ceWindowMain &GetWindowMain() const{ return pWindowMain; }
	
	
	
	/** \brief Conversation to monitor. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** \brief Set conversation to monitor. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Active file. */
	ceConversationFile *GetFile() const;
	
	/** \brief Active topic. */
	ceConversationTopic *GetTopic() const;
	
	/** \brief Active action. */
	ceConversationAction *GetAction() const;
	
	/** \brief Active action if it is an actor speak action. */
	ceCAActorSpeak *GetActionASpeak() const;
	
	/** \brief Active camera shot. */
	ceCameraShot *GetCameraShot() const;
	
	/** \brief Scroll time position. */
	int GetScrollTime() const;
	
	/** \brief Pixels per second. */
	inline float GetPixelPerSecond() const{ return pPixelPerSecond; }
	
	/** \brief Second per pixel. */
	inline float GetSecondPerPixel() const{ return pSecondPerPixel; }
	
	/** \brief Zoom time factor. */
	inline float GetZoomTime() const{ return pZoomTime; }
	
	/** \brief Set time zoom factor. */
	void SetZoomTime( float zoom );
	
	/** \brief Maximum time of all lines. */
	float GetMaximumLinesTime() const;
	
	/** \brief Time in seconds for a given x position. */
	float GetTimeForX( int x ) const;
	
	/** \brief X position for time in seconds. */
	int GetXForTime( float time ) const;
	
	
	
	/** \brief Lane. */
	ceWDSLane &GetLane( eLanes lane ) const;
	
	/** \brief Lane at position. */
	ceWDSLane *GetLaneAtPosition( const decPoint &position ) const;
	
	/** \brief Voice audio preview. */
	inline ceWDSVAPreview &GetVAPreview() const{ return *pVAPreview; }
	
	
	
	/** \brief Voice audio preview height. */
	inline int GetVAPreviewHeight() const{ return pVAPreviewHeight; }
	
	/** \brief Dope sheet size. */
	decPoint GetSizeDopeSheet() const;
	
	/** \brief Lane height. */
	int GetLaneHeight() const;
	
	/** \brief Dope sheet panel. */
	inline cDopeSheet &GetDopeSheet() const{ return ( cDopeSheet& )( igdeWidget& )pDopeSheet; }
	
	/** \brief Text font. */
	inline igdeFont *GetFontText() const{ return pFontText; }
	
	
	
	/** \brief Action changed. */
	void OnActionChanged();
	
	/** \brief Playback state changed. */
	void OnPlaybackChanged();
	
	/** \brief Create and add lane canvas. */
	void CreateDopeSheetCanvas( igdeViewRenderWindow &view );
	
	/** \brief Resize dope sheet canvas and lanes. */
	void ResizeDopeSheetCanvas();
	
	/** \brief Time changed. */
	void OnTimeChanged();
	
	
	
	/** \brief Game engine has been started. */
	void OnAfterEngineStart();
	
	/** \brief Game engine is about to be stopped. */
	void OnBeforeEngineStop();
	
	/** \brief Game like frame update. */
	void OnFrameUpdate( float elapsed );
	
	/** \brief Set if rendering is enabled. */
	void SetEnableRendering( bool enable );
	/*@}*/
	
	
	
protected:
	void pGetFontText();
	void pUpdateActions();
	void pUpdateScrollbars();
	void pRebuildTimeLinesAndLabels();
	void pUpdateCanvasVAPreviewTime();
};

#endif
