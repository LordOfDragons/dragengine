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

#ifndef _CEVIEWCONVO_H_
#define _CEVIEWCONVO_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseCameraListenerReference.h>
#include <deigde/gui/event/igdeMouseKeyListenerReference.h>

#include <dragengine/resources/canvas/deCanvasPaintReference.h>


class ceViewConversationListener;
class ceCanvasRuleOfThirdsAid;
class ceWindowMain;
class ceConversation;
class decBoundary;



/**
 * \brief Conversation View.
 */
class ceViewConversation : public igdeViewRenderWindow{
private:
	ceWindowMain &pWindowMain;
	ceViewConversationListener *pListener;
	
	ceConversation *pConversation;
	
	ceCanvasRuleOfThirdsAid *pRuleOfThirdsAid;
	deCanvasPaintReference pCanvasBackground;
	
	igdeMouseCameraListenerReference pCameraMouseListener;
	igdeMouseKeyListenerReference pPlaybackListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation view. */
	ceViewConversation( ceWindowMain &windowMain );
	
	/** \brief Cleans up conversation view. */
	virtual ~ceViewConversation();
	/*@}*/
	
	
	
	/** \name Management */
	/*@{*/
	/** \brief Reset view. */
	void ResetView();
	
	/** \brief Monitored conversation. */
	inline ceConversation *GetConversation() const{ return pConversation; }
	
	/** \brief Set conversation to monitor. */
	void SetConversation( ceConversation *conversation );
	
	/** \brief Viewport boundary using the ratio stored in the conversation object if existing. */
	void GetViewportWithRatio( decBoundary &viewport ) const;
	
	/** \brief Rule of thirds aid canvas. */
	inline ceCanvasRuleOfThirdsAid *GetRuleOfThirdsAid() const{ return pRuleOfThirdsAid; }
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate( float elapsed );
	/*@}*/
};

#endif
