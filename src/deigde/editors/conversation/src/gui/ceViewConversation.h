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

#ifndef _CEVIEWCONVO_H_
#define _CEVIEWCONVO_H_

#include <deigde/gui/igdeViewRenderWindow.h>
#include <deigde/gui/event/igdeMouseCameraListener.h>
#include <deigde/gui/event/igdeMouseKeyListener.h>

#include <dragengine/resources/canvas/deCanvasPaint.h>


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
	ceViewConversationListener::Ref pListener;
	
	ceConversation::Ref pConversation;
	
	ceCanvasRuleOfThirdsAid *pRuleOfThirdsAid;
	deCanvasPaint::Ref pCanvasBackground;
	
	igdeMouseCameraListener::Ref pCameraMouseListener;
	igdeMouseKeyListener::Ref pPlaybackListener;
	
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create conversation view. */
	ceViewConversation(ceWindowMain &windowMain);
	
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
	void SetConversation(ceConversation::Ref conversation);
	
	/** \brief Viewport boundary using the ratio stored in the conversation object if existing. */
	void GetViewportWithRatio(decBoundary &viewport) const;
	
	/** \brief Rule of thirds aid canvas. */
	inline ceCanvasRuleOfThirdsAid *GetRuleOfThirdsAid() const{ return pRuleOfThirdsAid; }
	
	/** \brief Create canvas. */
	virtual void CreateCanvas();
	
	/** \brief Widget size changed. */
	virtual void OnResize();
	
	/** \brief Game like frame update. */
	virtual void OnFrameUpdate(float elapsed);
	/*@}*/
};

#endif
