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

#ifndef _CEUCAACTORADDSETALIASID_H_
#define _CEUCAACTORADDSETALIASID_H_

#include <deigde/undo/igdeUndo.h>

#include "../../../conversation/action/ceCAActorAdd.h"
#include "../../../conversation/topic/ceConversationTopic.h"



/**
 * \brief Undo action actor add conversation action set alias id.
 */
class ceUCAActorAddSetAliasID : public igdeUndo{
private:
	ceConversationTopic::Ref pTopic;
	ceCAActorAdd::Ref pAction;
	decString pOldAliasID;
	decString pNewAliasID;
	
public:
	/** \brief Type holding strong reference. */
	typedef deTObjectReference<ceUCAActorAddSetAliasID> Ref;
	
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Creates a new undo object. */
	ceUCAActorAddSetAliasID(ceConversationTopic *topic, ceCAActorAdd *action, const char *newAliasID);
	/** \brief Cleans up the undo object. */
	virtual ~ceUCAActorAddSetAliasID();
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
