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

#ifndef _CEUCAACTORREMOVESETACTORID_H_
#define _CEUCAACTORREMOVESETACTORID_H_

#include <deigde/undo/igdeUndo.h>

#include "../../../conversation/action/ceCAActorRemove.h"
#include "../../../conversation/topic/ceConversationTopic.h"



/**
 * \brief Undo action actor remove conversation action set actor id.
 */
class ceUCAActorRemoveSetActor : public igdeUndo{
public:
	typedef deTObjectReference<ceUCAActorRemoveSetActor> Ref;
	
	
private:
	ceConversationTopic::Ref pTopic;
	ceCAActorRemove::Ref pAction;
	decString pOldActorID;
	decString pNewActorID;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new undo object. */
	ceUCAActorRemoveSetActor(ceConversationTopic *topic, ceCAActorRemove *action, const char *newActorID);
	/** \brief Cleans up the undo object. */
	virtual ~ceUCAActorRemoveSetActor();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Undo action. */
	virtual void Undo();
	/** \brief Redo action. */
	virtual void Redo();
	/*@}*/
};

#endif
