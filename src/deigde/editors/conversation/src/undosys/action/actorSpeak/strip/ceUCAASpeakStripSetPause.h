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

#ifndef _CEUCAASPEAKSTRIPSETPAUSE_H_
#define _CEUCAASPEAKSTRIPSETPAUSE_H_

#include <deigde/undo/igdeUndo.h>

#include "../../../../conversation/action/ceCAActorSpeak.h"
#include "../../../../conversation/topic/ceConversationTopic.h"
#include "../../../../conversation/strip/ceStrip.h"



/**
 * \brief Undo action actor speak conversation action set strip pause.
 */
class ceUCAASpeakStripSetPause : public igdeUndo{
public:
	typedef deTObjectReference<ceUCAASpeakStripSetPause> Ref;
	
	
private:
	ceConversationTopic::Ref pTopic;
	ceCAActorSpeak::Ref pActorSpeak;
	ceStrip::Ref pStrip;
	float pOldPause;
	float pNewPause;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new undo object. */
	ceUCAASpeakStripSetPause(ceConversationTopic *topic, ceCAActorSpeak *actorSpeak, ceStrip *strip, float newPause);
	/** \brief Cleans up the undo object. */
	virtual ~ceUCAASpeakStripSetPause();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Set new pause. */
	void SetNewPause(float pause);
	
	/** \brief Undo action. */
	virtual void Undo();
	/** \brief Redo action. */
	virtual void Redo();
	/** \brief Progressive redo action. */
	void ProgressiveRedo();
	/*@}*/
};

#endif
