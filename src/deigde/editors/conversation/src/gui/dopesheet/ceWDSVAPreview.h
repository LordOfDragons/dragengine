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

#ifndef _CEWDSVAPREVIEW_H_
#define _CEWDSVAPREVIEW_H_

#include <deigde/gui/event/igdeMouseKeyListenerReference.h>

#include <dragengine/common/collection/decObjectList.h>
#include <dragengine/common/math/decMath.h>
#include <dragengine/resources/canvas/deCanvasViewReference.h>
#include <dragengine/resources/canvas/deCanvasImageReference.h>
#include <dragengine/resources/image/deImageReference.h>
#include <dragengine/resources/sound/deSpeakerReference.h>

class ceWindowDopeSheet;
class deSound;
class deSpeaker;


/**
 * \brief Dope sheet voice audio preview.
 */
class ceWDSVAPreview{
private:
	ceWindowDopeSheet &pWindow;
	
	deCanvasViewReference pCanvas;
	deCanvasImageReference pCanvasPreview;
	
	igdeMouseKeyListenerReference pMouseKeyListener;
	
	float pCurTime;
	
	deSpeakerReference pSpeaker;
	deImageReference pImagePreview;
	bool pDirtyPreview;
	char *pPreviewSamples;
	
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create dope sheet lane. */
	ceWDSVAPreview( ceWindowDopeSheet &dopeSheet );
	
	/** \brief Clean up dope sheet lane. */
	~ceWDSVAPreview();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Dope sheet. */
	inline ceWindowDopeSheet &GetWindow() const{ return pWindow; }
	
	/** \brief Canvas. */
	inline deCanvasView *GetCanvas() const{ return pCanvas; }
	
	/** \brief Mouse listener. */
	inline igdeMouseKeyListener *GetMouseKeyListener() const{ return pMouseKeyListener; }
	
	
	
	/** \brief Current time. */
	inline float GetCurTime() const{ return pCurTime; }
	
	/** \brief Set current time. */
	void SetCurTime( float time );
	
	/** \brief Set preview speaker. */
	void SetSpeaker( deSpeaker *speaker );
	
	
	
	/** \brief Play back voice audio. */
	void PlayVoiceAudio();
	
	
	
	/** \brief Dope sheet size changed. */
	void OnResize();
	
	/** \brief Action changed. */
	void OnActionChanged();
	
	/** \brief Invalidate preview. */
	void InvalidatePreview();
	
	
	
	/** \brief Create canvas. */
	void CreateCanvas();
	
	/** \brief Update canvas. */
	void UpdateCanvas();
	
	/** \brief Rebuild canvas. */
	void RebuildCanvas();
	/*@}*/
	
	
	
private:
	void UpdateVAPreviewImage();
};

#endif
