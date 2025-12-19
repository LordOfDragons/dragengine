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

#ifndef _CEUCCLOGICPASTE_H_
#define _CEUCCLOGICPASTE_H_

#include "../../../conversation/condition/ceConversationCondition.h"
#include "../../../conversation/condition/ceCConditionLogic.h"
#include "../../../conversation/action/ceConversationAction.h"
#include "../../../conversation/topic/ceConversationTopic.h"

#include <deigde/undo/igdeUndo.h>




/**
 * \brief Undo Logic Conversation Condition Paste Condition.
 */
class ceUCCLogicPaste : public igdeUndo{
public:
	typedef deTObjectReference<ceUCCLogicPaste> Ref;
	
	
private:
	ceConversationTopic::Ref pTopic;
	ceConversationAction::Ref pAction;
	ceCConditionLogic::Ref pLogic;
	ceConversationCondition::List pConditions;
	
public:
	/** \name Constructors and Destructors */
	/*@{*/
	/** \brief Create undo. */
	ceUCCLogicPaste(ceConversationTopic *topic, ceConversationAction *action,
		ceCConditionLogic *logic, const ceConversationCondition::List &conditions);
protected:
	/** \brief Clean up undo. */
	virtual ~ceUCCLogicPaste();
	/*@}*/
	
public:
	/** \name Management */
	/*@{*/
	/** \brief Retrieves the list of conditions. */
	inline const ceConversationCondition::List &GetConditions() const{ return pConditions; }
	
	/** \brief Undo. */
	virtual void Undo();
	/** \brief Redo. */
	virtual void Redo();
	/*@}*/
	
protected:
	ceConversationCondition *ActivateConditionAfterRemove() const;
};

#endif
