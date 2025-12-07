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

#ifndef _CEUCAASPEAKSTRIPSSCALE_H_
#define _CEUCAASPEAKSTRIPSSCALE_H_

#include <deigde/undo/igdeUndo.h>
#include "../../../../conversation/strip/ceStripList.h"

class ceCAActorSpeak;
class ceConversationTopic;



/**
 * \brief Undo action actor speak conversation action scale strips.
 */
class ceUCAASpeakStripsScale : public igdeUndo{
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUCAASpeakStripsScale> Ref;
	
	struct sStrip{
		float pause;
		float duration;
	};
	
private:
	ceConversationTopic *pTopic;
	ceCAActorSpeak *pActorSpeak;
	ceStripList pStrips;
	sStrip *pOldStates;
	float pScaling;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new undo object. */
	ceUCAASpeakStripsScale(ceConversationTopic *topic, ceCAActorSpeak *actorSpeak);
	/** \brief Cleans up the undo object. */
	virtual ~ceUCAASpeakStripsScale();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Sets the list of strips capturing the current state. */
	void SetStrips(const ceStripList &strips);
	/** \brief Set new scaling. */
	void SetScaling(float scaling);
	
	/** \brief Undo action. */
	virtual void Undo();
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
