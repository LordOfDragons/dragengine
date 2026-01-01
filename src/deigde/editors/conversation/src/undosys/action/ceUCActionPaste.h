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

#ifndef _CEUCACTIONPASTE_H_
#define _CEUCACTIONPASTE_H_

#include "../../conversation/action/ceConversationAction.h"
#include "../../conversation/topic/ceConversationTopic.h"

#include <deigde/undo/igdeUndo.h>




/**
 * \brief Undo Action Add Conversation Action.
 */
class ceUCActionPaste : public igdeUndo{
public:
	typedef deTObjectReference<ceUCActionPaste> Ref;
	
	
private:
	ceConversationTopic::Ref pTopic;
	ceConversationAction::List pActions;
	int pIndex;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCActionPaste(ceConversationTopic *topic, const ceConversationAction::List &actions, int index);
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCActionPaste();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Topic. */
	inline ceConversationTopic &GetTopic() const{ return *pTopic; }
	
	/** \brief Retrieves the list of actions. */
	inline const ceConversationAction::List &GetActions() const{ return pActions; }
	
	/** \brief Index. */
	inline int GetIndex() const{ return pIndex; }
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
protected:
	void pInsertActions(ceConversationAction::List &list);
	void pRemoveActions(ceConversationAction::List &list);
	void pSelectInserted();
	ceConversationAction *ActivateActionAfterRemove(const ceConversationAction::List &list) const;
};

#endif
